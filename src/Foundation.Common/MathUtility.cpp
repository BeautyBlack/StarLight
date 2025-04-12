#include "pch.h"
#include "MathUtility.h"

#include "Foundations/Common/Math/Point2D.h"

double CMathUtility::ComputeDistance(double vector1[], double vector2[], int dim /* = 3*/)
{
	if (vector1 == NULL_SYMBAL || vector2 == NULL_SYMBAL)
		return 0;

	auto sum = 0.0;

	for (auto i = 0; i < dim; i++)
		sum += (vector1[i] - vector2[i]) * (vector1[i] - vector2[i]);

	return sqrt(sum);
}

double CMathUtility::ComputeDistance(sctPoint3D &pt1, sctPoint3D &pt2)
{
	auto sum = 0.0;

	sum += (pt1.x - pt2.x) * (pt1.x - pt2.x);
	sum += (pt1.y - pt2.y) * (pt1.y - pt2.y);
	sum += (pt1.z - pt2.z) * (pt1.z - pt2.z);

	return sqrt(sum);
}

double CMathUtility::ComputeAngle2D(double vector1[], double vector2[])
{
	return ComputeAngle2D(vector1[0], vector1[1], vector2[0], vector2[1]);
}

double CMathUtility::ComputeAngle2D(double dsx, double dsy, double dex, double dey)
{
	double cosfi = dsx * dex + dsy * dey;
	const double norm = (dsx * dsx + dsy * dsy) * (dex * dex + dey * dey);

	cosfi /= sqrt(norm);

	if (cosfi >= 1.0 || cosfi <= -1.0)
		return 0;

	return acos(cosfi);
}

double CMathUtility::ComputeAngle2D(
	sctPoint2D &p0,
	sctPoint2D &p1,
	sctPoint2D &p2,
	double rowPixelSpacing,
	double columnPixelSpacing)
{
	// 中心点与起点
	auto p01 = sqrt(pow((p0.x - p1.x) * columnPixelSpacing, 2) + pow((p0.y - p1.y) * rowPixelSpacing, 2));
	// 中心点与终点
	auto p02 = sqrt(pow((p0.x - p2.x) * columnPixelSpacing, 2) + pow((p0.y - p2.y) * rowPixelSpacing, 2));
	// 起点与终点
	auto p12 = sqrt(pow((p1.x - p2.x) * columnPixelSpacing, 2) + pow((p1.y - p2.y) * rowPixelSpacing, 2));

	// 计算弧度
	auto dh = acos(
		(pow(p01, 2) + pow(p02, 2) - pow(p12, 2)) / (2 * p01 * p02)
	);

	// 计算方向 AB与AC叉乘求出逆时针还是顺时针旋转
	auto direction =
		(p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x);

	return dh * (direction > 0 ? 1 : -1);
}

double CMathUtility::ComputeAngle3D(double vector1[], double vector2[])
{
	return ComputeAngle3D(vector1[0], vector1[1], vector1[2],
		vector2[0], vector2[1], vector2[2]);
}

double CMathUtility::ComputeAngle3D(
	double dsx, double dsy, double dsz,
	double dex, double dey, double dez)
{
	auto dbProduct = dsx * dex + dsy * dey + dsz * dez;
	auto dbTmpI = dsx * dsx + dsy * dsy + dsz * dsz;
	auto dbTmpS = dex * dex + dey * dey + dez * dez;

	if (abs(dbTmpI) < 0.0001 || abs(dbTmpS) < 0.0001)
		return acos(0.0);
	
	dbTmpI = sqrt(dbTmpI);
	dbTmpS = sqrt(dbTmpS);
	dbProduct = dbProduct / dbTmpI / dbTmpS;
	
	if (dbProduct > 1)
		dbProduct = 1;
	
	if (dbProduct < -1)
		dbProduct = -1;
	
	return acos(dbProduct);
}

