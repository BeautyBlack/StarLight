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

#ifndef MPR_RESULT_34212AC7EC324C8CAAEC50F591022BF0
#define MPR_RESULT_34212AC7EC324C8CAAEC50F591022BF0

#pragma once

#include <Foundations/Interfaces/ServiceResult.h>
#include <Foundations/Common/ToDefine.h>
#include <Services/Mpr/ExportFlagDefine.h>
#include <Foundations/Common/Math/Point3D.h>
#include <Foundations/Common/Math/Point2D.h>
#include <Foundations/Common/Math/Normal3D.h>
#include <Foundations/Common/Math/SegmentLine3D.h>

#include <Utilities/ImageModule/Modules/StudyModule.h>

class SERVICEMPR_API CMprResult : public CServiceResult
{
public:
	CMprResult();
	~CMprResult();

//private:
//	void CreateSeriesModule();
//	void CreateInstanceModule();
//
//	void UpdateToImageModule();
	
private:
	/*
	 * Parameter��ƽ�淽��
	 */
	AUTO_PROPERTY(PlanDirection, ImagePlanDirection);
	
	/**
	 * X�ᶨλ��
	 */
	AUTO_PROPERTY_REF(ScoutLineX, sctSegmentLine3D);

	/**
	 * Z�ᶨλ��
	 */
	AUTO_PROPERTY_REF(ScoutLineZ, sctSegmentLine3D);
	
	/*
	 * ������λ��X/Z�����ĵ�
	 */
	AUTO_PROPERTY_REF(Center, sctPoint3D);

	/*
	 * ��ǰ����Ա�ƽ�淽���λ��[����λ�õ�]
	 */
	AUTO_PROPERTY(ScrollSlicePosition, int);

	/*
	 * ��ƽ�淽�����
	 */
	AUTO_PROPERTY(ScrollSlicePositionStart, int);
	AUTO_PROPERTY(ScrollSlicePositionEnd, int);

	AUTO_PROPERTY_REF(ReferenceStudy, std::shared_ptr<CStudyModule>);
	/*AUTO_PROPERTY_REF(ReferenceSeries, std::shared_ptr<CSeriesModule>);
	AUTO_PROPERTY_REF(ReferenceInstance, std::shared_ptr<CImageInstanceModule>);
	*/
};

#endif
