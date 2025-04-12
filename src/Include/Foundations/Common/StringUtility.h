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

#ifndef STRING_UTILITY_DB0C71A991894858931AFBC8FE563D48
#define STRING_UTILITY_DB0C71A991894858931AFBC8FE563D48

#pragma once

//#include <Foundations/Interfaces/MineType.h>
//
//#pragma comment(lib, "Ole32.lib")
//

class CStringUtility
{
public:
	/**
	 * 生成GUID
	 */
	static CString GenerateGuid();

	/**
	 * UTF8 字符串转换为 GB2312 格式
	 */
	static CString ConvertUTF8ToGB(const char* strSoruce, int nSourceSize = 0);


	/**
	 * GB2312 字符串转换为 UTF8 格式
	 */
	static CString ConvertGBToUTF8(const char* strSoruce, int nSourceSize = 0);

};

#endif
