include(../qtproject.pri)
TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS +=  Client \
    Server \
    PSBroker \
    TCPRecverTest
