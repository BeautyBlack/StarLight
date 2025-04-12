#include "stdafx.h"
#include <ViewModels/ImageFrameViewModel/StudyTabItemViewModel.h>


#include "Foundations/Interfaces/Enums/ImageProcessErrorCode.h"
#include "Foundations/Interfaces/ServiceRequest.h"
#include "utilities/Messenger/EventArgs/UI/MprFrame/MprLoadImageEventArgs.h"
#include "utilities/Messenger/Messenger.h"
#include "utilities/Messenger/EventArgs/UI/VRFrame/VRGetRenderWindowParentHandleEventArgs.h"


#include <vector>

#include "../Foundation.Common/MathUtility.h"
#include "utilities/Messenger/EventArgs/UI/MessageBox/RequestMessageBoxEventArgs.h"
#include <utilities/Messenger/EventArgs/UI/VRFrame/VRLoadImageEventArgs.h>

#include "utilities/Messenger/EventArgs/UI/Paronama/ParonamaLoadImageEventArgs.h"

CStudyTabItemViewModel::CStudyTabItemViewModel()
{
	SetDisplayImageMode(DisplayImageMode::Series);

	SetLayoutRows(2);
	SetLayoutColumn(2);
}

CStudyTabItemViewModel::~CStudyTabItemViewModel()
{
}

void CStudyTabItemViewModel::SwitchToSeriesMode()
{
	if (!GetStudy())
		return;

	for (auto &aImage : GetDisplayImages())
	{
		aImage->Release();
	}

	GetDisplayImages().clear();

	for (auto &series : GetStudy()->GetSeries())
	{
		auto &instances = series->GetInstances();

		auto displayImage = GetDisplayImagesCache()[
			instances[0]->GetInstanceUID().c_str()];

		for (auto &instance : instances)
		{
			auto t = GetDisplayImagesCache()[
				instance->GetInstanceUID().c_str()];

			displayImage->GetDisplayImages().push_back(t);

		}

		GetDisplayImages().push_back(displayImage);
	}

	for(auto &displayImage : GetDisplayImages())
	{
		displayImage->SwitchToFrame();
	}
}

void CStudyTabItemViewModel::SwitchToImageMode()
{
	if (!GetStudy())
		return;
	
	for(auto &aImage : GetDisplayImages())
	{
		aImage->Release();
	}

	GetDisplayImages().clear();

	for(auto &series : GetStudy()->GetSeries())
	{
		for(auto &instance : series->GetInstances())
		{
			auto displayImage = GetDisplayImagesCache()[
				instance->GetInstanceUID().c_str()];

			GetDisplayImages().push_back(displayImage);
		}
	}

	for (auto &displayImage : GetDisplayImages())
	{
		displayImage->SwitchToFrame();
	}
}

bool CStudyTabItemViewModel::IsMatchStudyUID(const char* strStudyUID)
{
	return GetStudy()
		? GetStudy()->GetStudyInstanceUID().compare(strStudyUID) == 0
		: false;
}

void CStudyTabItemViewModel::ReloadStudy()
{
	CreateDisplayImages();

	if (GetDisplayImageMode() == DisplayImageMode::Series)
		SwitchToSeriesMode();
	else
		SwitchToImageMode();
}

ImageProcessErrorCode CStudyTabItemViewModel::SendToMpr(std::shared_ptr<CDisplayImage> selectedImage)
{
	auto t = GenerateRequest(
		selectedImage->GetReferencedImageInstance()->GetReferenceSeries(),
		[](void)
	{
		return std::make_shared<CServiceRequest>();
	});

	auto result = std::get<0>(t);

	if (result != ImageProcessErrorCode::Success)
	{
		CRequestMessageBoxEventArgs args;

		args.SetTitleText("MPR 失败");

		switch(result)
		{
		case ImageProcessErrorCode::ImagesWidthOrHeightConflict:
			args.SetMessageText("序列内图像宽高不一致");
			break;
			
		case ImageProcessErrorCode::ImagesCountNotEnought:
			args.SetMessageText("序列图像数量少于3张");
			break;

		case ImageProcessErrorCode::ImagesSlopOrInterceptConflict:
		case ImageProcessErrorCode::ImagesCoordinateConflict:
		case ImageProcessErrorCode::ImagesSpacingConflict:
		default:
			args.SetMessageText("序列内图像参数信息不一致");
			break;
		}
		
		CMessenger::GetInstance().SendMessage(
			EMessengerKey::DisplayMessageBox,
			&args);
		
		return result;
	}
	
	CMprLoadImageEventArgs eventArgs;

	eventArgs.SetServiceRequest(std::get<1>(t));

	CMessenger::GetInstance().SendMessage(
		EMessengerKey::MprLoadImages,
		&eventArgs);
	
	return result;
}

