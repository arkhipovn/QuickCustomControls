#include "dialog.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CTranslator::getInstance()->init(QLocale::English, &a);
    CTranslator::getInstance()->appendLanguage(QLocale::Russian, ":/example_ru.qm");

    Dialog w;
    w.show();
    return a.exec();
}
