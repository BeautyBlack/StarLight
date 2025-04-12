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

#ifndef STUDY_MODULE_2874835A9A86496080356D5082E7E654
#define STUDY_MODULE_2874835A9A86496080356D5082E7E654

#pragma once

#include <Foundations/Interfaces/Sex.h>
#include <Foundations/Interfaces/AgeUnit.h>
#include <Foundations/Interfaces/Modality.h>

#include "Foundations/Common/ToDefine.h"

#include <Utilities/ImageModule/Modules/SeriesModule.h>

#include <Foundations/DataModule/StudyDto.h>

#include <string>
#include <list>
#include <unordered_map>
#include "Foundations/Interfaces/Modules/SerializeBase.h"


#include <utilities/ImageModule/ExportFlagDefine.h>

/*
 * Description
 * Study 信息
 */
class UTILITIESIMAGEMODULE_API CStudyModule : public CStudyDto, public CSerializeBase
{
public:
	CStudyModule();
	~CStudyModule();

	//static std::unique_ptr<CStudyModule> DeserializeFrom(STRING_TYPE& strJsonText);

	//static std::unique_ptr<CStudyModule> DeserializeFrom(const char* strJsonText);

	//static STRING_TYPE SeriealizeTo(CStudyModule &studyModule);

	STRING_TYPE SerializeTo() override;

	bool DeserializeFrom(STRING_TYPE& text) override;

	bool DeserializeFrom(const char* text) override;

	std::shared_ptr<CSeriesModule> GetSeries(const char* strSeriesUID);

	void AddSeriesModule(std::shared_ptr<CSeriesModule> seriesModule);
	
public:
	using Series_Array_Type = std::vector<std::shared_ptr<CSeriesModule>>;

private:

	/*
	 * Series Instance UIDs 序列UID集合
	 */        
	//AUTO_PROPERTY_REF_WITH_JSON_KEY(SeriesInstanceUids, std::hash_set<STRING_TYPE>);

	/*
	 * Series Series集合
	 */
	AUTO_PROPERTY_REF_NO_SET(Series, Series_Array_Type);
};

#endif
