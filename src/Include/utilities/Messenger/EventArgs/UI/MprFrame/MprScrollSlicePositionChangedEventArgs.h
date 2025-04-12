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

#ifndef MPR_SCROLL_SLICE_POSITION_CHANGED_EVENT_ARGS_C1C00188E81844A3A08991329D43D55E
#define MPR_SCROLL_SLICE_POSITION_CHANGED_EVENT_ARGS_C1C00188E81844A3A08991329D43D55E

#pragma once

#include <utilities/Messenger/ExportFlagDefine.h>
#include "utilities/Messenger/EventArgsBase.h"

#include "Foundations/Common/ToDefine.h"
#include "Foundations/Interfaces/ImagePlanDirection.h"

class UTILITIESMESSENGER_API CMprScrollSlicePositionChangedEventArgs : public CEventArgsBase
{
public:
	CMprScrollSlicePositionChangedEventArgs();

public:
	AUTO_PROPERTY_REF(NewPosition, int);

	AUTO_PROPERTY(ImagePlan, ImagePlanDirection);
};

#endif
