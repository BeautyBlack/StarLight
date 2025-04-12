#include "stdafx.h"
#include <ViewModels/ImageFrameViewModel/DisplayImage.h>
#include "Foundations/Common/TimeHelper.h"
#include <minwinbase.h>
#include <QString>

CDisplayImage::CDisplayImage()
{
	SetDisplayImagePixelsSize(0);
	
	SetWindowCenterInternal(0);
	SetWindowWidthInternal(1);

	SetIsSelected(false);
	SetIsActive(false);

	SetCurrentDisplayImageIndex(0);

	m_IsDisplayImageInfo = true;

	SetImageScaleInternal(1);

	SetTranslateXInternal(0);
	SetTranslateYInternal(0);
	SetViewSizeHeight(0);
	SetViewSizeWidth(0);

	SetImageSizeChanged(true);
}

CDisplayImage::~CDisplayImage()
{
}

void CDisplayImage::Release()
{
	GetDisplayImages().clear();

	SetCurrentDisplayImageIndex(0);

	SetIsSelected(false);
}

void CDisplayImage::ResetImage(int nWidth, int nHeight)
{
	if (GetCurrentDisplayImage().expired())
		return;

	auto image = GetCurrentDisplayImage().lock();
	
	auto imagePlan = image->GetImagePlan();

	ApplayWindowLevel(imagePlan->GetWindowCenter(), imagePlan->GetWindowWidth());

	image->ClearAnnotations();

	for (auto &item : GetDisplayImages())
	{
		auto itemT = item.lock();

		itemT->SetImageScaleInternal(1);

		itemT->SetTranslateXInternal(0);
		itemT->SetTranslateYInternal(0);
		itemT->SetViewSizeHeight(0);
		itemT->SetViewSizeWidth(0);

		itemT->ResizeInternal(nWidth, nHeight);

		itemT->SetWindowWidthInternal(imagePlan->GetWindowWidth());
		itemT->SetWindowCenterInternal(imagePlan->GetWindowCenter());
	}

	image->UpdateQImage();
}

/**
 * 设置窗宽窗位
 */
void CDisplayImage::ApplayWindowLevel(double windowCenter, double windowWidth)
{
	auto image = GetCurrentDisplayImage().lock();
	
	image->SetWindowWidthInternal(windowWidth);
	image->SetWindowCenterInternal(windowCenter);

	image->WindowLevelPixels(windowWidth, windowCenter);
}

/**
 * 获取CT值
 */
double CDisplayImage::CTValue(int x, int y)
{
	auto image = GetCurrentDisplayImage().lock();

	const auto &imagePlan = image->GetImagePlan();

	const auto pixels = imagePlan->GetCurrentFramePixelData();

	const auto nIndex = x + y * image->GetImageWidth();

	if(imagePlan->GetIsSigned())
	{
		if(imagePlan->GetIsSampleWord())
		{
			short *pPixel = (short*)pixels;

			return ToCTValue(pPixel[nIndex], imagePlan->GetSlope(), imagePlan->GetIntercept());
		}
		else
		{
			int8_t *pPixel = (int8_t*)pixels;

			return ToCTValue(pPixel[nIndex], imagePlan->GetSlope(), imagePlan->GetIntercept());
		}
	}
	else
	{
		if (imagePlan->GetIsSampleWord())
		{
			ushort *pPixel = (ushort*)pixels;

			return ToCTValue(pPixel[nIndex], imagePlan->GetSlope(), imagePlan->GetIntercept());
		}
		else
		{
			uint8_t *pPixel = (uint8_t*)pixels;

			return ToCTValue(pPixel[nIndex], imagePlan->GetSlope(), imagePlan->GetIntercept());
		}
	}
}

int CDisplayImage::GetFrameIndex()
{
	return GetReferencedImageInstance()->GetImagePlan()->GetFrameIndex();
}

