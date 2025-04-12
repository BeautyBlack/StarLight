#include "stdafx.h"
#include "ViewModels/ImageFrameViewModel/OriginalToViewportTransformMatrix.h"


#include <cmath>
#include <algorithm>

#include "Foundations/Common/ToDefine.h"

COriginalToViewportTransformMatrix::COriginalToViewportTransformMatrix(
	double viewSpaceX, double viewSpaceY,
	double displayImgX, double displayImgY,
	double imgWidth, double imgHeight)
{
	SetImageFactor(1);

	ZeroMemory(_transOptoVp, sizeof(_transOptoVp));
	ZeroMemory(_transVptoOp, sizeof(_transVptoOp));
	
	_transOptoVp[0][0] = displayImgX / imgWidth;
	_transOptoVp[1][1] = displayImgY / imgHeight;
	_transOptoVp[0][2] = (viewSpaceX - imgWidth/*displayImgX*/) / 2;
	_transOptoVp[1][2] = (viewSpaceY - imgHeight/*displayImgY*/) / 2;
	_transOptoVp[2][2] = 1;

	ComputeVptoOpMatrix();

	_viewSize.Width = viewSpaceX;
	_viewSize.Height = viewSpaceY;

	_displayImgSize.Width = displayImgX;
	_displayImgSize.Height = displayImgY;

	_imgSize.Width = imgWidth;
	_imgSize.Height = imgHeight;

	_orientImageVertex[0].X = 0;
	_orientImageVertex[0].Y = 0;

	_orientImageVertex[1].X = imgWidth;
	_orientImageVertex[1].Y = 0;

	_orientImageVertex[2].X = 0;
	_orientImageVertex[2].Y = imgHeight;

	_orientImageVertex[3].X = imgWidth;
	_orientImageVertex[3].Y = imgHeight;

	SetImageFactor(min(_transOptoVp[0][0], _transOptoVp[1][1]));
	
	_imgHeaderPoint = GetHeaderPoint();
}

COriginalToViewportTransformMatrix::COriginalToViewportTransformMatrix(
	const COriginalToViewportTransformMatrix &copyMatrix)
{
	CopyMatrix(_transOptoVp, const_cast<double(*)[3]>(copyMatrix._transOptoVp));
	CopyMatrix(_transVptoOp, const_cast<double(*)[3]>(copyMatrix._transVptoOp));

	_viewSize.Width = copyMatrix._viewSize.Width;
	_viewSize.Height = copyMatrix._viewSize.Height;

	_displayImgSize.Width = copyMatrix._displayImgSize.Width;
	_displayImgSize.Height = copyMatrix._displayImgSize.Height;

	_imgSize.Width = copyMatrix._imgSize.Width;
	_imgSize.Height = copyMatrix._imgSize.Height;

	for (int i = 0; i < 4; i++)
	{
		_orientImageVertex[i].X = copyMatrix._orientImageVertex[i].X;
		_orientImageVertex[i].Y = copyMatrix._orientImageVertex[i].Y;
	}

	_imgHeaderPoint.X = copyMatrix._imgHeaderPoint.X;
	_imgHeaderPoint.Y = copyMatrix._imgHeaderPoint.Y;
}

/// <summary>
/// 坐标点转换[从图像坐标转换到视野显示坐标]
/// </summary>
/// <param name="transPoint">需要转换的点[Array]</param>
/// <returns>转换后的点[Array[</returns>
void COriginalToViewportTransformMatrix::TransOpToVp(
	PointF transPoint[], int nTransPointCount, PointF resultPoint[])
{
	//auto retPt = new PointF[nTransPointCount];

	for (int i = 0; i < nTransPointCount; i++)
	{
		resultPoint[i].X = _transOptoVp[0][0] * transPoint[i].X
			+ _transOptoVp[0][1] * transPoint[i].Y
			+ _transOptoVp[0][2] * 1;

		resultPoint[i].Y = _transOptoVp[1][0] * transPoint[i].X
			+ _transOptoVp[1][1] * transPoint[i].Y
			+ _transOptoVp[1][2] * 1;

		//retPt[i] = new PointF(dbTmpX, dbTmpY);
	}

	//return retPt;
}

