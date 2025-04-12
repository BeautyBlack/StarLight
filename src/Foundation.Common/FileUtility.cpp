#include "pch.h"

#include <Foundations/Common/FileUtility.h>
#include <libloaderapi.h>
#include <ATLComTime.h>

#include <afx.h>


#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <ImageHlp.h>
#pragma comment(lib, "dbghelp.lib")

CFileUtility CFileUtility::m_Instance;

CFileUtility::CFileUtility()
{
	char strTmp[MAX_PATH_SIZE] = { 0 };

	GetModuleFileName(NULL_SYMBAL, strTmp, sizeof(strTmp));
	PathRemoveFileSpec(strTmp);

	SetRunningDirectoryPath(strTmp);
}

char* CFileUtility::ReadWholeFile(const char* strPath, int &nFileSize)
{
	nFileSize = 0;
	if (!PathFileExists(strPath))
		return NULL_SYMBAL;

	const int nSize = GetFileSize(strPath);

	const int nAllocSize = nSize + 8;
	auto result = new char[nAllocSize];

	ZeroMemory(result, nAllocSize);

	FILE *pFile = NULL_SYMBAL;

	fopen_s(&pFile, strPath, "r");

	if (pFile == NULL_SYMBAL)
		return NULL_SYMBAL;

	fread_s(result, nSize + 8, sizeof(char), nSize, pFile);
	
	nFileSize = nSize;

	fclose(pFile);

	return result;
}

long CFileUtility::GetFileSize(const char* strPath)
{
	if ( !PathFileExists(strPath))
		return 0;

	FILE* fp = NULL_SYMBAL;
	
	fopen_s(&fp, strPath, "r");
	
	fseek(fp, 0, SEEK_END);
	
	const auto size = ftell(fp);
	
	fclose(fp);
	
	return size;
}

long CFileUtility::RecordToFile(const char* strText, int nLen, const char* strPath)
{
	FILE* fp = NULL_SYMBAL;

	fopen_s(&fp, strPath, "w+");

	if (fp == NULL_SYMBAL)
	{
		DWORD dwError = GetLastError();
		return 0;
	}

	fwrite(strText, sizeof(char), nLen, fp);

	const auto size = ftell(fp);

	fclose(fp);

	return size;
}

void CFileUtility::SearchPaths(std::vector<STRING_TYPE>& vecPaths,
	const char* strPathT,
	const char* strPatten,
	bool bSearchSub)
{
	if (strPathT == NULL_SYMBAL || !PathFileExists(strPathT))
		return;

	CFileFind tmpFileFind;

	char strPath[MAX_PATH_SIZE] = { 0 };

	if (strPatten == NULL_SYMBAL)
		_snprintf_s(strPath, sizeof(strPath), _TRUNCATE, "%s\\*.*", strPathT);
	else
		_snprintf_s(strPath, sizeof(strPath), _TRUNCATE, "%s\\*.%s", strPathT, strPatten);

	CFileFind ff;
	BOOL res = ff.FindFile(strPath);
	if (res)
	{
		while (res)
		{
			res = ff.FindNextFile();
			if (ff.IsDots())
				continue;

			auto tmpFilePath = ff.GetFilePath();

			if (PathIsDirectory(tmpFilePath) && bSearchSub)
				SearchPaths(vecPaths, tmpFilePath, strPatten, bSearchSub);
			else
				vecPaths.push_back(tmpFilePath.GetString());
		}
	}

	ff.Close();
}

void CFileUtility::SearchPaths(std::function<void(const char*)> callback,
	const char* strPathT,
	const char* strPatten,
	bool bSearchSub)
{
	if (strPathT == NULL_SYMBAL || !PathFileExists(strPathT))
		return;

	CFileFind tmpFileFind;

	char strPath[MAX_PATH_SIZE] = { 0 };

	if (strPatten == NULL_SYMBAL)
		_snprintf_s(strPath, sizeof(strPath), _TRUNCATE, "%s\\*.*", strPathT);
	else
		_snprintf_s(strPath, sizeof(strPath), _TRUNCATE, "%s\\*.%s", strPathT, strPatten);

	CFileFind ff;
	BOOL res = ff.FindFile(strPath);
	if (res)
	{
		while (res)
		{
			res = ff.FindNextFile();
			if (ff.IsDots())
				continue;

			auto tmpFilePath = ff.GetFilePath();

			if (PathIsDirectory(tmpFilePath) && bSearchSub)
				SearchPaths(callback, tmpFilePath, strPatten, bSearchSub);
			else
				callback(tmpFilePath.GetString());
		}

	}

	ff.Close();
}

bool CFileUtility::IsExist(const char* strPath)
{
	return PathFileExists(strPath);
}

bool CFileUtility::DeleteOneFile(const char* strPath)
{
	SetFileAttributes(strPath, 0);  // 修改属性,以防为只读
	return DeleteFile(strPath);
}

bool CFileUtility::DeleteOneFolder(const char* strPath)
{
	if (IsFile(strPath))
	{
		DeleteFile(strPath);
		return true;
	}

	CFileFind find;

	BOOL bFind = find.FindFile(strPath);

	while (bFind)
	{
		bFind = find.FindNextFile();
		auto strTmpPath = find.GetFilePath();

		if (find.IsDots())
		{
			continue;
		}
		else if (!find.IsDirectory())
		{
			DeleteFile(strTmpPath);
		}
		else if (find.IsDirectory())
		{
			DeleteOneFolder(strTmpPath + "\\*.*");
			RemoveDirectory(strTmpPath);
		}

	}

	find.Close();

	return true;
}

bool CFileUtility::IsFile(const char* path)
{
	return (GetFileAttributes(path)
			& (FILE_ATTRIBUTE_HIDDEN
			| FILE_ATTRIBUTE_DIRECTORY))
		== 0; 
}



