#include "frmdevicepanel.h"
#include "ui_frmdevicepanel.h"
#include "quiwidget.h"

frmDevicePanel::frmDevicePanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmDevicePanel)
{
    ui->setupUi(this);
    this->initForm();
}

frmDevicePanel::~frmDevicePanel()
{
    delete ui;
}

void frmDevicePanel::initForm()
{
    ui->line->setFixedHeight(1);
    ui->frame->setStyleSheet("QFrame#frame{border:none;}");
    ui->frame->layout()->setMargin(0);

    deviceID = 1;
    deviceName = "默认设备";
    deviceAddr = 1;

    groupID = 1;
    groupName = "默认区域";
    portID = 1;
    portName = "默认端口";

    tempUpper = 35;
    tempLimit = 0;
    tempValue = 0;
    tempSign = "℃";

    dampUpper = 75;
    dampLimit = 35;
    dampValue = 35;
    dampSign = "%RH";

    currentTemp = App::PlotDefaultValue;
    currentDamp = App::PlotDefaultValue;
    onLine = false;
    tempUpperAlarm = false;
    tempLimitAlarm = false;
    dampUpperAlarm = false;
    dampLimitAlarm = false;

    QFont font;
    font.setBold(true);
    font.setPointSize(App::FontSize + 1);
    ui->labDeviceName->setFont(font);

    sound = new QSound("");
    deviceSound = "11.wav";

    setCurrentTempDamp(0, 0);
    setPowerValue(0);
    setCount(0, 0);
}

void frmDevicePanel::checkAlarm()
{
    //当前设备必须处于报警状态
    bool alarm = (tempUpperAlarm || tempLimitAlarm || dampUpperAlarm || dampLimitAlarm);

    if (!alarm) {
        return;
    }

    //停止报警声音
    stopPlaySound();
}

void frmDevicePanel::startPlaySound()
{
    QString soundFile = QString("%1/sound/%2").arg(QUIHelper::appPath()).arg(deviceSound);
    sound->play(soundFile);
}

void frmDevicePanel::stopPlaySound()
{
    sound->stop();
}

void frmDevicePanel::appendMsgEmail(int id, QString content, double value, QString sign)
{
    //将告警内容添加到队列
    QString str = QString("告警编号:%1;区域:%2;%3;%4;告警值:%5%6;%7")
                  .arg(id).arg(groupName).arg(deviceName)
                  .arg(content).arg(value).arg(sign).arg(DATETIME);

    //    if (App::MsgInterval <= 1440) {
    //        App::AppendMsg(str);
    //    }
    //    if (App::EmailInterval <= 1440) {
    //        App::AppendEmail(str);
    //    }
}

