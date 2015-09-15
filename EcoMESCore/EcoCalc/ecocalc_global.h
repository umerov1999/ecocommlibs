#ifndef ECOCALC_GLOBAL_H
#define ECOCALC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ECOCALC_LIBRARY)
#  define ECOCALCSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ECOCALCSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ECOCALC_GLOBAL_H
