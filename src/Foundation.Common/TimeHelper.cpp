#include "pch.h"
#include <Foundations/Common/TimeHelper.h>

STRING_TYPE CTimeHelper::ToDateTimeText(SYSTEMTIME& sysTime)
{
	char strText[32] = { 0 };

	_snprintf_s(strText, sizeof(strText), _TRUNCATE, "%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	return strText;
}

STRING_TYPE CTimeHelper::ToDateText(SYSTEMTIME& sysTime)
{
	char strText[32] = { 0 };

	_snprintf_s(strText, sizeof(strText), _TRUNCATE, "%04d-%02d-%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay);

	return strText;
}

SYSTEMTIME CTimeHelper::ParseTime(LPCTSTR strText, LPCTSTR strFormat)
{
	int nYear = 0, nMonth = 0, nDay = 0, nHour = 0, nMinute = 0, nSecond = 0;

	sscanf_s(strText, strFormat, &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond);

	SYSTEMTIME result;

	result.wDayOfWeek = 0;
	result.wMilliseconds = 0;
	result.wYear = nYear;
	result.wMonth = nMonth;
	result.wDay = nDay;
	result.wHour = nHour;
	result.wMinute = nMinute;
	result.wSecond = nSecond;

	return result;
}

void CTimeHelper::ParseTime(LPCTSTR strDate, SYSTEMTIME& sysTime, LPCTSTR strFormat)
{
	char strYear[] = "YYYY", strMonth[] = "MM", strDay[] = "DD";
	char strHour[] = "HH", strMinute[] = "mm", strSecond[] = "ss";

	auto nIndex = 0;
	auto nLen = strlen(strDate);

	if (strDate != NULL_SYMBAL && nLen >= DicomDateFormatTextLength/*yyyyMMDD*/)
	{
		nIndex = -1;

		strYear[0] = strDate[++nIndex];
		strYear[1] = strDate[++nIndex];
		strYear[2] = strDate[++nIndex];
		strYear[3] = strDate[++nIndex];

		++nIndex;

		strMonth[0] = strDate[++nIndex];
		strMonth[1] = strDate[++nIndex];

		++nIndex;

		strDay[0] = strDate[++nIndex];
		strDay[1] = strDate[++nIndex];
	}

	if (nLen >= DicomDateFormatTextLength + DicomTimeFormatTextLength/*HHmmss*/)
	{
		++nIndex;

		strHour[0] = strDate[++nIndex];
		strHour[1] = strDate[++nIndex];

		++nIndex;

		strMinute[0] = strDate[++nIndex];
		strMinute[1] = strDate[++nIndex];

		++nIndex;

		strSecond[0] = strDate[++nIndex];
		strSecond[1] = strDate[++nIndex];
	}

	sysTime.wYear = atoi(strYear);
	sysTime.wMonth = atoi(strMonth);
	sysTime.wDay = atoi(strDay);
	sysTime.wHour = atoi(strHour);
	sysTime.wMinute = atoi(strMinute);
	sysTime.wSecond = atoi(strSecond);

}

void CTimeHelper::ParseDicomDateTime(LPCTSTR strDate, LPCTSTR strTime, SYSTEMTIME& sysTime)
{
	char strYear[] = "YYYY", strMonth[] = "MM", strDay[] = "DD";
	char strHour[] = "HH", strMinute[] = "mm", strSecond[] = "ss";

	auto nIndex = 0;

	if (strDate != NULL_SYMBAL && strlen(strDate) >= DicomDateFormatTextLength/*yyyyMMDD*/)
	{
		nIndex = -1;

		strYear[0] = strDate[++nIndex];
		strYear[1] = strDate[++nIndex];
		strYear[2] = strDate[++nIndex];
		strYear[3] = strDate[++nIndex];

		strMonth[0] = strDate[++nIndex];
		strMonth[1] = strDate[++nIndex];

		strDay[0] = strDate[++nIndex];
		strDay[1] = strDate[++nIndex];
	}
	
	if(strTime != NULL_SYMBAL && strlen(strTime) >= DicomTimeFormatTextLength/*HHmmss*/)
	{
		nIndex = -1;
		
		strHour[0] = strTime[++nIndex];
		strHour[1] = strTime[++nIndex];

		strMinute[0] = strTime[++nIndex];
		strMinute[1] = strTime[++nIndex];

		strSecond[0] = strTime[++nIndex];
		strSecond[1] = strTime[++nIndex];
	}

	sysTime.wYear = atoi(strYear);
	sysTime.wMonth = atoi(strMonth);
	sysTime.wDay = atoi(strDay);
	sysTime.wHour = atoi(strHour);
	sysTime.wMinute = atoi(strMinute);
	sysTime.wSecond = atoi(strSecond);
}

STRING_TYPE CTimeHelper::FormatFromDicomDateTime(STRING_TYPE& strDate, STRING_TYPE& strTime)
{
	char strResult[] = "YYYY-MM-DD 00:00:00";

	auto nIndex = -1;
	strResult[0] = strDate.at(++nIndex);
	strResult[1] = strDate.at(++nIndex);
	strResult[2] = strDate.at(++nIndex);
	strResult[3] = strDate.at(++nIndex);

	strResult[5] = strDate.at(++nIndex);
	strResult[6] = strDate.at(++nIndex);

	strResult[8] = strDate.at(++nIndex);
	strResult[9] = strDate.at(++nIndex);

	if(strTime.length() >= 6)
	{	
		nIndex = -1;
		strResult[11] = strTime.at(++nIndex);
		strResult[12] = strTime.at(++nIndex);

		strResult[14] = strTime.at(++nIndex);
		strResult[15] = strTime.at(++nIndex);

		strResult[17] = strTime.at(++nIndex);
		strResult[18] = strTime.at(++nIndex);
	}
	return strResult;
}

SYSTEMTIME CTimeHelper::CalculateBirthDateFromAge(int nAge, AgeUnit unit, SYSTEMTIME * pSysTime)
{
	CTime test = CTime::GetCurrentTime();
	
	if (pSysTime != NULL_SYMBAL)
		test = CTime(*pSysTime);
	
	SYSTEMTIME result;

	ZeroMemory(&result, sizeof(result));

	switch(unit)
	{
	case AgeUnit::Year:
		result.wYear = test.GetYear() - nAge;
		result.wMonth = test.GetMonth();
		result.wDay = test.GetDay();

		return result;

	case AgeUnit::Month:
		{
			CTimeSpan span(nAge * 30, 0, 0, 0);
			test = test - span;
			break;
		}

	case AgeUnit::Week:
		{
			CTimeSpan span(nAge * 7, 0, 0, 0);
			test = test - span;
			break;
		}

	case AgeUnit::Day:
		{
			CTimeSpan span(nAge, 0, 0, 0);
			test = test - span;
			break;
		}
	}

	result.wYear = test.GetYear();
	result.wMonth = test.GetMonth();
	result.wDay = test.GetDay();

	return result;
}
