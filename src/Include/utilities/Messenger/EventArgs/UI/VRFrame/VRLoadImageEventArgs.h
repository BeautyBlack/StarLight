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

#ifndef VR_LOAD_IMAGE_EVENT_ARGS_76F65F05F020459DB870426B04268E0A
#define VR_LOAD_IMAGE_EVENT_ARGS_76F65F05F020459DB870426B04268E0A

#pragma once

#include <utilities/Messenger/EventArgs/UI/MprFrame/MprLoadImageEventArgs.h>

#include <Foundations/Interfaces/Services/VRServiceRequest.h>

class UTILITIESMESSENGER_API CVRLoadImageEventArgs : public CEventArgsBase
{
private:

	AUTO_PROPERTY(ServiceRequest, std::shared_ptr<CVRServiceRequest>);

};

#endif
