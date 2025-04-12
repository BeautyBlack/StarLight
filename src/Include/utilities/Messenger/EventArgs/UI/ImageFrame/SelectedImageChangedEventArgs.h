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

#ifndef SELECTED_IMAGE_CHANGED_EVENT_ARGS_117B3382998F4821AB6A4245CC1E178E
#define SELECTED_IMAGE_CHANGED_EVENT_ARGS_117B3382998F4821AB6A4245CC1E178E

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <utilities/Messenger/ExportFlagDefine.h>

class UTILITIESMESSENGER_API CSelectedImageChangedEventArgs
{
public:
	CSelectedImageChangedEventArgs();
	
private:
	AUTO_PROPERTY_REF(IsMultiSelectMode, bool);

	AUTO_PROPERTY_REF(ActivadeTarget, void*);
};


#endif

