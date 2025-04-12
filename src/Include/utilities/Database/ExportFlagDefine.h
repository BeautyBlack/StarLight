
#pragma once


#ifdef UTILITIESDATABASE_EXPORTS
#define UTILITIESDATABASE_API __declspec(dllexport)
#else
#define UTILITIESDATABASE_API __declspec(dllimport)
#pragma comment(lib, "Utilities.Database.lib")
#endif
