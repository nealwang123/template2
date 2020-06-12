#include "appinit.h"
#include "quiwidget.h"
#include "savelog.h"
#include "saveruntime.h"
#include "dblocalthread.h"

AppInit *AppInit::self = NULL;
AppInit::AppInit(QObject *parent) : QObject(parent)
{
}

bool AppInit::eventFilter(QObject *obj, QEvent *evt)
{
    QWidget *w = (QWidget *)obj;
    if (!w->property("canMove").toBool()) {
        return QObject::eventFilter(obj, evt);
    }

    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
    if (event->type() == QEvent::MouseButtonPress) {
        if (event->button() == Qt::LeftButton) {
            mousePressed = true;
            mousePoint = event->globalPos() - w->pos();
            return true;
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        mousePressed = false;
        return true;
    } else if (event->type() == QEvent::MouseMove) {
        if (mousePressed && (event->buttons() && Qt::LeftButton)) {
            w->move(event->globalPos() - mousePoint);
            return true;
        }
    }

    return QObject::eventFilter(obj, evt);
}

void AppInit::start()
{
    QUIHelper::setCode();
    QUIHelper::setTranslator(":/imageTest/qt_zh_CN.qm");
    QUIHelper::setTranslator(":/imageTest/widgets.qm");

    //初始化随机数种子
    QUIHelper::initRand();
    QUIConfig::IconMain = 0xf1c0;
    App::ConfigFile = QUIHelper::appPath() + "/DbTool_Config.ini";
    App::readConfig();

#if 0//不支持多线程
    QUIHelper::newDir("log");

    SaveLog::Instance()->setPath(QUIHelper::appPath() + "/log");
    SaveLog::Instance()->setName(QUIHelper::appName());
    SaveLog::Instance()->start();

    SaveRunTime::Instance()->setPath(QUIHelper::appPath() + "/log");
    SaveRunTime::Instance()->setName(QUIHelper::appName());
    SaveRunTime::Instance()->start();
#endif

    qApp->installEventFilter(this);
    //应用全局样式
    QUIHelper::setStyle(App::StyleName, QUIConfig::TextColor,
                        QUIConfig::PanelColor, QUIConfig::BorderColor,
                        QUIConfig::NormalColorStart, QUIConfig::NormalColorEnd,
                        QUIConfig::DarkColorStart, QUIConfig::DarkColorEnd,
                        QUIConfig::HighColor);

    //先判断对应的数据库文件是否存在或者数据库服务器IP地址是否存在,否则会卡很久
    if (App::LocalDBType.toUpper() == "SQLITE") {
        QString dbFile = QUIHelper::appPath() + App::LocalDBName;
        if (!QUIHelper::fileIsExist(dbFile)) {
            QUIHelper::showMessageBoxError("sqlite数据库不存在!");
        }

        DbLocalThread::Instance()->setConnInfo(DbLocalThread::DbType_Sqlite, "qt_sql_default_connection", App::LocalDBIP, App::LocalDBPort, QUIHelper::appPath() + App::LocalDBName, App::LocalUserName, App::LocalUserPwd);
    } else if (App::LocalDBType.toUpper() == "MYSQL") {
        if (!QUIHelper::ipLive(App::LocalDBIP, App::LocalDBPort)) {
            QUIHelper::showMessageBoxError("mysql数据库服务器不存在!");
        }

        DbLocalThread::Instance()->setConnInfo(DbLocalThread::DbType_MySql, "qt_sql_default_connection", App::LocalDBIP, App::LocalDBPort, App::LocalDBName, App::LocalUserName, App::LocalUserPwd);
    }

    //程序启动后打开数据库连接,在任何窗体需要的地方调用数据库
    if (DbLocalThread::Instance()->openDb()) {
        DbLocalThread::Instance()->setCheckInterval(3 * 60);//定时检查连接
        DbLocalThread::Instance()->start();
    }
}
