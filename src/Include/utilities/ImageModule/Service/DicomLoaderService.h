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

#ifndef DICOM_LOADER_SERVICE_66D5E8458306440F9F4025DD9F3C2732
#define DICOM_LOADER_SERVICE_66D5E8458306440F9F4025DD9F3C2732

#pragma once

#include <Foundations/Interfaces/ServiceBase.h>
#include <utilities/ImageModule/ExportFlagDefine.h>

#include <Foundations/Common/ToDefine.h>
#include <utilities/ImageModule/Modules/StudyModule.h>
#include <vector>

class UTILITIESIMAGEMODULE_API CDicomLoaderService
{
public:
	CDicomLoaderService();

	~CDicomLoaderService();

	/*
	 * Description
	 * 加载影像文件
	 * @filePaths 文件列表
	 */
	static std::vector<std::shared_ptr<CStudyModule>> LoadImageFiles(std::vector<STRING_TYPE>& filePaths);
	//
	///*
	// * Description
	// * 加载影像
	// * @strStudyUids 检查Study UID列表
	// */
	//static std::vector<std::shared_ptr<CStudyModule>> LoadStudies(std::vector<STRING_TYPE>& strStudyUids);

	/*
	 * Description
	 * 加载影像
	 * @strStudyPath Study 存储路径
	 */
	static std::shared_ptr<CStudyModule> LoadStudy(const char* strStudyPath);

	static void ReleaseStudy(const char* strStudyUID);
};

#endif

