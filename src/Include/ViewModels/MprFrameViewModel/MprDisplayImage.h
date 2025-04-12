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

#ifndef MPR_DISPLAY_IMAGE_55B1CBF99A104C48A16C9D02CAEBA945
#define MPR_DISPLAY_IMAGE_55B1CBF99A104C48A16C9D02CAEBA945

#pragma once

#include "ViewModels/ImageFrameViewModel/DisplayImage.h"

#include <ViewModels/MprFrameViewModel/MprFrameViewModel_global.h>

#include "Services/Mpr/MprResult.h"

class MPRFRAMEVIEWMODEL_API CMprDisplayImage : public CDisplayImage
{
public:
	void FromMprResult(std::shared_ptr<CMprResult> mprResult);

	virtual QString GetFormatedTopLeftText() override;

	virtual QString GetFormatedTopRightText() override;

	virtual QString GetFormatedBottomLeftText() override;

	virtual QString GetFormatedBottomRightText() override;

	void UpdateScoutLineCenter(double x, double y);

private:
	void CreateSeriesModule(std::shared_ptr<CMprResult> mprResult);
	void CreateInstanceModule(std::shared_ptr<CMprResult> mprResult);
	
private:
	//AUTO_PROPERTY_REF(ReferenceStudy, std::shared_ptr<CStudyModule>);
	//AUTO_PROPERTY_REF(ReferenceSeries, std::shared_ptr<CSeriesModule>);
	//AUTO_PROPERTY_REF(ReferenceInstance, std::shared_ptr<CImageInstanceModule>);


	/*
	 * Parameter��ƽ�淽��
	 */
	AUTO_PROPERTY(PlanDirection, ImagePlanDirection);

	////////////   ͼ����ʾ���[2D]    ///////////////
	/*
	 * ��λ��X��ʼ��[2D]
	 */
	AUTO_PROPERTY_REF(ScoutLineBeginX2D, sctPoint2D);

	/*
	 * ��λ��X�յ�[2D]
	 */
	AUTO_PROPERTY_REF(ScoutLineEndX2D, sctPoint2D);

	/*
	 * ��λ��Z��ʼ��[2D]
	 */
	AUTO_PROPERTY_REF(ScoutLineBeginZ2D, sctPoint2D);

	/*
	 * ��λ��Z�յ�[2D]
	 */
	AUTO_PROPERTY_REF(ScoutLineEndZ2D, sctPoint2D);

	/*
	 * ������λ��X/Z�����ĵ�[2D]
	 */
	AUTO_PROPERTY_REF(Center2D, sctPoint2D);

	/*
	 * ��ǰ����Ա�ƽ�淽���λ��[����λ�õ�]
	 */
	AUTO_PROPERTY(ScrollSlicePosition, int);

	AUTO_PROPERTY(ScrollSlicePositionStart, int);

	AUTO_PROPERTY(ScrollSlicePositionEnd, int);
};

#endif
