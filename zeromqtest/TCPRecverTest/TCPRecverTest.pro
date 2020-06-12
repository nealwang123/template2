TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS+= -L../ -lzmq
#INCLUDEPATH+= D:\qtPro\zeroMQTest\C
INCLUDEPATH+= ../C++
INCLUDEPATH+= ../
HEADERS += \
    ../C++/zhelpers.hpp
SOURCES += \
    main.cpp \
