#include "pch.h"

#include <Foundations/Common/StringUtility.h>
#include <Foundations/Common/ToDefine.h>

CString CStringUtility::GenerateGuid()
{
	GUID guid;
	
	CoCreateGuid(&guid);
	
	char cBuffer[64] = { 0 };
	
	sprintf_s(cBuffer, sizeof(cBuffer),
		"%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2,
		guid.Data3, guid.Data4[0],
		guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4],
		guid.Data4[5], guid.Data4[6],
		guid.Data4[7]);
	
	return cBuffer;
}

CString CStringUtility::ConvertUTF8ToGB(const char* strSoruce, int nSourceSize)
{
	CString strResult;
	
	if (strSoruce == NULL_SYMBAL)
		return strResult;

	nSourceSize = nSourceSize > 0 ? nSourceSize : strlen(strSoruce);
	
	if (nSourceSize <= 0)
		return strResult;

	int nWSize = MultiByteToWideChar(CP_UTF8, 0, strSoruce, -1, 0, 0);

	if (nWSize <= 0)
		return strResult;

	WCHAR *wTmp = new WCHAR[nWSize];

	ZeroMemory(wTmp, sizeof(WCHAR)*nWSize);

	int nTransLen = MultiByteToWideChar(CP_UTF8, 0, strSoruce, nSourceSize, wTmp, nWSize);

	int nGBSize = WideCharToMultiByte(CP_ACP, 0, wTmp, -1, NULL_SYMBAL, 0, 0, 0) + 2;

	if (nGBSize <= 0)
	{
		delete[] wTmp;

		return strResult;
	}

	char *pResult = new char[nGBSize];

	ZeroMemory(pResult, nGBSize);

	WideCharToMultiByte(CP_ACP, 0, wTmp, nTransLen, pResult, nGBSize, 0, 0);

	delete[] wTmp;

	strResult.Format("%s", pResult);

	delete[] pResult;

	return strResult;
}

CString CStringUtility::ConvertGBToUTF8(const char* strSoruce, int nSourceSize)
{
	CString strResult;

	if (strSoruce == NULL_SYMBAL)
		return strResult;

	nSourceSize = nSourceSize > 0 ? nSourceSize : strlen(strSoruce);

	if (nSourceSize <= 0)
		return strResult;

	int nWSize = MultiByteToWideChar(CP_ACP, 0, strSoruce, -1, 0, 0);

	if (nWSize <= 0)
		return strResult;

	WCHAR *wTmp = new WCHAR[nWSize];

	ZeroMemory(wTmp, sizeof(WCHAR)*nWSize);

	int nTransLen = MultiByteToWideChar(CP_ACP, 0, strSoruce, nSourceSize, wTmp, nWSize);

	int nGBSize = WideCharToMultiByte(CP_UTF8, 0, wTmp, -1, NULL_SYMBAL, 0, 0, 0) + 2;

	if (nGBSize <= 0)
	{
		delete[] wTmp;

		return strResult;
	}

	char *pResult = new char[nGBSize];

	ZeroMemory(pResult, nGBSize);

	WideCharToMultiByte(CP_UTF8, 0, wTmp, nTransLen, pResult, nGBSize, 0, 0);

	delete[] wTmp;

	strResult.Format("%s", pResult);

	delete[] pResult;

	return strResult;
}

