/*
 *****************************************************************
 *                 C O P Y R I G H T  (c) 2020
 *                    All Rights Reserved
 *****************************************************************
 *
 *       THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF
 *                      Zhang Jiaqi.
 *      The copyright notice above does not evidence any
 *     actual or intended publication of such source code.
 *
 *****************************************************************
 *
 *  Author: Zhang Jiaqi
 *
 */

#ifndef FILE_UTILITY_2180AF2C54E14B8BAB50CC3CA37268C7
#define FILE_UTILITY_2180AF2C54E14B8BAB50CC3CA37268C7

#pragma once

#include <Foundations/Common/ToDefine.h>
#include <vector>
#include <functional>

class CFileUtility
{
private:
	CFileUtility();
	
public:
	static long GetFileSize(const char* strPath);

	static char* ReadWholeFile(const char* strPath, int &nFileSize);

	static long RecordToFile(const char* strText, int nLen, const char* strPath);

	static void SearchPaths(std::vector<STRING_TYPE> &vecPaths, const char* strPath, const char* strPatten,
		bool bSearchSub = false);

	static void SearchPaths(std::function<void(const char*)> callback,
		const char* strPath,
		const char* strPatten = NULL_SYMBAL,
		bool bSearchSub = false);

	template <class T>
	void CombinePath(STRING_TYPE &strDestPath, const T& t);

	template <class T>
	void CombinePath(STRING_TYPE &strDestPath);

	template<class ... Args>
	static void CombinePath(STRING_TYPE &strDestPath, const Args& ... left);

	static bool IsExist(const char* strPath);

	static bool DeleteOneFile(const char* strPath);

	static bool DeleteOneFolder(const char* strPath);

	static bool IsFile(const char* path);
	
private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CFileUtility);

	AUTO_PROPERTY_REF(RunningDirectoryPath, STRING_TYPE);
	
};

template <class T>
void CFileUtility::CombinePath(STRING_TYPE &strDestPath, const T& t)
{
	strDestPath.append(t).append("/");
}

template <class T>
void CFileUtility::CombinePath(STRING_TYPE &strDestPath)
{
}

template <class ... Args>
void CFileUtility::CombinePath(STRING_TYPE &strDestPath, const Args& ... left)
{
	CombinePath(strDestPath, left...);

	if(sizeof...(left) <= 1)
		strDestPath.erase(strDestPath.length() - 1);
}

#endif
