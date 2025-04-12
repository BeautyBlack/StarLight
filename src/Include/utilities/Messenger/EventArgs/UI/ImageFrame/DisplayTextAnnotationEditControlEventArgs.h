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

#ifndef DISPLAY_TEXT_ANNOTATION_EDIT_CONTROL_EVENT_ARGS_39FD4FB646F045C186182BA8B398F347
#define DISPLAY_TEXT_ANNOTATION_EDIT_CONTROL_EVENT_ARGS_39FD4FB646F045C186182BA8B398F347

#pragma once

#include <utilities/Messenger/ExportFlagDefine.h>
#include <utilities/Messenger/EventArgsBase.h>

#include <Foundations/Common/ToDefine.h>


class UTILITIESMESSENGER_API CDisplayTextAnnotationEditControlEventArgs : public CEventArgsBase
{
public:
	CDisplayTextAnnotationEditControlEventArgs() = default;
	~CDisplayTextAnnotationEditControlEventArgs() = default;

private:
	AUTO_PROPERTY_REF(AnnotationText, STRING_TYPE);

	AUTO_PROPERTY_REF(TextBoundLeft, int);
	AUTO_PROPERTY_REF(TextBoundTop, int);
	AUTO_PROPERTY_REF(TextBoundRight, int);
	AUTO_PROPERTY_REF(TextBoundBottom, int);

};

#endif
