#ifndef CANDATAMODELFACTORY_H
#define CANDATAMODELFACTORY_H
#include"candatamodel.h"
#include"abstractdatamodelfactory.h"

class CANDataModelFactory : public AbstractDataModelFactory
{
public:
    CANDataModelFactory();
    ~CANDataModelFactory(){}//基类写为虚析构函数
    virtual AbstractDataModel* createDeviceMethod();
};

#endif // CANDATAMODELFACTORY_H
