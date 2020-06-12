#include "psbrokermonitor.h"

PSBrokerMonitor::PSBrokerMonitor(int brokerType,int exitFlage):m_brokerType(brokerType),m_exitFlage(exitFlage)
{
    qDebug()<<"PSBrokerMonitor::PSBrokerMonitor()"<<QThread::currentThread();
}
void PSBrokerMonitor::run(){
    qDebug()<<"PSBrokerMonitor::run()"<<QThread::currentThread();
    //启动监控端
    zmq::context_t context (1);
    //  Socket to  server
    zmq::socket_t listen (context, ZMQ_SUB);
//    zmq::socket_t listen (context, ZMQ_XSUB);
    listen.bind("tcp://*:7777");
    zmq::socket_t broker (context, ZMQ_PUB);
//    zmq::socket_t broker (context, ZMQ_XPUB);
    broker.bind("tcp://*:7888");
    // 订阅所有消息
    listen.setsockopt(ZMQ_SUBSCRIBE, "", 0);


    if(this->m_brokerType==0){//自定义,支持多帧消息
        zmq::proxy(static_cast<void*>(listen),static_cast<void*>(broker),nullptr);
    }else if(this->m_brokerType==1){//自定义,支持多帧消息
        while (this->m_exitFlage) {
            while (1) {
                zmq::message_t message;
                int more;
                size_t more_size = sizeof (more);

                //  Process all parts of the message
                listen.recv(&message);
                listen.getsockopt( ZMQ_RCVMORE, &more, &more_size);
                broker.send(message, more? ZMQ_SNDMORE: 0);
                if (!more)
                    break;      //  Last message part
            }
        }
    }

    qDebug()<<"PSBrokerMonitor::run() 无法运行到的地方！";

}
int PSBrokerMonitor::setExitFlage(int exit){
    this->m_exitFlage=exit;
}
int PSBrokerMonitor::setType(QString type){
    if(type=="TYPE0"){
        this->m_brokerType=0;
    }else if(type=="TYPE1"){
        this->m_brokerType=1;
    }

}
