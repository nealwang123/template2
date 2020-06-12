#include "psbrokerwidget.h"
#include "ui_psbrokerwidget.h"

PSBrokerWidget::PSBrokerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PSBrokerWidget)
{
    ui->setupUi(this);
    brokerthread=new PSBrokerMonitor();


}

PSBrokerWidget::~PSBrokerWidget()
{
    delete ui;
}

void PSBrokerWidget::on_pushButton_start_clicked()
{
    if(ui->pushButton_start->text()=="start"){
        brokerthread->setType(ui->lineEdit->text());
        brokerthread->setExitFlage(1);
        brokerthread->start();
        ui->textBrowser->append("Broker Open!Type:"+ui->lineEdit->text());
        ui->pushButton_start->setText("stop");
    }else{
        brokerthread->setExitFlage(0);
        brokerthread->terminate();
        ui->textBrowser->append("Broker Closed!Type:"+ui->lineEdit->text());
        ui->pushButton_start->setText("start");
    }
}
