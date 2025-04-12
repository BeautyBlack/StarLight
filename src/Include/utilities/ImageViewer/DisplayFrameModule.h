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
	 * ���㴰��λ
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
	 * ��ʾ����
	 */
	AUTO_PROPERTY(DisplayPixels, std::shared_ptr<BYTE>);

	///////////////////   �������   /////////////////////

	/*
	 * ��λ
	 */
	AUTO_PROPERTY(WindowCenter, double);

	/*
	 * ����
	 */
	AUTO_PROPERTY(WindowWidth, double);

	/*
	 * PhotometricInterpretation ������ʾ���뷽ʽ
	 */
	AUTO_PROPERTY(PhotometricInterpretation, PhotometricInterpretation);

	/*
	 * ���ű���
	 */
	AUTO_PROPERTY(Scale, double);

	/*
	 * X����ƫ����
	 */
	AUTO_PROPERTY(TranslateX, double);

	/*
	 * Y����ƫ����
	 */
	AUTO_PROPERTY(TranslateY, double);


	///////////////////   ͼ�����ز������[ֻ����]   /////////////////////

	AUTO_PROPERTY_REF_NO_SET(Width, int);

	AUTO_PROPERTY_REF_NO_SET(Height, int);

	AUTO_PROPERTY_REF_NO_SET(IsSigned, bool);

	AUTO_PROPERTY_REF_NO_SET(IsWord, bool);

	AUTO_PROPERTY_REF_NO_SET(Slop, double);

	AUTO_PROPERTY_REF_NO_SET(Intercept, double);

	///////////////////   ͼ����ʾ�������[ֻ����]   /////////////////////

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

