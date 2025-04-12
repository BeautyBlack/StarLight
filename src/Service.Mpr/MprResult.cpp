#include "pch.h"
#include <Services/Mpr/MprResult.h>
CMprResult::CMprResult()
{
	SetScrollSlicePositionStart(0);
	SetScrollSlicePositionEnd(0);

}

CMprResult::~CMprResult()
{
}

//std::shared_ptr<CDisplayImage> CMprResult::ToDisplayImage()
//{
//	CreateSeriesModule();
//	CreateInstanceModule();
//	
//	auto displayImage = std::make_shared<CMprDisplayImage>();
//
//	displayImage->SetReferencedStudy(GetReferenceStudy());
//	displayImage->SetReferencedSeries(GetReferenceSeries());
//	displayImage->SetReferencedImageInstance(GetReferenceInstance());
//
//	return displayImage;
//}
//
//void CMprResult::CreateSeriesModule()
//{
//	if (GetReferenceSeries() || !GetReferenceStudy())
//		return;
//
//	SetReferenceSeries(std::make_shared<CSeriesModule>());
//
//	// 不需要在study中添加series
//	GetReferenceSeries()->SetReferenceStudy(GetReferenceStudy());
//	
//	GetReferenceSeries()->SetSeriesInstanceUID(
//		CDicomFileImporter::GenerateNewStudyInstanceUID());
//
//	GetReferenceSeries()->SetSeriesNumber((int)GetImagePlan());
//}
//
//void CMprResult::CreateInstanceModule()
//{
//	if (GetReferenceInstance())
//		return;
//
//	SetReferenceInstance(std::make_shared<CImageInstanceModule>());
//
//	auto imagePlan = std::make_shared<CImagePlanModule>();
//	GetReferenceInstance()->SetImagePlan(imagePlan);
//	imagePlan->SetReferenceImageInstance(GetReferenceInstance());
//
//	GetReferenceSeries()->AddInstanceModule(GetReferenceInstance());
//
//	// Instance
//	GetReferenceInstance()->SetInstanceNumber(1);
//	GetReferenceInstance()->SetInstanceUID(CDicomFileImporter::GenerateNewStudyInstanceUID());
//
//	// Image Plan
//	imagePlan->SetHeight(GetImageHeight());
//	imagePlan->SetWidth(GetImageWidth());
//
//	imagePlan->SetPixelSpacingX(GetPixelSpacingX());
//	imagePlan->SetPixelSpacingY(GetPixelSpacingY());
//	imagePlan->SetSpacingBetweenSlice(GetSpacingBetweenSlice());
//	imagePlan->SetSliceThickness(0);
//
//	imagePlan->SetIsSigned(GetIsPixelSigned());
//	imagePlan->SetIsSampleWord(GetIsPixelWord());
//
//	imagePlan->SetFrameCount(1);
//	imagePlan->SetPhotometricInterpretation(GetPhotometricInterpretation());
//	imagePlan->SetIntercept(GetIntercept());
//	imagePlan->SetSlope(GetSlop());
//
//	imagePlan->SetPosition(GetImagePosition());
//	imagePlan->SetOrientationX(GetOrientationX());
//	imagePlan->SetOrientationY(GetOrientationY());
//
//	imagePlan->SetWindowCenter(GetWindowCenter());
//	imagePlan->SetWindowWidth(GetWindowWidth());
//
//	imagePlan->SetOneFramePixelSize(imagePlan->GetWidth() * imagePlan->GetHeight());
//	imagePlan->SetOneFramePixelSizeInBytes(
//		imagePlan->GetIsSampleWord()
//		? 2 * imagePlan->GetOneFramePixelSize()
//		: imagePlan->GetOneFramePixelSize());
//
//	imagePlan->SetAllFramePixels(GetImageData());
//
//	imagePlan->SetFrameIndex(CImagePlanModule::DefaultFrameIndex);
//
//}