template<typename T>
void ConvertWindowLevel(T *sourcePixels,
	byte destPixels[],
	bool isRevert,
	int height,
	int sourceWidth,
	int destWidth,
	int minValue,
	int maxValue,
	int windowWidth)
{
	TRACE("\nWindow Level - %d, %d, %d\n", height, sourceWidth, destWidth);
	
	if (destPixels == NULL_SYMBAL || sourcePixels == NULL_SYMBAL
		|| height <= 0
		|| sourceWidth <= 0
		|| destWidth <= 0)
		return;

	ZeroMemory(destPixels, height * destWidth);

	if(isRevert)
	{
		for(auto column = 0; column < height; ++column)
		{
			auto sourceRow = column * sourceWidth;
			auto destRow = column * destWidth;

			auto sourctPtr = sourcePixels + sourceRow;
			auto destPtr = destPixels + destRow;

			for(auto x = 0, y = min(sourceWidth, destWidth); x < y; ++x)
			{
				const auto aPixel = *sourctPtr;

				if (aPixel <= minValue)
					*destPtr = 255;
				else if (aPixel >= maxValue)
					*destPtr = 0;
				else
					*destPtr = 255 - static_cast<BYTE_TYPE>(round((aPixel - minValue) * 255.0 / windowWidth));

				++sourctPtr;
				++destPtr;
			}
		}
	}
	else
	{
		for (auto column = 0; column < height; ++column)
		{
			auto sourceRow = column * sourceWidth;
			auto destRow = column * destWidth;

			auto sourctPtr = sourcePixels + sourceRow;
			auto destPtr = destPixels + destRow;

			for (auto x = 0, y = min(sourceWidth, destWidth); x < y; ++x)
			{
				const auto aPixel = *sourctPtr;//sourcePixels[x];

				if (aPixel <= minValue)
					*destPtr = 0;
				else if (aPixel >= maxValue)
					*destPtr = 255;
				else
					*destPtr = static_cast<BYTE_TYPE>(round((aPixel - minValue) * 255.0 / windowWidth));

				++sourctPtr;
				++destPtr;
			}
		}
	}
}

void CDisplayImage::WindowLevelPixels(double windowWidth, double windowCenter)
{
	const auto imagePlan = GetImagePlan();
	const auto frameSize = imagePlan->GetOneFramePixelSize();

	const auto height = GetImageHeight();
	const auto width = GetImageWidth();

	const auto adjustWidth = WIDTHSTEP(width, 8);

	SetDisplayImageHeightInternal(height);
	SetDisplayImageWidthInternal(adjustWidth);

	auto newSize = adjustWidth * height;
	if (!GetDisplayImagePixelsInternal()
		|| newSize > GetDisplayImagePixelsSize())
	{
		SetDisplayImagePixelsInternal(
			std::shared_ptr<BYTE_TYPE>(new BYTE_TYPE[newSize],
				std::default_delete<BYTE_TYPE[]>()));

		SetDisplayImagePixelsSize(newSize);
	}
	
	auto minValue = windowCenter - windowWidth / 2;
	auto maxValue = windowCenter + windowWidth / 2;

	minValue = (minValue - imagePlan->GetIntercept()) / imagePlan->GetSlope();
	maxValue = (maxValue - imagePlan->GetIntercept()) / imagePlan->GetSlope();

	auto pixels = imagePlan->GetCurrentFramePixelData();

	auto displayImagePixels = GetDisplayImagePixelsInternal().get();


	if(imagePlan->GetIsSampleWord())
	{
		if(imagePlan->GetIsSigned())
			ConvertWindowLevel<short>((short*)pixels,
				displayImagePixels,
				imagePlan->GetPhotometricInterpretation() == PhotometricInterpretation::MONOCHROME1,
				height,
				width,
				adjustWidth,
				minValue,
				maxValue,
				windowWidth
			);
		else
			ConvertWindowLevel<ushort>((ushort*)pixels,
				displayImagePixels,
				imagePlan->GetPhotometricInterpretation() == PhotometricInterpretation::MONOCHROME1,
				height,
				width,
				adjustWidth,
				minValue,
				maxValue,
				windowWidth
			);
	}
	else
	{
		if (imagePlan->GetIsSigned())
			ConvertWindowLevel<BYTE_TYPE>((BYTE_TYPE*)pixels,
				displayImagePixels,
				imagePlan->GetPhotometricInterpretation() == PhotometricInterpretation::MONOCHROME1,
				height,
				width,
				adjustWidth,
				minValue,
				maxValue,
				windowWidth
				);
		else
			ConvertWindowLevel<unsigned char>((unsigned char*)pixels,
				displayImagePixels,
				imagePlan->GetPhotometricInterpretation() == PhotometricInterpretation::MONOCHROME1,
				height,
				width,
				adjustWidth,
				minValue,
				maxValue,
				windowWidth
				);
	}
}

