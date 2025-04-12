// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the SERVICEMPR_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// SERVICEMPR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SERVICEMPR_EXPORTS
#define SERVICEMPR_API __declspec(dllexport)
#else
#define SERVICEMPR_API __declspec(dllimport)
#endif

// This class is exported from the dll
class SERVICEMPR_API CServiceMpr {
public:
	CServiceMpr(void);
	// TODO: add your methods here.
};

extern SERVICEMPR_API int nServiceMpr;

SERVICEMPR_API int fnServiceMpr(void);
