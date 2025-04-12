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
	 * 重置
	 */
	void ResetImage(int nWidth, int nHeight);

	/**
	 * 设置窗宽窗位
	 */
	void ApplayWindowLevel(double windowCenter, double windowWidth);

	/**
	 * 获取CT值
	 */
	double CTValue(int x, int y);

	///////////////// Mouse/UI Actions  /////////////////////////
	/**
	 * 切换下上层
	 * @isForword : true - 切换至前一层； false - 切换至后一层
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
	 * 添加标注
	 */
	void AddAnnotation(std::shared_ptr<CAnnotationBase> annotation);

	/**
	 * 删除标注
	 */
	void RemoveAnnotation(std::shared_ptr<CAnnotationBase> annotation);

	void RemoveAnnotation();
	///////////////// Mouse/UI Actions  /////////////////////////
	
	virtual QString GetFormatedTopLeftText();

	virtual QString GetFormatedTopRightText();

	virtual QString GetFormatedBottomLeftText();

	virtual QString GetFormatedBottomRightText();
	
	// X方向分辨率
	double GetPixelSpacingX();

	// Y方向分辨率
	double GetPixelSpacingY();

	// 层间距
	double GetSpacingBetweenSlice();

	// 层厚
	double GetSliceThickness();

	// 斜率
	double GetSlop();

	// 截距
	double GetIntercept();

	int GetOneFramePixelSize();

	std::shared_ptr<BYTE_TYPE> GetDisplayImagePixels();

	int GetDisplayImageWidth();

	int GetDisplayImageHeight();

	QImage* GetQImage();

	AnnotationArrayType& GetAnnotations();

	/////////////////   UI 显示相关 ///////////////////////

	// 更新显示宽度
	void Resize(int viewportWidth, int viewportHeight);
	
	int GetTranslateX();
	int GetTranslateY();

	double GetImageScale();

protected:
	
	// 图像宽度
	int GetImageWidth();

	// 图像高度
	int GetImageHeight();

	// 帧索引号
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
	// 窗位
	AUTO_PROPERTY(WindowCenterInternal, double);
	// 窗宽
	AUTO_PROPERTY(WindowWidthInternal, double);

	// 经过窗宽窗位后的显示像素
	AUTO_PROPERTY(DisplayImagePixelsInternal, std::shared_ptr<BYTE_TYPE>);

	// 经过窗宽窗位后的显示图像宽度
	AUTO_PROPERTY(DisplayImageWidthInternal, int);

	// 经过窗宽窗位后的显示图像高度
	AUTO_PROPERTY(DisplayImageHeightInternal, int);

private:
	AUTO_PROPERTY_REF(ReferencedStudy, std::shared_ptr<CStudyModule>);

	AUTO_PROPERTY_REF(ReferencedSeries, std::shared_ptr<CSeriesModule>);

	AUTO_PROPERTY_REF(ReferencedImageInstance, std::shared_ptr<CImageInstanceModule>);

	// 当前显示的Image
	AUTO_PROPERTY_REF(CurrentDisplayImage, std::weak_ptr<CDisplayImage>);

	// 当前显示的Image
	AUTO_PROPERTY_REF(CurrentDisplayImageIndex, int);

	// 当前显示的Image
	AUTO_PROPERTY_REF(DisplayImages, std::vector<std::weak_ptr<CDisplayImage>>);

	// Annotation 集合
	AUTO_PROPERTY_REF(AnnotationsInternal, AnnotationArrayType);

	//////////////////  UI  逻辑相关 ///////////////////////
	
	// 选中状态
	AUTO_PROPERTY(IsSelected, bool);
	// 选中状态
	AUTO_PROPERTY(IsActive, bool);

	/////////////////   UI  显示信息【位置、旋转等】////////////////////////

	AUTO_PROPERTY_REF_NO_SET(IsDisplayImageInfo, bool);

	AUTO_PROPERTY(ImageScaleInternal, double);

	AUTO_PROPERTY(TranslateXInternal, int);
	AUTO_PROPERTY(TranslateYInternal, int);
	AUTO_PROPERTY(ViewSizeWidth, int);
	AUTO_PROPERTY(ViewSizeHeight, int);

	/**
	 * 灰度影像
	 */
	AUTO_PROPERTY_REF(QImageInternal, std::shared_ptr<QImage>);

	AUTO_PROPERTY(DisplayImagePixelsSize, int);

	AUTO_PROPERTY(ImageSizeChanged, bool);
};

#endif

