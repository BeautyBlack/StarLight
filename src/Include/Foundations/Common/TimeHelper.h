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

#ifndef TIME_HELPER_4CB0836253464CABB9E84435B9BADCA1
#define TIME_HELPER_4CB0836253464CABB9E84435B9BADCA1

#pragma once

#include "Foundations/Common/ToDefine.h"

#include "Foundations/Interfaces/AgeUnit.h"

class CTimeHelper
{
public:
	/*
	 * Description
	 * ��ʽ��ʱ��
	 */
	static STRING_TYPE ToDateTimeText(SYSTEMTIME &sysTime);

	/*
	 * Description
	 * ��ʽ��ʱ��
	 */
	static STRING_TYPE ToDateText(SYSTEMTIME &sysTime);

	/*
	 * Description
	 * ת��ʱ��
	 */
	static SYSTEMTIME ParseTime(LPCTSTR strText, LPCTSTR strFormat = NormalDatetimeFormatText);

	/*
	 * Description
	 * ת��ʱ��
	 */
	static void ParseTime(LPCTSTR strText, SYSTEMTIME &sysTime, LPCTSTR strFormat = NormalDatetimeFormatText);

	/*
	 * Description
	 * ת��Dicomʱ��
	 */
	static void ParseDicomDateTime(LPCTSTR strDate, LPCTSTR strTime, SYSTEMTIME &sysTime);
	
	/*
	 * Description
	 * ת��Dicom��ʽʱ��
	 * @strDate YYYYMMDD
	 * @strTime HHmmss
	 * @return YYYY-MM-DD HH:mm:ss
	 */
	static STRING_TYPE FormatFromDicomDateTime(STRING_TYPE& strDate, STRING_TYPE& strTime);

	static SYSTEMTIME CalculateBirthDateFromAge(int nAge, AgeUnit unit = AgeUnit::Year, SYSTEMTIME *pSysTime = NULL_SYMBAL);

};

#endif