void frmDevicePanel::setCurrentTempDamp(double currentTemp, double currentDamp)
{
    this->currentTemp = currentTemp;
    this->currentDamp = currentDamp;
    ui->labCurrentTemp->setText(QString("温度 : %1 %2").arg(currentTemp).arg(tempSign));
    ui->labCurrentDamp->setText(QString("湿度 : %1 %2").arg(currentDamp).arg(dampSign));

    //如果当前温湿度值超过警戒值则触发告警,对应插入告警记录,界面红色警示
    //同时将DO1控制插入任务队列等待任务定时器处理
    //如果在警戒值内,则判断是否处于告警状态,处于则插入告警解除记录,界面解除正常
    //温湿度值+温湿度校准值>温湿度上限 或者<温湿度下限则告警(传过来的时候就已经是+校准值后的值)
    //温湿度告警及告警解除的判断,需要加上缓冲值,例如告警值是30,缓冲值是1,那真实告警值是31,告警解除值是29

    //温度值告警判断
    if (currentTemp > (tempUpper + App::TempUpperBuffer)) {
        if (!tempUpperAlarm) {
            this->setTempUpperAlarm(true);
            appendMsgEmail(1, "高温告警", currentTemp, tempSign);
        }
    } else if (currentTemp < (tempUpper - App::TempUpperBuffer)) {
        if (tempUpperAlarm) {
            this->setTempUpperAlarm(false);
            checkAlarm();
            appendMsgEmail(1, "高温告警解除", currentTemp, tempSign);
        }
    }

    if (currentTemp < (tempLimit + App::TempLimitBuffer)) {
        if (!tempLimitAlarm) {
            this->setTempLimitAlarm(true);
            appendMsgEmail(1, "低温告警", currentTemp, tempSign);
        }
    } else if (currentTemp > (tempLimit - App::TempLimitBuffer)) {
        if (tempLimitAlarm) {
            this->setTempLimitAlarm(false);
            checkAlarm();
            appendMsgEmail(1, "低温告警解除", currentTemp, tempSign);
        }
    }

    //湿度值告警判断
    if (currentDamp > (dampUpper + App::DampUpperBuffer)) {
        if (!dampUpperAlarm) {
            this->setDampUpperAlarm(true);
            appendMsgEmail(1, "高湿告警", currentDamp, dampSign);
        }
    } else if (currentDamp < (dampUpper - App::DampUpperBuffer)) {
        if (dampUpperAlarm) {
            this->setDampUpperAlarm(false);
            checkAlarm();
            appendMsgEmail(1, "高湿告警解除", currentDamp, dampSign);
        }
    }

    if (currentDamp < (dampLimit + App::DampLimitBuffer)) {
        if (!dampLimitAlarm) {
            this->setDampLimitAlarm(true);
            appendMsgEmail(1, "低湿告警", currentDamp, dampSign);
        }
    } else if (currentDamp > (dampLimit - App::DampLimitBuffer)) {
        if (dampLimitAlarm) {
            this->setDampLimitAlarm(false);
            checkAlarm();
            appendMsgEmail(1, "低湿告警解除", currentDamp, dampSign);
        }
    }
}

void frmDevicePanel::setPowerValue(int powerValue)
{
    this->powerValue = powerValue;
    ui->btnPower->setValue(powerValue);
}

void frmDevicePanel::setCount(int countAll, int countNoRead)
{
    this->countAll = countAll;
    this->countNoRead = countNoRead;
    ui->labCountAll->setText(QString("总条数 : %1").arg(countAll));
    ui->labCountNoRead->setText(QString("未读 : %1").arg(countNoRead));
}

void frmDevicePanel::setDeviceID(int deviceID)
{
    this->deviceID = deviceID;
}

void frmDevicePanel::setDeviceName(QString deviceName)
{
    this->deviceName = deviceName;
    ui->labDeviceName->setText(deviceName);
    ui->labDeviceName->setToolTip(deviceName);
}

void frmDevicePanel::setDeviceAddr(int deviceAddr)
{
    this->deviceAddr = deviceAddr;
}

void frmDevicePanel::setGroupID(int groupID)
{
    this->groupID = groupID;
}

void frmDevicePanel::setGroupName(QString groupName)
{
    this->groupName = groupName;
}

void frmDevicePanel::setPortID(int portID)
{
    this->portID = portID;
}

void frmDevicePanel::setPortName(QString portName)
{
    this->portName = portName;
}

void frmDevicePanel::setTempUpper(double tempUpper)
{
    this->tempUpper = tempUpper;
}

void frmDevicePanel::setTempLimit(double tempLimit)
{
    this->tempLimit = tempLimit;
}

void frmDevicePanel::setTempValue(double tempValue)
{
    this->tempValue = tempValue;
}

void frmDevicePanel::setTempSign(QString tempSign)
{
    this->tempSign = tempSign;
}

void frmDevicePanel::setDampUpper(double dampUpper)
{
    this->dampUpper = dampUpper;
}

void frmDevicePanel::setDampLimit(double dampLimit)
{
    this->dampLimit = dampLimit;
}

void frmDevicePanel::setDampValue(double dampValue)
{
    this->dampValue = dampValue;
}

void frmDevicePanel::setDampSign(QString dampSign)
{
    this->dampSign = dampSign;
}

