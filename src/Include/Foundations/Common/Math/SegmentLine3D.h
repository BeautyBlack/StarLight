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

#ifndef SEGMENT_LINE_3D_3D0F587BE1E240AD9588CB550414A9D8
#define SEGMENT_LINE_3D_3D0F587BE1E240AD9588CB550414A9D8

#pragma once

#include <Foundations/Common/Math/Point3D.h>

struct sctSegmentLine3D
{
	sctPoint3D begin;
	sctPoint3D end;

	sctSegmentLine3D() = default;

	sctSegmentLine3D(const sctSegmentLine3D &copySrc)
	{
		begin = copySrc.begin;
		end = copySrc.end;
	}

	sctSegmentLine3D& operator=(const sctSegmentLine3D &copySrc)
	{
		begin = copySrc.begin;
		end = copySrc.end;
		
		return *this;
	}
};

#endif
