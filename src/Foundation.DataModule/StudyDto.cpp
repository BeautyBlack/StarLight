#include "pch.h"
#include "Foundations/DataModule/StudyDto.h"

CStudyDto::CStudyDto()
{
	SetDeleted(false);

	ZeroMemory(&GetStudyDateTime(), sizeof(SYSTEMTIME));
	ZeroMemory(&GetBirthDate(), sizeof(SYSTEMTIME));

	SetSex(Sex::Other);

	SetAge(1);
	SetAgeUnit(AgeUnit::Year);

	SetReportState(false);
}

CStudyDto::CStudyDto(const CStudyDto& copySrcT)
{
	const CStudyDto *pTemp = &copySrcT;
	CStudyDto &copySrc = *const_cast<CStudyDto*>(pTemp);

	SetDeleted(copySrc.GetDeleted());
	SetPatientId(copySrc.GetPatientId());
	SetPatientName(copySrc.GetPatientName());

	memcpy_s(&GetStudyDateTime(), sizeof(SYSTEMTIME), &copySrc.GetStudyDateTime(), sizeof(SYSTEMTIME));

	SetAccessionNumber(copySrc.GetAccessionNumber());

	SetSex(copySrc.GetSex());

	SetAge(copySrc.GetAge());
	SetAgeUnit(copySrc.GetAgeUnit());

	SetModalitys(copySrc.GetModalitys());
	SetStudyDescription(copySrc.GetStudyDescription());
	SetBodyParts(copySrc.GetBodyParts());

	memcpy_s(&GetBirthDate(), sizeof(SYSTEMTIME), &copySrc.GetBirthDate(), sizeof(SYSTEMTIME));

	SetStudyInstanceUID(copySrc.GetStudyInstanceUID());
	SetStudyId(copySrc.GetStudyId());

	SetReportState(copySrc.GetReportState());
}