std::shared_ptr<CImagePlanModule> CDisplayImage::GetImagePlan()
{
	return GetReferencedImageInstance()->GetImagePlan();
}

void CDisplayImage::ClearAnnotations()
{
	if(!GetCurrentDisplayImage().expired())
		GetCurrentDisplayImage().lock()->GetAnnotationsInternal().clear();
}

///////////////// Mouse/UI Actions  /////////////////////////
/**
 * 切换下上层
 * @isForword : true - 切换至前一层； false - 切换至后一层
 */
void CDisplayImage::SwitchToNextFrame(bool isForword /* = false*/)
{
	const int nSize = GetDisplayImages().size();

	const auto diff = isForword ? -1 : 1;
	
	if (GetCurrentDisplayImage().expired())
		return;

	auto image = GetCurrentDisplayImage().lock();

	const auto nextFrameIndex =
		image->GetReferencedImageInstance()
			->GetImagePlan()
			->GetFrameIndex() + diff;
	auto switchToNextImage = false;
	auto nextImageIndex = GetCurrentDisplayImageIndex() + diff;

	if (nextFrameIndex >= GetReferencedImageInstance()->GetImagePlan()->GetFrameCount()
		|| nextFrameIndex < 0)
		switchToNextImage = true;

	if (switchToNextImage) // 切换到下一张图像
	{
		if (nextImageIndex >= nSize)
			nextImageIndex = 0;
		else if (nextImageIndex < 0)
			nextImageIndex = max(0, nSize - 1);

		SetCurrentDisplayImageIndex(nextImageIndex);

		SetCurrentDisplayImage(GetDisplayImages()[GetCurrentDisplayImageIndex()]);

		GetCurrentDisplayImage().lock()->GetReferencedImageInstance()
			->GetImagePlan()
			->SetFrameIndex(0);
	}
	else
	{
		GetCurrentDisplayImage().lock()->GetReferencedImageInstance()
			->GetImagePlan()
			->SetFrameIndex(nextFrameIndex);
	}
}

void CDisplayImage::SwitchToFrame(int nFrameIndex /* = 0 */)
{
	GetCurrentDisplayImage().reset();

	SetCurrentDisplayImageIndex(nFrameIndex);

	if (GetDisplayImages().empty())
		return;

	SetCurrentDisplayImage(
		GetDisplayImages()[GetCurrentDisplayImageIndex()]);
}

void CDisplayImage::SetIsDisplayImageInfo(bool display)
{
	for (auto &item : GetDisplayImages())
	{
		if( !item.expired())
			item.lock()->m_IsDisplayImageInfo = display;
	}
}

void CDisplayImage::TranslateImage(int nOffsetX, int nOffsetY)
{
	if (!GetCurrentDisplayImage().expired())
		GetCurrentDisplayImage().lock()->TranslateImageInternal(nOffsetX, nOffsetY);
}

void CDisplayImage::TranslateImageInternal(int nOffsetX, int nOffsetY)
{
	SetTranslateXInternal(GetTranslateXInternal() + nOffsetX);
	SetTranslateYInternal(GetTranslateYInternal() + nOffsetY);
}

void CDisplayImage::WindowLevel(int nOffsetCenter, int nOffsetWidth)
{
	if (GetCurrentDisplayImage().expired())
		return;
	
	auto windowCenter = GetCurrentDisplayImage().lock()->GetWindowCenterInternal();
	auto windowWidth = GetCurrentDisplayImage().lock()->GetWindowWidthInternal();

	ApplayWindowLevel(windowCenter + nOffsetCenter, windowWidth + nOffsetWidth);

	GetCurrentDisplayImage().lock()->UpdateQImage();
}

void CDisplayImage::Scale(double dbFactor)
{
	for (auto it = GetDisplayImages().begin();
		it != GetDisplayImages().end();
		++it)
	{
		if( !it->expired())
			it->lock()->SetImageScaleInternal(GetImageScaleInternal() + dbFactor);
	}
}

bool CDisplayImage::IsScaleOverSize(double dbFactor)
{
	if (GetCurrentDisplayImage().expired())
		return false;

	const auto t = GetCurrentDisplayImage().lock()->GetImageScaleInternal() + dbFactor;

	return t > 4 || t < 0.25;
}

void CDisplayImage::AddAnnotation(std::shared_ptr<CAnnotationBase> annotation)
{
	auto image = GetCurrentDisplayImage();

	if (image.expired())
		return;
	
	image.lock()->GetAnnotationsInternal().push_back(annotation);

	annotation->SetReferenceImage(image);
	annotation->SetImagePlanModule(image.lock()->GetImagePlan());
}

