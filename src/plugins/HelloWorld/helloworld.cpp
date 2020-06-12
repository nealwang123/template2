#include "helloworld.h" 
#include "helloworldwidget.h"

#include <QDebug>

 HelloWorld::HelloWorld()
{
    m_pShareMemory=NULL;
    m_readShareMemory_timer=new QTimer(this);
    QObject::connect(m_readShareMemory_timer,SIGNAL(timeout()),this,SLOT(slot_readShareMemory()),Qt::QueuedConnection);
    m_readShareMemory_timer->start(70);
}

QString HelloWorld::name() const
{
    return QString("HelloWorld");
    //return tr("HelloWorld");
}

QString HelloWorld::iconPath() const
{
return "://HelloWorld.png";
}

QWidget *HelloWorld::widget(QWidget *parent) const
{
    HelloWorldWidget *widget=new HelloWorldWidget(parent);
    connect(this,SIGNAL(sig_recMsgfromManager(int)),widget,SLOT(slot_recMsgfromManager(int)),Qt::QueuedConnection);
    return widget;
}

AUTOROAD_WIDGETTYPE HelloWorld::autoroad_Widget() const
{
    return AUTOROAD_WIDGETTYPE::DOCKWIDGET;
}

Qt::DockWidgetArea HelloWorld::autoroad_DockWidgetArea() const
{
    return Qt::LeftDockWidgetArea;
}
void HelloWorld::recMsgfromManager(PluginMetaData metaData)
{
    QString srcPlugin=metaData.src;
    if(metaData.map.contains(srcPlugin))
    {
        QVariant value=metaData.map.find(srcPlugin).value();
        emit sig_recMsgfromManager(value.toInt());
    }
}

void HelloWorld::slot_writeShareMemory()
{
    if(NULL==m_pShareMemory)
    {
        m_pShareMemory=new QSharedMemory();
        QString strNativeKey=QString("RKWebViewModelSM");
        m_pShareMemory->setNativeKey(strNativeKey);
    }
    //判断对象是否attach内存，解除
    if(m_pShareMemory->isAttached())
    {
        m_pShareMemory->detach();
    }
    //写入共享内存数据
    QString qstrId=QString("HelloWorld::slot_writeShareMemory");
    char *from=qstrId.toLocal8Bit().data();
    m_pShareMemory->create(qstrId.toLocal8Bit().size());
    m_pShareMemory->lock();

    char *to=(char*)m_pShareMemory->data();
    memcpy(to,from,qstrId.toLocal8Bit().size());
    m_pShareMemory->unlock();

}

void HelloWorld::slot_readShareMemory()
{
    if(NULL==m_pShareMemory)
    {
        m_pShareMemory=new QSharedMemory();
        QString strNativeKey=QString("RKWebViewModelSM");
        //m_pShareMemory->setKey(strNativeKey);//??
        m_pShareMemory->setNativeKey(strNativeKey);
    }
    //sm对象attach到共享内存
    if(!m_pShareMemory->attach(QSharedMemory::ReadOnly))
    {
        return;
    }
    //从共享内存中读取数据
    m_pShareMemory->lock();
    QByteArray buffer;
    buffer.setRawData((char*)m_pShareMemory->data(),m_pShareMemory->size());
    QString qstrHouseId=QString(buffer);
    qDebug()<<"HelloWorld"<<qstrHouseId;
    //qDebug()<<"HelloWorld====="<<m_pShareMemory->size();
    m_pShareMemory->unlock();
    //解除attach
    m_pShareMemory->detach();
}
