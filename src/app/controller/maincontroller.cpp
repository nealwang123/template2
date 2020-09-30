#include "maincontroller.h"
//QScopedPointer<MainController> MainController::self;
//MainController *MainController::Instance()
//{
//    if (self.isNull()) {
//        QMutex mutex;
//        QMutexLocker locker(&mutex);
//        if (self.isNull()) {
//            self.reset(new MainController);
//        }
//    }

//    return self.data();
//}
MainController::MainController(QObject *parent) : QObject(parent)
{

    /*****************can设备*****************/
//    m_canDeviceManager=new CanDeviceManager();
//    m_CanDeviceFactory=new CANDeviceFactory();
//    //读取配置，目前从sqlite数据库
//    m_canDeviceManager->readConfig();
//    //根据工厂方法创建设备对象
//    m_canDeviceManager->InitDeviceFactory(m_CanDeviceFactory);

    /*******************can DataModel***********************/
    //创建工厂
    m_CanDataModelFactory=new CANDataModelFactory();
    //获取对象
    m_CanDataModel=m_CanDataModelFactory->createDeviceMethod();

    /************初始化控制器连接，绑定试图对象，响应界面控制***************/
    initConnect();






}
MainController::~MainController(){

}
SocketFormMain & MainController::getSocketFormMain(){
    return socketformmain;
}
MainForm& MainController::getMainForm(){
    return frm;
}
UDSForm& MainController::getUDSForm(){
    return udsfrm;
}
void MainController::closeAll(){
    udsfrm.closeAll();
}
void MainController::freeDevice(){
    //关闭candata
    m_CanDataModel->stopDeviceProcess();//关闭两个线程
    QThread::msleep(100);
    m_canDeviceManager->stopProcess();
    //反初始化设备
    m_canDeviceManager->UnInitDevice();


}
void MainController::initDevice(){
    //启动candata
    m_CanDataModel->startDeviceProcess();//开启两个线程

//    //初始化设备
//    m_canDeviceManager->InitDevice();
//    //启动can处理线程
//    m_canDeviceManager->startProcess();



}

void MainController::initConnect(){
    connect(&frm,&MainForm::unInitDevice,this,&MainController::freeDevice);
    connect(&frm,&MainForm::initDevice,this,&MainController::initDevice);
    connect(m_CanDataModel,SIGNAL(sig_msgToPC(QString )),&frm,SLOT(slot_msgToPC(QString )),Qt::QueuedConnection);
    connect(&frm,SIGNAL(sig_msgToRadar(QString)),m_CanDataModel,SLOT(slot_msgToRadar(QString)),Qt::QueuedConnection);
    connect(m_CanDataModel,SIGNAL(sig_stateInfo(QString )),&frm,SLOT(slot_stateInfoDataModel(QString )),Qt::QueuedConnection);

}
