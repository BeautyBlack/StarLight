#include "stdafx.h"
#include <ViewModels/MprFrameViewModel/MprFrameViewModel.h>

#include "utilities/Messenger/EventArgs/UI/MprFrame/MprLoadImageEventArgs.h"
#include "utilities/Messenger/Messenger.h"
#include "utilities/Messenger/EventArgs/UI/MprFrame/MprUpdateScoutLineCenterEventArgs.h"

#include <Services/Mpr/MprParameter.h>


#include "utilities/Messenger/EventArgs/UI/MprFrame/MprRotateScoutLineEventArgs.h"
#include "utilities/Messenger/EventArgs/UI/MprFrame/MprScrollSlicePositionChangedEventArgs.h"


CMprFrameViewModel CMprFrameViewModel::m_Instance;


CMprFrameViewModel::~CMprFrameViewModel()
{
	CMessenger::GetInstance().Unregist(EMessengerKey::MprUpdateScoutLineCenter, this);
	CMessenger::GetInstance().Unregist(EMessengerKey::MprLoadImages, this);
	CMessenger::GetInstance().Unregist(EMessengerKey::MprScrollPositionChanged, this);
	CMessenger::GetInstance().Unregist(EMessengerKey::MprScoutLineAngleChanged, this);
}

void CMprFrameViewModel::Initialize()
{
	CMessenger::GetInstance().Regist(EMessengerKey::MprLoadImages,
		this,
		[&](void* pParam)
	{
		CMprLoadImageEventArgs *pArgs = (CMprLoadImageEventArgs*)pParam;

		if (pArgs == NULL_SYMBAL || !pArgs->GetServiceRequest())
			return NULL_SYMBAL;

		auto request = pArgs->GetServiceRequest();

		RemoveCurrentStudy();
		Load(request);

		CMessenger::GetInstance().SendMessage(EMessengerKey::MprUpdateDisplayImages,
			NULL_SYMBAL);
		
		return NULL_SYMBAL;
	});

	CMessenger::GetInstance().Regist(
		EMessengerKey::MprUpdateScoutLineCenter,
		this,
		[&](void *pParam)
	{
		auto pEventArgs = (CMprUpdateScoutLineCenterEventArgs*)pParam;

		if (pEventArgs == NULL_SYMBAL)
			return NULL_SYMBAL;

		std::shared_ptr<CMprDisplayImage> image;

		if(pEventArgs->GetImagePlan() == ImagePlanDirection::Sagittal)
			image = GetSaggitalDisplayImage();
		else if (pEventArgs->GetImagePlan() == ImagePlanDirection::Transverse)
			image = GetTransverseDisplayImage();
		else if (pEventArgs->GetImagePlan() == ImagePlanDirection::Coronal)
			image = GetCoronalDisplayImage();

		if(image)
		{
			GetServiceInstance()->GetMprParameter()->GetParameter(pEventArgs->GetImagePlan()).MoveCenter(
				image->GetReferencedImageInstance()->GetImagePlan()->ImagePointToPatientPoint(
					pEventArgs->GetNewCenter()));

			GetServiceInstance()->Process();

			UpdateDisplayImages();

			CMessenger::GetInstance().SendMessage(EMessengerKey::MprUpdateDisplayImages,
				NULL_SYMBAL);
		}

		return NULL_SYMBAL;
	});

	CMessenger::GetInstance().Regist(
		EMessengerKey::MprScrollPositionChanged,
		this,
		[&](void *pParam)
	{
		auto pEventArgs = (CMprScrollSlicePositionChangedEventArgs*)pParam;

		if (pEventArgs == NULL_SYMBAL)
			return NULL_SYMBAL;

		GetServiceInstance()
			->GetMprParameter()
			->GetParameter(pEventArgs->GetImagePlan())
			.SetCurrentScrollSlicePosition(pEventArgs->GetNewPosition());
			
		GetServiceInstance()->Process();

		UpdateDisplayImages();

		CMessenger::GetInstance().SendMessage(EMessengerKey::MprUpdateDisplayImages,
			NULL_SYMBAL);
		
		return NULL_SYMBAL;
	});

	CMessenger::GetInstance().Regist(
	EMessengerKey::MprScoutLineAngleChanged,
		this,
		[&](void *pParam)
	{
		auto pEventArgs = (CMprRotateScoutLineEventArgs*)pParam;

		if (pEventArgs == NULL_SYMBAL)
			return NULL_SYMBAL;

		auto &mprParam = GetServiceInstance()
			->GetMprParameter()
			->GetParameter(pEventArgs->GetImagePlan());

		if (pEventArgs->GetIsVectorX())
			mprParam.RotateNormalX(pEventArgs->GetAngle());
		else
			mprParam.RotateNormalZ(pEventArgs->GetAngle());

		GetServiceInstance()->Process();

		UpdateDisplayImages();

		CMessenger::GetInstance().SendMessage(EMessengerKey::MprUpdateDisplayImages,
			NULL_SYMBAL);
		
		return NULL_SYMBAL;
	});
}

void CMprFrameViewModel::RemoveCurrentStudy()
{
	GetServiceInstance().reset();

	GetSaggitalDisplayImage().reset();
	GetCoronalDisplayImage().reset();
	GetTransverseDisplayImage().reset();
}

void CMprFrameViewModel::Load(std::shared_ptr<CServiceRequest> request)
{
	SetServiceInstance(std::make_shared<CMprService>());
	
	GetServiceInstance()->SetStudy(request->GetReferenceStudy());
	GetServiceInstance()->Initialize(request);
	GetServiceInstance()->Process();

	SetSaggitalDisplayImage(std::make_shared<CMprDisplayImage>());
	SetCoronalDisplayImage(std::make_shared<CMprDisplayImage>());
	SetTransverseDisplayImage(std::make_shared<CMprDisplayImage>());

	GetSaggitalDisplayImage()->GetDisplayImages().push_back(GetSaggitalDisplayImage());
	GetCoronalDisplayImage()->GetDisplayImages().push_back(GetCoronalDisplayImage());
	GetTransverseDisplayImage()->GetDisplayImages().push_back(GetTransverseDisplayImage());
	
	UpdateDisplayImages();
}

void CMprFrameViewModel::Reset()
{
	GetServiceInstance()->Reset();

	GetServiceInstance()->Process();

	UpdateDisplayImages();

	CMessenger::GetInstance().SendMessage(EMessengerKey::MprUpdateDisplayImages,
		NULL_SYMBAL);
}

void CMprFrameViewModel::UpdateDisplayImages()
{

	GetSaggitalDisplayImage()->FromMprResult(GetServiceInstance()->GetSagitalImageData());
	GetCoronalDisplayImage()->FromMprResult(GetServiceInstance()->GetCoronalImageData());
	GetTransverseDisplayImage()->FromMprResult(GetServiceInstance()->GetTranseverseImageData());
}


