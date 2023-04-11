#include "cchartview.h"

CChartView::CChartView(QQuickItem *parent) : QQuickItem(parent)
{
    //    QObject::connect(this, &QQuickItem::childrenChanged, this, &CChartView::childrenChanging);  //@N мб есть уже слот?
    axis = nullptr;
    setClip(true);
}

void CChartView::componentComplete()
{
    if(window() != nullptr) {
        auto f = window()->format();
        f.setSamples(32);
        window()->setFormat(f);
    }

    bool seriesAdded = false;
    CAxis *caxis = nullptr;
    // найдем axis и series
    for(auto c : children()) {
        auto s = qobject_cast<CSeries *>(c);
        if(s) {
            if(!series.contains(s)) {
                series.append(s);
                seriesAdded = true;
            }
        }
        else {
            auto a = qobject_cast<CAxis *>(c);
            if(a) caxis = a;
        }
    }

    // если не нашли, то найдем у series
    if(caxis == nullptr) {
        for(auto s : series) {
            if(s->getAxis() != nullptr) {
                caxis = s->getAxis();
                break;
            }
        }
    }

    // добавим ось
    if(caxis != nullptr) {
        if(axis == caxis)
            for(auto s : series) s->setAxis(this->axis);
        else setAxis(caxis);
    }

    if(seriesAdded) emit seriesChanged();
    QQuickItem::componentComplete();
}

void CChartView::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    if(axis == nullptr) {
        for(auto s : series) {
            s->setSize(QSizeF(newGeometry.width(), newGeometry.height()));
        }
    }
    else axis->setSize(QSizeF(newGeometry.width(), newGeometry.height()));
}

void CChartView::childrenChanging()
{
    qDebug() << "childrenChanging" << children().size();
}

int CChartView::countData(QQmlListProperty<CSeries> *list)
{
    QList<CSeries*> *data = static_cast<QList<CSeries*> *>(list->data);
    return data->size();
}

CSeries *CChartView::atData(QQmlListProperty<CSeries> *list, int index)
{
    QList<CSeries*> *data = static_cast<QList<CSeries*> *>(list->data);
    return data->at(index);
}

QFont CChartView::getLabelsFont() const { return labelsFont; }

QQmlListProperty<CSeries> CChartView::getListSeries()
{
    return QQmlListProperty<CSeries>(static_cast<QObject *>(this), static_cast<void *>(&series),
                                     &CChartView::countData, &CChartView::atData);
}

void CChartView::setAxis(CAxis *axis)
{
    if(this->axis != axis) {
        if(this->axis != nullptr) delete this->axis; // удаляем предыдущую ось
        this->axis = axis;
        this->axis->setSize(QSizeF(width(), height())); // зададим размеры
        this->axis->setLabelsFont(labelsFont);
        for(auto s : series) s->setAxis(this->axis);
        emit axisChanged();
    }
}

CSeries *CChartView::createSeries(int /*CSeries::Type*/ type, const QString &label, const QString &color)
{
    static QStringList colors = { "#9F4141", "blue", "green", "red", "cyan", "yellow",
                                  "magenta", "darkRed", "darkGreen", "darkBlue", "darkMagenta", "darkCyan", "darkYellow", "darkGray"};

    CSeries *s = nullptr;
    switch (type) {
        case CSeries::LineSeries: {
            auto lineseries = new CLineSeries(this);

            if(!color.isEmpty()) lineseries->setColor(color);    // если задали
            else if(series.size() >= colors.size()) lineseries->setColor(QColor(255 * qrand()/(double)RAND_MAX, 255 * qrand()/(double)RAND_MAX, 255 * qrand()/(double)RAND_MAX));
            else lineseries->setColor(colors.at(series.size()));

            s = lineseries;
            break;
        }
        default: break;
    }

    if(s != nullptr) {
        if(axis == nullptr) s->setSize(QSizeF(width(), height()));
        else s->setAxis(axis);
        s->setLabel(label);
        series.append(s);
        emit seriesChanged();
    }

    return s;
}

void CChartView::removeSeries(int ind)
{
    if(ind < 0 || ind >= series.size()) return;
    series[ind]->deleteLater();
    series.removeAt(ind);
    emit seriesChanged();
}

void CChartView::removeSeries()
{
    for(int i = 0; i < series.size(); ++i) {
        CChartView::removeSeries(i);
        --i;
    }
}

CSeries *CChartView::getSeries(int ind) const
{
    if(ind >= series.size() || ind < 0) {
        Q_ASSERT("CChartView::getSeries. Index < 0 or index >= size");
        return nullptr;
    }
    return series.at(ind);
}

CAxis *CChartView::getAxis() const { return axis; }

QPointF CChartView::mapToPosition(const QPointF &value)
{
    if(axis == nullptr) return QPointF(0, 0);
    return axis->mapToPosition(value);
}

QPointF CChartView::mapToValue(const QPointF &point)
{
    if(axis == nullptr) return QPointF(0, 0);
    return axis->mapToValue(point);
}

void CChartView::clear()
{
    for(auto s : series) s->clear();
}

int CChartView::count()
{
    return series.size();
}