ImageProcessErrorCode CStudyTabItemViewModel::SendToVR(std::shared_ptr<CDisplayImage> selectedImage)
{
	auto t = GenerateRequest(
		selectedImage->GetReferencedImageInstance()->GetReferenceSeries(),
		[](void)
	{
		return std::make_shared<CVRServiceRequest>();
	});

	auto result = std::get<0>(t);

	if (result != ImageProcessErrorCode::Success)
	{
		CRequestMessageBoxEventArgs args;

		args.SetTitleText("VR 失败");

		switch (result)
		{
		case ImageProcessErrorCode::ImagesWidthOrHeightConflict:
			args.SetMessageText("序列内图像宽高不一致");
			break;

		case ImageProcessErrorCode::ImagesCountNotEnought:
			args.SetMessageText("序列图像数量少于3张");
			break;

		case ImageProcessErrorCode::ImagesSlopOrInterceptConflict:
		case ImageProcessErrorCode::ImagesCoordinateConflict:
		case ImageProcessErrorCode::ImagesSpacingConflict:
		default:
			args.SetMessageText("序列内图像参数信息不一致");
			break;
		}

		CMessenger::GetInstance().SendMessage(
			EMessengerKey::DisplayMessageBox,
			&args);

		return result;
	}

	CVRLoadImageEventArgs eventArgs;

	eventArgs.SetServiceRequest(
		std::dynamic_pointer_cast<CVRServiceRequest>(
		std::get<1>(t)));
	
	CVRGetRenderWindowParentHandleEventArgs getEventArgs;
	CMessenger::GetInstance().SendMessage(
		EMessengerKey::GetVRRenderWindowParentHandle,
		&getEventArgs,
		[&](void* pHandle) 
	{
		eventArgs.GetServiceRequest()->SetParentRenderWindowHandle(pHandle);
	});

	CMessenger::GetInstance().SendMessage(
		EMessengerKey::VRLoadImages,
		&eventArgs);

	return result;
}

ImageProcessErrorCode CStudyTabItemViewModel::SendToParonama(std::shared_ptr<CDisplayImage> selectedImage)
{
	auto t = GenerateRequest(
		selectedImage->GetReferencedImageInstance()->GetReferenceSeries(),
		[](void)
	{
		return std::make_shared<CVRServiceRequest>();
	});

	auto result = std::get<0>(t);

	if (result != ImageProcessErrorCode::Success)
	{
		CRequestMessageBoxEventArgs args;

		args.SetTitleText("牙弓线 失败");

		switch (result)
		{
		case ImageProcessErrorCode::ImagesWidthOrHeightConflict:
			args.SetMessageText("序列内图像宽高不一致");
			break;

		case ImageProcessErrorCode::ImagesCountNotEnought:
			args.SetMessageText("序列图像数量少于3张");
			break;

		case ImageProcessErrorCode::ImagesSlopOrInterceptConflict:
		case ImageProcessErrorCode::ImagesCoordinateConflict:
		case ImageProcessErrorCode::ImagesSpacingConflict:
		default:
			args.SetMessageText("序列内图像参数信息不一致");
			break;
		}

		CMessenger::GetInstance().SendMessage(
			EMessengerKey::DisplayMessageBox,
			&args);

		return result;
	}

	CParonamaLoadImageEventArgs eventArgs;

	eventArgs.SetServiceRequest( std::get<1>(t));

	CMessenger::GetInstance().SendMessage(
		EMessengerKey::ParonamaLoadImages,
		&eventArgs);

	return result;
}


