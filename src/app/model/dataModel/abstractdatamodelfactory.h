#ifndef ABSTRACTDATAMODELFACTORY_H
#define ABSTRACTDATAMODELFACTORY_H

#include <QObject>
#include "abstractdatamodel.h"
class AbstractDataModelFactory : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDataModelFactory(QObject *parent = nullptr);
    ~AbstractDataModelFactory(){}//基类写为虚析构函数
    virtual AbstractDataModel* createDeviceMethod()=0;

signals:

public slots:
};

#endif // ABSTRACTDATAMODELFACTORY_H
