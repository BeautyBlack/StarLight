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

#ifndef SERIALIZE_BASE_1D65E7C0303C4805B814E57E08D57C44
#define SERIALIZE_BASE_1D65E7C0303C4805B814E57E08D57C44

#pragma once

#include <Foundations/Common/ToDefine.h>

_declspec(novtable) class CSerializeBase
{
public:
	virtual STRING_TYPE SerializeTo() = 0;

	virtual bool DeserializeFrom(STRING_TYPE& text) = 0;

	virtual bool DeserializeFrom(const char* text) = 0;
};

#endif
