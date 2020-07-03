#-------------------------------------------------
#
# Project created by QtCreator 2020-04-23T14:08:28
#
#-------------------------------------------------
include(../../qtproject.pri)
include(../../rpath.pri)
QT+ = core
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PSBroker
#TEMPLATE = app

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

LIBS *= -L$$PWD/../../ThirdPartLib/ZeroMQ/ -llibzmq-v141-mt-4_3_2
INCLUDEPATH += $$PWD/../../ThirdPartLib/ZeroMQ
INCLUDEPATH+= $$PWD/../../ThirdPartLib/ZeroMQ/C++
INCLUDEPATH+= $$PWD/../../ThirdPartLib/ZeroMQ/C
DEPENDPATH += $$PWD/../../ThirdPartLib/ZeroMQ

SOURCES += \
        main.cpp \
#        psbrokerwidget.cpp \
        psbrokermonitor.cpp

HEADERS += \
#        psbrokerwidget.h \
        psbrokermonitor.h

#FORMS += \
#        psbrokerwidget.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
DISTFILES += \
    $$PWD/app_version.h.in

QMAKE_SUBSTITUTES += $$PWD/app_version.h.in
