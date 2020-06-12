#include "AbstractDeviceManager.h"

AbstractDeviceManager::AbstractDeviceManager(QObject *parent) : QObject(parent)
{
    qDebug()<<"AbstractDeviceManager construct!"<<QThread::currentThread();

}

int AbstractDeviceManager::startProcess(){//遍历设备列表启动其线程处理
    qDebug()<<"AbstractDeviceManager::startProcess()...m_device_list.length()"<<m_device_list.length();
    //遍历设备对象 list
    for(int i=0; i<m_device_list.length();i++) {
//        qDebug()<<"AbstractDeviceManager::startProcess()"<<m_device_list.length();
        m_device_list.at(i)->startDeviceProcess();
    }
}
int AbstractDeviceManager::stopProcess(){//遍历设备列表关闭其线程处理
    qDebug()<<"AbstractDeviceManager::stopProcess()... m_device_list.length():"<<m_device_list.length();
    //遍历设备对象 list
    for(int i=0; i<m_device_list.length();i++) {
        m_device_list.at(i)->stopDeviceProcess();

    }
}
int AbstractDeviceManager::readConfig(){
    if(this->initSql()!=0){//初始化数据库
        qDebug()<<"initSql() failed!";
        return -1;
    }
    if(this->unInitSql()!=0){//关闭数据库连接
        qDebug()<<"unInitSql() failed!";
        return -1;
    }
}

int AbstractDeviceManager::initSql(){//初始化数据库
    qDebug()<<"initSql m_DeviceManagerName="<<m_DeviceManagerName;
    //判断链接是否存在
    if (QSqlDatabase::contains(m_DeviceManagerName)){
        _db = QSqlDatabase::database(m_DeviceManagerName);
    }else{
        //建立和sqlite数据的连接
        _db = QSqlDatabase::addDatabase("QSQLITE",m_DeviceManagerName);
        //设置数据库文件的名字
        _db.setDatabaseName(QUIHelper::appPath() + App::LocalDBName);
    }
   QString path=QUIHelper::appPath() + App::LocalDBName;
   bool ret;
   if(QFile::exists(path))
   {
       // 存在则打开
      ret=_db.open();
   }else{//报错警告，或向上抛出异常
        /*
        //       // 不存在打开并创建数据库表 补充：SQLite是基于文件的数据库，当打开时如果数据库文件不存在将自动创建一个
        //       ret=_db.open();
        //       QSqlQuery query;
        //       // set为sqlite关键字，不能使用
        //       bool bRet = query.exec("create table init(" \
        //                              "no   INT    PRIMARY KEY NOT NULL,"\
        //                              "name TEXT   NOT NULL,"\
        //                              "content TEXT,"\
        //                              "description TEXT"\
        //                              ");");
        //       if(!bRet) {
        //           _db.close();
        //           QFile::remove(path);
        //       }else{
        //           // 此处演示了三种不同insert插入方法
        //           query.exec("insert into init values" \
        //                      "(1, '启动界面背景', 'images/background.jpg', '请使用1920*1080的图片，图片格式可以为png、jpg。');");
        //           query.exec("insert into init       " \
        //                      "(no,name,content,description) values(2, '欢迎视频', '','进入启动界面后，循环播放的视频,必须为mp4格式，如果为空则循环播放欢迎音乐');");
        //           query.exec("insert into init       " \
        //                      "(no,name,description) values (3, '欢迎音乐','进去启动界面后，循环播放的音频文件，可选择mp3,wav格式');");
        //       }
        */

   }
   if(ret == false){
       qDebug() <<this->m_DeviceManagerName<< ":连接数据失败！";
       return false;
   }
   qDebug() <<this->m_DeviceManagerName << ":连接数据库成功";

   //查询设备对应表
   QString tableName = this->m_DeviceManagerName;
//   countName = "DeviceLogID";

   //查询最大记录编号,防止重复
   QString sql = QString("select * from %2").arg(tableName);
   QSqlQuery query(_db);
   query.exec(sql);
   int linecount=0;
   QStringList list;

   while(query.next())
   {
        list.clear();
        qDebug()<<query.value(1);
        qDebug()<<"数据库一条记录：";
        for(int i=0;i<query.value(1).toInt();i++){
            qDebug()<<query.value(i);
            list.append(query.value(i).toString());
        }
        m_listMap.insert(linecount,list);
        linecount++;
   }
    qDebug()<<"读取完毕,record count:"<<linecount;
    return 0;
}

int AbstractDeviceManager::unInitSql(){//设置设备相关属性

    return 0;
}
int AbstractDeviceManager::setAttrubute(){//设置设备相关属性
    //遍历设备对象 list
    for(int i=0; i<m_device_list.length();i++) {
        qDebug()<<"AbstractDeviceManager::setAttrubute()"<<m_device_list.length();
        m_device_list.at(i)->setDeviceAttrubute(m_listMap.find(i).value());
    }
}
int AbstractDeviceManager::InitDevice(){
    qDebug()<<"AbstractDeviceManager::InitDevice";
    if(this->setAttrubute()!=0){//根据数据库内容设置设备对象属性
        qDebug()<<"setAttrubute() failed!";
        return -1;
    }

    if(this->Open()!=0){//打开设备
        qDebug()<<"Open() failed!";
        return -1;
    }

    qDebug()<<"InitDevice() ok!";
    return 0;
}
//初始化工厂,并通过工厂创建对象
int AbstractDeviceManager::InitDeviceFactory(AbstractDeviceFactory* device_factory){

    this->m_device_factory=device_factory;
    if(!this->m_device_list.isEmpty()){
        this->m_device_list.clear();
    }
    //可能创建多个对象
    QMap<int, QStringList>::const_iterator iter;
    for (iter = m_listMap.constBegin(); iter != m_listMap.constEnd(); ++iter) {
        qDebug() << "添加设备对象参数 key:" << iter.key()<< "value:" << iter.value() << "\n";
        this->m_device_list.append(this->m_device_factory->createDeviceMethod());
    }
}
//todo:
int AbstractDeviceManager::UnInitDevice(){
    if(Close()!=0){
        qDebug()<<"Close() failed!";
        return -1;
    }
    return 0;
}

//打开设备，无参数，目前对数据库记录所有设备进行操作
int AbstractDeviceManager::Open(){
    qDebug()<<"AbstractDeviceManager::Open()"<<this->objectName()<<m_DeviceManagerName;
    int count=0;
    QMap<int,QStringList>::const_iterator iter;
    for(iter=m_listMap.constBegin();iter!=m_listMap.constEnd();++iter){
        qDebug()<<"AbstractDeviceManager::Open() iter index:"<<count++<<"m_device_list.length():"<<m_device_list.length();
        if(iter.key()<m_device_list.length()){
            QStringList list=iter.value();
            m_device_list.at(iter.key())->setDeviceAttrubute(list);
            int ret=m_device_list.at(iter.key())->open();
            qDebug()<<"m_device_list.at(iter.key())->open() ret :"<<ret;
        }
    }
    return 0;
}


int AbstractDeviceManager::Close(){//关闭设备
    int count=0;
    QMap<int,QStringList>::const_iterator iter;
    for(iter=m_listMap.constBegin();iter!=m_listMap.constEnd();++iter){
        qDebug()<<"CanDeviceManager::Close() iter index:"<<count++<<"m_device_list.length():"<<m_device_list.length();
        if(iter.key()<m_device_list.length()){
            m_device_list.at(iter.key())->close();
        }
    }
    return 0;
}
