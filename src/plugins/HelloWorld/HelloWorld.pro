#------------------------------------------------- 
# 
# Project created by NewProject 
# 
#------------------------------------------------- 


QT       += widgets 

TARGET = HelloWorld
TEMPLATE = lib 


DEFINES += HELLOWORLD_LIBRARY 

SOURCES += helloworld.cpp\
    helloworldwidget.cpp


HEADERS += helloworld.h\
        helloworld_global.h \
    helloworldwidget.h

FORMS += \
    helloworldwidget.ui



