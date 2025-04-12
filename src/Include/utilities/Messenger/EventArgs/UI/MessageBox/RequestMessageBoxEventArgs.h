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

#ifndef REQUEST_MESSAGE_BOX_EVENT_ARGS_A7E0B53D118548378142BBF67F763217
#define REQUEST_MESSAGE_BOX_EVENT_ARGS_A7E0B53D118548378142BBF67F763217

#pragma once

#include <utilities/Messenger/ExportFlagDefine.h>
#include <utilities/Messenger/EventArgsBase.h>

#include <Foundations/Common/ToDefine.h>

class UTILITIESMESSENGER_API CRequestMessageBoxEventArgs : public CEventArgsBase
{ 
public:
	CRequestMessageBoxEventArgs() = default;
	~CRequestMessageBoxEventArgs() = default;

private:
	AUTO_PROPERTY_REF(TitleText, STRING_TYPE);

	AUTO_PROPERTY_REF(MessageText, STRING_TYPE);
};

#endif