/// <summary>
/// 坐标点转换[从视野显示坐标转换到图像坐标]
/// </summary>
/// <param name="transPoint">需要转换的点[Array]</param>
/// <returns>转换后的点[Array[</returns>
void COriginalToViewportTransformMatrix::TransVpToOp(
	PointF transPoint[], int nTransPointCount, PointF resultPoint[])
{
	//PointF* retPt = new PointF[nTransPointCount];

	for (int i = 0; i < nTransPointCount; i++)
	{
		resultPoint[i].X = _transVptoOp[0][0] * transPoint[i].X
			+ _transVptoOp[0][1] * transPoint[i].Y
			+ _transVptoOp[0][2] * 1;

		resultPoint[i].Y = _transVptoOp[1][0] * transPoint[i].X
			+ _transVptoOp[1][1] * transPoint[i].Y
			+ _transVptoOp[1][2] * 1;

		//retPt[i] = new PointF((float)dbTmpX, (float)dbTmpY);
	}

	//return retPt;
}

/// <summary>
/// 图像框Viewport变大、变小
/// Viewport发生变化后， Display Image Size 也将按照viewport变化的比例对应发生变化
/// </summary>
/// <param name="newViewSize"></param>
void COriginalToViewportTransformMatrix::SizeChange(SizeF newViewSize)
{
	auto dbFactorX = newViewSize.Width / _viewSize.Width;
	auto dbFactorY = newViewSize.Height / _viewSize.Height;

	SizeF newDisplayImgSize(_displayImgSize);// = new SizeF(_displayImgSize);
	newDisplayImgSize.Width *= dbFactorX;
	newDisplayImgSize.Height *= dbFactorY;

	_transOptoVp[0][0] *= dbFactorX;
	_transOptoVp[0][1] *= dbFactorY;

	_transOptoVp[1][0] *= dbFactorX;
	_transOptoVp[1][1] *= dbFactorY;

	_transOptoVp[0][2] = (
		(_transOptoVp[0][2]
			- (_viewSize.Width - _displayImgSize.Width) / 2) * dbFactorX)
	+ (newViewSize.Width - newDisplayImgSize.Width) / 2;
	_transOptoVp[1][2] = ((_transOptoVp[1][2] - (_viewSize.Height - _displayImgSize.Height) / 2) * dbFactorY) + (newViewSize.Height - newDisplayImgSize.Height) / 2;

	_transOptoVp[2][2] = 1;

	ComputeVptoOpMatrix();

	_viewSize.Width = newViewSize.Width;
	_viewSize.Height = newViewSize.Height;

	_displayImgSize.Width = newDisplayImgSize.Width;
	_displayImgSize.Height = newDisplayImgSize.Height;

	SetImageFactor(min(_transOptoVp[0][0], _transOptoVp[1][1]));
	_imgHeaderPoint = GetHeaderPoint();
}

/// <summary>
/// 缩放
/// </summary>
/// <param name="dbFactor">缩放比例</param>
void COriginalToViewportTransformMatrix::Scale(double dbFactor)
{
	if (dbFactor <= 0)
		return;

	double scaleMatrix[3][3] = { { dbFactor, 0, 0}, {0, dbFactor, 0}, {0, 0, 1} };

	ComputeOpToVpMatrix(scaleMatrix);

	ComputeVptoOpMatrix();

	_displayImgSize.Width *= dbFactor;
	_displayImgSize.Height *= dbFactor;

	_imgHeaderPoint = GetHeaderPoint();
}

/// <summary>
/// 平移
/// </summary>
/// <param name="dbOffsetX">X轴偏移量</param>
/// <param name="dbOffsetY">Y轴偏移量</param>
void COriginalToViewportTransformMatrix::Move(double dbOffsetX, double dbOffsetY)
{
	if (abs(dbOffsetX) < MinDoubleValue && abs(dbOffsetY) < MinDoubleValue)
		return;

	double offsetMatrix[3][3] = { {1, 0, dbOffsetX}, {0, 1, dbOffsetY}, {0, 0, 1} };
	double origPoint[3] = { _transOptoVp[0][2], _transOptoVp[1][2], _transOptoVp[2][2] };
	double tmp[3] = { 0, 0, 0 };

	MatrixMutiplyVector(offsetMatrix, origPoint, tmp);

	_transOptoVp[0][2] = tmp[0];
	_transOptoVp[1][2] = tmp[1];
	_transOptoVp[2][2] = tmp[2];

	ComputeVptoOpMatrix();

	_imgHeaderPoint = GetHeaderPoint();
}

