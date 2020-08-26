#include "helloworldwidget.h"
#include "ui_helloworldwidget.h"

#include <QDebug>
HelloWorldWidget::HelloWorldWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelloWorldWidget)
{
    ui->setupUi(this);
    setWindowTitle("HelloWorld");
}

HelloWorldWidget::~HelloWorldWidget()
{
    delete ui;
}

void HelloWorldWidget::slot_recMsgfromManager(int value)
{
    switch (value) {
    case 0:
        ui->verticalSlider->setValue(0);
        break;
    case 1:
        ui->verticalSlider->setValue(50);
        break;
    case 2:
        ui->verticalSlider->setValue(90);
        break;
    default:
        break;
    }

}
