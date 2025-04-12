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

#ifndef DISPLAY_PIXEL_SEGMENT_400D08284C41447E93D7C09859A7B1C9
#define DISPLAY_PIXEL_SEGMENT_400D08284C41447E93D7C09859A7B1C9

#pragma once


#include <Services/VR/ExportFlagDefine.h>
#include <Foundations/Common/ToDefine.h>

class VR_SERVICEMPR_API CDisplayPixelSegment
{
public:
	CDisplayPixelSegment();
	CDisplayPixelSegment(const CDisplayPixelSegment& copySrc);
	
private:
	AUTO_PROPERTY(Pixel, int);
	AUTO_PROPERTY(Value, double);
	
	AUTO_PROPERTY(Midpoint, double);
	AUTO_PROPERTY(Sharpness, double);
};

#endif

