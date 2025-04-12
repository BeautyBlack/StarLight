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

#ifndef MPR_ROTATE_SCOUT_LINE_EVENT_ARGS_C2F57E00C9144C379AB90CA35C7CE696
#define MPR_ROTATE_SCOUT_LINE_EVENT_ARGS_C2F57E00C9144C379AB90CA35C7CE696

#pragma once

#include <utilities/Messenger/ExportFlagDefine.h>
#include "utilities/Messenger/EventArgsBase.h"

#include "Foundations/Common/ToDefine.h"
#include "Foundations/Interfaces/ImagePlanDirection.h"

class UTILITIESMESSENGER_API CMprRotateScoutLineEventArgs : public CEventArgsBase
{
public:
	CMprRotateScoutLineEventArgs();

private:
	AUTO_PROPERTY_REF(IsVectorX, bool);
	
	AUTO_PROPERTY_REF(Angle, double);

	AUTO_PROPERTY(ImagePlan, ImagePlanDirection);
	
};

#endif