void CDisplayImage::RemoveAnnotation(std::shared_ptr<CAnnotationBase> annotation)
{
	if ( !annotation || GetCurrentDisplayImage().expired())
		return;

	auto image = GetCurrentDisplayImage().lock();
	auto &annotations = image->GetAnnotationsInternal();
	
	for(auto it = annotations.begin();
		it != annotations.end();
		++it)
	{
		if((*it)->IsMe(annotation.get()))
		{
			annotations.erase(it);
			break;
		}
	}
}

void CDisplayImage::RemoveAnnotation()
{
	if ( GetCurrentDisplayImage().expired())
		return;

	auto image = GetCurrentDisplayImage().lock();
	auto &annotations = image->GetAnnotationsInternal();

	if (annotations.empty())
		return;
	
	annotations.pop_back();
}

///////////////// Mouse/UI Actions  /////////////////////////

QString CDisplayImage::GetFormatedTopLeftText()
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
	strText.Format("%s/%d%s/%s\n%s/%s\n%s",
		study->GetPatientName().c_str(),
		study->GetAge(),
		AgeUnitText(study->GetAgeUnit()),
		SexText(study->GetSex()),
		study->GetPatientId().c_str(),
		study->GetAccessionNumber().c_str(),
		series->GetBodyPart().c_str());

	return QString::fromLocal8Bit(
			strText.GetString(),
		strText.GetLength());

}

QString CDisplayImage::GetFormatedTopRightText()
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
	strText.Format("%s\nSe: %d/%d\nIm: %d/%d",
		CTimeHelper::ToDateTimeText(study->GetStudyDateTime()).c_str(),
		series->GetSeriesNumber(),
		(int)study->GetSeries().size(),
		instance->GetInstanceNumber(),
		(int)series->GetInstances().size());

	return QString::fromLocal8Bit(
		strText.GetString(), strText.GetLength());
}

QString CDisplayImage::GetFormatedBottomLeftText()
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
	strText.Format("THK: %0.2lf/%0.2lf\n%s\n%s",
		imagePlan->GetSliceThickness(),
		imagePlan->GetSpacingBetweenSlice(),
		series->GetSeriesDescription().c_str(),
		ModalityText(series->GetModality()));

	return QString::fromLocal8Bit(
		strText.GetString(), strText.GetLength());
}

QString CDisplayImage::GetFormatedBottomRightText()
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
	
	// 右下
	strText.Format("W: %d\nC: %d\n%0.2lfX\nFOV: %dX%d",
		(int)GetWindowWidthInternal(),
		(int)GetWindowCenterInternal(),
		GetImageScaleInternal(),
		(int)(imagePlan->GetPixelSpacingX() * imagePlan->GetWidth()),
		(int)(imagePlan->GetPixelSpacingY() * imagePlan->GetHeight()));

	return QString::fromLocal8Bit(
		strText.GetString(), strText.GetLength());
}

int CDisplayImage::GetTranslateX()
{
	if (GetCurrentDisplayImage().expired())
		return 0;

	return GetCurrentDisplayImage().lock()->GetTranslateXInternal();
}

int CDisplayImage::GetTranslateY()
{
	if (GetCurrentDisplayImage().expired())
		return 0;

	return GetCurrentDisplayImage().lock()->GetTranslateYInternal();
}

double CDisplayImage::GetImageScale()
{
	if (GetCurrentDisplayImage().expired())
		return 1;

	return GetCurrentDisplayImage().lock()->GetImageScaleInternal();
}

// X方向分辨率
double CDisplayImage::GetPixelSpacingX()
{
	return GetReferencedImageInstance()
			? GetReferencedImageInstance()->GetImagePlan()->GetPixelSpacingX()
			: 1;
}

// Y方向分辨率
double CDisplayImage::GetPixelSpacingY()
{
	return GetReferencedImageInstance()
		? GetReferencedImageInstance()->GetImagePlan()->GetPixelSpacingY()
		: 1;
}

// 层间距
double CDisplayImage::GetSpacingBetweenSlice()
{
	return GetReferencedImageInstance()
		? GetReferencedImageInstance()->GetImagePlan()->GetSpacingBetweenSlice()
		: 0;
}

