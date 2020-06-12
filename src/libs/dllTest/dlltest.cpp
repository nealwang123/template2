#include "dlltest.h"


DllTest::DllTest()
{
    qDebug()<<"我是一个测试库";

    QByteArray bytes("abcd");
    QString str(bytes);

    QString param = QString::fromLocal8Bit(bytes.data(),bytes.size()) ;
    qDebug()<<param;
}
