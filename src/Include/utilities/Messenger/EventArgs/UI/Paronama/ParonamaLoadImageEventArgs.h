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

#ifndef PARONAMA_LOAD_IMAGE_EVENT_ARGS_5F0B602A626347A98B286541601C8297
#define PARONAMA_LOAD_IMAGE_EVENT_ARGS_5F0B602A626347A98B286541601C8297

#pragma once

#include <utilities/Messenger/ExportFlagDefine.h>
#include "utilities/Messenger/EventArgsBase.h"

#include <Foundations/Interfaces/ServiceRequest.h>

#include <Foundations/Common/ToDefine.h>

class UTILITIESMESSENGER_API CParonamaLoadImageEventArgs : public CEventArgsBase
{
private:
	AUTO_PROPERTY(ServiceRequest, std::shared_ptr<CServiceRequest>);
};

#endif

