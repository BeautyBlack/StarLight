#include "pch.h"
#include "utilities/DicomClient/DcmParam.h"


#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>

CDcmParam::CDcmParam(const CDcmParam &srcCopy)
{
	DeepCopy(srcCopy);
}

CDcmParam& CDcmParam::operator =(const CDcmParam &srcCopy)
{
	DeepCopy(srcCopy);

	return *this;
}

void CDcmParam::DeepCopy(const CDcmParam &srcCopyT)
{
	CDcmParam* pCopyT = (CDcmParam*)((LPVOID)&srcCopyT);

	CDcmParam &srcCopy = *pCopyT;

	SetStudyInstanceUID(srcCopy.GetStudyInstanceUID());
	SetModality(srcCopy.GetModality());
	SetStudyDate(srcCopy.GetStudyDate());
	SetPatientID(srcCopy.GetPatientID());
	SetAccessionNum(srcCopy.GetAccessionNum());
	SetStudyID(srcCopy.GetStudyID());
	SetPatientName(srcCopy.GetPatientName());
}

CDcmParam CDcmParam::Load(DcmDataset *pDataset)
{
	CDcmParam result;

	if(pDataset == NULL_SYMBAL)
		return result;

	LPCTSTR strValue = NULL_SYMBAL;

	if(pDataset->findAndGetString(DCM_StudyInstanceUID, strValue).good())
		result.SetStudyInstanceUID(strValue);

	if(pDataset->findAndGetString(DCM_ModalitiesInStudy, strValue).good())
		result.SetModality(strValue);

	if(pDataset->findAndGetString(DCM_StudyDate, strValue).good())
		result.SetStudyDate(strValue);

	if(pDataset->findAndGetString(DCM_PatientID, strValue).good())
		result.SetPatientID(strValue);

	if(pDataset->findAndGetString(DCM_AccessionNumber, strValue).good())
		result.SetAccessionNum(strValue);

	if(pDataset->findAndGetString(DCM_StudyID, strValue).good())
		result.SetStudyID(strValue);

	if(pDataset->findAndGetString(DCM_PatientName, strValue).good())
		result.SetPatientName(strValue);

	return result;
}

void CDcmParam::CopyTo(DcmDataset *pDataset)
{
	if(pDataset == NULL_SYMBAL)
		return;

	pDataset->putAndInsertString(DCM_PatientID, GetPatientID());
	pDataset->putAndInsertString(DCM_StudyInstanceUID, GetStudyInstanceUID());
	pDataset->putAndInsertString(DCM_ModalitiesInStudy, GetModality());
	pDataset->putAndInsertString(DCM_StudyDate, GetStudyDate());
	pDataset->putAndInsertString(DCM_AccessionNumber, GetAccessionNum());
	pDataset->putAndInsertString(DCM_StudyID, GetStudyID());
	pDataset->putAndInsertString(DCM_PatientName, GetPatientName());
}
