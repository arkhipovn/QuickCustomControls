#include "qccutils.h"
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QLocale>
#include <QPainter>
#include <QDateTime>

QCCUtils::QCCUtils(QObject *parent) : QObject(parent)
{

}

void QCCUtils::saveChart(const QString &fileName, const QImage &img)
{
    if(fileName.isEmpty()) {
        QString filters = "Bitmap file (*.bmp);; Portable Network Graphics file (*.png);; TIF file (*.tif);; JPEG file (*.jpeg);; All files (*.*)";
        QString defaultFilter("Bitmap file (*.bmp)");

        QString name = QFileDialog::getSaveFileName(0, tr("Save file"), QDir::currentPath(),
                                                    filters, &defaultFilter);
        img.save(name);
    }
    else img.save(fileName);
}

void QCCUtils::printChart(const QImage &img, const QString &title)
{
    QPrinter printer(QPrinter::HighResolution);

    QString docName = title;
    if (docName.isEmpty()) docName = "--";

    printer.setOrientation(QPrinter::Landscape);
    printer.setResolution(300);
    printer.setOutputFormat(QPrinter::NativeFormat);
    //    printer.setPageMargins(0,0,0,0, QPrinter::Millimeter);
    printer.setPaperSize(QPrinter::A4);
    auto img2 = img.scaled(printer.pageRect().width(), printer.pageRect().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);


    QPrintDialog printDialog(&printer);
    printDialog.setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom)); //@N

    auto exec = printDialog.exec();

    if(exec == QDialog::Accepted) {
        QPainter painter(&printer);
        double xscale = printer.pageRect().width() / double(img2.width());
        double yscale = printer.pageRect().height() / double(img2.height());
        double scale = qMin(xscale, yscale);
        painter.translate(printer.paperRect().x() + printer.pageRect().width() / 2,
                          printer.paperRect().y() + printer.pageRect().height() / 2);
        painter.scale(scale, scale);
        painter.translate(-img2.width() / 2, -img2.height() / 2);

        painter.drawImage(QPoint(0,0), img2);
        painter.end();
    }
}

QObject *QCCUtils::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    //    static QCCUtils utils;
    //    return &utils;
    return new QCCUtils(engine);
}

QDateTime QCCUtils::dateTimeFromQml(const QDateTime &qmlTime, Qt::TimeSpec timeSpec, int offsetFromUtc)
{
    QDateTime dt(qmlTime);

    switch (timeSpec) {
        case Qt::UTC:
            dt.setTimeSpec(Qt::UTC);
            break;
        case Qt::TimeZone:
        case Qt::OffsetFromUTC:
            dt = dt.addSecs(dt.toLocalTime().offsetFromUtc() - offsetFromUtc);
            break;
        case Qt::LocalTime:
            dt.setTimeSpec(Qt::LocalTime);
            break;
        default:
            break;
    }

    return dt;
}

QDateTime QCCUtils::qmlDateTime(const QDateTime &dt, Qt::TimeSpec timeSpec, int offsetFromUtc)
{
    switch (timeSpec) {
        case Qt::UTC:
            return dt.addSecs(-dt.toLocalTime().offsetFromUtc());
            //            return dt.addSecs(-dt.toLocalTime().offsetFromUtc() + timeZone.offsetFromUtc(dt));
        case Qt::TimeZone:
        case Qt::OffsetFromUTC:
            return dt.addSecs(-dt.toLocalTime().offsetFromUtc() + offsetFromUtc);
        case Qt::LocalTime: return dt.toLocalTime();
        default:
            break;
    }

    return dt;
}
