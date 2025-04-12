// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H


#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit


#include "resource.h"

#include <atlbase.h>
#include <atlcom.h>

#undef _WINDOWS_
#include <afx.h>
#include <afxwin.h>

using namespace ATL;


// add headers that you want to pre-compile here
#include "framework.h"

#include <iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
// reference additional headers your program requires here

#pragma warning(disable:4996)

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Netapi32.lib")	

#include <dcmtk/config/osconfig.h>
#define INCLUDE_CSTDLIB
#define INCLUDE_CSTRING
#define INCLUDE_CSTDARG
#define INCLUDE_CCTYPE
#define INCLUDE_CSIGNAL
#define INCLUDE_CSTDIO
#define INCLUDE_CSTDLIB
#define INCLUDE_CSTRING
#include "dcmtk/ofstd/ofstdinc.h"

#include <ImportThirdPartedLibs.h>

#include <rapidjson/document.h>


#pragma comment(lib, "Foundation.Common.lib")
#pragma comment(lib, "Foundations.Interfaces.lib")
#pragma comment(lib, "Foundation.DataModule.lib")
#pragma comment(lib, "Foundation.Log.lib")
#pragma comment(lib, "Utilities.Configuration.lib")
#pragma comment(lib, "jsoncpp.lib")



#endif //PCH_H
