﻿#ifndef CONTROLLERPLUGIN_GLOBAL_H
#define CONTROLLERPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CONTROLLERPLUGIN_LIBRARY)
#  define CONTROLLERPLUGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CONTROLLERPLUGINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CONTROLLERPLUGIN_GLOBAL_H
