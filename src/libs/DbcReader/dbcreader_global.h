﻿#ifndef DBCREADER_GLOBAL_H
#define DBCREADER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DBCREADER_LIBRARY)
#  define DBCREADERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DBCREADERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DBCREADER_GLOBAL_H
