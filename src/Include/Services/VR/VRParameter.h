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

#ifndef VR_PARAMETER_C2F61BB578504EC6A2554568355D1F52
#define VR_PARAMETER_C2F61BB578504EC6A2554568355D1F52

#pragma once

#include <Services/VR/ExportFlagDefine.h>


#include "Services/VR/DisplayPixelSegment.h"
#include "Foundations/Interfaces/ServiceParameterBase.h"

#include <vector>

class VR_SERVICEMPR_API CVRParameter : public CServiceParameterBase
{
public:
	CVRParameter();
	
	void Reset();

private:
	using CDisplayPixelSegments = std::vector<CDisplayPixelSegment>;

	AUTO_PROPERTY_REF(ColorSegments, CDisplayPixelSegments);
	AUTO_PROPERTY_REF(OpacitySegments, CDisplayPixelSegments);

	AUTO_PROPERTY(Ambient, double);
	AUTO_PROPERTY(Diffuse, double);
	AUTO_PROPERTY(Specular, double);
	AUTO_PROPERTY(SpecularPower, double);

};

#endif

