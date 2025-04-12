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

#ifndef AGE_UNIT_11E102342F2E46AEA56A15311D33FEB7
#define AGE_UNIT_11E102342F2E46AEA56A15311D33FEB7

#pragma once

#include <Foundations/Common/ToDefine.h>

/*
 * Description
 * 年龄单位
 */
enum class AgeUnit
{
	UnkownAgeUnit = 0,

	/*
	 * 岁
	 */
	Year = 1,

	/*
	 * 月
	 */
	Month = 2,

	/*
	 * 周
	 */
	Week = 3,

	/*
	 * 日
	 */
	Day = 4,

	InvalidAgeUnit = 0xEFFFF
};

inline const char* AgeUnitText(AgeUnit ageUnit)
{
	switch (ageUnit)
	{
	case AgeUnit::Year:
		return "岁";

	case AgeUnit::Month:
		return "月";

	case AgeUnit::Week:
		return "周";

	case AgeUnit::Day:
		return "天";

	default:
		return EMPTY_STRING;
	}
}

#endif 
