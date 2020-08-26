#ifndef CANDATAMODEL_H
#define CANDATAMODEL_H
#include"abstractdatamodel.h"
#include"zhelpers.hpp"
class CANDataModel : public AbstractDataModel
{
public:
    CANDataModel();

    //线程1
    virtual void processHandle();//处理线程
    virtual void handleMsgToRadar(QString str);
    //线程2
    virtual void star();
    virtual void stateThread();

signals:

public slots:

private:
    zmq::context_t context;
    zmq::socket_t subscriber;
    zmq::context_t context2;
    zmq::socket_t publisher;
};

#endif // CANDATAMODEL_H
