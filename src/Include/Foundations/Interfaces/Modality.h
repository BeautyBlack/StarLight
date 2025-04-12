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

#ifndef MODALITY_H_65AAA9F3DD0F4604A9C0893A4F9C24AD
#define MODALITY_H_65AAA9F3DD0F4604A9C0893A4F9C24AD

#pragma once

#include <Foundations/Common/ToDefine.h>

enum class Modality
{
	UnkownModality = 0,

	CT = 1,

	DX = 2,

	ES = 3,

	MR = 4,

	SR = 5,

	SC = 6,

	InvalidModality = 0xEFFFFF
};

inline Modality ParseTextToModality(const char* strModality)
{
	if (_stricmp(strModality, "CT") == 0)
		return Modality::CT;

	if (_stricmp(strModality, "DX") == 0)
		return Modality::DX;
	
	if (_stricmp(strModality, "ES") == 0)
		return Modality::ES;
	
	if (_stricmp(strModality, "MR") == 0)
		return Modality::MR;
	
	if (_stricmp(strModality, "SR") == 0)
		return Modality::SR;
	
	if (_stricmp(strModality, "SC") == 0)
		return Modality::SC;

	return Modality::UnkownModality;
}

inline const char* ModalityText(Modality modality)
{
	static STRING_TYPE CT = "CT",
		DX = "DX",
		ES = "ES",
		MR = "MR",
		SR = "SR",
		SC = "SC";

	switch (modality)
	{
	case Modality::CT:
		return CT.c_str();

	case Modality::DX:
		return DX.c_str();

	case Modality::ES:
		return ES.c_str();

	case Modality::MR:
		return MR.c_str();

	case Modality::SR:
		return SR.c_str();

	case Modality::SC:
		return SC.c_str();

	default:
		return EMPTY_STRING;
	}
}
#endif
