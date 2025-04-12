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

#ifndef MPR_FRAME_VIEWMODEL_3E40E51E333342EFB1D13C3035AEEEAC
#define MPR_FRAME_VIEWMODEL_3E40E51E333342EFB1D13C3035AEEEAC

#pragma once

#include <Foundations/Common/ToDefine.h>

#include "Services/Mpr/MprService.h"

#include <ViewModels/MprFrameViewModel/MprFrameViewModel_global.h>
#include <ViewModels/MprFrameViewModel/MprDisplayImage.h>

#include "Foundations/Interfaces/ServiceRequest.h"
#include "utilities/Messenger/Messenger.h"

class MPRFRAMEVIEWMODEL_API CMprFrameViewModel : public CMessengerOwner
{
public:
	CMprFrameViewModel() = default;
	~CMprFrameViewModel();

	void Initialize();

	void RemoveCurrentStudy();
	
	void Load(std::shared_ptr<CServiceRequest> request);

	void Reset();
	
private:
	void UpdateDisplayImages();

private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CMprFrameViewModel);
	AUTO_PROPERTY_REF(ServiceInstance, std::shared_ptr<CMprService>);

	AUTO_PROPERTY(SaggitalDisplayImage, std::shared_ptr<CMprDisplayImage>);
	AUTO_PROPERTY(CoronalDisplayImage, std::shared_ptr<CMprDisplayImage>);
	AUTO_PROPERTY(TransverseDisplayImage, std::shared_ptr<CMprDisplayImage>);
};

#endif

