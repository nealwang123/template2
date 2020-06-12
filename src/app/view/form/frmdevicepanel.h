#ifndef FRMDEVICEPANEL_H
#define FRMDEVICEPANEL_H

#include <QWidget>

#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtMultimedia>
#else
#include <QSound>
#endif

class QMenu;

namespace Ui
{
    class frmDevicePanel;
}

class frmDevicePanel : public QWidget
{
    Q_OBJECT

public:
    explicit frmDevicePanel(QWidget *parent = 0);
    ~frmDevicePanel();

    //设置设备信息
    void setDeviceID(int deviceID);
    void setDeviceName(QString deviceName);
    void setDeviceAddr(int deviceAddr);

    void setGroupID(int groupID);
    void setGroupName(QString groupName);
    void setPortID(int portID);
    void setPortName(QString portName);

    void setTempUpper(double tempUpper);
    void setTempLimit(double tempLimit);
    void setTempValue(double tempValue);
    void setTempSign(QString tempSign);

    void setDampUpper(double dampUpper);
    void setDampLimit(double dampLimit);
    void setDampValue(double dampValue);
    void setDampSign(QString dampSign);

    void setSaveInterval(int saveInterval);
    void setDeviceType(QString deviceType);
    void setDeviceSound(QString deviceSound);

public:
    int getPowerValue()const
    {
        return this->powerValue;
    }
    int getCountAll()const
    {
        return this->countAll;
    }
    int getCountNoRead()const
    {
        return this->countNoRead;
    }

    int getDeviceID()const
    {
        return this->deviceID;
    }
    QString getDeviceName()const
    {
        return this->deviceName;
    }
    int getDeviceAddr()const
    {
        return this->deviceAddr;
    }

    int getGroupID()const
    {
        return this->groupID;
    }
    QString getGroupName()const
    {
        return this->groupName;
    }
    int getPortID()const
    {
        return this->portID;
    }
    QString getPortName()const
    {
        return this->portName;
    }

    double getTempUpper()const
    {
        return this->tempUpper;
    }
    double getTempLimit()const
    {
        return this->tempLimit;
    }
    double getTempValue()const
    {
        return this->tempValue;
    }
    QString getTempSign()const
    {
        return this->tempSign;
    }

    double getDampUpper()const
    {
        return this->dampUpper;
    }
    double getDampLimit()const
    {
        return this->dampLimit;
    }
    double getDampValue()const
    {
        return this->dampValue;
    }
    QString getDampSign()const
    {
        return this->dampSign;
    }

    int getSaveInterval()const
    {
        return this->saveInterval;
    }
    QString getDeviceType()const
    {
        return this->deviceType;
    }
    QString getDeviceSound()const
    {
        return this->deviceSound;
    }

    double getCurrentTemp()const
    {
        return this->currentTemp;
    }
    double getCurrentDamp()const
    {
        return this->currentDamp;
    }
    bool getDeviceOnLine()const
    {
        return this->onLine;
    }

    bool getTempUpperAlarm()const
    {
        return this->tempUpperAlarm;
    }
    bool getTempLimitAlarm()const
    {
        return this->tempLimitAlarm;
    }
    bool getDampUpperAlarm()const
    {
        return this->dampUpperAlarm;
    }
    bool getDampLimitAlarm()const
    {
        return this->dampLimitAlarm;
    }

private slots:
    void initForm();
    void setTempUpperAlarm(bool tempUpperAlarm);
    void setTempLimitAlarm(bool tempLimitAlarm);
    void setDampUpperAlarm(bool dampUpperAlarm);
    void setDampLimitAlarm(bool dampLimitAlarm);    

public slots:
    void changeStatus(QString type, bool alarm);

    void checkAlarm();          //确认告警
    void startPlaySound();      //播放报警声音
    void stopPlaySound();       //停止报警声音

    //插入短信和邮件告警
    void appendMsgEmail(int id, QString content, double value, QString sign);

    //设置当前温湿度值
    void setCurrentTempDamp(double currentTemp, double currentDamp);
    //设置电池电量
    void setPowerValue(int powerValue);
    //设置记录数
    void setCount(int countAll, int countNoRead);
    //设置设备在线离线
    void setDeviceOnLine(bool onLine);

private:
    Ui::frmDevicePanel *ui;

    QSound *sound;      //报警声音对象

    int powerValue;     //电量
    int countAll;       //总记录数
    int countNoRead;    //未读记录数

    int deviceID;       //设备编号
    QString deviceName; //设备名称
    int deviceAddr;     //设备地址

    int groupID;        //区域编号
    QString groupName;  //区域名称
    int portID;         //端口编号
    QString portName;   //端口名称

    double tempUpper;   //温度上限
    double tempLimit;   //温度下限
    double tempValue;   //温度校准值
    QString tempSign;   //温度符号

    double dampUpper;   //湿度上限
    double dampLimit;   //湿度下限
    double dampValue;   //湿度校准值
    QString dampSign;   //湿度符号

    int saveInterval;   //存储周期
    QString deviceType; //设备类型
    QString deviceSound;//报警声音

    double currentTemp; //当前温度
    double currentDamp; //当前湿度
    bool onLine;        //是否在线

    bool tempUpperAlarm;//高温告警
    bool tempLimitAlarm;//低温告警
    bool dampUpperAlarm;//高湿告警
    bool dampLimitAlarm;//低湿告警
};

#endif // FRMDEVICEPANEL_H