/// <summary>
/// 旋转
/// </summary>
/// <param name="dbAngle">旋转角度 范围 : [-360 , 360]</param>
void COriginalToViewportTransformMatrix::Rotate(double dbAngle)
{
	auto dbDestAngle = (int)dbAngle % 360;

	if (abs(dbDestAngle) < MinDoubleValue)
		return;

	double degree = dbAngle * 3.1415926 / 180.0;
	double rotateMatrix[3][3] = { {cos(degree), -sin(degree), 0}, {sin(degree), cos(degree), 0}, {0, 0, 1} };

	RotateOriginImageVertex(degree);
	auto newImgSize = ComputeImageSizeByOriginImageVertex();

	ComputeOpToVpMatrix(rotateMatrix);
	ComputeVptoOpMatrix();

	_displayImgSize.Width *= newImgSize.Width / _imgSize.Width;
	_displayImgSize.Height *= newImgSize.Height / _imgSize.Height;

	_imgSize.Width = newImgSize.Width;
	_imgSize.Height = newImgSize.Height;

	_imgHeaderPoint = GetHeaderPoint();
}

/// <summary>
/// 垂直翻转
/// </summary>
void COriginalToViewportTransformMatrix::FilpVert()
{
	_transOptoVp[1][0] = -_transOptoVp[1][0];
	_transOptoVp[1][1] = -_transOptoVp[1][1];
	_transOptoVp[1][2] = _viewSize.Height - _transOptoVp[1][2];

	for (int i = 0; i < 4; i++)
		_orientImageVertex[i].Y = _imgSize.Height - _orientImageVertex[i].Y;

	ComputeVptoOpMatrix();
	_imgHeaderPoint = GetHeaderPoint();
}

/// <summary>
/// 水平翻转
/// </summary>
void COriginalToViewportTransformMatrix::FlipHorz()
{
	_transOptoVp[0][0] = -_transOptoVp[0][0];
	_transOptoVp[0][1] = -_transOptoVp[0][1];
	_transOptoVp[0][2] = _viewSize.Width - _transOptoVp[0][2];

	for (int i = 0; i < 4; i++)
		_orientImageVertex[i].X = _imgSize.Width - _orientImageVertex[i].X;

	ComputeVptoOpMatrix();
	_imgHeaderPoint = GetHeaderPoint();
}

const PointF& COriginalToViewportTransformMatrix::GetDisplayImageHeaderPoint() const
{
	return _imgHeaderPoint;
}

/// <summary>
/// 获取图像显示首顶点[显示坐标]
/// </summary>
/// <returns></returns>
PointF COriginalToViewportTransformMatrix::GetHeaderPoint()
{
	const static int nVertextCount = 4;

	PointF vertext[nVertextCount] = { PointF(0, 0),
		PointF(_imgSize.Width, 0),
		PointF(0, _imgSize.Height),
		PointF(_imgSize.Width, _imgSize.Height) };

	PointF disVertex[nVertextCount];
	
	TransOpToVp(vertext, nVertextCount, disVertex);

	PointF retPoint(FLT_MAX, FLT_MAX);

	for (auto& tmpPt : disVertex)
	{
		retPoint.X = tmpPt.X < retPoint.X ? tmpPt.X : retPoint.X;
		retPoint.Y = tmpPt.Y < retPoint.Y ? tmpPt.Y : retPoint.Y;
	}

	//delete[] disVertex;

	return retPoint;
}

void COriginalToViewportTransformMatrix::ComputeVptoOpMatrix()
{
	/**************************************
	OPtoVP : | Fax  Ax  Px |
			 | By   Fay Py |
			 | 0    0   1  |

	VPtoOP:  |     Fay                  -Ax               AxPy - FayPx    |
			 |   ---------          -------------       ---------------   |
			 |   FaxFay - AxBy      FaxFay - AxBy        FaxFay - AxBy    |
			 |                                                            |
			 |     By                  Fax                ByPx - FaxPy    |
			 |   -----------       --------------       ----------------  |
			 |   FaxFay - AxBy      FaxFay - AxBy        FaxFay - AxBy    |
			 |                                                            |
			 |     0                   0                       1          |  前提条件：FaxFay - AxBy != 0
	**************************************/
	double dbTemp = _transOptoVp[0][0] * _transOptoVp[1][1] - _transOptoVp[0][1] * _transOptoVp[1][0];

	if (abs(dbTemp) < 1e-8)
		return; // newtrans不存在矩阵的逆

	_transVptoOp[0][0] = _transOptoVp[1][1] / dbTemp;
	_transVptoOp[0][1] = -_transOptoVp[0][1] / dbTemp;
	_transVptoOp[0][2] = (_transOptoVp[0][1] * _transOptoVp[1][2] - _transOptoVp[1][1] * _transOptoVp[0][2]) / dbTemp;

	_transVptoOp[1][0] = -_transOptoVp[1][0] / dbTemp;
	_transVptoOp[1][1] = _transOptoVp[0][0] / dbTemp;
	_transVptoOp[1][2] = (_transOptoVp[1][0] * _transOptoVp[0][2] - _transOptoVp[0][0] * _transOptoVp[1][2]) / dbTemp;

	_transVptoOp[2][0] = 0;
	_transVptoOp[2][1] = 0;
	_transVptoOp[2][2] = 1.0f;
}

