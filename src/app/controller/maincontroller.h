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

#include"candatamodelfactory.h"

#include"socketformmain.h"

class MainController : public QObject
{
    Q_OBJECT
public:
//    static MainController *Instance();
    explicit MainController(QObject *parent = nullptr);
    ~MainController();
   MainForm& getMainForm();
   SocketFormMain & getSocketFormMain();
   void initConnect();

signals:

public slots:
    void initDevice();
    void freeDevice();

private:
//       static QScopedPointer<MainController> self;
    AbstractDeviceManager* m_canDeviceManager;
    AbstractDeviceFactory* m_CanDeviceFactory;

    AbstractDataModelFactory* m_CanDataModelFactory;
    AbstractDataModel* m_CanDataModel;
    MainForm frm;
    SocketFormMain socketformmain;

};

#endif // MAINCONTROLLER_H