void CMathUtility::RotateVector3(
	double vector[3], double normalVector[3], double dbAngle, double result[3])
{
	auto c = cos(dbAngle);
	auto s = sin(dbAngle);

	double matrix[16] = { 0 };

	matrix[4 * 0 + 0] = c + (1 - c) * normalVector[0] * normalVector[0];
	matrix[4 * 1 + 0] = (1 - c) * normalVector[0] * normalVector[1] + s * normalVector[2];
	matrix[4 * 2 + 0] = (1 - c) * normalVector[2] * normalVector[0] - s * normalVector[1];
	matrix[4 * 3 + 0] = 0;

	matrix[4 * 0 + 1] = (1 - c) * normalVector[1] * normalVector[0] - s * normalVector[2];
	matrix[4 * 1 + 1] = c + (1 - c) * normalVector[1] * normalVector[1];
	matrix[4 * 2 + 1] = (1 - c) * normalVector[1] * normalVector[2] + s * normalVector[0];
	matrix[4 * 3 + 1] = 0;

	matrix[4 * 0 + 2] = (1 - c) * normalVector[2] * normalVector[0] + s * normalVector[1];
	matrix[4 * 1 + 2] = (1 - c) * normalVector[2] * normalVector[1] - s * normalVector[0];
	matrix[4 * 2 + 2] = c + (1 - c) * normalVector[2] * normalVector[2];
	matrix[4 * 3 + 2] = 0;

	matrix[4 * 0 + 3] = 0;
	matrix[4 * 1 + 3] = 0;
	matrix[4 * 2 + 3] = 0;
	matrix[4 * 3 + 3] = 1;

	double t[3];
	t[0] = vector[0];
	t[1] = vector[1];
	t[2] = vector[2];

	return MatrixMultiplyVector(matrix,
		t,
		3,
		(double*)result);
}

void CMathUtility::MatrixMultiplyVector(double* matrix, double* vector, int dim, double* result)
{
	if (matrix == NULL_SYMBAL || vector == NULL_SYMBAL)
		return;

	for (auto i = 0; i < dim; i++) {
		double sum = 0;
		for (auto k = 0; k < dim; k++) {
			sum += matrix[i * dim + k] * vector[k];
		}

		result[i] = sum;
	}
}

bool CMathUtility::IsHitLine(int x1, int y1, int x2, int y2, int p1, int p2, int offset)
{
	const auto a = y1 - y2;
	const auto b = x2 - x1;
	const auto c = x1 * y2 - x2 * y1;

	if (abs(a) < 0.00001 && abs(b) < 0.00001)
		return false;

	return offset >=
		abs(a * p1 + b * p2 + c)
		/ sqrt(a * a + b * b);
}

void CMathUtility::VectorMulVector3X3(double vector[3], double vector2[3], double result[3])
{
	if (vector == NULL_SYMBAL || vector2 == NULL_SYMBAL)
		return;

	double imgMatrix[3] = {0};

	imgMatrix[0] = vector[1] * vector2[2] - vector[2] * vector2[1]; // ZX
	imgMatrix[1] = -(vector[0] * vector2[2] - vector[2] * vector2[0]); // ZY
	imgMatrix[2] = vector[0] * vector2[1] - vector[1] * vector2[0]; // ZZ

	NormalizeVector(imgMatrix, 3, result);
}

double CMathUtility::VectorDotVector(
	double* vector1, double* vector2, int nDim)
{
	if (vector1 == NULL_SYMBAL
		|| vector2 == NULL_SYMBAL
		|| nDim < 1)
		return 0;
	
	auto result = 0.0;

	for (auto i = 0; i < nDim; ++i)
		result += vector1[i] * vector2[i];
	
	return result;
}

void CMathUtility::VectorSubVector(double* result, double* vector1, double* vector2, int nDim)
{
	if (result == NULL_SYMBAL
		|| vector1 == NULL_SYMBAL
		|| vector2 == NULL_SYMBAL
		|| nDim < 1)
		return;
	
	for (auto i = 0; i < nDim; ++i)
		result[i] = vector1[i] - vector2[i];
}

void CMathUtility::VectorMultiply(double* result, double* vector1, int nDim, double dbFacor)
{
	if (result == NULL_SYMBAL || vector1 == NULL_SYMBAL || nDim < 1)
		return;

	for (int i = 0; i < nDim; ++i)
		result[i] = vector1[i] * dbFacor;
}

void CMathUtility::VectorAddVector(double* result, double* vector1, double* vector2, int nDim)
{
	if (result == NULL_SYMBAL
		|| vector1 == NULL_SYMBAL
		|| vector2 == NULL_SYMBAL
		|| nDim < 1)
		return;

	for (auto i = 0; i < nDim; ++i)
		result[i] = vector1[i] + vector2[i];
}