void COriginalToViewportTransformMatrix::ComputeOpToVpMatrix(double matrix[][3])
{
	double ovTemp[3][3] = { 0 };

	MatrixMultiplyMatrix(matrix, _transOptoVp, ovTemp);

	double result[3] = { 0 };

	CalcRotatePoint(matrix, 0, 0, result);

	ovTemp[0][2] = result[0];
	ovTemp[1][2] = result[1];
	ovTemp[2][2] = result[2];

	CopyMatrix(_transOptoVp, ovTemp);
}

void COriginalToViewportTransformMatrix::CopyMatrix(double destMatrix[3][3], double srcMatrix[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int n = 0; n < 3; n++)
			destMatrix[i][n] = srcMatrix[i][n];
	}
}

void COriginalToViewportTransformMatrix::CalcRotatePoint(double mulMatrix[3][3], double dbShiftX, double dbShiftY, double result[3])
{
	if (mulMatrix == NULL_SYMBAL)
		return;

	double rotateT1[3][3] = { {1, 0, -(_viewSize.Width / 2)}, {0, 1, -_viewSize.Height / 2}, {0, 0, 1} };
	double rotateT2[3][3] = { {1, 0, _viewSize.Width / 2}, {0, 1, _viewSize.Height / 2}, {0, 0, 1} };

	double origPoint[3] = { 0 };
	origPoint[0] = _transOptoVp[0][2] + dbShiftX;
	origPoint[1] = _transOptoVp[1][2] + dbShiftY;
	origPoint[2] = _transOptoVp[2][2];

	double temp1[3] = { 0 };
	double temp2[3] = { 0 };

	MatrixMutiplyVector(rotateT1, origPoint, temp1);
	MatrixMutiplyVector(mulMatrix, temp1, temp2);
	MatrixMutiplyVector(rotateT2, temp2, result);
}

void COriginalToViewportTransformMatrix::MatrixMultiplyMatrix(double matrix1[3][3], double matrix2[3][3], double result[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result[i][j] = matrix1[i][0] * matrix2[0][j]
				+ matrix1[i][1] * matrix2[1][j]
				+ matrix1[i][2] * matrix2[2][j];
		}
	}
}

void COriginalToViewportTransformMatrix::MatrixMutiplyVector(double matrix[3][3], double vect[3], double result[3])
{
	for (int i = 0; i < 3; i++)
	{
		result[i] = matrix[i][0] * vect[0]
			+ matrix[i][1] * vect[1]
			+ matrix[i][2] * vect[2];
	}
}

void COriginalToViewportTransformMatrix::RotateOriginImageVertex(double dbRadians)
{
	double dbCos = cos(dbRadians);
	double dbSin = sin(dbRadians);

	double dbCenterX = _imgSize.Width / 2.0;
	double dbCenterY = _imgSize.Height / 2.0;

	for (int i = 0; i < _countof(_orientImageVertex); i++)
	{
		auto verPt = _orientImageVertex[i];

		double dbT = verPt.X;

		verPt.X = (float)((dbT - dbCenterX) * dbCos - (verPt.Y - dbCenterY) * dbSin + dbCenterX);
		verPt.Y = (float)((dbT - dbCenterX) * dbSin + (verPt.Y - dbCenterY) * dbCos + dbCenterY);
	}
}

SizeF COriginalToViewportTransformMatrix::ComputeImageSizeByOriginImageVertex()
{
	// 使用源图四顶点计算
	double left = min(_orientImageVertex[0].X, _orientImageVertex[1].X);
	double right = max(_orientImageVertex[0].X, _orientImageVertex[1].X);
	double top = min(_orientImageVertex[0].Y, _orientImageVertex[1].Y);
	double bottom = max(_orientImageVertex[0].Y, _orientImageVertex[1].Y);

	// Bottom-Left
	left = min(left, _orientImageVertex[2].X);
	right = max(right, _orientImageVertex[2].X);
	top = min(top, _orientImageVertex[2].Y);
	bottom = max(bottom, _orientImageVertex[2].Y);

	// Bottom-Right
	left = min(left, _orientImageVertex[3].X);
	right = max(right, _orientImageVertex[3].X);
	top = min(top, _orientImageVertex[3].Y);
	bottom = max(bottom, _orientImageVertex[3].Y);

	return SizeF(0, 0, (int)abs(bottom - top + 0.5), (int)abs(right - left + 0.5));
}

