// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the SERVICEVR_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// SERVICEVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SERVICEVR_EXPORTS
#define SERVICEVR_API __declspec(dllexport)
#else
#define SERVICEVR_API __declspec(dllimport)
#endif

// This class is exported from the dll
class SERVICEVR_API CServiceVR {
public:
	CServiceVR(void);
	// TODO: add your methods here.
};

extern SERVICEVR_API int nServiceVR;

SERVICEVR_API int fnServiceVR(void);
