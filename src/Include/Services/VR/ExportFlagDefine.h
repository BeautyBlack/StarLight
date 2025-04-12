
#pragma once


#ifdef VR_SERVICEVR_EXPORTS
#define VR_SERVICEMPR_API __declspec(dllexport)
#else
#define VR_SERVICEMPR_API __declspec(dllimport)
#pragma comment(lib, "Service.VR.lib")
#include <ImportThirdPartedLibs.h>
#endif

