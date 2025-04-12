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

#ifndef PARONAMA_PARAMETER_44D64DE82B93478CA40B7D60C485A3FD
#define PARONAMA_PARAMETER_44D64DE82B93478CA40B7D60C485A3FD

#pragma once

#include "Foundations/Interfaces/ServiceParameterBase.h"

#include "Services/Paronama/ExportFlagDefine.h"

#include <Foundations/Common/Math/Point3D.h>
#include <vector>

class PARONAMA_SERVICEMPR_API CParonamaParameter : public CServiceParameterBase
{
private:
	using PARONAMA_PATH_POINTS = std::vector<sctPoint3D>;
	
	AUTO_PROPERTY_REF(ParonamaPath, PARONAMA_PATH_POINTS);
	
	/*
	 * ԭʼ���е�Pixel Spacing X
	 */
	AUTO_PROPERTY_REF(SourcePixelSpacingX, double);

	/*
	 * ԭʼ���е�Pixel Spacing Y
	 */
	AUTO_PROPERTY_REF(SourcePixelSpacingY, double);

	/*
	 * ԭʼ���еĲ���
	 */
	AUTO_PROPERTY(SourceSpacingBetweenSlice, double);

	/**
	 * ���ؿ�
	 */
	AUTO_PROPERTY_REF(VolumeWidth, int);

	/**
	 * ���س�
	 */
	AUTO_PROPERTY_REF(VolumeHeight, int);

	/**
	 * ���ظ�
	 */
	AUTO_PROPERTY_REF(VolumeSliceCount, int);
};

#endif

