#ifndef BALLISTICLIB_GLOBAL_H
#define BALLISTICLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TLELIB_LIBRARY)
#  define TLESHARED_EXPORT Q_DECL_EXPORT
#else
#  define TLESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif
