
#pragma once


#ifdef PARONAMA_SERVICEVR_EXPORTS
#define PARONAMA_SERVICEMPR_API __declspec(dllexport)
#else
#define PARONAMA_SERVICEMPR_API __declspec(dllimport)

#pragma comment(lib, "Service.Paronama.lib")

#include <ImportThirdPartedLibs.h>

#endif
