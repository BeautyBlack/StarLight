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
	 * ɾ����ǩ
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(Deleted, bool);
	
	/*
	 * Patient ID ����id
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(PatientId, STRING_TYPE, ���˺�, 0);

	/*
	 * Patient Name ��������
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(PatientName, STRING_TYPE, ����, 1);

	/*
	 * Study DateTime ���ʱ��[����Date��Time]
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(StudyDateTime, SYSTEMTIME, ���ʱ��, 2);

	/*
	 * Accession Number ��ˮ��
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(AccessionNumber, STRING_TYPE, ����, 3);

	/*
	 * Sex �����Ա�
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(Sex, Sex, �Ա�, 4);

	/*
	 * Age ����
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(Age, int, ����, 5);

	/*
	 * Age Unit ���䵥λ
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(AgeUnit, AgeUnit);

	/*
	 * Modality �豸����
	 * ���Modality�ԡ�/���ָ�
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(Modalitys, STRING_TYPE, �豸����, 6);
	
	/*
	 * Study Description �����Ŀ
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(StudyDescription, STRING_TYPE, �����Ŀ, 7);

	/*
	 * BodyParts ��鲿λ
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(BodyParts, STRING_TYPE, ��鲿λ, 8);

	/*
	 * BirthDate ����ʱ��
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(BirthDate, SYSTEMTIME, ����ʱ��, 9);

	/*
	 * Study Instance UID ���Ψһ��
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(StudyInstanceUID, STRING_TYPE);

	/*
	 * Study Id, ���˼��ID
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(StudyId, STRING_TYPE, �豸��, 10);

	/*
	 * ��д��ǩ
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(ReportState, bool, ����, 11);
};

#ifndef FOUNDATION_DATAMODULE_LIB
#pragma comment(lib, "Foundation.DataModule.lib")
#endif

#endif