void CChartView::forceAxisRange(bool init)
{
    if(axis == nullptr) return;

    double minX = 1, maxX = 10, minY = 1, maxY = 10;

    switch (series.size()) {
        case 0: break;
        case 1: {
            auto s = series.first();
            auto lineSeries = qobject_cast<CLineSeries *>(s);
            if(!lineSeries) break;

            if(!(lineSeries->getSize() == 0 || (!init && !lineSeries->isEnabled()))) {
                minX = lineSeries->getMinX(); maxX = lineSeries->getMaxX();
                minY = lineSeries->getMinY(); maxY = lineSeries->getMaxY();

                double diff = maxY - minY;
                double offset = 0;
                if(qFuzzyCompare(diff + 1, 1.)) {
                    // одинаковые
                    if(qFuzzyCompare(std::fabs(maxY) + 1, 1.)) offset = 0.1;   // макc число равно 0
                    else offset = std::fabs(maxY) / 20;
                }
                else offset = diff / 20;

                if(axis->getType() == CAxis::LogValueAxisType) {
                    if(minY - offset > 0) minY -= offset;
                    else minY -= minY / 20;
                }
                else minY -= offset;
                maxY += offset;
            }
            break;
        }
        default: {   // > 1
            int n = 0;
            for(auto s : series) {
                auto lineSeries = qobject_cast<CLineSeries *>(s);
                if(!lineSeries) continue;

                if((!init && !lineSeries->isEnabled()) || lineSeries->getSize() == 0) continue;
                if(n == 0) {
                    minX = lineSeries->getMinX(); maxX = lineSeries->getMaxX();
                    minY = lineSeries->getMinY(); maxY = lineSeries->getMaxY();
                }
                else {
                    if(minX > lineSeries->getMinX()) minX = lineSeries->getMinX();
                    if(maxX < lineSeries->getMaxX()) maxX = lineSeries->getMaxX();

                    if(minY > lineSeries->getMinY()) minY = lineSeries->getMinY();
                    if(maxY < lineSeries->getMaxY()) maxY = lineSeries->getMaxY();
                }
                ++n;
            }

            if(n > 0) {
                double diff = maxY - minY;
                double offset = 0;
                if(qFuzzyCompare(diff + 1, 1.)) {
                    // одинаковые
                    if(qFuzzyCompare(std::fabs(maxY) + 1, 1.)) offset = 0.1;   // макc число равно 0
                    else offset = std::fabs(maxY) / 20;
                }
                else offset = diff / 20;

                if(axis->getType() == CAxis::LogValueAxisType) {
                    if(minY - offset > 0) minY -= offset;
                    else minY -= minY / 20;
                }
                else minY -= offset;
                maxY += offset;
            }
        }
            break;
    }

    axis->setMinX(minX); axis->setMaxX(maxX);
    axis->setMinY(minY); axis->setMaxY(maxY);
}

void CChartView::saveChart(const QString &fileName, const QImage &img)
{
    if(fileName.isEmpty()) {
        QString filters = "Bitmap file (*.bmp);; Portable Network Graphics file (*.png);; TIF file (*.tif);; JPEG file (*.jpeg);; All files (*.*)";
        QString defaultFilter("Bitmap file (*.bmp)");

        QString name = QFileDialog::getSaveFileName(0, "Save file", QDir::currentPath(),
                                                    filters, &defaultFilter);
        img.save(name);
    }
    else img.save(fileName);
}

void CChartView::printChart(const QImage &img, const QString &title)
{
    qDebug() << "1 printChart";
    QPrinter printer(QPrinter::HighResolution);

    qDebug() << "2 printChart" << &printer;


    QString docName = title;
    if (docName.isEmpty()) docName = "--";
    qDebug() << "3 printChart" << docName;
    printer.setOrientation(QPrinter::Landscape);
    printer.setResolution(300);
    printer.setOutputFormat(QPrinter::NativeFormat);
    //    printer.setPageMargins(0,0,0,0, QPrinter::Millimeter);
    printer.setPaperSize(QPrinter::A4);
    auto img2 = img.scaled(printer.pageRect().width(), printer.pageRect().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    qDebug() << "4 printChart" << img2;

    QPrintDialog printDialog(&printer);
    printDialog.setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); //@N

    qDebug() << "5 printChart" << &printDialog;

    auto exec = printDialog.exec();
    qDebug() << "6 printChart" << exec << QDialog::Accepted;

    if(exec == QDialog::Accepted) {
        QPainter painter(&printer);
        double xscale = printer.pageRect().width() / double(img2.width());
        double yscale = printer.pageRect().height() / double(img2.height());
        double scale = qMin(xscale, yscale);
        painter.translate(printer.paperRect().x() + printer.pageRect().width() / 2,
                          printer.paperRect().y() + printer.pageRect().height() / 2);
        painter.scale(scale, scale);
        painter.translate(-img2.width() / 2, -img2.height() / 2); // note uses the form width/height! use pix.h/w if random image

        painter.drawImage(QPoint(0,0), img2);
        painter.end();
    }
}

void CChartView::setLabelsFont(const QFont &font)
{
    if(labelsFont != font) {
        labelsFont = font;
        if(axis != nullptr) axis->setLabelsFont(font);
        emit labelsFontChanged();
    }
}

