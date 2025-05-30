// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

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

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>

// add headers that you want to pre-compile here
//#include "framework.h"



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
#pragma comment(lib, "jsoncpp.lib")


template<typename T, typename N, typename M>
void ReadTextFromJsonObject(T& jsonObject, N value, M key)
{
	value = jsonObject[key.c_str()].GetString();
}


template<typename T, typename N, typename M>
void WriteTextToJsonObject(T& jsonObject, N value, M key)
{
	rapidjson::Value jsonValue;

	jsonValue.SetString(value.c_str(), value.length());

	jsonObject.AddMember(
		rapidjson::GenericStringRef<char>(key.c_str()),
		jsonValue,
		jsonObject.GetAllocator());
}

template<typename T, typename N, typename M>
void ReadTextArrayFromJsonObject(T& jsonObject, N value, M key)
{
	const auto textArray = jsonObject[key.c_str()].GetArray();
	for (auto i = 0; i < textArray.Size(); i++)
	{
		value.push_back(textArray[i].GetString());
	}
}

template<typename T, typename N, typename M>
void WriteTextArrayToJsonObject(T& jsonObject, N value, M key)
{
	rapidjson::Value jsonValue(rapidjson::kArrayType);

	for (auto& t : value)
	{
		jsonValue.PushBack(rapidjson::GenericStringRef<char>(t.c_str()), jsonObject.GetAllocator());
	}
	
	jsonObject.AddMember(
		rapidjson::GenericStringRef<char>(key.c_str()),
		jsonValue,
		jsonObject.GetAllocator());
}

template<typename T, typename N, typename M>
void ReadIntFromJsonObject(T& jsonObject, N value, M key)
{
	value = static_cast<N>(jsonObject[key.c_str()].GetInt());
}

template<typename T, typename N, typename M>
void WriteIntToJsonObject(T& jsonObject, N value, M key)
{
	rapidjson::Value jsonValue;

	jsonValue.SetInt(value);

	jsonObject.AddMember(
		rapidjson::GenericStringRef<char>(key.c_str()),
		jsonValue,
		jsonObject.GetAllocator());
}

template<typename T, typename N, typename M>
void ReadDateFromJsonObject(T& jsonObject, N value, M key)
{
	int datetime = jsonObject[key.c_str()].GetInt();

	ZeroMemory(&value, sizeof(SYSTEMTIME));

	value.wYear = datetime / 10000;
	value.wMonth = (datetime % 10000) / 100;
	value.wDay = datetime % 100;
}

template<typename T, typename N, typename M>
void WriteDateToJsonObject(T& jsonObject, N value, M key)
{
	rapidjson::Value jsonValue;

	jsonValue.SetInt(value.wYear * 10000 + value.wMonth * 100 + value.wDay);

	jsonObject.AddMember(
		rapidjson::GenericStringRef<char>(key.c_str()),
		jsonValue,
		jsonObject.GetAllocator());
}

template<typename T, typename N, typename M>
void ReadDateTimeFromJsonObject(T& jsonObject, N value, M key)
{
	auto datetime = jsonObject[key.c_str()].GetInt64();
	auto date = datetime >> 32;
	auto time = datetime & 0xFFFFFFFF;

	ZeroMemory(&value, sizeof(SYSTEMTIME));

	value.wYear = date / 10000;
	value.wMonth = (date % 10000) / 100;
	value.wDay = date % 100;

	value.wHour = time / 10000;
	value.wMinute = (time % 10000) / 100;
	value.wSecond = time % 100;
}

template<typename T, typename N, typename M>
void WriteDateTimeToJsonObject(T& jsonObject, N value, M key)
{
	rapidjson::Value jsonValue;

	INT64 nValue = value.wYear * 10000 + value.wMonth * 100 + value.wDay;
	nValue = nValue << 32;
	
	jsonValue.SetInt64(
		nValue
		| (value.wHour * 10000 + value.wMinute * 100 + value.wSecond));

	jsonObject.AddMember(
		rapidjson::GenericStringRef<char>(key.c_str()),
		jsonValue,
		jsonObject.GetAllocator());
}
