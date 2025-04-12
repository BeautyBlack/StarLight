
#pragma once


#ifdef SERVICEMPR_EXPORTS
#define SERVICEMPR_API __declspec(dllexport)
#else
#define SERVICEMPR_API __declspec(dllimport)
#pragma comment(lib, "Service.Mpr.lib")
#endif

