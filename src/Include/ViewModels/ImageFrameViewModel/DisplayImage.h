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

#ifndef DISPLAY_IMAGE_6DFBC13423714F52B22D813EF3175303
#define DISPLAY_IMAGE_6DFBC13423714F52B22D813EF3175303

#pragma once


#include <Foundations/Common/ToDefine.h>

#include <ViewModels/ImageFrameViewModel/imageframeviewmodel_global.h>
#include <ViewModels/ImageFrameViewModel/OriginalToViewportTransformMatrix.h>
#include <ViewModels/ImageFrameViewModel/DisplayImage.h>
#include <viewmodels/ImageFrameViewModel/Annotation/AnnotationBase.h>

#include <QImage>

#include <Utilities/ImageModule/Modules/ImageInstanceModule.h>
#include <Utilities/ImageModule/Modules/StudyModule.h>
#include <Utilities/ImageModule/Modules/SeriesModule.h>
#include "Annotation/AnnotationBase.h"

class CStudyModule;
class CSeriesModule;
class CImageInstanceModule;

class IMAGEFRAMEVIEWMODEL_API CDisplayImage
{
public:
	using AnnotationArrayType = std::vector<std::shared_ptr<CAnnotationBase>>;

public:
	CDisplayImage();
	~CDisplayImage();

	virtual void Release();
	
public:
	///////////////// Image Actions  /////////////////////////

	/**
	 * ����
	 */
	void ResetImage(int nWidth, int nHeight);

	/**
	 * ���ô���λ
	 */
	void ApplayWindowLevel(double windowCenter, double windowWidth);

	/**
	 * ��ȡCTֵ
	 */
	double CTValue(int x, int y);

	///////////////// Mouse/UI Actions  /////////////////////////
	/**
	 * �л����ϲ�
	 * @isForword : true - �л���ǰһ�㣻 false - �л�����һ��
	 */
	void SwitchToNextFrame(bool isForword = false);

	void SwitchToFrame(int nFrameIndex = 0);

	void SetIsDisplayImageInfo(bool display);

	void TranslateImage(int nOffsetX, int nOffsetY);

	void WindowLevel(int nOffsetCenter, int nOffsetWidth);

	void Scale(double dbFactor);

	bool IsScaleOverSize(double dbFactor);

	///////////////// Annotations  /////////////////////////////
	/**
	 * ��ӱ�ע
	 */
	void AddAnnotation(std::shared_ptr<CAnnotationBase> annotation);

	/**
	 * ɾ����ע
	 */
	void RemoveAnnotation(std::shared_ptr<CAnnotationBase> annotation);

	void RemoveAnnotation();
	///////////////// Mouse/UI Actions  /////////////////////////
	
	virtual QString GetFormatedTopLeftText();

	virtual QString GetFormatedTopRightText();

	virtual QString GetFormatedBottomLeftText();

	virtual QString GetFormatedBottomRightText();
	
	// X����ֱ���
	double GetPixelSpacingX();

	// Y����ֱ���
	double GetPixelSpacingY();

	// ����
	double GetSpacingBetweenSlice();

	// ���
	double GetSliceThickness();

	// б��
	double GetSlop();

	// �ؾ�
	double GetIntercept();

	int GetOneFramePixelSize();

	std::shared_ptr<BYTE_TYPE> GetDisplayImagePixels();

	int GetDisplayImageWidth();

	int GetDisplayImageHeight();

	QImage* GetQImage();

	AnnotationArrayType& GetAnnotations();

	/////////////////   UI ��ʾ��� ///////////////////////

	// ������ʾ���
	void Resize(int viewportWidth, int viewportHeight);
	
	int GetTranslateX();
	int GetTranslateY();

	double GetImageScale();

protected:
	
	// ͼ����
	int GetImageWidth();

	// ͼ��߶�
	int GetImageHeight();

	// ֡������
	int GetFrameIndex();

	void WindowLevelPixels(double windowWidth, double windowCenter);

	std::shared_ptr<CImagePlanModule> GetImagePlan();

	void ClearAnnotations();

	void ResizeInternal(int viewportWidth, int viewportHeight);

	void TranslateImageInternal(int nOffsetX, int nOffsetY);

	void UpdateQImage();

private:
	template<class T>
	static double ToCTValue(T pixel, double slop, double intercept)
	{
		return pixel * slop + intercept;
	}

private:
	// ��λ
	AUTO_PROPERTY(WindowCenterInternal, double);
	// ����
	AUTO_PROPERTY(WindowWidthInternal, double);

	// ��������λ�����ʾ����
	AUTO_PROPERTY(DisplayImagePixelsInternal, std::shared_ptr<BYTE_TYPE>);

	// ��������λ�����ʾͼ����
	AUTO_PROPERTY(DisplayImageWidthInternal, int);

	// ��������λ�����ʾͼ��߶�
	AUTO_PROPERTY(DisplayImageHeightInternal, int);

private:
	AUTO_PROPERTY_REF(ReferencedStudy, std::shared_ptr<CStudyModule>);

	AUTO_PROPERTY_REF(ReferencedSeries, std::shared_ptr<CSeriesModule>);

	AUTO_PROPERTY_REF(ReferencedImageInstance, std::shared_ptr<CImageInstanceModule>);

	// ��ǰ��ʾ��Image
	AUTO_PROPERTY_REF(CurrentDisplayImage, std::weak_ptr<CDisplayImage>);

	// ��ǰ��ʾ��Image
	AUTO_PROPERTY_REF(CurrentDisplayImageIndex, int);

	// ��ǰ��ʾ��Image
	AUTO_PROPERTY_REF(DisplayImages, std::vector<std::weak_ptr<CDisplayImage>>);

	// Annotation ����
	AUTO_PROPERTY_REF(AnnotationsInternal, AnnotationArrayType);

	//////////////////  UI  �߼���� ///////////////////////
	
	// ѡ��״̬
	AUTO_PROPERTY(IsSelected, bool);
	// ѡ��״̬
	AUTO_PROPERTY(IsActive, bool);

	/////////////////   UI  ��ʾ��Ϣ��λ�á���ת�ȡ�////////////////////////

	AUTO_PROPERTY_REF_NO_SET(IsDisplayImageInfo, bool);

	AUTO_PROPERTY(ImageScaleInternal, double);

	AUTO_PROPERTY(TranslateXInternal, int);
	AUTO_PROPERTY(TranslateYInternal, int);
	AUTO_PROPERTY(ViewSizeWidth, int);
	AUTO_PROPERTY(ViewSizeHeight, int);

	/**
	 * �Ҷ�Ӱ��
	 */
	AUTO_PROPERTY_REF(QImageInternal, std::shared_ptr<QImage>);

	AUTO_PROPERTY(DisplayImagePixelsSize, int);

	AUTO_PROPERTY(ImageSizeChanged, bool);
};

#endif

