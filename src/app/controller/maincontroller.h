#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <app/app_version.h>
#include "quiwidget.h"
#include "appinit.h"
#include "trayicon.h"
#include"frmmain.h"
#include"mainform.h"

#include"candevicefactory.h"
#include"usbdevicefactory.h"
#include"candevicemanager.h"
#include"usbdevicemanager.h"
class MainController : public QObject
{
    Q_OBJECT
public:
//    static MainController *Instance();
    explicit MainController(QObject *parent = nullptr);
    ~MainController();
   MainForm& getMainForm();
   void initConnect();

signals:

public slots:
    void initDevice();
    void freeDevice();

private:
//       static QScopedPointer<MainController> self;
    AbstractDeviceManager* m_canDeviceManager;
    AbstractDeviceFactory* m_CanDeviceFactory;
    MainForm frm;
};

#endif // MAINCONTROLLER_H
