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

#ifndef MATH_UTILITY_A6E3446F15FF4AA1AEADBF2F3776137C
#define MATH_UTILITY_A6E3446F15FF4AA1AEADBF2F3776137C

#pragma once
#include "Foundations/Common/ToDefine.h"

#include "Foundations/Common/Math/Point3D.h"
#include "Foundations/Common/Math/Normal3D.h"

#define _USE_MATH_DEFINES
#include <math.h>

struct sctPoint2D;

class CMathUtility
{
public:
	static inline double PI() { return M_PI; }
	
	/**
	 * 点是否在直线上
	 * @offset : 偏移量, 在 正负
	 */
	static bool IsHitLine(
		int x1, int y1,
		int x2, int y2,
		int p1, int p2,
		int offset = 5);

	static double ComputeDistance(double vector1[], double vector2[], int dim = 3);
	static double ComputeDistance(sctPoint3D &pt1, sctPoint3D &pt2);
	
	static double ComputeAngle2D(double vector1[], double vector2[]);

	static double ComputeAngle2D(double dsx, double dsy, double dex, double dey);

	static double ComputeAngle2D(
		sctPoint2D &p0,
		sctPoint2D &p1,
		sctPoint2D &p2,
		double rowPixelSpacing,
		double columnPixelSpacing);

	static double ComputeAngle3D(double vector1[], double vector2[]);

	static double ComputeAngle3D(double dsx, double dsy, double dsz, double dex, double dey, double dez);

	/**
	 * 向量vector 围绕 法线normalVector 和中心点center 旋转 angle角度
	 * @param vector 被旋转的向量
	 * @param normalVector 围绕中轴或法线
	 * @param dbAngle 旋转角度，单位：弧度
	 */
	static void RotateVector3(double vector[3], double normalVector[3], double dbAngle, double result[3]);
	
	/**
	 * 为了简化逻辑判断，要求matrix/vector都必须满足齐次性
	 * 矩阵顺序
	 *  \ 1 2 3 ... n  \
	 *  \ ..........n*2\
	 *  \..............\
	 *  \ ..........n*n\
	 */
	static void MatrixMultiplyVector(double *matrix, double *vector, int dim, double *result);

	static void VectorMulVector3X3(double vector[3], double vector2[3], double result[3]);

	static double VectorDotVector(double *vector, double *vector2, int nDim);

	static void VectorSubVector(double *result, double *vector1, double *vector2, int nDim);

	static void VectorMultiply(double* result, double* vector1, int nDim, double dbFacor);

	static void VectorAddVector(double* result, double* vector1, double *vector2, int nDim);

	static void NormalizeVector(double *vector, int dim, double *result);

	static void FormatImagePlanMatrix(sctNormal3D &normal1, sctNormal3D &normal2, double imgMatrix[4][4]);

	static sctPoint3D ComputePlanPosition(
		double xOffset, double yOffset, double zOffset,
		double orientPosition[3],
		double vectorX[3], double vectorY[3], double vectorZ[3],
		double spacingX = 1, double spacingY = 1, double spacingZ = 1);

	static int GetImageViewOrientation(double dImagePos[4][4]);

	static double CalcTwoVectorAngle(double dbVector[4], double dbVector2[4]);
};


#endif

