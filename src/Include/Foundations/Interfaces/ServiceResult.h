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

#ifndef SERVICE_RESULT_F05AC918B48F4B66A776408A894FD61C
#define SERVICE_RESULT_F05AC918B48F4B66A776408A894FD61C

#pragma once

#include <Foundations/Interfaces/ImagePlanDirection.h>
#include <Foundations/Common/ToDefine.h>
#include <Foundations/Common/Math/Point3D.h>
#include <Foundations/Common/Math/Point2D.h>
#include <Foundations/Common/Math/Normal3D.h>
#include <Foundations/Interfaces/PhotometricInterpretation.h>

/*
 * 服务结果基类
 */
class CServiceResult
{
public:
	CServiceResult();
	
	virtual ~CServiceResult();
	
private:
	/*
	 * 图像的方向面
	 */
	AUTO_PROPERTY(ImagePlan, ImagePlanDirection);

	/*
	 * 图像宽度
	 */
	AUTO_PROPERTY(ImageWidth, int);

	/*
	 * 图像高度
	 */
	AUTO_PROPERTY(ImageHeight, int);

	/*
	 * 图像像素
	 */
	AUTO_PROPERTY_REF(ImageData, std::shared_ptr<BYTE_TYPE>);

	AUTO_PROPERTY_REF(PhotometricInterpretation, PhotometricInterpretation);

	/*
	 * 窗宽窗位[窗宽]
	 */
	AUTO_PROPERTY(WindowWidth, double);

	/*
	 * 窗宽窗位[窗位]
	 */
	AUTO_PROPERTY(WindowCenter, double);

	/*
	 * 图像像素是否为有符号
	 */
	AUTO_PROPERTY(IsPixelSigned, bool);

	/*
	 * 图像像素是否为2字节整数
	 */
	AUTO_PROPERTY(IsPixelWord, bool);

	/*
	 * X方向PixelSpacing
	 */
	AUTO_PROPERTY(PixelSpacingX, double);

	/*
	 * Y方向PixelSpacing
	 */
	AUTO_PROPERTY(PixelSpacingY, double);

	/*
	 * 层间距
	 */
	AUTO_PROPERTY(SpacingBetweenSlice, double);

	/*
	 * CT值斜率
	 */
	AUTO_PROPERTY(Slop, double);

	/*
	 * CT值截距
	 */
	AUTO_PROPERTY(Intercept, double);


	////////////   图像位置相关[3D]    ///////////////

	/**
	 * 图像左上角顶点空间坐标点[原始DICOM序列病人坐标系]
	 */
	AUTO_PROPERTY_REF(ImagePosition, sctPoint3D);

	/*
	 * 图像横方向方向余弦
	 */
	AUTO_PROPERTY_REF(OrientationX, sctNormal3D);

	/*
	 * 图像纵方向方向余弦
	 */
	AUTO_PROPERTY_REF(OrientationY, sctNormal3D);
};

#endif
