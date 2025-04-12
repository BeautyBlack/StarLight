#include "stdafx.h"
#include "StudyModuleLoader.h"

#include <utilities/ImageModule/Modules/StudyModule.h>
#include "Foundations/Common/FileUtility.h"
#include <Foundations/Common/TimeHelper.h>
#include <dcmtk/oflog/config.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <Foundations/Common/ToDefine.h>

#include "Foundations/Common/StringUtility.h"


void CStudyModuleLoader::ParseAgeText(LPCTSTR strAge, int& nAge, AgeUnit& ageUnit)
{
	nAge = atoi(strAge);

	auto nLen = strlen(strAge);

	ageUnit = AgeUnit::UnkownAgeUnit;

	for (int i = nLen - 1; i >= 0; --i)
	{
		if (strAge[i] == 'Y' || strAge[i] == 'y')
		{
			ageUnit = AgeUnit::Year;
			break;
		}

		if (strAge[i] == 'M' || strAge[i] == 'm')
		{
			ageUnit = AgeUnit::Month;
			break;
		}

		if (strAge[i] == 'W' || strAge[i] == 'w')
		{
			ageUnit = AgeUnit::Week;
			break;
		}

		if (strAge[i] == 'D' || strAge[i] == 'd')
		{
			ageUnit = AgeUnit::Day;
			break;
		}
	}

	if (ageUnit == AgeUnit::UnkownAgeUnit)
	{
		if (strstr(strAge, "岁") != NULL_SYMBAL
			|| strstr(strAge, "年") != NULL_SYMBAL)
			ageUnit = AgeUnit::Year;
		else if (strstr(strAge, "月") != NULL_SYMBAL)
			ageUnit = AgeUnit::Month;
		else if (strstr(strAge, "周") != NULL_SYMBAL)
			ageUnit = AgeUnit::Week;
		else if (strstr(strAge, "天") != NULL_SYMBAL
			|| strstr(strAge, "日") != NULL_SYMBAL)
			ageUnit = AgeUnit::Day;
	}

	ageUnit = ageUnit == AgeUnit::UnkownAgeUnit ? AgeUnit::Year : ageUnit;
}

void CStudyModuleLoader::Load(CStudyModule* studyModule, DcmDataset* dataset)
{
	LPCTSTR strValue = NULL_SYMBAL;

	UINT16 nValue16 = 0;

	LPCTSTR strCharset = NULL_SYMBAL;
	dataset->findAndGetString(DCM_SpecificCharacterSet, strCharset);

	bool isUTF8 = false;

	if (strCharset != NULL_SYMBAL
		&& strstr(strCharset, "192"))
		isUTF8 = true;

	strValue = NULL_SYMBAL;
	if (dataset->findAndGetString(DCM_AccessionNumber, strValue).good()
		&& strValue != NULL_SYMBAL)
		studyModule->SetAccessionNumber(strValue);

	strValue = NULL_SYMBAL;
	if (dataset->findAndGetString(DCM_PatientAge, strValue).good()
		&& strValue != NULL_SYMBAL)
		ParseAgeText(strValue, studyModule->GetAge(), studyModule->GetAgeUnit());

	strValue = NULL_SYMBAL;
	if (dataset->findAndGetString(DCM_PatientID, strValue).good()
		&& strValue != NULL_SYMBAL)
	{
		if (isUTF8)
			studyModule->SetPatientId(
				CStringUtility::ConvertUTF8ToGB(
					strValue).GetString());
		else
			studyModule->SetPatientId(strValue);
	}

	strValue = NULL_SYMBAL;
	if (dataset->findAndGetString(DCM_PatientName, strValue).good()
		&& strValue != NULL_SYMBAL)
	{
		if (isUTF8)
			studyModule->SetPatientName(
				CStringUtility::ConvertUTF8ToGB(
					strValue).GetString());
		else
			studyModule->SetPatientName(strValue);
	}

	strValue = NULL_SYMBAL;
	if (dataset->findAndGetString(DCM_PatientSex, strValue).good()
		&& strValue != NULL_SYMBAL)
		studyModule->SetSex(ParseTextToSex(strValue));

	LPCTSTR strDate = NULL_SYMBAL, strTime = NULL_SYMBAL;
	if (dataset->findAndGetString(DCM_StudyDate, strDate).good()
		&& strValue != NULL_SYMBAL)
	{
		dataset->findAndGetString(DCM_StudyTime, strTime);

		CTimeHelper::ParseDicomDateTime(strDate, strTime, studyModule->GetStudyDateTime());
	}

	strValue = NULL_SYMBAL;
	if (dataset->findAndGetString(DCM_StudyDescription, strValue).good()
		&& strValue != NULL_SYMBAL)
	{
		if (isUTF8)
			studyModule->SetStudyDescription(
				CStringUtility::ConvertUTF8ToGB(
					strValue).GetString());
		else
			studyModule->SetStudyDescription(strValue);
	}

	strValue = NULL_SYMBAL;
	if (dataset->findAndGetString(DCM_StudyID, strValue).good()
		&& strValue != NULL_SYMBAL)
	{
		if (isUTF8)
			studyModule->SetStudyId(
				CStringUtility::ConvertUTF8ToGB(
					strValue).GetString());
		else
			studyModule->SetStudyId(strValue);
	}
}
