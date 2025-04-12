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

#ifndef PARONAMA_FRAME_VIEW_MODEL_88BC1F1E6ED6428989B9A56EE96C82BB
#define PARONAMA_FRAME_VIEW_MODEL_88BC1F1E6ED6428989B9A56EE96C82BB


#pragma once

#include <ViewModels/ParonamaFrameViewModel/paronamaframeviewmodel_global.h>

#include <ViewModels/ParonamaFrameViewModel/ParonamaDisplayImage.h>
#include <ViewModels/ParonamaFrameViewModel/ParonamaDisplayImage.h>
#include <ViewModels/ParonamaFrameViewModel/ParonamaDisplayImage.h>

#include <Foundations/Common/ToDefine.h>

#include "Foundations/Interfaces/ServiceRequest.h"
#include "utilities/Messenger/Messenger.h"
#include <Services/Paronama/ParonamaService.h>

#include <vector>

class PARONAMAFRAMEVIEWMODEL_API CParonamaFrameViewModel : public CMessengerOwner
{
public:
	CParonamaFrameViewModel();

	void Initialize();

	void RemoveCurrentStudy();

	void Load(std::shared_ptr<CServiceRequest> request);

	void Reset();

private:
	void UpdateDisplayImages();

	void InitializeDisplayImages();

	std::shared_ptr<CParonamaDisplayImage> Load(std::shared_ptr<CParonamaResult> resultImage);
	
private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CParonamaFrameViewModel);
	
	AUTO_PROPERTY_REF(ServiceInstance, std::shared_ptr<CParonamaService>);

	AUTO_PROPERTY_REF(DisplayImages, std::vector<std::shared_ptr<CDisplayImage>>);

	AUTO_PROPERTY(TransverseImage, std::shared_ptr<CParonamaDisplayImage>);
	AUTO_PROPERTY(SaggitalImage, std::shared_ptr<CParonamaDisplayImage>);
	AUTO_PROPERTY(CoronalImage, std::shared_ptr<CParonamaDisplayImage>);
	AUTO_PROPERTY(ParonamaImage, std::shared_ptr<CParonamaDisplayImage>);
};


#endif
