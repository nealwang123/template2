#include "candatamodel.h"

CANDataModel::CANDataModel()
{
    context =zmq::context_t(1);
    publisher = zmq::socket_t(context, ZMQ_PUB);
    publisher.connect("tcp://localhost:7777");
}
//线程1 发送 zeromq消息 主题TORADAR
void CANDataModel::processHandle(){

//    while(m_exitStateThread1){
//        qDebug()<<"CANDataModel::processHandle()结束线程1"<<m_exitStateThread1;

//        QThread::msleep(1);
//    }
    qDebug()<<"CANDataModel::processHandle()结束线程1";
}//处理线程
//线程2
void CANDataModel::star(){
    qDebug()<<"QtConcurrent::run(this,&CANDataModel::stateThread);//多线程执行死循环启动:";

    subscriber = zmq::socket_t(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:7888");
    //  Subscribe to zipcode, default is NYC, 10001
    const char *filter = "TOPC ";//s
//    const char *filter = "CANSTATE ";//s
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));
    m_exitStateThread2=1;
    QtConcurrent::run(this,&CANDataModel::stateThread);//多线程执行死循环启动，可以带参数，具体格式可以查阅网上其它资料
}
//线程2 接收zeromq消息 主题TOPC
void CANDataModel::stateThread(){
    while(m_exitStateThread2){
        std::string toPCMsg = s_recv(subscriber);
//        //  Read identify
//        std::string identify = s_recv(subscriber);
//        //  Read message contents
//        std::string contents = s_recv(subscriber);
//        std::cout << "[" << address << "] "<< "[" << identify << "] " << contents << std::endl;
        std::cout <<m_exitStateThread2<< "[topc:]"<<toPCMsg<< std::endl;
        emit sig_msgToPC(QString::fromStdString(toPCMsg));

    }
    qDebug()<<"CANDataModel::stateThread()结束线程2";
}

void CANDataModel::handleMsgToRadar(QString str){
//    qDebug()<<"after send3!!!!!";
    qDebug()<<"指令给雷达："<<str;
    s_send(publisher,str.toStdString());
//    qDebug()<<"after send4!!!!!";
}
