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
 * ����������
 */
class CServiceResult
{
public:
	CServiceResult();
	
	virtual ~CServiceResult();
	
private:
	/*
	 * ͼ��ķ�����
	 */
	AUTO_PROPERTY(ImagePlan, ImagePlanDirection);

	/*
	 * ͼ����
	 */
	AUTO_PROPERTY(ImageWidth, int);

	/*
	 * ͼ��߶�
	 */
	AUTO_PROPERTY(ImageHeight, int);

	/*
	 * ͼ������
	 */
	AUTO_PROPERTY_REF(ImageData, std::shared_ptr<BYTE_TYPE>);

	AUTO_PROPERTY_REF(PhotometricInterpretation, PhotometricInterpretation);

	/*
	 * ����λ[����]
	 */
	AUTO_PROPERTY(WindowWidth, double);

	/*
	 * ����λ[��λ]
	 */
	AUTO_PROPERTY(WindowCenter, double);

	/*
	 * ͼ�������Ƿ�Ϊ�з���
	 */
	AUTO_PROPERTY(IsPixelSigned, bool);

	/*
	 * ͼ�������Ƿ�Ϊ2�ֽ�����
	 */
	AUTO_PROPERTY(IsPixelWord, bool);

	/*
	 * X����PixelSpacing
	 */
	AUTO_PROPERTY(PixelSpacingX, double);

	/*
	 * Y����PixelSpacing
	 */
	AUTO_PROPERTY(PixelSpacingY, double);

	/*
	 * ����
	 */
	AUTO_PROPERTY(SpacingBetweenSlice, double);

	/*
	 * CTֵб��
	 */
	AUTO_PROPERTY(Slop, double);

	/*
	 * CTֵ�ؾ�
	 */
	AUTO_PROPERTY(Intercept, double);


	////////////   ͼ��λ�����[3D]    ///////////////

	/**
	 * ͼ�����ϽǶ���ռ������[ԭʼDICOM���в�������ϵ]
	 */
	AUTO_PROPERTY_REF(ImagePosition, sctPoint3D);

	/*
	 * ͼ��᷽��������
	 */
	AUTO_PROPERTY_REF(OrientationX, sctNormal3D);

	/*
	 * ͼ���ݷ���������
	 */
	AUTO_PROPERTY_REF(OrientationY, sctNormal3D);
};

#endif
