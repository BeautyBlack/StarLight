// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#include <iostream>


// reference additional headers your program requires here
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
// reference additional headers your program requires here

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
#pragma comment(lib, "Utilities.Configuration.lib")
#pragma comment(lib, "Utilities.ImageModule.lib")

#pragma comment(lib, "jsoncpp.lib")
