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

#ifndef VR_FRAME_VIEWMODEL_B7EC1348C2BA4F2597182DC1FB074879
#define VR_FRAME_VIEWMODEL_B7EC1348C2BA4F2597182DC1FB074879

#pragma once


#include <Foundations/Common/ToDefine.h>

#include "Foundations/Interfaces/ServiceRequest.h"
#include "utilities/Messenger/Messenger.h"

#include <ViewModels/VRFrameViewModel/ExportFlagDefine.h>

class CVRService;
class CVRColorSetting;

class VR_FRAME_SERVICEMPR_API CVRFrameViewModel : public CMessengerOwner
{
public:
	CVRFrameViewModel() = default;
	~CVRFrameViewModel();

	void Initialize();

	void RemoveCurrentStudy();

	void Load(std::shared_ptr<CServiceRequest> request);

	void Reset();

private:
	void LoadColor(CVRColorSetting *pSetting);

private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CVRFrameViewModel);
	
	AUTO_PROPERTY_REF(ServiceInstance, std::shared_ptr<CVRService>);
};


#endif