// 层厚
double CDisplayImage::GetSliceThickness()
{
	return GetReferencedImageInstance()
		? GetReferencedImageInstance()->GetImagePlan()->GetSliceThickness()
		: 0;
}

// 斜率
double CDisplayImage::GetSlop()
{
	return GetReferencedImageInstance()
		? GetReferencedImageInstance()->GetImagePlan()->GetSlope()
		: 1;
}

// 截距
double CDisplayImage::GetIntercept()
{
	return GetReferencedImageInstance()
		? GetReferencedImageInstance()->GetImagePlan()->GetIntercept()
		: 0;
}

int CDisplayImage::GetOneFramePixelSize()
{
	const auto imagePlan = GetImagePlan();
	return imagePlan->GetOneFramePixelSize();
}

int CDisplayImage::GetImageWidth()
{
	return GetReferencedImageInstance()->GetImagePlan()->GetWidth();
}

int CDisplayImage::GetImageHeight()
{
	return GetReferencedImageInstance()->GetImagePlan()->GetHeight();
}

std::shared_ptr<BYTE_TYPE> CDisplayImage::GetDisplayImagePixels()
{
	return GetCurrentDisplayImage().lock()->GetDisplayImagePixelsInternal();
}

int CDisplayImage::GetDisplayImageWidth()
{
	return GetCurrentDisplayImage().lock()->GetDisplayImageWidthInternal();
}

int CDisplayImage::GetDisplayImageHeight()
{
	return GetCurrentDisplayImage().lock()->GetDisplayImageHeightInternal();
}

void CDisplayImage::UpdateQImage()
{
	auto pixels = GetDisplayImagePixelsInternal();

	if (!pixels)
		return;

	const auto imageWidth = GetDisplayImageWidthInternal();
	const auto imageHeight = GetDisplayImageHeightInternal();

	SetQImageInternal(std::make_shared<QImage>(
		pixels.get(),
		imageWidth,
		imageHeight,
		QImage::Format::Format_Grayscale8));
}

QImage* CDisplayImage::GetQImage()
{
	if (GetCurrentDisplayImage().expired())
		return NULL_SYMBAL;

	auto displayImage = GetCurrentDisplayImage().lock();

	if (displayImage->GetQImageInternal())
		return displayImage->GetQImageInternal().get();

	auto pixels = GetDisplayImagePixels();

	if (!pixels)
	{
		auto imagePlan = displayImage->GetImagePlan();

		ApplayWindowLevel(imagePlan->GetWindowCenter(), imagePlan->GetWindowWidth());

		pixels = GetDisplayImagePixels();
	}

	if (!pixels)
		return NULL_SYMBAL;

	const auto imageWidth = GetDisplayImageWidth();
	const auto imageHeight = GetDisplayImageHeight();

	displayImage->SetQImageInternal(std::make_shared<QImage>(
		pixels.get(),
		imageWidth,
		imageHeight,
		QImage::Format::Format_Grayscale8));

	return displayImage->GetQImageInternal().get();
}

CDisplayImage::AnnotationArrayType& CDisplayImage::GetAnnotations()
{
	return GetCurrentDisplayImage().expired()
		? GetAnnotationsInternal()
		: GetCurrentDisplayImage().lock()->GetAnnotationsInternal();
}

void CDisplayImage::Resize(int viewportWidth, int viewportHeight)
{
	for (auto &item : GetDisplayImages())
	{
		if( !item.expired())
			item.lock()->ResizeInternal(viewportWidth, viewportHeight);
	}
}

void CDisplayImage::ResizeInternal(int viewportWidth, int viewportHeight)
{
	if (GetViewSizeWidth() == viewportWidth
		&& GetViewSizeHeight() == viewportHeight
		&& !GetImageSizeChanged())
		return;

	SetImageSizeChanged(false);
	
	const auto factor = min(viewportWidth / (double)GetImageWidth(),
		viewportHeight / (double)GetImageHeight());

	TRACE("\nviewPort[%d,%d], Image[%d,%d]\n",
		viewportWidth, viewportHeight,
		GetImageWidth(), GetImageHeight());
	
	SetViewSizeHeight(viewportHeight);
	SetViewSizeWidth(viewportWidth);
	
	SetTranslateXInternal((GetViewSizeWidth() - GetImageWidth() * factor) / 2);
	SetTranslateYInternal((GetViewSizeHeight() - GetImageHeight() * factor) / 2);

	SetImageScaleInternal(factor);
}
