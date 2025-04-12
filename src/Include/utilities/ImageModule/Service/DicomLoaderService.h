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
	 * ����Ӱ���ļ�
	 * @filePaths �ļ��б�
	 */
	static std::vector<std::shared_ptr<CStudyModule>> LoadImageFiles(std::vector<STRING_TYPE>& filePaths);
	//
	///*
	// * Description
	// * ����Ӱ��
	// * @strStudyUids ���Study UID�б�
	// */
	//static std::vector<std::shared_ptr<CStudyModule>> LoadStudies(std::vector<STRING_TYPE>& strStudyUids);

	/*
	 * Description
	 * ����Ӱ��
	 * @strStudyPath Study �洢·��
	 */
	static std::shared_ptr<CStudyModule> LoadStudy(const char* strStudyPath);

	static void ReleaseStudy(const char* strStudyUID);
};

#endif

