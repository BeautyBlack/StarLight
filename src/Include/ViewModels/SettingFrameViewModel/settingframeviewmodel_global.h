#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SETTINGFRAMEVIEWMODEL_LIB)
#  define SETTINGFRAMEVIEWMODEL_EXPORT Q_DECL_EXPORT
# else
#  define SETTINGFRAMEVIEWMODEL_EXPORT Q_DECL_IMPORT
# endif
#else
# define SETTINGFRAMEVIEWMODEL_EXPORT
#endif

#ifdef SETTINGFRAMEVIEWMODEL_LIB
#define SETTINGFRAMEVIEWMODEL_API __declspec(dllexport)
#else
#define SETTINGFRAMEVIEWMODEL_API __declspec(dllimport)
#pragma comment(lib, "SettingFrameViewModel.lib")
#endif


