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

#ifndef POINT_2D_828B918CB9494FD2B6A3A7E08ABA3116
#define POINT_2D_828B918CB9494FD2B6A3A7E08ABA3116

#pragma once

struct sctPoint2D
{
	double x;
	double y;

	sctPoint2D()
	{
		x = y = 0;
	}

	sctPoint2D(double nx, double ny)
	{
		x = nx;
		y = ny;
	}

	sctPoint2D(const sctPoint2D& copySrc)
	{
		x = copySrc.x;
		y = copySrc.y;
	}

	sctPoint2D& operator=(const sctPoint2D& copySrc)
	{
		x = copySrc.x;
		y = copySrc.y;

		return *this;
	}

	void updateFrom(double nx, double ny)
	{
		x = nx;
		y = ny;
	}
};

#endif
