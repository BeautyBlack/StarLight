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

#ifndef SERIES_MODULE_65AAA9F3DD0F4604A9C0893A4F9C24AD
#define SERIES_MODULE_65AAA9F3DD0F4604A9C0893A4F9C24AD

#pragma once

#include <Foundations/Interfaces/Modality.h>
#include <Foundations/Common/ToDefine.h>
#include <Utilities/ImageModule/Modules/ImageInstanceModule.h>

#include <string>
#include <list>
#include <unordered_map>

class CStudyModule;

#include <utilities/ImageModule/ExportFlagDefine.h>

class UTILITIESIMAGEMODULE_API CSeriesModule
{
public:
	CSeriesModule();
	~CSeriesModule();

	std::shared_ptr<CImageInstanceModule> GetInstance(const char* strInstanceUID);

	void AddInstanceModule(std::shared_ptr<CImageInstanceModule> instanceModule);
	
private:
	/*
	 * Reference Study 归属Study
	 */
	AUTO_PROPERTY_REF(ReferenceStudy, std::weak_ptr<CStudyModule>);

	/*
	 * Series Instance UID 序列唯一码
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(SeriesInstanceUID, STRING_TYPE);

	/*
	 * Series Number 序列号
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(SeriesNumber, int);

	/*
	 * Series Description 序列描述
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(SeriesDescription, STRING_TYPE);

	/*
	 * BodyPart 检查部位
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(BodyPart, STRING_TYPE);

	/*
	 * Modality 设备类型
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(Modality, Modality);

	/*
	 * Series Date Time 序列检查时间[包含Date、Time]
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(SeriesDateTime, SYSTEMTIME);

	/*
	 * Instance UIDs Instance UID集合
	 */
	//AUTO_PROPERTY_REF_NO_SET(InstanceUIDs, std::list<STRING_TYPE>);

	using InstanceArrayType = std::vector<std::shared_ptr<CImageInstanceModule>>;
	
	AUTO_PROPERTY_REF_NO_SET(Instances, InstanceArrayType);
};

#endif
