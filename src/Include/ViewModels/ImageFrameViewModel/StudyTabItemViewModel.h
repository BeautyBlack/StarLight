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

#ifndef STUDY_VIEW_MODEL_A879E6626A5746D8A3C3BE193A5B7718
#define STUDY_VIEW_MODEL_A879E6626A5746D8A3C3BE193A5B7718

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <ViewModels/ImageFrameViewModel/DisplayImage.h>

#include <ViewModels/ImageFrameViewModel/imageframeviewmodel_global.h>

#include <Utilities/ImageModule/Modules/StudyModule.h>

#include <vector>


#include "Foundations/Interfaces/Enums/ImageProcessErrorCode.h"
#include "Foundations/Interfaces/ServiceRequest.h"

enum class IMAGEFRAMEVIEWMODEL_API DisplayImageMode
{
	/**
	 * 序列模式
	 */
	Series = 0,

	/**
	 * 图像模式
	 */
	Image = 1
};

class IMAGEFRAMEVIEWMODEL_API CStudyTabItemViewModel
{
public:
	CStudyTabItemViewModel();
	~CStudyTabItemViewModel();
	
public:
	/**
	 * 切换至序列模式【默认】
	 */
	void SwitchToSeriesMode();

	/**
	 * 切换至图像模式
	 */
	void SwitchToImageMode();

	/**
	 * 校验Study UID
	 */
	bool IsMatchStudyUID(const char *strStudyUID);

	/**
	 * 重新加载Study
	 */
	void ReloadStudy();

	ImageProcessErrorCode SendToMpr(std::shared_ptr<CDisplayImage> selectedImage);
	ImageProcessErrorCode SendToVR(std::shared_ptr<CDisplayImage> selectedImage);
	ImageProcessErrorCode SendToParonama(std::shared_ptr<CDisplayImage> selectedImage);

	std::tuple<ImageProcessErrorCode, std::shared_ptr<CServiceRequest>>
	GenerateRequest(std::weak_ptr<CSeriesModule> series,
		std::function<std::shared_ptr<CServiceRequest>()> createRequestFunc);
	
private:
	void CreateDisplayImages();
	
private:
	AUTO_PROPERTY_REF(Study, std::shared_ptr<CStudyModule>);

	//AUTO_PROPERTY(SelectDisplayImageIndex, int);

	using DisplayImagesType = std::unordered_map<std::string, std::shared_ptr<CDisplayImage>>;

	// DisplayImage 缓存
	AUTO_PROPERTY_REF(DisplayImagesCache, DisplayImagesType);

	// 显示模式, 默认序列模式
	AUTO_PROPERTY(DisplayImageMode, DisplayImageMode);

	// DisplayImages为每个显示框内的内容, DisplayImagesCache是缓存了所有的DisplayImage, 注意区分
	AUTO_PROPERTY_REF(DisplayImages, std::vector<std::shared_ptr<CDisplayImage>>);

	/**
	 * 布局 -- 行数
	 */
	AUTO_PROPERTY(LayoutRows, int);

	/**
	 * 布局 -- 行数
	 */
	AUTO_PROPERTY(LayoutColumn, int);
};


#endif

