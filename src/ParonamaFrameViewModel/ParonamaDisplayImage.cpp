#include "stdafx.h"
#include "ViewModels/ParonamaFrameViewModel/ParonamaDisplayImage.h"

#include "Services/Paronama/ParonamaResult.h"
#include "utilities/ImageModule/Service/DicomFileImporter.h"

void CParonamaDisplayImage::From(std::shared_ptr<CParonamaResult> resultImage)
{
	/*CreateSeriesModule(resultImage);
	CreateInstanceModules(resultImage);
	
	SetPlanDirection(resultImage->GetPlanDirection());

	SetImageSizeChanged(true);

	auto imagePlan = GetImagePlan();

	imagePlan->SetPosition(resultImage->GetImagePosition());
	imagePlan->SetOrientationX(resultImage->GetOrientationX());
	imagePlan->SetOrientationY(resultImage->GetOrientationY());

	auto center2D = imagePlan->PatientPointToImagePlane(resultImage->GetCenter());

	auto scoutLineXBegin = imagePlan->PatientPointToImagePlane(
		resultImage->GetScoutLineX().begin);
	auto scoutLineXEnd = imagePlan->PatientPointToImagePlane(
		resultImage->GetScoutLineX().end);

	auto scoutLineZBegin = imagePlan->PatientPointToImagePlane(
		resultImage->GetScoutLineZ().begin);
	auto scoutLineZEnd = imagePlan->PatientPointToImagePlane(
		resultImage->GetScoutLineZ().end);

	SetCenter2D(center2D);

	SetScoutLineBeginX2D(scoutLineXBegin);
	SetScoutLineEndX2D(scoutLineXEnd);

	SetScoutLineBeginZ2D(scoutLineZBegin);
	SetScoutLineEndZ2D(scoutLineZEnd);

	SetScrollSlicePositionStart(resultImage->GetScrollSlicePositionStart());
	SetScrollSlicePositionEnd(resultImage->GetScrollSlicePositionEnd());
	SetScrollSlicePosition(resultImage->GetScrollSlicePosition());*/
}

QString CParonamaDisplayImage::GetFormatedTopLeftText()
{
	if (GetCurrentDisplayImage().expired())
		return EMPTY_STRING;

	auto study = GetReferencedStudy();
	auto series = GetReferencedSeries();

	if (!study || !series)
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

QString CParonamaDisplayImage::GetFormatedTopRightText()
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

	// 右上
	strText.Format("Im: %d/%d",
		instance->GetInstanceNumber(),
		(int)series->GetInstances().size());

	return QString::fromLocal8Bit(
		strText.GetString(), strText.GetLength());
}

QString CParonamaDisplayImage::GetFormatedBottomLeftText()
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

QString CParonamaDisplayImage::GetFormatedBottomRightText()
{
	return EMPTY_STRING;
}


std::shared_ptr<CSeriesModule> CParonamaDisplayImage::CreateSeriesModule(
	std::shared_ptr<CParonamaResult> result)
{
	if (!result)
		return NULL_SYMBAL;
	
	auto series = std::make_shared<CSeriesModule>();

	// 不需要在study中添加series
	series->SetReferenceStudy(result->GetReferenceStudy());
	series->SetSeriesDescription("牙弓线");
	series->SetSeriesInstanceUID(
		CDicomFileImporter::GenerateNewStudyInstanceUID());

	switch (result->GetImagePlan())
	{
	case ImagePlanDirection::Transverse:
		series->SetSeriesNumber(1);
		break;
	case ImagePlanDirection::Coronal:
		series->SetSeriesNumber(2);
		break;
	case ImagePlanDirection::Sagittal:
		series->SetSeriesNumber(3);
		break;
	case ImagePlanDirection::Paronama:
		series->SetSeriesNumber(4);
		break;
	}

	return series;
}

void CParonamaDisplayImage::CreateInstanceModules(std::shared_ptr<CSeriesModule> series,
	std::shared_ptr<CParonamaResult> result)
{
	if (!result || !result->GetResultImages())
		return;
	
	auto itBegin = result->GetResultImages()->begin();
	auto itEnd = result->GetResultImages()->end();

	auto nInstanceIndex = 1;
	
	while(itBegin != itEnd)
	{
		auto instance = std::make_shared<CImageInstanceModule>();
		auto imagePlan = std::make_shared<CImagePlanModule>();
		
		instance->SetImagePlan(imagePlan);
		imagePlan->SetReferenceImageInstance(instance);

		series->AddInstanceModule(instance);

		// Instance
		instance->SetInstanceNumber(nInstanceIndex++);
		instance->SetInstanceUID(CDicomFileImporter::GenerateNewStudyInstanceUID());

		imagePlan->SetIsSigned((*itBegin)->GetIsPixelSigned());
		imagePlan->SetIsSampleWord((*itBegin)->GetIsPixelWord());

		imagePlan->SetFrameCount(1);
		imagePlan->SetPhotometricInterpretation((*itBegin)->GetPhotometricInterpretation());
		imagePlan->SetIntercept((*itBegin)->GetIntercept());
		imagePlan->SetSlope((*itBegin)->GetSlop());

		// Image Plan
		imagePlan->SetHeight((*itBegin)->GetImageHeight());
		imagePlan->SetWidth((*itBegin)->GetImageWidth());

		imagePlan->SetPixelSpacingX((*itBegin)->GetPixelSpacingX());
		imagePlan->SetPixelSpacingY((*itBegin)->GetPixelSpacingY());
		imagePlan->SetSpacingBetweenSlice((*itBegin)->GetSpacingBetweenSlice());
		imagePlan->SetSliceThickness(0);

		imagePlan->SetPosition((*itBegin)->GetImagePosition());
		imagePlan->SetOrientationX((*itBegin)->GetOrientationX());
		imagePlan->SetOrientationY((*itBegin)->GetOrientationY());

		imagePlan->SetWindowCenter((*itBegin)->GetWindowCenter());
		imagePlan->SetWindowWidth((*itBegin)->GetWindowWidth());

		imagePlan->SetOneFramePixelSize(imagePlan->GetWidth() * imagePlan->GetHeight());
		imagePlan->SetOneFramePixelSizeInBytes(
			imagePlan->GetIsSampleWord()
			? 2 * imagePlan->GetOneFramePixelSize()
			: imagePlan->GetOneFramePixelSize());

		imagePlan->SetAllFramePixels((*itBegin)->GetImageData());

		imagePlan->SetFrameIndex(CImagePlanModule::DefaultFrameIndex);

		++itBegin;
	}
}

