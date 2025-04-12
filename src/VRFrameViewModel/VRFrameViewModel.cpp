#include "pch.h"
#include <ViewModels/VRFrameViewModel/VRFrameViewModel.h>

#include "utilities/Messenger/EventArgs/UI/MprFrame/MprLoadImageEventArgs.h"
#include "utilities/Messenger/Messenger.h"
#include "utilities/Messenger/EventArgs/UI/VRFrame/VRLoadImageEventArgs.h"

#include "utilities/Messenger/EventArgs/UI/MprFrame/MprRotateScoutLineEventArgs.h"
#include "utilities/Messenger/EventArgs/UI/MprFrame/MprScrollSlicePositionChangedEventArgs.h"


#include "Services/VR/VRService.h"

#include <ViewModels/VRFrameViewModel/ExportFlagDefine.h>


#include "utilities/Configuration/Configuration.h"
#include "utilities/Configuration/VRColorSetting.h"

CVRFrameViewModel CVRFrameViewModel::m_Instance;


CVRFrameViewModel::~CVRFrameViewModel()
{
	CMessenger::GetInstance().Unregist(EMessengerKey::VRLoadImages, this);
}

void CVRFrameViewModel::Initialize()
{
	CMessenger::GetInstance().Regist(EMessengerKey::VRLoadImages,
		this,
		[&](void* pParam)
	{
		CVRLoadImageEventArgs *pArgs = (CVRLoadImageEventArgs*)pParam;

		if (pArgs == NULL_SYMBAL || !pArgs->GetServiceRequest())
			return NULL_SYMBAL;

		auto request = pArgs->GetServiceRequest();

		RemoveCurrentStudy();
		Load(request);

		return NULL_SYMBAL;
	});


	CMessenger::GetInstance().Regist(
		EMessengerKey::UpdateColorSetting,
		this,
		[&](void *pParam)
	{
		CVRColorSetting *pSetting = (CVRColorSetting*)pParam;

		LoadColor(pSetting);

		GetServiceInstance()->UpdateColor();
		GetServiceInstance()->ManualRender();

		return NULL_SYMBAL;
	});
}

void CVRFrameViewModel::RemoveCurrentStudy()
{
	GetServiceInstance().reset();
}

void CVRFrameViewModel::Load(std::shared_ptr<CServiceRequest> request)
{
	SetServiceInstance(std::make_shared<CVRService>());

	GetServiceInstance()->SetStudy(request->GetReferenceStudy());

	LoadColor(&CConfiguration::GetInstance().GetVRInfo());
	
	GetServiceInstance()->Initialize(request);
	GetServiceInstance()->Process();
}

void CVRFrameViewModel::Reset()
{
	GetServiceInstance()->Reset();

	GetServiceInstance()->Process();

}

void CVRFrameViewModel::LoadColor(CVRColorSetting* pSetting)
{
	if (pSetting == NULL_SYMBAL)
		return;

	auto param = GetServiceInstance()->GetVRParameter();

	param->SetAmbient(pSetting->GetAmbient());
	param->SetDiffuse(pSetting->GetDiffuse());
	param->SetSpecularPower(pSetting->GetSpecularPower());
	param->SetSpecular(pSetting->GetSpecular());

	param->GetColorSegments().clear();
	param->GetOpacitySegments().clear();

	for (auto &it : pSetting->GetColors())
	{
		CDisplayPixelSegment segment;

		segment.SetPixel(it.ctValue);
		segment.SetValue(it.r << 16 | it.g << 8 | it.b);

		param->GetColorSegments().push_back(segment);
	}

	for (auto &it : pSetting->GetOpacities())
	{
		CDisplayPixelSegment segment;

		segment.SetPixel(it.ctValue);
		segment.SetValue(it.opacity / 255.0);

		param->GetOpacitySegments().push_back(segment);
	}
}

