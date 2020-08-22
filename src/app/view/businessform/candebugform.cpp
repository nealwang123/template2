#include "candebugform.h"
#include "ui_candebugform.h"

CanDebugForm::CanDebugForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CanDebugForm)
{
    ui->setupUi(this);
}

CanDebugForm::~CanDebugForm()
{
    delete ui;
}
void CanDebugForm::slot_sendCanData(VCI_CAN_OBJ& obj){
    if(ui->checkBox_2->isChecked()){
        QString str="";
        for (int i=0;i<obj.DataLen;i++) {
            str.append(QString("%1 ").arg(obj.Data[i],2,16,QChar('0')));
        }

        ui->textBrowser->append(QString("--> ID:0x%1 DataLen:%2 Time:0x%3 Data:%4").arg(obj.ID,4,16,QChar('0')).toUpper()
                                .arg(obj.DataLen,2,10,QChar('0'))
                                .arg(obj.TimeStamp,8,16,QChar('0')).toUpper()
                                .arg(str.toUpper())
                                );
    }
}
void CanDebugForm::slot_recvCanData(VCI_CAN_OBJ& obj){

    if(ui->checkBox_2->isChecked()&&((!ui->lineEdit_FilterText->text().isEmpty()&&(obj.ID==ui->lineEdit_FilterText->text().toInt(nullptr,16)))||ui->lineEdit_FilterText->text().isEmpty())){
        QString str="";
        for (int i=0;i<obj.DataLen;i++) {
            str.append(QString("%1 ").arg(obj.Data[i],2,16,QChar('0')));
        }
        ui->textBrowser->append(QString("<-- ID:0x%1 DataLen:%2 Time:0x%3 Data:%4").arg(obj.ID,4,16,QChar('0')).toUpper()
                                .arg(obj.DataLen,2,10,QChar('0'))
                                .arg(obj.TimeStamp,8,16,QChar('0')).toUpper()
                                .arg(str.toUpper())
                                );
    }
}

void CanDebugForm::on_pushButton_Clear_released()
{
    ui->textBrowser->clear();
}
