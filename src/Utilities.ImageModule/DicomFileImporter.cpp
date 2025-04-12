#include "stdafx.h"
#include <Utilities/ImageModule/Service/DicomLoaderService.h>
#include <utilities/ImageModule/Modules/StudyModule.h>
#include <utilities/ImageModule/Service/DicomFileImporter.h>
#include "Foundations/Common/FileUtility.h"
#include "Foundations/Interfaces/MineType.h"
#include <Foundations/Common/TimeHelper.h>
#include <utilities/Configuration/Configuration.h>
#include <utilities/DicomServer/DcmStoreSCP.h>
#include "utilities/ImageModule/ModuleHelper.h"
#include <utilities/Database/DatabaseManager.h>
#include <Foundations/Log/LogHelper.h>
#include <Foundations/Common/ServiceStopFlag.h>
#include <Foundations/Common/StringUtility.h>

#include <dcmtk/oflog/config.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dctag.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <ImageHlp.h>
#include <Shlwapi.h>

//use for MakeSureDirectoryPathExists
#pragma comment(lib, "dbghelp.lib")

#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmsr/dsrdoc.h>
#include <dcmtk/dcmdata/dcdeftag.h>


bool CDicomFileImporter::ImportFile(const char * strFilePath)
{
	DcmFileFormat dcmFileHandle;

	if (dcmFileHandle.loadFile(OFFilename(strFilePath)).bad())
		return false;

	return ImportFile(&dcmFileHandle);
}

bool CDicomFileImporter::ImportFile(DcmFileFormat * fileFormat)
{
	auto pDataset = fileFormat->getDataset();

	auto studyDto = DatasetToStudyDto(pDataset);

	char strDcmPath[MAX_PATH] = { 0 };

	PathCombine(strDcmPath,
		CConfiguration::GetInstance().GetSystemInfo().GetStoragePath().c_str(),
		//pParentSCP->GetImageRootPath().GetString(),
		studyDto.GetStudyInstanceUID().c_str());

	LPCTSTR strInstanceUID = NULL_SYMBAL;
	pDataset->findAndGetString(DCM_SOPInstanceUID, strInstanceUID);

	CString strFileName;

	strFileName.Format("%s.dcm", strInstanceUID);

	PathAppend(strDcmPath, strFileName);

	MakeSureDirectoryPathExists(strDcmPath);

	fileFormat->saveFile(strDcmPath, EXS_LittleEndianExplicit, EET_ExplicitLength, EGL_withGL);

	CDatabaseManager::GetInstance().RecordStudy(studyDto);
	
	return true;
}

STRING_TYPE CDicomFileImporter::GenerateNewStudyInstanceUID()
{
	char strUid[MAX_PATH] = { 0 };
	
	dcmGenerateUniqueIdentifier(strUid);

	return (char*)strUid;
}

CStudyDto CDicomFileImporter::DatasetToStudyDto(DcmDataset* pDataset)
{
	CStudyDto studyDto;

	LPCTSTR strTemp = NULL_SYMBAL;

	LPCTSTR strCharset = NULL_SYMBAL;
	pDataset->findAndGetString(DCM_SpecificCharacterSet, strCharset);

	bool isUTF8 = false;

	if (strCharset != NULL_SYMBAL
		&& strstr(strCharset, "192"))
		isUTF8 = true;

	if (pDataset->findAndGetString(DCM_StudyInstanceUID, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetStudyInstanceUID(strTemp);

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_AccessionNumber, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetAccessionNumber(strTemp);

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_PatientAge, strTemp).good()
		&& strTemp != NULL_SYMBAL)
	{
		const int nLen = strlen(strTemp);

		studyDto.SetAge(atoi(strTemp));

		if (strTemp[nLen - 1] == 'Y')
			studyDto.SetAgeUnit(AgeUnit::Year);
		else if (strTemp[nLen - 1] == 'M')
			studyDto.SetAgeUnit(AgeUnit::Month);
		else if (strTemp[nLen - 1] == 'W')
			studyDto.SetAgeUnit(AgeUnit::Week);
		else if (strTemp[nLen - 1] == 'D')
			studyDto.SetAgeUnit(AgeUnit::Day);
	}

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_PatientBirthDate, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		CTimeHelper::ParseDicomDateTime(strTemp, NULL_SYMBAL, studyDto.GetBirthDate());
	
	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_BodyPartExamined, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetBodyParts(
			isUTF8 ? CStringUtility::ConvertUTF8ToGB(strTemp).GetString() : strTemp);

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_Modality, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetModalitys(strTemp);

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_PatientID, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetPatientId(strTemp);

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_PatientName, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetPatientName(
			isUTF8 ? CStringUtility::ConvertUTF8ToGB(strTemp).GetString() : strTemp);

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_PatientSex, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetSex(ParseTextToSex(strTemp));

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_StudyDate, strTemp).good()
		&& strTemp != NULL_SYMBAL)
	{
		LPCTSTR strTime = NULL_SYMBAL;
		pDataset->findAndGetString(DCM_StudyTime, strTime);

		CTimeHelper::ParseDicomDateTime(strTemp, strTime, studyDto.GetStudyDateTime());
	}

	if(studyDto.GetBirthDate().wYear <= 0)
	{
		auto birthDate = CTimeHelper::CalculateBirthDateFromAge(
			studyDto.GetAge(), studyDto.GetAgeUnit(), &studyDto.GetStudyDateTime());

		memcpy_s(&studyDto.GetBirthDate(), sizeof(birthDate),
			&birthDate, sizeof(birthDate));
	}

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_StudyDescription, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetStudyDescription(
			isUTF8 ? CStringUtility::ConvertUTF8ToGB(strTemp).GetString() : strTemp);

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_StudyID, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetStudyId(strTemp);

	return studyDto;
}

