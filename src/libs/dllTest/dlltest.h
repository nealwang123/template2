#ifndef DLLTEST_H
#define DLLTEST_H

#include "dlltest_global.h"
#include<QDebug>
#include<QString>
#include<QByteArray>

#include <QObject>
#include <windows.h>
#include <windef.h>
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QMap>
#include <QRegExp>
#include <QMutex>
#include <QScopedPointer>
#include <QtEndian>


class DLLTESTSHARED_EXPORT DllTest
{

public:
    DllTest();
};

#endif // DLLTEST_H
