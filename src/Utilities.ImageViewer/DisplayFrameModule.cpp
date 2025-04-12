#include "stdafx.h"
//#include <utilities/ImageViewer/DisplayFrameModule.h>
//#include "Foundations/Interfaces/Modality.h"
//
//#include <Utilities/ImageModule/Modules/SeriesModule.h>
//#include <Utilities/ImageModule/Modules/StudyModule.h>
//#include "Foundations/Common/TimeHelper.h"
//
//
//std::unique_ptr<CDisplayFrameModule> CDisplayFrameModule::CreateDisplayFrameModule(
//	std::shared_ptr<CImageInstanceModule> module)
//{
//	auto result = std::make_unique<CDisplayFrameModule>();
//
//	result->SetReferenceInstance(module);
//	result->SetReferenceImagePlan(module->GetImagePlan());
//
//	result->LoadFromImageInstance();
//
//	result->Reset();
//
//	return result;
//}
//
//CDisplayFrameModule::CDisplayFrameModule()
//{
//	SetWindowCenter(1);
//	SetWindowWidth(1);
//	SetScale(1);
//	SetTranslateX(0);
//	SetTranslateY(0);
//
//	m_Width = m_Height = 0;
//	m_IsSigned = false;
//	m_IsWord = true;
//	m_Slop = 1;
//	m_Intercept = 0;
//
//	m_PixelSpacingX = m_PixelSpacingY = m_SliceThickness = m_SpaceBetweenSlice = 1;
//
//	m_TotalSeries = m_SeriesNumber = 0;
//	m_TotalInstance = m_InstanceNumber = 0;
//}
//
//CDisplayFrameModule::~CDisplayFrameModule()
//{
//	GetDisplayPixels().reset();
//}
//
//void CDisplayFrameModule::UpdateWindowWidth(double dbWindowWidth, double dbWindowCenter)
//{
//	SetWindowWidth(dbWindowWidth);
//	SetWindowCenter(dbWindowCenter);
//
//	if (!GetDisplayPixels())
//		SetDisplayPixels(std::make_shared<BYTE_TYPE>(GetWidth() * GetHeight()));
//
//	auto minValue = dbWindowCenter - dbWindowWidth / 2;
//	auto maxValue = dbWindowCenter + dbWindowWidth / 2;
//
//	minValue = (minValue - GetIntercept()) / GetSlop();
//	maxValue = (maxValue - GetIntercept()) / GetSlop();
//
//
//	if (GetIsWord())
//	{
//		if(GetIsSigned())
//		{
//			const auto pixels = reinterpret_cast<INT16*>(GetReferenceImagePlan()->GetCurrentFramePixelData());
//
//			const auto endPixels = pixels + GetReferenceImagePlan()->GetOneFramePixelSize();
//
//			auto sourceIndex = pixels;
//
//			if (GetPhotometricInterpretation() == PhotometricInterpretation::MONOCHROME1)
//			{
//				for (auto destIndex = GetDisplayPixels().get();
//					sourceIndex != endPixels;
//					++sourceIndex, ++destIndex)
//				{
//					const auto aPixel = *sourceIndex;
//
//					if (aPixel < minValue)
//						*destIndex = 255;
//					else if (aPixel >= maxValue)
//						*destIndex = 0;
//					else
//						*destIndex = (255 - round((aPixel - minValue) / dbWindowWidth * 255.0));
//				}
//			}
//			else
//			{
//				for (auto destIndex = GetDisplayPixels().get();
//					sourceIndex != endPixels;
//					++sourceIndex, ++destIndex)
//				{
//					const auto aPixel = *sourceIndex;
//
//					if (aPixel < minValue)
//						*destIndex = 255;
//					else if (aPixel >= maxValue)
//						*destIndex = 0;
//					else
//						*destIndex = round((aPixel - minValue) / dbWindowWidth * 255.0);
//				}
//			}
//		}
//		else
//		{
//			const auto pixels = reinterpret_cast<UINT16*>(GetReferenceImagePlan()->GetPixels());
//
//			const auto endPixels = pixels + GetReferenceImagePlan()->GetOneFramePixelSize();
//
//			auto sourceIndex = pixels;
//
//			if (GetPhotometricInterpretation() == PhotometricInterpretation::MONOCHROME1)
//			{
//				for(auto destIndex = GetDisplayPixels().get();
//					sourceIndex != endPixels;
//					++sourceIndex, ++destIndex)
//				{
//					const auto aPixel = *sourceIndex;
//
//					if (aPixel < minValue)
//						*destIndex = 255;
//					else if (aPixel >= maxValue)
//						*destIndex = 0;
//					else
//						*destIndex = (255 - round((aPixel - minValue) / dbWindowWidth * 255.0));
//				}
//			}
//			else
//			{
//				for (auto destIndex = GetDisplayPixels().get();
//					sourceIndex != endPixels;
//					++sourceIndex, ++destIndex)
//				{
//					const auto aPixel = *sourceIndex;
//
//					if (aPixel < minValue)
//						*destIndex = 255;
//					else if (aPixel >= maxValue)
//						*destIndex = 0;
//					else
//						*destIndex = round((aPixel - minValue) / dbWindowWidth * 255.0);
//				}
//			}
//		}
//		
//	}
//	else
//	{
//		if (GetPhotometricInterpretation() == PhotometricInterpretation::MONOCHROME1)
//		{
//			auto end = GetReferenceImagePlan()->GetPixels() + GetReferenceImagePlan()->GetOneFramePixelSizeInBytes();
//
//			for(auto destIndex = GetDisplayPixels().get(), sourceIndex = GetReferenceImagePlan()->GetPixels();
//				sourceIndex != end;
//				++sourceIndex, ++destIndex)
//				*destIndex = 255 - (*destIndex);
//		}
//		else
//		{
//			memcpy_s(GetDisplayPixels().get(),
//				GetReferenceImagePlan()->GetOneFramePixelSizeInBytes(),
//				GetReferenceImagePlan()->GetPixels(),
//				GetReferenceImagePlan()->GetOneFramePixelSizeInBytes());
//		}
//	}
//}
//
//double CDisplayFrameModule::GetCTValue(int x, int y)
//{
//	if (x < 0 || y < 0)
//		return 0;
//
//	if (x >= GetWidth() || y >= GetHeight())
//		return 0;
//
//	int nValue = 0;
//
//	if(GetIsWord())
//	{
//		UINT16 *pPixel = reinterpret_cast<UINT16*>(GetReferenceImagePlan()->GetPixels());
//
//		const auto pixelValuePtr = pPixel + (x + y * GetHeight());
//
//		nValue = GetIsSigned() ? (*reinterpret_cast<INT16*>(pixelValuePtr)) : (*pixelValuePtr);
//		
//	}
//	else
//	{
//		const auto pixelValuePtr = GetReferenceImagePlan()->GetPixels() + (x + y * GetHeight());
//
//		nValue = GetIsSigned() ? *reinterpret_cast<INT8*>(pixelValuePtr) : (0x00FF & (*pixelValuePtr));
//	}
//
//	return nValue * GetScale() + GetIntercept();
//}
//
//void CDisplayFrameModule::Reset()
//{
//	SetWindowCenter(1);
//	SetWindowWidth(1);
//	SetScale(1);
//	SetTranslateX(0);
//	SetTranslateY(0);
//
//	UpdateWindowWidth(GetReferenceImagePlan()->GetWindowWidth(), GetReferenceImagePlan()->GetWindowCenter());
//}
//
//void CDisplayFrameModule::LoadFromImageInstance()
//{
//	if (!GetReferenceImagePlan())
//		return;
//
//	m_Width = GetReferenceImagePlan()->GetWidth();
//	m_Height = GetReferenceImagePlan()->GetHeight();
//	m_IsSigned = GetReferenceImagePlan()->GetIsSigned();
//	m_IsWord = GetReferenceImagePlan()->GetIsSampleWord();
//	m_Slop = GetReferenceImagePlan()->GetSlope();
//	m_Intercept = GetReferenceImagePlan()->GetIntercept();
//
//	m_PixelSpacingX = GetReferenceImagePlan()->GetPixelSpacingX();
//	m_PixelSpacingY = GetReferenceImagePlan()->GetPixelSpacingY();
//	m_SliceThickness = GetReferenceImagePlan()->GetSliceThickness();
//	m_SpaceBetweenSlice = GetReferenceImagePlan()->GetSpacingBetweenSlice();
//
//	auto series = GetReferenceInstance()->GetReferenceSeries().lock();
//
//	m_ModalityText = ModalityText(series->GetModality());
//
//	auto study = series->GetReferenceStudy().lock();
//
//	m_PatientName = study->GetPatientName();
//	m_PatientId = study->GetPatientId();
//	m_PatientAge = study->GetAge();
//	m_PatientAge += AgeUnitText(study->GetAgeUnit());
//	m_BirthDate = CTimeHelper::ToDateText(study->GetBirthDate());
//	m_Sex = SexText(study->GetSex());
//	m_StudyId = study->GetStudyId();
//	m_StudyDatetime = CTimeHelper::ToDateTimeText(study->GetStudyDateTime());
//	m_TotalSeries = study->GetSeries().size();
//	m_TotalInstance = series->GetInstances().size();
//	m_SeriesNumber = series->GetSeriesNumber();
//	m_InstanceNumber = GetReferenceInstance()->GetInstanceNumber();
//}
