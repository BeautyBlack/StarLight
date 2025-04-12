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

#ifndef Normal_3D_E5C1500310AA4DFB8184F6947818D6A4
#define Normal_3D_E5C1500310AA4DFB8184F6947818D6A4

#pragma once

struct sctNormal3D
{
	double x;
	double y;
	double z;

	sctNormal3D()
	{
		x = y = 0;
		z = 1;
	}

	sctNormal3D(double xt, double yt, double zt)
	{
		x = xt;
		y = yt;
		z = zt;
	}

	sctNormal3D(const sctNormal3D& copySrc)
	{
		x = copySrc.x;
		y = copySrc.y;
		z = copySrc.z;
	}

	sctNormal3D& operator=(const sctNormal3D& copySrc)
	{
		x = copySrc.x;
		y = copySrc.y;
		z = copySrc.z;

		return *this;
	}

	void Update(double xt, double yt, double zt)
	{
		x = xt;
		y = yt;
		z = zt;
	}
	
	void Update(double vectorT[3])
	{
		x = vectorT[0];
		y = vectorT[1];
		z = vectorT[2];
	}

	void ToArray(double destVector[3])
	{
		destVector[0] = x;
		destVector[1] = y;
		destVector[2] = z;
	}

	bool Equal(const sctNormal3D& copySrc) const
	{
		return abs(copySrc.x - x) < 0.001
			&& abs(copySrc.y - y) < 0.001
			&& abs(copySrc.z - z) < 0.001;
	}
};

#endif
