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

#include <Services/Mpr/ExportFlagDefine.h>

class SERVICEMPR_API CMprDisplayImage : public CDisplayImage
{
private:

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
	AUTO_PROPERTY_REF(Center2D, sctPoint2D);
	
	/*
	 * 当前层相对本平面方向的位置[整数位置点]
	 */
	AUTO_PROPERTY(ScrollSlicePosition, int);
	
	AUTO_PROPERTY(ScrollSlicePositionStart, int);
	
	AUTO_PROPERTY(ScrollSlicePositionEnd, int);
};

#endif
