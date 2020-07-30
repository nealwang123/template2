#include "initialphaseform.h"
#include "ui_initialphaseform.h"
#include"udsform.h"
InitialPhaseForm::InitialPhaseForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InitialPhaseForm)
{
    ui->setupUi(this);
    //
    QUIHelper::readFileList("","initialphasecansend.txt",m_list_canSendCommand,m_list_canSendDiscrib);
    ui->cBoxcansend->clear();
    ui->cBoxcansend->addItems(m_list_canSendCommand);
    ui->cBoxCanSendDiscrib->addItems(m_list_canSendDiscrib);
}

InitialPhaseForm::~InitialPhaseForm()
{
    delete ui;
}
void InitialPhaseForm::displayWorkmode(QString str){
    ui->label_5->setText(str);
}
void InitialPhaseForm::on_cBoxcansend_activated(int index)
{
    ui->cBoxCanSendDiscrib->setCurrentIndex(index);
    m_commandIndex=ui->cBoxcansend->currentIndex();
    m_selfSendStr=ui->cBoxcansend->currentText();
    ui->cBoxCanSendDiscrib->setCurrentIndex(index);
    QByteArray array=QUIHelper::hexStrToByteArray(m_selfSendStr);
    //qDebug()<<"m_selfSendStr:"<<m_selfSendStr<<"array.size()"<<array.size();
    ECANStatus ret=UDS::Instance()->NormalSendAndReceive(CANApi::SEND_CAN_ID_Self1,(byte*)array.data(),array.size()); //ptr就是主窗口的指针，可以用来操作主窗口
    if(ret==_STATUS_OK){
        QPixmap pixmap(QString(":/imageTest/buttongreen.png"));
        ui->label_state->setFixedSize(64,64);
        QPixmap fitpixmap = pixmap.scaled(64,64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
        ui->label_state->setPixmap(fitpixmap);
        ui->label_state->setScaledContents(true);
    }else if(ret==_STATUS_ERR){
        QPixmap pixmap(QString(":/imageTest/buttonred.png"));
        ui->label_state->setFixedSize(64,64);
        QPixmap fitpixmap = pixmap.scaled(64,64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
        ui->label_state->setPixmap(fitpixmap);
        ui->label_state->setScaledContents(true);
    }else{
        QPixmap pixmap(QString(":/imageTest/buttonyellow.png"));
        ui->label_state->setFixedSize(64,64);
        QPixmap fitpixmap = pixmap.scaled(64,64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
        ui->label_state->setPixmap(fitpixmap);
        ui->label_state->setScaledContents(true);
    }


}

void InitialPhaseForm::on_cBoxCanSendDiscrib_activated(int index)
{
    ui->cBoxcansend->setCurrentIndex(index);
    qDebug()<<"on_cBoxCanSendDiscrib_activated";

}
