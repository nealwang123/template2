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
    m_canDeviceManager=new CanDeviceManager();
    m_CanDeviceFactory=new CANDeviceFactory();
    //读取配置，目前从sqlite数据库
    m_canDeviceManager->readConfig();
    //根据工厂方法创建设备对象
    m_canDeviceManager->InitDeviceFactory(m_CanDeviceFactory);

    initConnect();

}
MainController::~MainController(){

}
MainForm& MainController::getMainForm(){
    return frm;
}
void MainController::freeDevice(){
    m_canDeviceManager->stopProcess();
    //反初始化设备
    m_canDeviceManager->UnInitDevice();
}
void MainController::initDevice(){
    //初始化设备
    m_canDeviceManager->InitDevice();
    //启动can处理线程
    m_canDeviceManager->startProcess();
}

void MainController::initConnect(){
    connect(&frm,&MainForm::unInitDevice,this,&MainController::freeDevice);
    connect(&frm,&MainForm::initDevice,this,&MainController::initDevice);

}
