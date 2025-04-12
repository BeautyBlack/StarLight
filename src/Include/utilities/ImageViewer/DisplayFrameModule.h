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

#ifndef DISPLAY_FRAME_MODULE_6A353B78961D43E28EBD8E5F386A0333
#define DISPLAY_FRAME_MODULE_6A353B78961D43E28EBD8E5F386A0333

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <utilities/ImageModule/Modules/ImageInstanceModule.h>
#include <utilities/ImageModule/Modules/ImagePlanModule.h>


#ifdef UTILITIESIMAGEVIEWER_EXPORTS
#define UTILITIESIMAGEVIEWER_API __declspec(dllexport)
#else
#define UTILITIESIMAGEVIEWER_API __declspec(dllimport)
#endif

class UTILITIESIMAGEVIEWER_API CDisplayFrameModule
{
public:
	// Statis Functions

	static std::unique_ptr<CDisplayFrameModule> CreateDisplayFrameModule(
		std::shared_ptr<CImageInstanceModule>);

public:
	CDisplayFrameModule();
	~CDisplayFrameModule();

	/*
	 * Description
	 * 计算窗宽窗位
	 */
	void UpdateWindowWidth(double dbWindowWidth, double dbWindowCenter);

	double GetCTValue(int x, int y);

	void Reset();

private:
	void LoadFromImageInstance();
	
private:
	AUTO_PROPERTY(ReferenceInstance, std::shared_ptr<CImageInstanceModule>);

	AUTO_PROPERTY(ReferenceImagePlan, std::shared_ptr<CImagePlanModule>);

	/*
	 * 显示像素
	 */
	AUTO_PROPERTY(DisplayPixels, std::shared_ptr<BYTE>);

	///////////////////   操作相关   /////////////////////

	/*
	 * 窗位
	 */
	AUTO_PROPERTY(WindowCenter, double);

	/*
	 * 窗宽
	 */
	AUTO_PROPERTY(WindowWidth, double);

	/*
	 * PhotometricInterpretation 像素显示编码方式
	 */
	AUTO_PROPERTY(PhotometricInterpretation, PhotometricInterpretation);

	/*
	 * 缩放倍率
	 */
	AUTO_PROPERTY(Scale, double);

	/*
	 * X方向偏移量
	 */
	AUTO_PROPERTY(TranslateX, double);

	/*
	 * Y方向偏移量
	 */
	AUTO_PROPERTY(TranslateY, double);


	///////////////////   图像像素参数相关[只读型]   /////////////////////

	AUTO_PROPERTY_REF_NO_SET(Width, int);

	AUTO_PROPERTY_REF_NO_SET(Height, int);

	AUTO_PROPERTY_REF_NO_SET(IsSigned, bool);

	AUTO_PROPERTY_REF_NO_SET(IsWord, bool);

	AUTO_PROPERTY_REF_NO_SET(Slop, double);

	AUTO_PROPERTY_REF_NO_SET(Intercept, double);

	///////////////////   图像显示参数相关[只读型]   /////////////////////

	AUTO_PROPERTY_REF_NO_SET(PixelSpacingX, double);

	AUTO_PROPERTY_REF_NO_SET(PixelSpacingY, double);

	AUTO_PROPERTY_REF_NO_SET(SliceThickness, double);

	AUTO_PROPERTY_REF_NO_SET(SpaceBetweenSlice, double);

	AUTO_PROPERTY_REF_NO_SET(ModalityText, STRING_TYPE);

	AUTO_PROPERTY_REF_NO_SET(PatientName, STRING_TYPE);

	AUTO_PROPERTY_REF_NO_SET(PatientId, STRING_TYPE);

	AUTO_PROPERTY_REF_NO_SET(PatientAge, STRING_TYPE);

	AUTO_PROPERTY_REF_NO_SET(BirthDate, STRING_TYPE);

	AUTO_PROPERTY_REF_NO_SET(Sex, STRING_TYPE);

	AUTO_PROPERTY_REF_NO_SET(StudyId, STRING_TYPE);

	AUTO_PROPERTY_REF_NO_SET(StudyDatetime, STRING_TYPE);

	AUTO_PROPERTY_REF_NO_SET(TotalSeries, int);

	AUTO_PROPERTY_REF_NO_SET(TotalInstance, int);

	AUTO_PROPERTY_REF_NO_SET(SeriesNumber, int);

	AUTO_PROPERTY_REF_NO_SET(InstanceNumber, int);
};

#endif

