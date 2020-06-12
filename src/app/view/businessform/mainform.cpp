#include "mainform.h"
#include "ui_mainform.h"

MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
    //      initFile();
    //      qDebug()<<"dbc loaded...";
    //    DllTest d;
    //    Aggregation *agg;

    //    frm=new frmMain();
    //    frm->show();

    initForm();
    commandTimer=new QTimer(this);
    connect(commandTimer, SIGNAL(timeout()), this, SLOT(slot_commandTimer()));
    context =zmq::context_t(1);
//    zmq::socket_t subscriber (context, ZMQ_SUB);
    publisher = zmq::socket_t(context, ZMQ_PUB);
    publisher.connect("tcp://localhost:7777");
}
MainForm::~MainForm()
{
    delete ui;
}
void MainForm::initForm(){

//   DemarcationDelegate* d1 = new DemarcationDelegate(this);
//   DemarcationDelegate* d2 = new DemarcationDelegate(this);
//   DemarcationDelegate* d3 = new DemarcationDelegate(this);
    demarcationForm = new DemarcationDelegate;
    canConfigForm = new CANConfigDelegate;
    //stackedWidget默认有多个窗口
    ui->stackedWidget->addWidget(demarcationForm);
    ui->stackedWidget->addWidget(canConfigForm);
    qDebug()<<"ui->stackedWidget->currentIndex()::"<<ui->stackedWidget->currentIndex()<<ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(0);
}
void MainForm::initFile(){
    dbc=new DBCReader;
    dbc->loadDBCFile("./radar状态交互协议.dbc");
    qDebug()<<"loadDBCFile end";
    dbc->debugDBCStruct(" ");
//    dbc.loadDBCFile("./test.dbc");
//    dbc->debugDBCStruct("");

}



void MainForm::changeStyle(const QString &qssFile)
{
    QFile file(qssFile);

    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QUIConfig::PanelColor = qss.mid(20, 7);
        QUIConfig::TextColor = qss.mid(49, 7);
        file.close();
    }

    App::StyleName = qssFile;
    App::writeConfig();
    emit changeStyle();

//    this->changeBtnStyle();
}
void MainForm::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void MainForm::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainForm::on_pushButton_4_clicked()
{
    if(commandTimer->isActive()){
        commandTimer->stop();
    }
    commandTimer->start(500);
}
void MainForm::slot_commandTimer(){
//    qDebug()<<"void MainForm::slot_commandTimer()";
    quint8 Data[8]={0x01,0x02,0x01,0x02,0x01,0x02,0x01,0x02};
    static int count =0;
    QString data;
    data.clear();
    QString temp;
    for(int j=0;j<8;j++){
        temp=QString("%1").arg(Data[j],2,16,QChar('0'));
        data.append(temp);
        data.append(" ");
    }
    QString header=QString("TORADAR %1 %2 %3").arg(4).arg(0).arg(0);
    QString senddata=QString(" %1 %2 %3 %4 %5")
            .arg(0,8,16,QChar('0'))
            .arg(0x600,8,16,QChar('0'))
            .arg(8,4,10,QChar('0'))
            .arg(data)
            .arg(count++,4,10,QChar('0'));
    header=header+senddata;

    s_send(publisher,header.toStdString());
    qDebug()<<header;
}
//断开设备
void MainForm::on_pushButton_5_clicked()
{
    emit(unInitDevice());
}

void MainForm::on_pushButton_6_clicked()
{
    emit(initDevice());
}
//开始标定，
void MainForm::on_pushButton_3_clicked()
{
    qDebug()<<"void MainForm::on_pushButton_3_clicked()";
    QStringList deviceinfo=canConfigForm->getModelData(0);
    qDebug()<<"deviceinfo"<<deviceinfo<<deviceinfo.length();
    demarcationForm->startDemarcation(deviceinfo);
}
