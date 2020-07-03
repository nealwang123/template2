
HEADERS +=  \
#    $$IDE_SOURCE_TREE/ThirdPartLib/USBSlider/MT_API.h \
    $$IDE_SOURCE_TREE/ThirdPartLib/ZeroMQ/C++/zhelpers.hpp \
     $$PWD/mythread.h \
     $$PWD/abstractdevice.h \
     $$PWD/abstractdevicefactory.h \
     $$PWD/abstractdevicemanager.h \
#     $$PWD/usbdevice.h \
#     $$PWD/usbdevicefactory.h \
#     $$PWD/usbdevicemanager.h \
     $$PWD/winpcapdevice.h \
     $$PWD/winpcapdevicefactory.h \
     $$PWD/winpcapdevicemanager.h \
     $$PWD/NetDevice.h \
     $$PWD/netdevicefactory.h \
     $$PWD/netdevicemanager.h \
     $$PWD/serialdevice.h \
     $$PWD/serialdevicefactory.h \
     $$PWD/serialdevicemanager.h \
     $$PWD/candevice.h \
     $$PWD/candevicefactory.h \
     $$PWD/candevicemanager.h

SOURCES += \
     $$PWD/abstractdevice.cpp \
     $$PWD/abstractdevicefactory.cpp \
     $$PWD/abstractdevicemanager.cpp \
     $$PWD/candevice.cpp \
     $$PWD/candevicefactory.cpp \
     $$PWD/candevicemanager.cpp \
     $$PWD/NetDevice.cpp \
     $$PWD/netdevicemanager.cpp \
     $$PWD/netdevicefactory.cpp \
#     $$PWD/usbdevice.cpp \
#     $$PWD/usbdevicefactory.cpp \
#     $$PWD/usbdevicemanager.cpp \
     $$PWD/winpcapdevice.cpp \
     $$PWD/winpcapdevicefactory.cpp \
     $$PWD/winpcapdevicemanager.cpp \
     $$PWD/serialdevice.cpp \
     $$PWD/serialdevicefactory.cpp \
     $$PWD/serialdevicemanager.cpp