void CMathUtility::NormalizeVector(double* vector, int dim, double* result)
{
	double sum = 0;

	for (auto i = 0; i < dim; ++i)
		sum += vector[i] * vector[i];

	sum = sqrt(sum);
	sum = sum > 0.0001 ? sum : 1;

	for (auto i = 0; i < dim; i++)
		result[i] = vector[i] / sum;
}

void CMathUtility::FormatImagePlanMatrix(sctNormal3D& rowVector, sctNormal3D& colVector, double imgMatrix[4][4])
{
	imgMatrix[0][3] = 0;
	imgMatrix[1][3] = 0;
	imgMatrix[2][3] = 0;
	imgMatrix[3][3] = 1;

	imgMatrix[0][0] = rowVector.x; // XX
	imgMatrix[1][0] = rowVector.y; // XY
	imgMatrix[2][0] = rowVector.z; // XZ
	imgMatrix[3][0] = 0;

	imgMatrix[0][1] = colVector.x; // YX
	imgMatrix[1][1] = colVector.y; // YY
	imgMatrix[2][1] = colVector.z; // YZ
	imgMatrix[3][1] = 0;

	imgMatrix[0][2] = rowVector.y * colVector.z - rowVector.z * colVector.y; // ZX
	imgMatrix[1][2] = -(rowVector.x * colVector.z - rowVector.z * colVector.x); // ZY
	imgMatrix[2][2] = rowVector.x * colVector.y - rowVector.y * colVector.x; // ZZ
	imgMatrix[3][2] = 0;

	const auto dbSqr = sqrt(imgMatrix[0][2] * imgMatrix[0][2] + imgMatrix[1][2] * imgMatrix[1][2] + imgMatrix[2][2] * imgMatrix[2][2]);
	if (abs(dbSqr) > 0.0001) {
		imgMatrix[0][2] /= dbSqr;
		imgMatrix[1][2] /= dbSqr;
		imgMatrix[2][2] /= dbSqr;
	}
}

sctPoint3D CMathUtility::ComputePlanPosition(
	double xOffset, double yOffset, double zOffset,
	double orientPosition[3],
	double vectorX[3], double vectorY[3], double vectorZ[3],
	double spacingX /*= 1*/, double spacingY /*= 1*/, double spacingZ /*= 1*/)
{
	return sctPoint3D(
		vectorX[0] * spacingX * xOffset + vectorY[0] * spacingY * yOffset + vectorZ[0] * spacingZ * zOffset + orientPosition[0],
			vectorX[1] * spacingX * xOffset + vectorY[1] * spacingY * yOffset + vectorZ[1] * spacingZ * zOffset + orientPosition[1],
			vectorX[2] * spacingX * xOffset + vectorY[2] * spacingY * yOffset + vectorZ[2] * spacingZ * zOffset + orientPosition[2]
	);
};

