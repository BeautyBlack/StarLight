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

#ifndef SEX_H_D452DC1529AE4F68B09A20EEE61B9CFD
#define SEX_H_D452DC1529AE4F68B09A20EEE61B9CFD

#pragma once

#include <Foundations/Common/ToDefine.h>

/*
 * Description
 * ÐÔ±ð
 */
enum class Sex
{
	UnkownSex = 0,

	Male = 1,

	Femal = 2,

	Other = 3,

	InvalidSex = 0xEFFFFF
};

inline const char* SexText(Sex sex)
{
	switch(sex)
	{
	case Sex::Male:
		return "M";

	case Sex::Femal:
		return "F";

	case Sex::Other:
		return "O";

	default:
		return EMPTY_STRING;
	}
}

inline Sex ParseTextToSex(const char* strText)
{
	if (strText == NULL_SYMBAL || strText[0] == '\0')
		return Sex::Other;

	if (strText[0] == 'M' || strText[0] == 'm')
		return Sex::Male;
	
	if (strText[0] == 'F' || strText[0] == 'f')
		return Sex::Femal;
	
	if (strText[0] == 'O' || strText[0] == 'o')
		return Sex::Other;
	
	if (strstr(strText, "ÄÐ"))
		return Sex::Male;
	
	if (strstr(strText, "Å®"))
		return Sex::Femal;
	
	return Sex::Other;
}

#endif
