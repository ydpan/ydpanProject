#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QGRAPHVIEWGUI_LIB)
#  define QGRAPHVIEWGUI_EXPORT Q_DECL_EXPORT
# else
#  define QGRAPHVIEWGUI_EXPORT Q_DECL_IMPORT
# endif
#else
# define QGRAPHVIEWGUI_EXPORT
#endif
