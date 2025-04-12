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

#ifndef PARONAMA_TRANSVERSE_DISPLAY_IMAGE_5ABC3A970E26424AA60292258CC4CF3E
#define PARONAMA_TRANSVERSE_DISPLAY_IMAGE_5ABC3A970E26424AA60292258CC4CF3E

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <ViewModels/ImageFrameViewModel/DisplayImage.h>

#include <ViewModels/ParonamaFrameViewModel/paronamaframeviewmodel_global.h>

#include "Services/Paronama/ParonamaResult.h"


class PARONAMAFRAMEVIEWMODEL_API CParonamaDisplayImage : public CDisplayImage
{
public:
	void From(std::shared_ptr<CParonamaResult> resultImage);

	virtual QString GetFormatedTopLeftText() override;

	virtual QString GetFormatedTopRightText() override;

	virtual QString GetFormatedBottomLeftText() override;

	virtual QString GetFormatedBottomRightText() override;

public:
	static std::shared_ptr<CSeriesModule> CreateSeriesModule(std::shared_ptr<CParonamaResult> mprResult);
	static void CreateInstanceModules(std::shared_ptr<CSeriesModule>, std::shared_ptr<CParonamaResult> mprResult);
	
private:
	
	/*
	 * Parameter的平面方向
	 */
	AUTO_PROPERTY(PlanDirection, ImagePlanDirection);

	////////////   图像显示相关[2D]    ///////////////
	/*
	 * 定位线X起始点[2D]
	 */
	AUTO_PROPERTY_REF(ScoutLineBeginX2D, sctPoint2D);

	/*
	 * 定位线X终点[2D]
	 */
	AUTO_PROPERTY_REF(ScoutLineEndX2D, sctPoint2D);

	/*
	 * 定位线Z起始点[2D]
	 */
	AUTO_PROPERTY_REF(ScoutLineBeginZ2D, sctPoint2D);

	/*
	 * 定位线Z终点[2D]
	 */
	AUTO_PROPERTY_REF(ScoutLineEndZ2D, sctPoint2D);

	/*
	 * 两条定位线X/Z的中心点[2D]
	 */
	AUTO_PROPERTY_REF(Center2D, sctPoint2D);/*
	 * 当前层相对本平面方向的位置[整数位置点]
	 */
	AUTO_PROPERTY(ScrollSlicePosition, int);

	AUTO_PROPERTY(ScrollSlicePositionStart, int);

	AUTO_PROPERTY(ScrollSlicePositionEnd, int);
};


#endif

