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

#ifndef IMAGE_FRAME_VIEW_MODEL_97E54D832BD84895A019B0B37A35D0F8
#define IMAGE_FRAME_VIEW_MODEL_97E54D832BD84895A019B0B37A35D0F8

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <Foundations/DataModule/StudyDto.h>

#include <ViewModels/ImageFrameViewModel/imageframeviewmodel_global.h>

#include <ViewModels/ImageFrameViewModel/StudyTabItemViewModel.h>

class IMAGEFRAMEVIEWMODEL_API CImageFrameViewModel
{
public:
    CImageFrameViewModel() = default;
	~CImageFrameViewModel() = default;

	/**
	 * Description
	 * 加载检查
	 */
	std::shared_ptr<CStudyTabItemViewModel> LoadStudy(CStudyDto &studyDto);



	/**
	 * 加载文件
	 * @strFileList : 文件集合
	 */
	void LoadStudy(const char* strStudyUID);

	/**
	 * 根据Study UID获取对应的 CStudyTabItemViewModel
	 * @strStudyUID Study UID
	 */
	std::shared_ptr<CStudyTabItemViewModel> GetStudyTabItem(const char* strStudyUID);

	/**
	 * 释放 CStudyTabItemViewModel
	 * @strStudyUID Study UID
	 */
	void ReleaseStudyTabItem(const char* strStudyUID);
	void ReleaseStudyTabItem(std::shared_ptr<CStudyTabItemViewModel> studyTab);

private:
	std::shared_ptr<CStudyTabItemViewModel> GenerateStudyTabItem(std::shared_ptr<CStudyModule> study);

private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CImageFrameViewModel);


	using StudyTabItemViewModelsType = std::vector<std::shared_ptr<CStudyTabItemViewModel>>;

	// 使用vector : 1、保持打开Study的顺序性
	//              2、一般情况下打开的Study不会太多，使用hashtable之类的容器查询性能不一定比vector快
	AUTO_PROPERTY_REF_NO_SET(StudyTabItemViewModels, StudyTabItemViewModelsType);

};

#endif
