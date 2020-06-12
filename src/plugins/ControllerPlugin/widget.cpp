#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    _Switch(false)
{
    ui->setupUi(this);
    setWindowTitle("控制器");
    switchIcon(_Switch);
    stateLedIcon(2);
    QObject::connect(ui->smSwitchBtn, SIGNAL(clicked(bool)), this, SLOT(smSwitchBtnClickedSlots()));
    QObject::connect(ui->smSettingBtn, SIGNAL(clicked(bool)), this, SLOT(smSettingBtnClickedSlots()));
    QObject::connect(ui->oswBtn, SIGNAL(clicked(bool)), this, SLOT(sowBtnClickedSlots()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::switchIcon(bool switchbool)
{
    if(switchbool){
        ui->smSwitchBtn->setIcon(QIcon(":/On.ico"));
        stateLedIcon(0);
    }else{
        ui->smSwitchBtn->setIcon(QIcon(":/Off.ico"));
        stateLedIcon(2);
    }
}

void Widget::stateLedIcon(int v)
{
    switch (v) {
    case 0:
        ui->stateLed->setIcon(QIcon(":/ledgreen.ico"));
        break;
    case 1:
        ui->stateLed->setIcon(QIcon(":/ledyellow.ico"));
        break;
    case 2:
        ui->stateLed->setIcon(QIcon(":/ledred.ico"));
        break;
    default:
        ui->stateLed->setIcon(QIcon(":/ledlightblue.ico"));
        break;
    }
    emit sendMsg2Manager(v);
}

void Widget::smSwitchBtnClickedSlots()
{
    if(_Switch) _Switch = false;
    else _Switch = true;
    switchIcon(_Switch);
}

void Widget::smSettingBtnClickedSlots()
{

}

void Widget::sowBtnClickedSlots()
{

}
