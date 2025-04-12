#pragma once

#include <QtCore/qglobal.h>

//#ifndef BUILD_STATIC
#if defined(PATIENTLISTFRAMEVIEWMODEL_LIB)
#define PATIENTLISTFRAMEVIEWMODEL_EXPORT Q_DECL_EXPORT
#else
#define PATIENTLISTFRAMEVIEWMODEL_EXPORT Q_DECL_IMPORT
#endif
//#else
//#define PATIENTLISTFRAMEVIEWMODEL_EXPORT
//#endif

#ifdef PATIENTLISTFRAMEVIEWMODEL_LIB
#define PATIENTLISTFRAMEVIEWMODEL_API __declspec(dllexport)
#else
#define PATIENTLISTFRAMEVIEWMODEL_API __declspec(dllimport)
#pragma comment(lib, "PatientListFrameViewModel.lib")
#endif