int CMathUtility::GetImageViewOrientation(double dImagePos[4][4])
{
	const double dbHalfPI = PI() * 0.5f;
	const double db1To4PI = PI() * 0.25f;
	const double db3To4PI = PI() * 0.75f;

	double dTestSag[4] = { 0 };
	double dTestCor[4] = { 0 };
	double dTestTra[4] = { 0 };
	double dTestImg[4] = { 0 };

	double dSag = 0;
	double dCor = 0;
	double dTra = 0;

	dTestImg[0] = dImagePos[0][2];
	dTestImg[1] = dImagePos[1][2];
	dTestImg[2] = dImagePos[2][2];

	dTestSag[0] = -1;
	dTestTra[2] = 1;
	dTestCor[1] = 1;

	dSag = CalcTwoVectorAngle(dTestImg, dTestSag);
	dTra = CalcTwoVectorAngle(dTestImg, dTestTra);
	dCor = CalcTwoVectorAngle(dTestImg, dTestCor);

	dSag = dSag > dbHalfPI ? PI() - dSag : dSag;
	dCor = dCor > dbHalfPI ? PI() - dCor : dCor;
	dTra = dTra > dbHalfPI ? PI() - dTra : dTra;

	double dMin = min(min(dSag, dTra), dCor);

	double dbXAngle = 0;
	double dbYAngle = 0;
	double dbXAngle2 = 0;

	double dbXVector[4] = { 0 }; // 图像Orientation X方向向量
	double dbYVector[4] = { 0 }; // 图像Orientation Y方向向量
	double dbXUpVector[4] = { 0 }; // 主方向（Sag/Tra/Cor）X正方向
	double dbYUpVector[4] = { 0 }; // 主方向Y正方向

	int nReturnValue = 0;

	dbXVector[0] = dImagePos[0][0];
	dbXVector[1] = dImagePos[1][0];
	dbXVector[2] = dImagePos[2][0];

	dbYVector[0] = dImagePos[0][1];
	dbYVector[1] = dImagePos[1][1];
	dbYVector[2] = dImagePos[2][1];

	bool bXPhase = false;

	if (dMin == dSag) // A H P F
	{
		dbXUpVector[0] = 0;
		dbXUpVector[1] = 1;
		dbXUpVector[2] = 0;

		dbYUpVector[0] = 0;
		dbYUpVector[1] = 0;
		dbYUpVector[2] = -1;

		dbXVector[0] = 0;
		dbYVector[0] = 0;

		dbXAngle = CalcTwoVectorAngle(dbXVector, dbXUpVector);
		dbXAngle2 = CalcTwoVectorAngle(dbXVector, dbYUpVector);

		if (dbXAngle < db1To4PI/* + 1*PI/180*/ || dbXAngle > db3To4PI/*-PI/180.0*/)
			bXPhase = true;

		// X方向贴近于主方向X轴
		if (bXPhase)
		{
			dbYAngle = CalcTwoVectorAngle(dbYVector, dbYUpVector);

			// 与X轴正方向相反
			if (dbXAngle > dbHalfPI)
			{
				if (dbYAngle > dbHalfPI) // 与Y轴正方向反向
					nReturnValue = 11; // P F A H
				else // 与Y轴正方向同向
					nReturnValue = 12; // P H A F
			}
			else // 与X轴正方向同向
			{
				if (dbYAngle > dbHalfPI) // 与Y轴正方向反向
					nReturnValue = 13; // A F P H
				else // 与Y轴正方向同向
					nReturnValue = 14; // A H P F
			}
		}
		else // 图像X方向贴近于主方向Y轴
		{
			dbYAngle = CalcTwoVectorAngle(dbYVector, dbXUpVector);

			// 与Y轴正方向相反
			if (dbXAngle2 > dbHalfPI)
			{
				// 与X轴正方向相反
				if (dbYAngle > dbHalfPI)
					nReturnValue = 18; // F P H A
				else // 与X轴正方向同向
					nReturnValue = 16; // F A H P
			}
			else // 与Y轴正方向同向
			{
				// 与X轴正方向相反
				if (dbYAngle > dbHalfPI)
					nReturnValue = 17; // H P F A
				else // 与X轴正方向同向
					nReturnValue = 15; // H A F P
			}
		}
	}
	else if (dMin == dTra) // R A L P
	{
		dbXUpVector[0] = 1;
		dbXUpVector[1] = 0;
		dbXUpVector[2] = 0;

		dbYUpVector[0] = 0;
		dbYUpVector[1] = 1;
		dbYUpVector[2] = 0;

		dbXVector[2] = 0;
		dbYVector[2] = 0;

		dbXAngle = CalcTwoVectorAngle(dbXVector, dbXUpVector);
		dbXAngle2 = CalcTwoVectorAngle(dbXVector, dbYUpVector);

		if (dbXAngle < db1To4PI/* + 1*PI/180*/ || dbXAngle > db3To4PI/*-PI/180.0*/)
			bXPhase = true;

		// 图像X方向贴近主方向X轴
		if (bXPhase)
		{
			dbYAngle = CalcTwoVectorAngle(dbYVector, dbYUpVector);

			// 与X轴正方向反向
			if (dbXAngle > dbHalfPI)
			{
				// 图像Y方向与主方向Y轴正方向反向
				if (dbYAngle > dbHalfPI)
					nReturnValue = 21; // L P R A
				else // 图像Y方向与主方向Y轴正方向同向
					nReturnValue = 22; // L A R P
			}
			else // 与X轴正方向同向
			{
				// 图像Y方向与主方向Y轴正方向反向
				if (dbYAngle > dbHalfPI)
					nReturnValue = 23; // R P L A
				else // 图像Y方向与主方向Y轴正方向同向
					nReturnValue = 24; // R A L P
			}
		}
		else // 图像X方向贴近主方向Y轴
		{
			dbYAngle = CalcTwoVectorAngle(dbYVector, dbXUpVector);

			// 与主方向Y轴正方向反向
			if (dbXAngle2 > dbHalfPI)
			{
				// 图像Y方向与主方向X轴正方向反向
				if (dbYAngle > dbHalfPI)
					nReturnValue = 25; // P L A R
				else // 图像Y方向与主方向X轴正方向同向
					nReturnValue = 27; // P R A L
			}
			else // 与主方向Y轴正方向同向
			{
				// 图像Y方向与主方向X轴正方向反向
				if (dbYAngle > dbHalfPI)
					nReturnValue = 26; // A L P R
				else // 图像Y方向与主方向X轴正方向同向
					nReturnValue = 28; // A R P L
			}
		}
	}
	else // Cor - R H L F
	{
		dbXUpVector[0] = 1;
		dbXUpVector[1] = 0;
		dbXUpVector[2] = 0;

		dbYUpVector[0] = 0;
		dbYUpVector[1] = 0;
		dbYUpVector[2] = -1;

		dbXVector[1] = 0;
		dbYVector[1] = 0;

		dbXAngle = CalcTwoVectorAngle(dbXVector, dbXUpVector);
		dbXAngle2 = CalcTwoVectorAngle(dbXVector, dbYUpVector);

		if (dbXAngle < db1To4PI/* + 1*PI/180*/ || dbXAngle > db3To4PI/*-PI/180.0*/)
			bXPhase = true;

		// 图像X方向贴近于主方向X轴
		if (bXPhase)
		{
			dbYAngle = CalcTwoVectorAngle(dbYVector, dbYUpVector);

			// X方向与X轴正方向反向
			if (dbXAngle > dbHalfPI)
			{
				// Y方向与Y轴正方向反向
				if (dbYAngle > dbHalfPI)
					nReturnValue = 31; // L F R H
				else // Y方向与Y轴正方向同向
					nReturnValue = 32; // L H R F
			}
			else // X方向与X轴正方向同向
			{
				// Y方向与Y轴正方向反向
				if (dbYAngle > dbHalfPI)
					nReturnValue = 33; // R F L H
				else // Y方向与Y轴正方向反向
					nReturnValue = 34; // R H L F
			}
		}
		else // 图像X方向贴近于主方向Y轴
		{
			dbYAngle = CalcTwoVectorAngle(dbYVector, dbXUpVector);

			// X方向与Y轴正方向反向
			if (dbXAngle2 > dbHalfPI)
			{
				// Y方向与X轴反向
				if (dbYAngle > dbHalfPI)
					nReturnValue = 38; // F L H R
				else // Y方向与X轴同向
					nReturnValue = 36; // F R H L
			}
			else // X方向与Y轴正方向同向
			{
				// Y方向与X轴反向
				if (dbYAngle > dbHalfPI)
					nReturnValue = 37; // H L F R
				else // Y方向与X轴同向
					nReturnValue = 35; // H R F L
			}
		}
	}

	return nReturnValue;
}

double CMathUtility::CalcTwoVectorAngle(double dbVector[4], double dbVector2[4])
{
	double dbProduct = dbVector[0] * dbVector2[0] + dbVector[1] * dbVector2[1] + dbVector[2] * dbVector2[2];

	double dTempI = dbVector[0] * dbVector[0] + dbVector[1] * dbVector[1] + dbVector[2] * dbVector[2];
	double dTempS = dbVector2[0] * dbVector2[0] + dbVector2[1] * dbVector2[1] + dbVector2[2] * dbVector2[2];

	if (dTempI == 0 || dTempS == 0)
		return acos(0.0f);

	dTempI = sqrt(dTempI);
	dTempS = sqrt(dTempS);

	dbProduct = dbProduct / dTempI / dTempS;

	return acos(dbProduct)/* * 180.0 / PI*/;
}
