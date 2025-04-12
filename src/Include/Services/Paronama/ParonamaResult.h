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

#ifndef PARONAMA_RESULT_D3A81866306D40C48C72BA7A11FEF4B4
#define PARONAMA_RESULT_D3A81866306D40C48C72BA7A11FEF4B4

#pragma once

#include "Foundations/Interfaces/ServiceResult.h"

#include "Services/Paronama/ExportFlagDefine.h"

#include <Foundations/Interfaces/ServiceResult.h>
#include <Foundations/Common/ToDefine.h>
#include "Services/Paronama/ExportFlagDefine.h"
#include <Foundations/Common/Math/Point3D.h>
#include <Foundations/Common/Math/Point2D.h>
#include <Foundations/Common/Math/Normal3D.h>
#include <Foundations/Common/Math/SegmentLine3D.h>

#include <Utilities/ImageModule/Modules/StudyModule.h>

#include <vector>

class PARONAMA_SERVICEMPR_API CParonamaResult : public CServiceResult
{
public:
	using RESULT_VECTOR = std::vector<std::shared_ptr<CParonamaResult>>;
	using RESULT_VECTOR_PTR = std::shared_ptr<RESULT_VECTOR>;

public:
	CParonamaResult();
	virtual ~CParonamaResult();
	
private:
	/*
	 * Parameter的平面方向
	 */
	AUTO_PROPERTY(PlanDirection, ImagePlanDirection);

	/**
	 * X轴定位线
	 */
	AUTO_PROPERTY_REF(ScoutLineX, sctSegmentLine3D);

	/**
	 * Z轴定位线
	 */
	AUTO_PROPERTY_REF(ScoutLineZ, sctSegmentLine3D);

	/*
	 * 两条定位线X/Z的中心点
	 */
	AUTO_PROPERTY_REF(Center, sctPoint3D);

	/*
	 * 当前层相对本平面方向的位置[整数位置点]
	 */
	AUTO_PROPERTY(ScrollSlicePosition, int);

	/*
	 * 本平面方向层数
	 */
	AUTO_PROPERTY(ScrollSlicePositionStart, int);
	AUTO_PROPERTY(ScrollSlicePositionEnd, int);

	AUTO_PROPERTY_REF(ReferenceStudy, std::shared_ptr<CStudyModule>);

	AUTO_PROPERTY_REF(ResultImages, RESULT_VECTOR_PTR);
};

#endif

