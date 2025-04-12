#include "stdafx.h"
#include <Utilities/ImageModule/Modules/StudyModule.h>
//#include <json/value.h>
//#include <json/reader.h>

#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>


CStudyModule::CStudyModule()
{
	SetSex(Sex::Male);
	SetAgeUnit(AgeUnit::Year);
	SetAge(1);
	
	ZeroMemory(&GetBirthDate(), sizeof(SYSTEMTIME));
	ZeroMemory(&GetStudyDateTime(), sizeof(SYSTEMTIME));
}


CStudyModule::~CStudyModule()
{
	GetSeries().clear();
}

bool CStudyModule::DeserializeFrom(STRING_TYPE& strJsonText)
{
	return DeserializeFrom(strJsonText.c_str());
}

bool CStudyModule::DeserializeFrom(const char* strJsonText)
{
	if (strJsonText == NULL_SYMBAL)
		return false;

	rapidjson::Document reader;
	reader.Parse<0>(strJsonText);

	ReadTextFromJsonObject(reader, GetPatientId(), GetPatientIdJsonKey());
	ReadTextFromJsonObject(reader, GetPatientName(), GetPatientNameJsonKey());
	ReadTextFromJsonObject(reader, GetAccessionNumber(), GetAccessionNumberJsonKey());
	ReadIntFromJsonObject(reader, GetSex(), GetSexJsonKey());
	ReadDateFromJsonObject(reader, GetBirthDate(), GetBirthDateJsonKey());
	ReadTextFromJsonObject(reader, GetBodyParts(), GetBodyPartsJsonKey());
	ReadIntFromJsonObject(reader, GetAge(), GetAgeJsonKey());
	ReadIntFromJsonObject(reader, GetAgeUnit(), GetAgeUnitJsonKey());
	//ReadTextArrayFromJsonObject(reader, GetSeriesInstanceUids(), GetSeriesInstanceUidsJsonKey());
	ReadTextFromJsonObject(reader, GetStudyId(), GetStudyIdJsonKey());
	ReadTextFromJsonObject(reader, GetStudyDescription(), GetStudyDescriptionJsonKey());
	ReadTextFromJsonObject(reader, GetModalitys(), GetModalitysJsonKey());
	ReadTextFromJsonObject(reader, GetPatientName(), GetPatientNameJsonKey());
	ReadTextFromJsonObject(reader, GetStudyInstanceUID(), GetStudyInstanceUIDJsonKey());
	ReadDateTimeFromJsonObject(reader, GetStudyDateTime(), GetStudyDateTimeJsonKey());

	return !GetStudyInstanceUID().empty();
}

std::shared_ptr<CSeriesModule> CStudyModule::GetSeries(const char* strSeriesUID)
{
	Series_Array_Type::iterator itBegin = GetSeries().begin();
	Series_Array_Type::iterator itEnd = GetSeries().end();

	while(itBegin != itEnd)
	{
		if ((*itBegin)->GetSeriesInstanceUID().compare(
				strSeriesUID) == 0)
			return (*itBegin);
		
		++itBegin;
	}

	return NULL_SYMBAL;
}

void CStudyModule::AddSeriesModule(std::shared_ptr<CSeriesModule> seriesModule)
{
	Series_Array_Type::iterator itBegin = GetSeries().begin();
	Series_Array_Type::iterator itEnd = GetSeries().end();

	while (itBegin != itEnd)
	{
		if ((*itBegin)->GetSeriesNumber() >= seriesModule->GetSeriesNumber())
			break;

		++itBegin;
	}

	GetSeries().insert(itBegin, seriesModule);
}

STRING_TYPE CStudyModule::SerializeTo()
{
	rapidjson::Document document;
	document.SetObject();	//如果不使用这个方法，AddMember将会报错

	WriteTextToJsonObject(document, GetPatientId(), GetPatientIdJsonKey());
	WriteTextToJsonObject(document, GetPatientName(), GetPatientNameJsonKey());
	WriteTextToJsonObject(document, GetAccessionNumber(), GetAccessionNumberJsonKey());
	WriteIntToJsonObject(document, static_cast<int>(GetSex()), GetSexJsonKey());
	WriteDateToJsonObject(document, GetBirthDate(), GetBirthDateJsonKey());
	WriteTextToJsonObject(document, GetBodyParts(), GetBodyPartsJsonKey());
	WriteIntToJsonObject(document, GetAge(), GetAgeJsonKey());
	WriteIntToJsonObject(document, static_cast<int>(GetAgeUnit()), GetAgeUnitJsonKey());
	//WriteTextArrayToJsonObject(document, GetSeriesInstanceUids(), GetSeriesInstanceUidsJsonKey());
	WriteTextToJsonObject(document, GetStudyId(), GetStudyIdJsonKey());
	WriteTextToJsonObject(document, GetStudyDescription(), GetStudyDescriptionJsonKey());
	WriteTextToJsonObject(document, GetModalitys(), GetModalitysJsonKey());
	WriteTextToJsonObject(document, GetPatientName(), GetPatientNameJsonKey());
	WriteTextToJsonObject(document, GetStudyInstanceUID(), GetStudyInstanceUIDJsonKey());
	WriteDateTimeToJsonObject(document, GetStudyDateTime(), GetStudyDateTimeJsonKey());

	rapidjson::StringBuffer jsonBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(jsonBuffer);
	document.Accept(writer);

	return jsonBuffer.GetString();
}

