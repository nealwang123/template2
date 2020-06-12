#-------------------------------------------------
#
# Project created by QtCreator 2019-06-26T22:31:10
#
#-------------------------------------------------

include(../../qtproject.pri)
#include($$QTCANPOOL_ROOT/src/shared/qtsingleapplication/qtsingleapplication.pri)
include(../../rpath.pri)

QT       += core gui network sql xml multimedia concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG -= qml_debug

TARGET = app
TEMPLATE = app
DESTDIR = $$IDE_APP_PATH

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

# lib can相关库
#LIBS *= -l$$qtLibraryName(ExtensionSystem) -l$$qtLibraryName(Aggregation) -l$$qtLibraryName(Utils)
LIBS *= -l$$qtLibraryName(DllTest) -l$$qtLibraryName(DbcReader)
LIBS *= -L$$IDE_SOURCE_TREE/ThirdPartLib/CAN/ControlCAN -lControlCAN
INCLUDEPATH+= $$IDE_SOURCE_TREE/ThirdPartLib/CAN

#zeromq debug库
LIBS+= -L$$IDE_SOURCE_TREE/ThirdPartLib/ZeroMQ -lzmq
INCLUDEPATH+= $$IDE_SOURCE_TREE/ThirdPartLib/ZeroMQ/C++
INCLUDEPATH+= $$IDE_SOURCE_TREE/ThirdPartLib/ZeroMQ/C
INCLUDEPATH+= $$IDE_SOURCE_TREE/ThirdPartLib/ZeroMQ

#USB转台相关库
LIBS+= -L$$IDE_SOURCE_TREE/ThirdPartLib/USBSlider -lMT_API
INCLUDEPATH+= $$IDE_SOURCE_TREE/ThirdPartLib/USBSlider


#添加外部资源
include             ($$PWD/api/api.pri)
include             ($$PWD/dbapi/dbapi.pri)
include             ($$PWD/usercontrol/usercontrol.pri)

include             ($$PWD/model/model.pri)
include             ($$PWD/view/view.pri)
include             ($$PWD/controller/controller.pri)
#包含目录
INCLUDEPATH         += $$PWD
INCLUDEPATH         += $$PWD/api
INCLUDEPATH         += $$PWD/dbapi
INCLUDEPATH         += $$PWD/usercontrol
INCLUDEPATH         += $$PWD/model/device
INCLUDEPATH         += $$PWD/view/form
INCLUDEPATH         += $$PWD/view/businessform
INCLUDEPATH         += $$PWD/controller

SOURCES += \
    main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    $$PWD/app_version.h.in

QMAKE_SUBSTITUTES += $$PWD/app_version.h.in


#通用工程资源

RESOURCES           += other/main.qrc
RESOURCES           += other/qss.qrc
#RC_FILE =app_resource.rc
