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

#ifndef IMAGE_PLAN_MODULE_BB93A4238F294757902C6CEDF5A94040
#define IMAGE_PLAN_MODULE_BB93A4238F294757902C6CEDF5A94040

#pragma once

#include <Foundations/Common/ToDefine.h>
#include <Foundations/Interfaces/PhotometricInterpretation.h>
#include "Foundations/Common/Math/Point3D.h"
#include "Foundations/Common/Math/Normal3D.h"

#include <memory>

#include "Foundations/Common/Math/Point2D.h"

class CImageInstanceModule;


#include <utilities/ImageModule/ExportFlagDefine.h>

class UTILITIESIMAGEMODULE_API CImagePlanModule
{
public:
	/*
	 * 默认层
	 */
	static int DefaultFrameIndex;

	CImagePlanModule();
	~CImagePlanModule();

	/*
	 * Description
	 * 切换层像素
	 */
	void SetFrameIndex(int nIndex);

	const unsigned char* GetCurrentFramePixelData();

	sctPoint2D PatientPointToImagePlane(sctPoint3D &point);
	sctPoint2D PatientPointToImagePlane(double x, double y, double z);
	
	sctPoint3D ImagePointToPatientPoint(sctPoint2D &point);
	sctPoint3D ImagePointToPatientPoint(double x, double y);


private:

	/*
	 * Reference Image Instance 归属Image Instance
	 */
	AUTO_PROPERTY_REF(ReferenceImageInstance, std::weak_ptr<CImageInstanceModule>);

	/*
	 * Width 宽度
	 */
	AUTO_PROPERTY_REF(Width, int);

	/*
	 * Height 高度
	 */
	AUTO_PROPERTY_REF(Height, int);

	/*
	 * Pixel Spacing X X方向分辨率
	 */
	AUTO_PROPERTY_REF(PixelSpacingX, double);

	/*
	 * Pixel Spacing Y Y方向分辨率
	 */
	AUTO_PROPERTY_REF(PixelSpacingY, double);

	/*
	 * Spacing Between Slice 层间距
	 */
	AUTO_PROPERTY_REF(SpacingBetweenSlice, double);

	/*
	 * 层厚
	 */
	AUTO_PROPERTY_REF(SliceThickness, double);

	/*
	 * Frame Count Frame层数
	 */
	AUTO_PROPERTY_REF(FrameCount, int);

	/*
	 * Frame 层索引
	 */
	AUTO_PROPERTY_REF_NO_SET(FrameIndex, int);

	/*
	 * Pixel 像素
	 */
	//AUTO_PROPERTY_REF(Pixels, BYTE_TYPE*);

	/*
	 * OneFramePixelSize 像素矩阵大小
	 */
	AUTO_PROPERTY_REF(OneFramePixelSize, int);

	/*
	 * OneFramePixelSizeInBytes 像素矩阵字节大小
	 */
	AUTO_PROPERTY_REF(OneFramePixelSizeInBytes, int);

	/*
	 * AllFramePixels 总像素
	 */
	AUTO_PROPERTY_REF(AllFramePixels, std::shared_ptr<unsigned char>);

	/*
	 * HighBit 最高有效位
	 */
	//AUTO_PROPERTY(HighBit, int);

	/*
	 * BitsStored 像素存储位
	 */
	//AUTO_PROPERTY(BitsStored, int);

	/*
	 * BitsAllocated 单像素字节位大小
	 */
	//AUTO_PROPERTY(BitsAllocated, int);

	/*
	 * PhotometricInterpretation 像素显示编码方式
	 */
	AUTO_PROPERTY_REF(PhotometricInterpretation, PhotometricInterpretation);

	/*
	 * IsSampleWord 双字节像素
	 */
	AUTO_PROPERTY_REF(IsSampleWord, bool);

	/*
	 * IsSigned 有符号像素
	 */
	AUTO_PROPERTY_REF(IsSigned, bool);

	/*
	 * Intercept 截距
	 */
	AUTO_PROPERTY_REF(Intercept, double);

	/*
	 * Slop 斜率
	 */
	AUTO_PROPERTY_REF(Slope, double);

	/*
	 * Position 首顶点
	 */
	AUTO_PROPERTY_REF(Position, sctPoint3D);

	/*
	 * OrientationX X方向方向余弦
	 */
	AUTO_PROPERTY_REF(OrientationX, sctNormal3D);

	/*
	 * OrientationY Y方向方向余弦
	 */
	AUTO_PROPERTY_REF(OrientationY, sctNormal3D);

	/*
	 * WindowCenter 窗位
	 */
	AUTO_PROPERTY_REF(WindowCenter, double);

	/*
	 * WindowWidth 窗宽
	 */
	AUTO_PROPERTY_REF(WindowWidth, double);
};

#endif
