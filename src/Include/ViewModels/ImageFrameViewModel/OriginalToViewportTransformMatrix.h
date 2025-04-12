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

#ifndef ORIGINAL_TO_VIEWPORT_TRANSFORM_MATRIX_A12AB5564E274042A8065142254BEAA6
#define ORIGINAL_TO_VIEWPORT_TRANSFORM_MATRIX_A12AB5564E274042A8065142254BEAA6

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <ViewModels/ImageFrameViewModel/imageframeviewmodel_global.h>

struct IMAGEFRAMEVIEWMODEL_API SizeF
{
	double X;
	double Y;

	double Width;
	double Height;

	SizeF()
	{
		X = Y = Width = Height = 0;
	}
	
	SizeF(double x, double y, double width, double height)
	{
		X = x;
		Y = y;

		Width = width;
		Height = height;
	}

	SizeF(const SizeF &copySrc)
	{
		X = copySrc.X;
		Y = copySrc.Y;

		Width = copySrc.Width;
		Height = copySrc.Height;
	}
};

struct IMAGEFRAMEVIEWMODEL_API PointF
{
	double X;
	double Y;

	PointF()
	{
		X = Y = 0;
	}

	PointF(double x, double y)
	{
		X = x;
		Y = y;
	}

	PointF(const PointF &copySrc)
	{
		X = copySrc.X;
		Y = copySrc.Y;
	}
};

/**
 * Image <---> View 坐标转换矩阵
 */
class IMAGEFRAMEVIEWMODEL_API COriginalToViewportTransformMatrix
{
public:

	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="viewSpaceX">图像显示窗口外框宽度</param>
	/// <param name="viewSpaceY">图像显示窗口外框高度</param>
	/// <param name="displayImgX">图像显示视野宽度</param>
	/// <param name="displayImgY">图像显示视野高度</param>
	/// <param name="imgWidth">图像宽度[Pixel宽度]</param>
	/// <param name="imgHeight">图像高度[Pixel高度]</param>
	COriginalToViewportTransformMatrix(
		double viewSpaceX, double viewSpaceY,
		double displayImgX, double displayImgY,
		double imgWidth, double imgHeight);

	COriginalToViewportTransformMatrix(
		const COriginalToViewportTransformMatrix &copyMatrix);

	/// <summary>
	/// 坐标点转换[从图像坐标转换到视野显示坐标]
	/// </summary>
	/// <param name="transPoint">需要转换的点[Array]</param>
	/// <returns>转换后的点[Array[</returns>
	void TransOpToVp(PointF transPoint[], int nTransPointCount, PointF[]);

	/// <summary>
	/// 坐标点转换[从视野显示坐标转换到图像坐标]
	/// </summary>
	/// <param name="transPoint">需要转换的点[Array]</param>
	/// <returns>转换后的点[Array[</returns>
	void TransVpToOp(PointF transPoint[], int nTransPointCount, PointF[]);

	/// <summary>
	/// 图像框Viewport变大、变小
	/// Viewport发生变化后， Display Image Size 也将按照viewport变化的比例对应发生变化
	/// </summary>
	/// <param name="newViewSize"></param>
	void SizeChange(SizeF newViewSize);

	/// <summary>
	/// 缩放
	/// </summary>
	/// <param name="dbFactor">缩放比例</param>
	void Scale(double dbFactor);

	/// <summary>
	/// 平移
	/// </summary>
	/// <param name="dbOffsetX">X轴偏移量</param>
	/// <param name="dbOffsetY">Y轴偏移量</param>
	void Move(double dbOffsetX, double dbOffsetY);

	/// <summary>
	/// 旋转
	/// </summary>
	/// <param name="dbAngle">旋转角度 范围 : [-360 , 360]</param>
	void Rotate(double dbAngle);

	/// <summary>
	/// 垂直翻转
	/// </summary>
	void FilpVert();

	/// <summary>
	/// 水平翻转
	/// </summary>
	void FlipHorz();

	const PointF& GetDisplayImageHeaderPoint() const;
	
private:
	/// <summary>
	/// 获取图像显示首顶点[显示坐标]
	/// </summary>
	/// <returns></returns>
	PointF GetHeaderPoint();
	
	void ComputeVptoOpMatrix();

	void ComputeOpToVpMatrix(double matrix[][3]);

	void CopyMatrix(double destMatrix[3][3], double srcMatrix[3][3]);

	void CalcRotatePoint(double mulMatrix[3][3],
		double dbShiftX, double dbShiftY, double result[3]);

	void MatrixMultiplyMatrix(double matrix1[3][3],
		double matrix2[3][3], double result[3][3]);

	void MatrixMutiplyVector(double matrix[3][3],
		double vect[3], double result[3]);

	void RotateOriginImageVertex(double dbRadians);

	SizeF ComputeImageSizeByOriginImageVertex();

private:
	double _transOptoVp[3][3];// = new double[, ]{ { 0, 0, 0 }, { 0, 0, 0, }, { 0, 0, 1 } };

	double _transVptoOp[3][3];// = new double[, ]{ { 0, 0, 0 }, { 0, 0, 0, }, { 0, 0, 1 } };

	SizeF _viewSize;// = new SizeF(0, 0);

	SizeF _displayImgSize;// = new SizeF(0, 0);

	const double MinDoubleValue = 1e-5;

	/// <summary>
	/// 图像高宽
	/// </summary>

	SizeF _imgSize;// = new SizeF(0, 0);

	/// <summary>
	/// 原始图像四顶点，如果图像发生旋转，则这四点为原始图像四顶点旋转后的位置，注意区分图像旋转后新图像的四顶点
	/// </summary>
	PointF _orientImageVertex[4];// = new PointF[]{ new PointF(0,0), new PointF(0,0), new PointF(0,0), new PointF(0,0) };

	/// <summary>
	/// 图像显示首顶点
	/// </summary>
	PointF _imgHeaderPoint;// = new PointF(0, 0);

	///// <summary>
	///// 图像放大比例
	///// </summary>
	//std::function<double()> ImageFactor = [&]()->double {return _imgSize.Width / _viewSize.Width; };// = > _imgSize.Width / _viewSize.Width;

	AUTO_PROPERTY(ImageFactor, double);
};

#endif

