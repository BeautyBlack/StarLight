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
 * Image <---> View ����ת������
 */
class IMAGEFRAMEVIEWMODEL_API COriginalToViewportTransformMatrix
{
public:

	/// <summary>
	/// ���캯��
	/// </summary>
	/// <param name="viewSpaceX">ͼ����ʾ���������</param>
	/// <param name="viewSpaceY">ͼ����ʾ�������߶�</param>
	/// <param name="displayImgX">ͼ����ʾ��Ұ���</param>
	/// <param name="displayImgY">ͼ����ʾ��Ұ�߶�</param>
	/// <param name="imgWidth">ͼ����[Pixel���]</param>
	/// <param name="imgHeight">ͼ��߶�[Pixel�߶�]</param>
	COriginalToViewportTransformMatrix(
		double viewSpaceX, double viewSpaceY,
		double displayImgX, double displayImgY,
		double imgWidth, double imgHeight);

	COriginalToViewportTransformMatrix(
		const COriginalToViewportTransformMatrix &copyMatrix);

	/// <summary>
	/// �����ת��[��ͼ������ת������Ұ��ʾ����]
	/// </summary>
	/// <param name="transPoint">��Ҫת���ĵ�[Array]</param>
	/// <returns>ת����ĵ�[Array[</returns>
	void TransOpToVp(PointF transPoint[], int nTransPointCount, PointF[]);

	/// <summary>
	/// �����ת��[����Ұ��ʾ����ת����ͼ������]
	/// </summary>
	/// <param name="transPoint">��Ҫת���ĵ�[Array]</param>
	/// <returns>ת����ĵ�[Array[</returns>
	void TransVpToOp(PointF transPoint[], int nTransPointCount, PointF[]);

	/// <summary>
	/// ͼ���Viewport��󡢱�С
	/// Viewport�����仯�� Display Image Size Ҳ������viewport�仯�ı�����Ӧ�����仯
	/// </summary>
	/// <param name="newViewSize"></param>
	void SizeChange(SizeF newViewSize);

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="dbFactor">���ű���</param>
	void Scale(double dbFactor);

	/// <summary>
	/// ƽ��
	/// </summary>
	/// <param name="dbOffsetX">X��ƫ����</param>
	/// <param name="dbOffsetY">Y��ƫ����</param>
	void Move(double dbOffsetX, double dbOffsetY);

	/// <summary>
	/// ��ת
	/// </summary>
	/// <param name="dbAngle">��ת�Ƕ� ��Χ : [-360 , 360]</param>
	void Rotate(double dbAngle);

	/// <summary>
	/// ��ֱ��ת
	/// </summary>
	void FilpVert();

	/// <summary>
	/// ˮƽ��ת
	/// </summary>
	void FlipHorz();

	const PointF& GetDisplayImageHeaderPoint() const;
	
private:
	/// <summary>
	/// ��ȡͼ����ʾ�׶���[��ʾ����]
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
	/// ͼ��߿�
	/// </summary>

	SizeF _imgSize;// = new SizeF(0, 0);

	/// <summary>
	/// ԭʼͼ���Ķ��㣬���ͼ������ת�������ĵ�Ϊԭʼͼ���Ķ�����ת���λ�ã�ע������ͼ����ת����ͼ����Ķ���
	/// </summary>
	PointF _orientImageVertex[4];// = new PointF[]{ new PointF(0,0), new PointF(0,0), new PointF(0,0), new PointF(0,0) };

	/// <summary>
	/// ͼ����ʾ�׶���
	/// </summary>
	PointF _imgHeaderPoint;// = new PointF(0, 0);

	///// <summary>
	///// ͼ��Ŵ����
	///// </summary>
	//std::function<double()> ImageFactor = [&]()->double {return _imgSize.Width / _viewSize.Width; };// = > _imgSize.Width / _viewSize.Width;

	AUTO_PROPERTY(ImageFactor, double);
};

#endif

