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


#ifndef POINT_3D_F6FE7C9A03234785A444F8595D0E9B41
#define POINT_3D_F6FE7C9A03234785A444F8595D0E9B41

#pragma once

struct sctPoint3D
{
	double x;
	double y;
	double z;

	sctPoint3D()
	{
		x = y = z = 0;
	}

	sctPoint3D(double xt, double yt, double zt)
	{
		x = xt;
		y = yt;
		z = zt;
	}

	sctPoint3D(const sctPoint3D &copySrc)
	{
		x = copySrc.x;
		y = copySrc.y;
		z = copySrc.z;
	}

	sctPoint3D& operator=(const sctPoint3D& copySrc)
	{
		x = copySrc.x;
		y = copySrc.y;
		z = copySrc.z;

		return *this;
	}

	bool Equal(const sctPoint3D& copySrc) const
	{
		return abs(copySrc.x - x) < 0.001
			&& abs(copySrc.y - y) < 0.001
			&& abs(copySrc.z - z) < 0.001;
	}

	void LoadFrom(double xt, double yt, double zt)
	{
		x = xt;
		y = yt;
		z = zt;
	}
};

#endif