void frmDevicePanel::setSaveInterval(int saveInterval)
{
    this->saveInterval = saveInterval;
}

void frmDevicePanel::setDeviceType(QString deviceType)
{
    this->deviceType = deviceType;

    if (deviceType == "温湿度" || deviceType == "其他") {
        ui->line->setVisible(false);
        ui->btnPower->setVisible(false);
    } else if (deviceType == "温湿度(带记录)") {
        ui->btnPower->setVisible(false);
    }
}

void frmDevicePanel::setDeviceSound(QString deviceSound)
{
    this->deviceSound = deviceSound;
}

void frmDevicePanel::setTempUpperAlarm(bool tempUpperAlarm)
{
    if (tempUpperAlarm) {
        ui->btnTemp->setRed();
    } else {
        ui->btnTemp->setGreen();
    }

    changeStatus("温度", tempUpperAlarm);
    this->tempUpperAlarm = tempUpperAlarm;
}

void frmDevicePanel::setTempLimitAlarm(bool tempLimitAlarm)
{
    if (tempLimitAlarm) {
        ui->btnTemp->setRed();
    } else {
        ui->btnTemp->setGreen();
    }

    changeStatus("温度", tempLimitAlarm);
    this->tempLimitAlarm = tempLimitAlarm;
}

void frmDevicePanel::setDampUpperAlarm(bool dampUpperAlarm)
{
    if (dampUpperAlarm) {
        ui->btnDamp->setRed();
    } else {
        ui->btnDamp->setGreen();
    }

    changeStatus("湿度", dampUpperAlarm);
    this->dampUpperAlarm = dampUpperAlarm;
}

void frmDevicePanel::setDampLimitAlarm(bool dampLimitAlarm)
{
    if (dampLimitAlarm) {
        ui->btnDamp->setRed();
    } else {
        ui->btnDamp->setGreen();
    }

    changeStatus("湿度", dampLimitAlarm);
    this->dampLimitAlarm = dampLimitAlarm;
}

void frmDevicePanel::changeStatus(QString type, bool alarm)
{
    QString text;
    int fontSize = App::FontSize;
    bool bold = false;
    QString color = QUIConfig::TextColor;

    //告警后,对应字体加粗,字号加大,红色突出显示
    if (alarm) {
        fontSize = App::FontSize + 1;
        bold = true;
        color = "#FF0000";
    }

    QFont font;
    font.setBold(bold);
    font.setPointSize(fontSize);

    QString qss = QString("QLabel{color:%1;}").arg(color);

    if (type == "湿度") {
        text = QString("%1 : %2 %3").arg(type).arg(currentDamp).arg(dampSign);
        ui->labCurrentDamp->setFont(font);
        ui->labCurrentDamp->setStyleSheet(qss);
        ui->labCurrentDamp->setText(text);
    } else if (type == "温度") {
        text = QString("%1 : %2 %3").arg(type).arg(currentTemp).arg(tempSign);
        ui->labCurrentTemp->setFont(font);
        ui->labCurrentTemp->setStyleSheet(qss);
        ui->labCurrentTemp->setText(text);
    }

    ui->labDeviceName->setStyleSheet(QString("QLabel{color:%1;}").arg(QUIConfig::TextColor));
}

void frmDevicePanel::setDeviceOnLine(bool onLine)
{
    if (onLine) {
        ui->btnDevice->setGreen();
        ui->btnTemp->setGreen();
        ui->btnDamp->setGreen();
        setPowerValue(4);
    } else {
        ui->btnDevice->setGray();
        ui->btnTemp->setGray();
        ui->btnDamp->setGray();
        setPowerValue(5);
    }

    changeStatus("温度", false);
    changeStatus("湿度", false);
    this->onLine = onLine;

    //每次上下线都要将报警状态清空
    tempUpperAlarm = false;
    tempLimitAlarm = false;
    dampUpperAlarm = false;
    dampLimitAlarm = false;
}
