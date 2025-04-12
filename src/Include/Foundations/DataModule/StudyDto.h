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

#ifndef STUDY_DTO_50C4C83C67384AA3862E673B1FE5F1D4
#define STUDY_DTO_50C4C83C67384AA3862E673B1FE5F1D4

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <Foundations/Interfaces/Sex.h>
#include <Foundations/Interfaces/AgeUnit.h>

class CStudyDto
{
public:
	CStudyDto();
	CStudyDto(const CStudyDto& copySrc);

	static int DisplayItemCounts() { return 11; }

private:
	/*
	 * 删除标签
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(Deleted, bool);
	
	/*
	 * Patient ID 病人id
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(PatientId, STRING_TYPE, 病人号, 0);

	/*
	 * Patient Name 病人姓名
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(PatientName, STRING_TYPE, 姓名, 1);

	/*
	 * Study DateTime 检查时间[包含Date、Time]
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(StudyDateTime, SYSTEMTIME, 检查时间, 2);

	/*
	 * Accession Number 流水号
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(AccessionNumber, STRING_TYPE, 检查号, 3);

	/*
	 * Sex 病人性别
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(Sex, Sex, 性别, 4);

	/*
	 * Age 年龄
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(Age, int, 年龄, 5);

	/*
	 * Age Unit 年龄单位
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(AgeUnit, AgeUnit);

	/*
	 * Modality 设备类型
	 * 多个Modality以‘/’分割
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(Modalitys, STRING_TYPE, 设备类型, 6);
	
	/*
	 * Study Description 检查项目
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(StudyDescription, STRING_TYPE, 检查项目, 7);

	/*
	 * BodyParts 检查部位
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(BodyParts, STRING_TYPE, 检查部位, 8);

	/*
	 * BirthDate 出生时间
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(BirthDate, SYSTEMTIME, 出生时间, 9);

	/*
	 * Study Instance UID 检查唯一码
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(StudyInstanceUID, STRING_TYPE);

	/*
	 * Study Id, 病人检查ID
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(StudyId, STRING_TYPE, 设备号, 10);

	/*
	 * 已写标签
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(ReportState, bool, 报告, 11);
};

#ifndef FOUNDATION_DATAMODULE_LIB
#pragma comment(lib, "Foundation.DataModule.lib")
#endif

#endif
