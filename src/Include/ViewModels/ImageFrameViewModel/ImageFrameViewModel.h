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
	 * ���ؼ��
	 */
	std::shared_ptr<CStudyTabItemViewModel> LoadStudy(CStudyDto &studyDto);



	/**
	 * �����ļ�
	 * @strFileList : �ļ�����
	 */
	void LoadStudy(const char* strStudyUID);

	/**
	 * ����Study UID��ȡ��Ӧ�� CStudyTabItemViewModel
	 * @strStudyUID Study UID
	 */
	std::shared_ptr<CStudyTabItemViewModel> GetStudyTabItem(const char* strStudyUID);

	/**
	 * �ͷ� CStudyTabItemViewModel
	 * @strStudyUID Study UID
	 */
	void ReleaseStudyTabItem(const char* strStudyUID);
	void ReleaseStudyTabItem(std::shared_ptr<CStudyTabItemViewModel> studyTab);

private:
	std::shared_ptr<CStudyTabItemViewModel> GenerateStudyTabItem(std::shared_ptr<CStudyModule> study);

private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CImageFrameViewModel);


	using StudyTabItemViewModelsType = std::vector<std::shared_ptr<CStudyTabItemViewModel>>;

	// ʹ��vector : 1�����ִ�Study��˳����
	//              2��һ������´򿪵�Study����̫�࣬ʹ��hashtable֮���������ѯ���ܲ�һ����vector��
	AUTO_PROPERTY_REF_NO_SET(StudyTabItemViewModels, StudyTabItemViewModelsType);

};

#endif
