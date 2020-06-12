#include "mydlltest.h"


MyDllTest::MyDllTest()
{
    qDebug()<<"我是一个测试库 MyDllTest";

    QByteArray bytes("MyDllTest");
    QString str(bytes);

    QString param = QString::fromLocal8Bit(bytes.data(),bytes.size()) ;
    qDebug()<<param;
}
