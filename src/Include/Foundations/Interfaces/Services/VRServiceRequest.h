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

#ifndef VR_SERVICE_REQUEST_2551390E216A4B38AEFCC1F41E46CA84
#define VR_SERVICE_REQUEST_2551390E216A4B38AEFCC1F41E46CA84

#pragma once

#include <Foundations/Interfaces/ServiceRequest.h>

class CVRServiceRequest : public CServiceRequest
{
public:
	CVRServiceRequest();

private:
	AUTO_PROPERTY(ParentRenderWindowHandle, void*);
};

#endif
