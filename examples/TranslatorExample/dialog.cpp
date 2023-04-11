#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    init();

    QObject::connect(ui->radioButton_en, &QRadioButton::toggled, [this](){
        if(ui->radioButton_en->isChecked()) CTranslator::getInstance()->setCurrentLanguage(QLocale::English);
        else CTranslator::getInstance()->setCurrentLanguage(QLocale::Russian);
    });
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::init()
{
    ui->label->setText(tr("English text"));
}

void Dialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) init();
}
