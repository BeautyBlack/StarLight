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

#ifndef SERVICE_REQUEST_C767601944B948429616B86FFAA0CCE7
#define SERVICE_REQUEST_C767601944B948429616B86FFAA0CCE7

#pragma once

#include <Foundations/Interfaces/ImagePlanDirection.h>
#include <Foundations/Common/ToDefine.h>
#include <Foundations/Common/Math/Point3D.h>
#include <Foundations/Common/Math/Point2D.h>
#include <Foundations/Common/Math/Normal3D.h>
#include <Foundations/Interfaces/ServiceRequestBase.h>

class CStudyModule;
class CSeriesModule;

/*
 * Description
 * �����������
 */
class CServiceRequest : CServiceRequestBase
{
public:
	CServiceRequest();
	virtual ~CServiceRequest() = default;

	bool IsValidImageData();
	
private:
	AUTO_PROPERTY(ReferenceStudy, std::shared_ptr<CStudyModule>);

	AUTO_PROPERTY_REF(ReferenceSeries, std::shared_ptr<CSeriesModule>);
	/*
	 * ͼ����
	 */
	AUTO_PROPERTY(ImageWidth, int);

	/*
	 * ͼ��߶�
	 */
	AUTO_PROPERTY(ImageHeight, int);

	/**
	 * ͼ�����
	 */
	AUTO_PROPERTY(SliceCount, int);

	/*
	 * ����������
	 */
	AUTO_PROPERTY_REF(ImageData, std::shared_ptr<BYTE_TYPE>);

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

	/**
	 * ͼ�����ϽǶ���ռ������[ԭʼDICOM���в�������ϵ]
	 */
	AUTO_PROPERTY_REF(LastSliceImagePosition, sctPoint3D);
};

#endif