std::tuple<ImageProcessErrorCode, std::shared_ptr<CServiceRequest>>
CStudyTabItemViewModel::GenerateRequest(
	std::weak_ptr<CSeriesModule> series,
	std::function<std::shared_ptr<CServiceRequest>()> createRequestFunc)
{
	ImageProcessErrorCode code = ImageProcessErrorCode::ImagesCountNotEnought;

	static auto emptyRequest = createRequestFunc();
	// std::shared_ptr<CServiceRequest>();
	
	auto nIndex = 0;
	auto instances = series.lock()->GetInstances();
	const int count = instances.size();

	if (count < 3)
		return std::tie(code, emptyRequest);
	
	auto request = createRequestFunc();
	// std::make_shared<CServiceRequest>();
	
	request->SetSliceCount(count);
	request->SetReferenceStudy(GetStudy());
	request->SetReferenceSeries(series.lock());

	code = ImageProcessErrorCode::Success;

	auto minWindowValue = 0, maxWindowValue = 0;
	
	for (auto aInstance : instances)
	{
		if(nIndex++ == 0)
		{
			request->SetImageHeight(aInstance->GetImagePlan()->GetHeight());
			request->SetImageWidth(aInstance->GetImagePlan()->GetWidth());

			request->SetImagePosition(aInstance->GetImagePlan()->GetPosition());
			request->SetOrientationX(aInstance->GetImagePlan()->GetOrientationX());
			request->SetOrientationY(aInstance->GetImagePlan()->GetOrientationY());
			
			request->SetIntercept(aInstance->GetImagePlan()->GetIntercept());
			request->SetSlop(aInstance->GetImagePlan()->GetSlope());

			request->SetIsPixelSigned(aInstance->GetImagePlan()->GetIsSigned());
			request->SetIsPixelWord(aInstance->GetImagePlan()->GetIsSampleWord());

			request->SetPixelSpacingX(aInstance->GetImagePlan()->GetPixelSpacingX());
			request->SetPixelSpacingY(aInstance->GetImagePlan()->GetPixelSpacingY());
			request->SetSpacingBetweenSlice(aInstance->GetImagePlan()->GetSpacingBetweenSlice());

			const auto half = aInstance->GetImagePlan()->GetWindowWidth() / 2;
			
			minWindowValue = aInstance->GetImagePlan()->GetWindowCenter() - half;
			maxWindowValue = aInstance->GetImagePlan()->GetWindowCenter() + half;
		}
		else
		{
			if(request->GetImageHeight()
				!= aInstance->GetImagePlan()->GetHeight()
				|| request->GetImageWidth()
				!= aInstance->GetImagePlan()->GetWidth())
			{
				code = ImageProcessErrorCode::ImagesWidthOrHeightConflict;
				break;
			}

			if(request->GetIntercept() != aInstance->GetImagePlan()->GetIntercept()
				|| request->GetSlop() != aInstance->GetImagePlan()->GetSlope())
			{
				code = ImageProcessErrorCode::ImagesSlopOrInterceptConflict;
				break;
			}

			if(abs(request->GetPixelSpacingX() - aInstance->GetImagePlan()->GetPixelSpacingX()) > 0.1
				|| abs(request->GetPixelSpacingY() - aInstance->GetImagePlan()->GetPixelSpacingY()) > 0.1
				|| abs(request->GetSpacingBetweenSlice() - aInstance->GetImagePlan()->GetSpacingBetweenSlice()) > 0.1)
			{
				code = ImageProcessErrorCode::ImagesSpacingConflict;
				break;
			}

			if(!request->GetOrientationX().Equal(aInstance->GetImagePlan()->GetOrientationX())
				|| !request->GetOrientationY().Equal(aInstance->GetImagePlan()->GetOrientationY()))
			{
				code = ImageProcessErrorCode::ImagesCoordinateConflict;
				break;
			}

			const auto half = aInstance->GetImagePlan()->GetWindowWidth() / 2;

			minWindowValue = min(minWindowValue, aInstance->GetImagePlan()->GetWindowCenter() - half);
			maxWindowValue = max(maxWindowValue, aInstance->GetImagePlan()->GetWindowCenter() + half);
		}
	}

	if(code != ImageProcessErrorCode::Success)
		return std::tie(code, emptyRequest);

	auto lastInstance = instances[count - 1];
	auto firstInstance = instances[0];

	auto firstPlan = firstInstance->GetImagePlan();
	auto lastPlan = lastInstance->GetImagePlan();

	request->SetLastSliceImagePosition(lastPlan->GetPosition());

	request->SetSpacingBetweenSlice(
		CMathUtility::ComputeDistance(request->GetImagePosition(),
			lastPlan->GetPosition())
		/ abs(lastInstance->GetInstanceNumber()
			- firstInstance->GetInstanceNumber()));

	request->SetWindowCenter((minWindowValue + maxWindowValue) / 2);
	request->SetWindowWidth(abs(maxWindowValue - minWindowValue));

	double sliceOrder[3] = {
		lastPlan->GetPosition().x - firstPlan->GetPosition().x,
		lastPlan->GetPosition().y - firstPlan->GetPosition().y,
		lastPlan->GetPosition().z - firstPlan->GetPosition().z};

	double imageMatrix[4][4] = { 0 };

	CMathUtility::FormatImagePlanMatrix(firstPlan->GetOrientationX(),
		firstPlan->GetOrientationY(),
		imageMatrix);

	// 求切片方向与图片法线夹角
	auto angle = CMathUtility::ComputeAngle3D(
		sliceOrder[0], sliceOrder[1], sliceOrder[2],
		imageMatrix[0][2], imageMatrix[1][2], imageMatrix[2][2]);
	
	int beginIndex = 0, endIndex = count, offset = 1;
	
	if (angle </*>=*/ CMathUtility::PI() / 2)
	{
		// 正序
		// empty
	}
	else
	{
		beginIndex = count-1;
		endIndex = -1;
		offset = -1;

		request->SetImagePosition(lastPlan->GetPosition());
		request->SetOrientationX(lastPlan->GetOrientationX());
		request->SetOrientationY(lastPlan->GetOrientationY());

		request->SetLastSliceImagePosition(firstPlan->GetPosition());
	}

	const auto oneFrameInBytes = firstPlan->GetOneFramePixelSizeInBytes();
	auto pixels = new BYTE_TYPE[oneFrameInBytes * count];
	
	request->SetImageData(std::shared_ptr<BYTE_TYPE>(pixels,
		std::default_delete<BYTE_TYPE[]>()));
	
	for( ; beginIndex != endIndex; beginIndex += offset, pixels += oneFrameInBytes)
	{
		memcpy_s(pixels,
			oneFrameInBytes,
			instances[beginIndex]->GetImagePlan()->GetCurrentFramePixelData(),
			oneFrameInBytes);
	}
	
	return std::tie(code, request);
}

void CStudyTabItemViewModel::CreateDisplayImages()
{
	if ( !GetStudy())
		return;

	for(auto &series : GetStudy()->GetSeries())
	{
		for(auto &instance : series->GetInstances())
		{
			auto itPos = GetDisplayImagesCache().find(
				instance->GetInstanceUID().c_str());

			if(itPos == GetDisplayImagesCache().end())
			{
				// 没有该Image, 则新建
				auto displayImage = std::make_shared<CDisplayImage>();

				displayImage->SetReferencedStudy(GetStudy());
				displayImage->SetReferencedSeries(series);
				displayImage->SetReferencedImageInstance(instance);

				GetDisplayImagesCache().insert_or_assign(
					instance->GetInstanceUID().c_str(),
					displayImage);
			}
		}
	}

}

