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

#ifndef MPR_UPDATE_SCOUT_LINE_CENTER_EVENT_ARGS_0CF019D7D7E9490D9174BABE36E2FD47
#define MPR_UPDATE_SCOUT_LINE_CENTER_EVENT_ARGS_0CF019D7D7E9490D9174BABE36E2FD47

#pragma once

#include "Foundations/Common/ToDefine.h"
#include "Foundations/Common/Math/Point2D.h"
#include "Foundations/Interfaces/ImagePlanDirection.h"

#include <utilities/Messenger/ExportFlagDefine.h>
#include "utilities/Messenger/EventArgsBase.h"

class UTILITIESMESSENGER_API CMprUpdateScoutLineCenterEventArgs : public CEventArgsBase
{
public:
	CMprUpdateScoutLineCenterEventArgs();

public:
	AUTO_PROPERTY_REF(NewCenter, sctPoint2D);

	AUTO_PROPERTY(ImagePlan, ImagePlanDirection);
};

#endif

