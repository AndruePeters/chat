// This will get factored out over time, but is a remntant from the book

#ifndef CMLIB_GLOBAL_H
#define CMLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CMLIB_LIBRARY)
#define CMLIB_EXPORT Q_DECL_EXPORT
#else
#define CMLIB_EXPORT Q_DECL_IMPORT
#endif

#endif    // CMLIB_GLOBAL_H
