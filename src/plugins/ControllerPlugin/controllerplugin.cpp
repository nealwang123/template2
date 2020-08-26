#include "controllerplugin.h"
#include "widget.h"
#include <QDebug>
#include <QDateTime>

ControllerPlugin::ControllerPlugin()
{
    m_pShareMemory=NULL;
    m_readShareMemory_timer=new QTimer(this);
    QObject::connect(m_readShareMemory_timer,SIGNAL(timeout()),this,SLOT(slot_writeShareMemory())/*slot_readShareMemory())*/,Qt::QueuedConnection);
    m_readShareMemory_timer->start(50);
}

QString ControllerPlugin::name() const
{
    return QString("控制器");
}
QString ControllerPlugin::iconPath() const
{
    return ":/Controller.ico";
}
QWidget *ControllerPlugin::widget(QWidget *parent) const
{
    Widget *widget=new Widget(parent);
    connect(widget,SIGNAL(sendMsg2Manager(int)),this,SLOT(slot_sendMsg2Manager(int)),Qt::QueuedConnection);
    return widget;
}
AUTOROAD_WIDGETTYPE ControllerPlugin::autoroad_Widget() const
{
    return AUTOROAD_WIDGETTYPE::MDIAREA;
}
Qt::DockWidgetArea ControllerPlugin::autoroad_DockWidgetArea() const
{
    return Qt::LeftDockWidgetArea;
}
void ControllerPlugin::recMsgfromManager(PluginMetaData metadate)
{
//    if(metedate.object)
//    {
//        qDebug()<<"metedata.object:"<<metedate.object->objectName();
    //    }
}

void ControllerPlugin::slot_sendMsg2Manager(int value)
{
    PluginMetaData metadate;
    metadate.src="ControllerPlugin";
    metadate.dest="HelloWorld";
    metadate.type=1;
    metadate.map.insert("ControllerPlugin",value);

    emit(this->sendMsg2Manager(metadate));
}

void ControllerPlugin::slot_writeShareMemory()
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
    m_pShareMemory->lock();
    QString currentTime=QDateTime::currentDateTime().toString("hh-MM-ss::zzz");
    QString qstrId=QString("ControllerPlugin::")+currentTime;
    char *from=qstrId.toLocal8Bit().data();
    m_pShareMemory->create(qstrId.toLocal8Bit().size());

    char *to=(char*)m_pShareMemory->data();
    memcpy(to,from,qstrId.toLocal8Bit().size());
    //qDebug()<<"ControllerPlugin====="<<m_pShareMemory->size();
    m_pShareMemory->unlock();
}

void ControllerPlugin::slot_readShareMemory()
{
    if(NULL==m_pShareMemory)
    {
        m_pShareMemory=new QSharedMemory();
        QString strNativeKey=QString("RKWebViewModelSM");
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
    qDebug()<<"ControllerPlugin"<<qstrHouseId;
    m_pShareMemory->unlock();
    //解除attach
//    m_pShareMemory->detach();//释放共享内存空间
}
