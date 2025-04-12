#include "stdafx.h"

#include "ViewModels/ParonamaFrameViewModel/ParonamaFrameViewModel.h"

#include "utilities/Messenger/EventArgs/UI/Paronama/ParonamaLoadImageEventArgs.h"

CParonamaFrameViewModel CParonamaFrameViewModel::m_Instance;

CParonamaFrameViewModel::CParonamaFrameViewModel()
{
}

void CParonamaFrameViewModel::InitializeDisplayImages()
{
	if (GetTransverseImage())
		GetTransverseImage()->GetDisplayImages().clear();
	
	if (GetSaggitalImage())
		GetSaggitalImage()->GetDisplayImages().clear();
	
	if (GetCoronalImage())
		GetCoronalImage()->GetDisplayImages().clear();
	
	if (GetParonamaImage())
		GetParonamaImage()->GetDisplayImages().clear();
	/*
	SetTransverseImage(std::make_shared<CParonamaDisplayImage>());
	SetSaggitalImage(std::make_shared<CParonamaDisplayImage>());
	SetCoronalImage(std::make_shared<CParonamaDisplayImage>());

	GetTransverseImage()->SetPlanDirection(ImagePlanDirection::Transverse);
	GetSaggitalImage()->SetPlanDirection(ImagePlanDirection::Sagittal);
	GetCoronalImage()->SetPlanDirection(ImagePlanDirection::Coronal);

	GetSaggitalImage()->GetDisplayImages().push_back(GetSaggitalImage());
	GetCoronalImage()->GetDisplayImages().push_back(GetCoronalImage());
	GetTransverseImage()->GetDisplayImages().push_back(GetTransverseImage());
	*/
}

void CParonamaFrameViewModel::Initialize()
{
	CMessenger::GetInstance().Regist(EMessengerKey::ParonamaLoadImages,
		this,
		[&](void* pParam)
	{
		CParonamaLoadImageEventArgs *pArgs = (CParonamaLoadImageEventArgs*)pParam;

		if (pArgs == NULL_SYMBAL || !pArgs->GetServiceRequest())
			return NULL_SYMBAL;

		auto request = pArgs->GetServiceRequest();

		RemoveCurrentStudy();
		Load(request);

		CMessenger::GetInstance().SendMessage(
			EMessengerKey::ParonamaUpdateDisplayImages,
			NULL_SYMBAL);

		return NULL_SYMBAL;
	});
}

void CParonamaFrameViewModel::RemoveCurrentStudy()
{
	if(GetServiceInstance())
		GetServiceInstance().reset();

	if(GetSaggitalImage())
		GetSaggitalImage().reset();

	if(GetCoronalImage())
		GetCoronalImage().reset();

	if(GetTransverseImage())
		GetTransverseImage().reset();
}

void CParonamaFrameViewModel::Load(std::shared_ptr<CServiceRequest> request)
{
	SetServiceInstance(std::make_shared<CParonamaService>());

	GetServiceInstance()->SetStudy(request->GetReferenceStudy());
	GetServiceInstance()->Initialize(request);
	GetServiceInstance()->Process();

	InitializeDisplayImages();

	UpdateDisplayImages();
}

void CParonamaFrameViewModel::Reset()
{
	GetServiceInstance()->Reset();

	GetServiceInstance()->Process();

	UpdateDisplayImages();

	CMessenger::GetInstance().SendMessage(EMessengerKey::ParonamaUpdateDisplayImages,
		NULL_SYMBAL);
}

void CParonamaFrameViewModel::UpdateDisplayImages()
{
	SetSaggitalImage(Load(GetServiceInstance()->GetSaggitalResult()));
	SetTransverseImage(Load(GetServiceInstance()->GetTransverseResult()));
	SetCoronalImage(Load(GetServiceInstance()->GetCoronalResult()));
	SetParonamaImage(Load(GetServiceInstance()->GetParonamaResult()));
}

std::shared_ptr<CParonamaDisplayImage> CParonamaFrameViewModel::Load(std::shared_ptr<CParonamaResult> resultImage)
{
	if ( !resultImage)
		return NULL_SYMBAL;
	
	auto series = CParonamaDisplayImage::CreateSeriesModule(
		resultImage);
	CParonamaDisplayImage::CreateInstanceModules(series,
		resultImage);

	std::shared_ptr<CParonamaDisplayImage> displayImage;
	
	for (auto &instance : series->GetInstances())
	{
		auto t = std::make_shared<CParonamaDisplayImage>();

		t->SetReferencedStudy(series->GetReferenceStudy().lock());
		t->SetReferencedSeries(series);
		t->SetReferencedImageInstance(instance);

		if ( !displayImage)
			displayImage = t;
		
		displayImage->GetDisplayImages().push_back(t);

		GetDisplayImages().push_back(t);
	}

	if (displayImage)
		displayImage->SwitchToFrame();

	return displayImage;
}

