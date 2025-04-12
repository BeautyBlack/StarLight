#include "stdafx.h"
#include <ViewModels/MprFrameViewModel/MprDisplayImage.h>

#include "utilities/ImageModule/Service/DicomFileImporter.h"


void CMprDisplayImage::FromMprResult(std::shared_ptr<CMprResult> mprResult)
{
	CreateSeriesModule(mprResult);
	CreateInstanceModule(mprResult);

	SetImageSizeChanged(true);

	SetPlanDirection(mprResult->GetPlanDirection());
	
	auto imagePlan = GetImagePlan();
	
	imagePlan->SetPosition(mprResult->GetImagePosition());
	imagePlan->SetOrientationX(mprResult->GetOrientationX());
	imagePlan->SetOrientationY(mprResult->GetOrientationY());

	auto center2D = imagePlan->PatientPointToImagePlane(mprResult->GetCenter());

	auto scoutLineXBegin = imagePlan->PatientPointToImagePlane(
		mprResult->GetScoutLineX().begin);
	auto scoutLineXEnd = imagePlan->PatientPointToImagePlane(
		mprResult->GetScoutLineX().end);

	auto scoutLineZBegin = imagePlan->PatientPointToImagePlane(
		mprResult->GetScoutLineZ().begin);
	auto scoutLineZEnd = imagePlan->PatientPointToImagePlane(
		mprResult->GetScoutLineZ().end);

	SetCenter2D(center2D);

	SetScoutLineBeginX2D(scoutLineXBegin);
	SetScoutLineEndX2D(scoutLineXEnd);
	
	SetScoutLineBeginZ2D(scoutLineZBegin);
	SetScoutLineEndZ2D(scoutLineZEnd);
	
	SetScrollSlicePositionStart(mprResult->GetScrollSlicePositionStart());
	SetScrollSlicePositionEnd(mprResult->GetScrollSlicePositionEnd());
	SetScrollSlicePosition(mprResult->GetScrollSlicePosition());
}


void CMprDisplayImage::CreateSeriesModule(
	std::shared_ptr<CMprResult> mprResult)
{
	if (GetReferencedSeries() && !GetReferencedStudy())
		return;

	SetReferencedStudy(mprResult->GetReferenceStudy());
	
	SetReferencedSeries(std::make_shared<CSeriesModule>());

	// 不需要在study中添加series
	GetReferencedSeries()->SetReferenceStudy(GetReferencedStudy());
	GetReferencedSeries()->SetSeriesDescription("MPR");
	GetReferencedSeries()->SetSeriesInstanceUID(
		CDicomFileImporter::GenerateNewStudyInstanceUID());

	GetReferencedSeries()->SetSeriesNumber((int)mprResult->GetImagePlan());
}

void CMprDisplayImage::CreateInstanceModule(
	std::shared_ptr<CMprResult> mprResult)
{
	if ( !GetReferencedImageInstance())
	{
		SetReferencedImageInstance(std::make_shared<CImageInstanceModule>());

		auto imagePlan = std::make_shared<CImagePlanModule>();
		GetReferencedImageInstance()->SetImagePlan(imagePlan);
		imagePlan->SetReferenceImageInstance(GetReferencedImageInstance());

		GetReferencedSeries()->AddInstanceModule(GetReferencedImageInstance());

		// Instance
		GetReferencedImageInstance()->SetInstanceNumber(1);
		GetReferencedImageInstance()->SetInstanceUID(CDicomFileImporter::GenerateNewStudyInstanceUID());

		imagePlan->SetIsSigned(mprResult->GetIsPixelSigned());
		imagePlan->SetIsSampleWord(mprResult->GetIsPixelWord());

		imagePlan->SetFrameCount(1);
		imagePlan->SetPhotometricInterpretation(mprResult->GetPhotometricInterpretation());
		imagePlan->SetIntercept(mprResult->GetIntercept());
		imagePlan->SetSlope(mprResult->GetSlop());

	}

	auto imagePlan = GetReferencedImageInstance()->GetImagePlan();
	
	// Image Plan
	imagePlan->SetHeight(mprResult->GetImageHeight());
	imagePlan->SetWidth(mprResult->GetImageWidth());

	imagePlan->SetPixelSpacingX(mprResult->GetPixelSpacingX());
	imagePlan->SetPixelSpacingY(mprResult->GetPixelSpacingY());
	imagePlan->SetSpacingBetweenSlice(mprResult->GetSpacingBetweenSlice());
	imagePlan->SetSliceThickness(0);
	
	imagePlan->SetPosition(mprResult->GetImagePosition());
	imagePlan->SetOrientationX(mprResult->GetOrientationX());
	imagePlan->SetOrientationY(mprResult->GetOrientationY());

	imagePlan->SetWindowCenter(mprResult->GetWindowCenter());
	imagePlan->SetWindowWidth(mprResult->GetWindowWidth());

	imagePlan->SetOneFramePixelSize(imagePlan->GetWidth() * imagePlan->GetHeight());
	imagePlan->SetOneFramePixelSizeInBytes(
		imagePlan->GetIsSampleWord()
		? 2 * imagePlan->GetOneFramePixelSize()
		: imagePlan->GetOneFramePixelSize());

	imagePlan->SetAllFramePixels(mprResult->GetImageData());

	imagePlan->SetFrameIndex(CImagePlanModule::DefaultFrameIndex);

	SetCurrentDisplayImage(GetDisplayImages()[0]);
	
	SwitchToFrame();
}

QString CMprDisplayImage::GetFormatedTopLeftText()
{
	if (GetCurrentDisplayImage().expired())
		return EMPTY_STRING;

	auto study = GetReferencedStudy();
	auto series = GetReferencedSeries();
	auto instance = GetCurrentDisplayImage().lock()->GetReferencedImageInstance();
	auto imagePlan = instance ? instance->GetImagePlan() : std::shared_ptr<CImagePlanModule>();

	if (!study || !series || !instance || !imagePlan)
		return EMPTY_STRING;

	CString strText;

	// 左上
	strText.Format("%s/%s\n%s",
		study->GetPatientName().c_str(),
		study->GetPatientId().c_str(),
		series->GetSeriesDescription().c_str());

	return QString::fromLocal8Bit(
		strText.GetString(),
		strText.GetLength());

}

QString CMprDisplayImage::GetFormatedTopRightText()
{
	return EMPTY_STRING;
}

QString CMprDisplayImage::GetFormatedBottomLeftText()
{
	if (GetCurrentDisplayImage().expired())
		return EMPTY_STRING;

	auto study = GetReferencedStudy();
	auto series = GetReferencedSeries();
	auto instance = GetCurrentDisplayImage().lock()->GetReferencedImageInstance();
	auto imagePlan = instance ? instance->GetImagePlan() : std::shared_ptr<CImagePlanModule>();

	if (!study || !series || !instance || !imagePlan)
		return EMPTY_STRING;

	CString strText;

	// 左下
	strText.Format("W: %d\nC: %d\n",
		(int)GetWindowWidthInternal(),
		(int)GetWindowCenterInternal());

	return QString::fromLocal8Bit(
		strText.GetString(), strText.GetLength());
}

QString CMprDisplayImage::GetFormatedBottomRightText()
{
	return EMPTY_STRING;
}


