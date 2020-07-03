TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS+= -L../ -llibzmq-v141-mt-4_3_2
#INCLUDEPATH+= D:\qtPro\zeroMQTest\C
INCLUDEPATH+= ../C++
INCLUDEPATH+= ../
HEADERS += \
    ../C++/zhelpers.hpp
SOURCES += \
    main.cpp \
