#include "pch.h"


#include <afx.h>
#include <utilities/Database/OdbcDBOperator.h>

#include <utilities/Database/OdbcDBConnector.h>



#include "Foundations/Common/TimeHelper.h"
#include "Foundations/Log/LogHelper.h"
#include "utilities/Configuration/Configuration.h"
#include "utilities/Database/SQLCommand.h"

#define MSADO15_FILE_NAME "msado15.dll"
#import MSADO15_FILE_NAME rename("EOF","adoEOF"),rename("BOF","adoBOF")
using namespace ADODB;

COdbcDBOperator::COdbcDBOperator()
{
	SetDBConnection(NULL_SYMBAL);
}

COdbcDBOperator::~COdbcDBOperator()
{
	CloseDB();
}

bool COdbcDBOperator::ConnectDB()
{
	if(GetDBConnection() == NULL_SYMBAL)
	{
		SetDBConnection(new COdbcDBConnector);

		auto &databaseInfo = CConfiguration::GetInstance().GetDatabaseInfo();
		
		GetDBConnection()->SetServerName(databaseInfo.GetServerName());
		GetDBConnection()->SetUserName(databaseInfo.GetUserName());
		GetDBConnection()->SetPassword(databaseInfo.GetPassword());
		GetDBConnection()->SetServerAddress(databaseInfo.GetServerAddress());
		GetDBConnection()->SetUseLocalServer(databaseInfo.GetUseLocalServer());
		GetDBConnection()->SetDefaultCatalogName(databaseInfo.GetDefaultCatalogName());
	}

	if(GetDBConnection() != NULL_SYMBAL
		&& !GetDBConnection()->GetIsConnected())
		GetDBConnection()->Connect();

	return GetDBConnection() == NULL_SYMBAL ? false : GetDBConnection()->GetIsConnected();
}

void COdbcDBOperator::CloseDB()
{
	if (GetDBConnection() != NULL_SYMBAL)
	{
		GetDBConnection()->Close();

		delete GetDBConnection();

		SetDBConnection(NULL_SYMBAL);
	}
}

bool COdbcDBOperator::IsRecordExist(const char* strColumnName, const char* strValue)
{
	if (!ConnectDB())
		return false;

	try
	{
		_RecordsetPtr recordset;
		_variant_t recordsetAffected;

		CString strCommand;

		strCommand.Format(STR_IS_STUDY_EXIST_COMMAND, strColumnName, strValue);

		recordset.CreateInstance("ADODB.Recordset");
		try
		{
			recordset = static_cast<COdbcDBConnector*>(GetDBConnection())
				->GetConnection()
				->Execute((_bstr_t)strCommand.GetString(), &recordsetAffected, adCmdText);
		}
		catch (...)
		{
			// 如果cmdText 模式报错，则切换至 Unkown模式
			recordset = static_cast<COdbcDBConnector*>(GetDBConnection())
				->GetConnection()
				->Execute((_bstr_t)strCommand.GetString(), &recordsetAffected, adCmdUnknown);
		}

		const auto result = !recordset->adoEOF;

		recordset->Close();

		return result;
	}
	catch (...)
	{
		return false;
	}
}

void COdbcDBOperator::RecordStudy(CStudyDto& studyDto)
{
	if (!ConnectDB()
		|| IsRecordExist(
			studyDto.GetStudyInstanceUIDJsonKey().c_str(),
			studyDto.GetStudyInstanceUID().c_str()))
		return;

	CString strCommand;

	strCommand.Format(STR_INSERT_STUDY_COMMAND,
		CStudyDto::GetAgeUnitJsonKey().c_str(),
		CStudyDto::GetDeletedJsonKey().c_str(),
		CStudyDto::GetStudyInstanceUIDJsonKey().c_str(),
		CStudyDto::GetAccessionNumberJsonKey().c_str(),
		CStudyDto::GetAgeJsonKey().c_str(),
		CStudyDto::GetBirthDateJsonKey().c_str(),
		CStudyDto::GetBodyPartsJsonKey().c_str(),
		CStudyDto::GetModalitysJsonKey().c_str(),
		CStudyDto::GetPatientIdJsonKey().c_str(),
		CStudyDto::GetPatientNameJsonKey().c_str(),
		CStudyDto::GetSexJsonKey().c_str(),
		CStudyDto::GetStudyDateTimeJsonKey().c_str(),
		CStudyDto::GetStudyDescriptionJsonKey().c_str(),
		CStudyDto::GetStudyIdJsonKey().c_str(),
		CStudyDto::GetReportStateJsonKey().c_str(),

		(int)studyDto.GetAgeUnit(),
		0,
		studyDto.GetStudyInstanceUID().c_str(),
		studyDto.GetAccessionNumber().c_str(),
		studyDto.GetAge(),
		CTimeHelper::ToDateText(studyDto.GetBirthDate()).c_str(),
		studyDto.GetBodyParts().c_str(),
		studyDto.GetModalitys().c_str(),
		studyDto.GetPatientId().c_str(),
		studyDto.GetPatientName().c_str(),
		(int)studyDto.GetSex(),
		CTimeHelper::ToDateTimeText(studyDto.GetStudyDateTime()).c_str(),
		studyDto.GetStudyDescription().c_str(),
		studyDto.GetStudyId().c_str(),
		(int)studyDto.GetReportState()
	);

	GetDBConnection()->Execute(strCommand);
}

std::vector<CStudyDto> COdbcDBOperator::QueryStudies(CQueryFilter& filter)
{
	std::vector<CStudyDto> result;

	if (!ConnectDB())
		return result;

	CString strCommand = STR_QUERY_STUDY_COMMAND;

	if (!filter.GetAccessionNum().empty())
		strCommand.AppendFormat(" AND %s like '%%%s%%'",
			CStudyDto::GetAccessionNumberJsonKey().c_str(),
			filter.GetAccessionNum().c_str());

	if (!filter.GetPatientName().empty())
		strCommand.AppendFormat(" AND %s like '%%%s%%'",
			CStudyDto::GetPatientNameJsonKey().c_str(),
			filter.GetPatientName().c_str());

	strCommand.AppendFormat(" AND %s=%d AND %s BETWEEN '%s' AND '%s';",
		CStudyDto::GetReportStateJsonKey().c_str(),
		filter.GetStudyReportState() == StudyReportState::NoReport ? 0 : 1,
		CStudyDto::GetStudyDateTimeJsonKey().c_str(),
		filter.GetDateFrom().Format("%Y-%m-%d"),
		filter.GetDateTo().Format("%Y-%m-%d"));

	try
	{
		_RecordsetPtr recordset;
		_variant_t recordsetAffected;

		CString strTmp;

		recordset.CreateInstance("ADODB.Recordset");
		try
		{
			recordset = static_cast<COdbcDBConnector*>(GetDBConnection())
			->GetConnection()
			->Execute((_bstr_t)strCommand.GetString(), &recordsetAffected, adCmdText);
		}
		catch (...)
		{
			// 如果cmdText 模式报错，则切换至 Unkown模式
			recordset = static_cast<COdbcDBConnector*>(GetDBConnection())
				->GetConnection()
				->Execute((_bstr_t)strCommand.GetString(), &recordsetAffected, adCmdUnknown);
		}

		try
		{
			char strIdValue[64] = { 0 };

			while (!recordset->adoEOF)
			{
				_variant_t valueT = recordset->GetCollect(
					CStudyDto::GetStudyInstanceUIDJsonKey().c_str());
				if (valueT.vt == VT_NULL)
				{
					recordset->MoveNext();
					continue;
				}

				CStudyDto studyDto;
				
				studyDto.SetStudyInstanceUID((LPCTSTR)(_bstr_t)valueT);
				
				valueT = recordset->GetCollect(
					studyDto.GetAgeJsonKey().c_str());
				if (valueT.vt == VT_INT || valueT.vt == VT_I4)
					studyDto.SetAge(valueT.intVal);
				else if (valueT.vt != VT_NULL)
					studyDto.SetAge(atoi((LPCTSTR)(_bstr_t)valueT));

				valueT = recordset->GetCollect(
					studyDto.GetPatientIdJsonKey().c_str());
				if (valueT.vt != VT_NULL)
					studyDto.SetPatientId((LPCTSTR)(_bstr_t)valueT);

				valueT = recordset->GetCollect(
					studyDto.GetAccessionNumberJsonKey().c_str());
				if (valueT.vt != VT_NULL)
					studyDto.SetAccessionNumber((LPCTSTR)(_bstr_t)valueT);

				valueT = recordset->GetCollect(
					studyDto.GetAgeUnitJsonKey().c_str());
				if(valueT.vt == VT_INT || valueT.vt == VT_I4 || valueT.vt == VT_I1)
					studyDto.SetAgeUnit((AgeUnit)valueT.intVal);
				else if (valueT.vt != VT_NULL)
					studyDto.SetAgeUnit((AgeUnit)atoi((LPCTSTR)(_bstr_t)valueT));

				valueT = recordset->GetCollect(
					studyDto.GetBodyPartsJsonKey().c_str());
				if (valueT.vt != VT_NULL)
					studyDto.SetBodyParts((LPCTSTR)(_bstr_t)valueT);

				valueT = recordset->GetCollect(
					studyDto.GetModalitysJsonKey().c_str());
				if (valueT.vt != VT_NULL)
					studyDto.SetModalitys((LPCTSTR)(_bstr_t)valueT);

				valueT = recordset->GetCollect(
					studyDto.GetPatientNameJsonKey().c_str());
				if (valueT.vt != VT_NULL)
					studyDto.SetPatientName((LPCTSTR)(_bstr_t)valueT);

				valueT = recordset->GetCollect(
					studyDto.GetSexJsonKey().c_str());
				if (valueT.vt == VT_INT || valueT.vt == VT_I4 || valueT.vt == VT_I1)
					studyDto.SetSex((Sex)valueT.intVal);
				else if (valueT.vt != VT_NULL)
					studyDto.SetSex((Sex)atoi((LPCTSTR)(_bstr_t)valueT));

				valueT = recordset->GetCollect(
					studyDto.GetStudyDescriptionJsonKey().c_str());
				if (valueT.vt != VT_NULL)
					studyDto.SetStudyDescription((LPCTSTR)(_bstr_t)valueT);

				valueT = recordset->GetCollect(
					studyDto.GetStudyIdJsonKey().c_str());
				if (valueT.vt != VT_NULL)
					studyDto.SetStudyId((LPCTSTR)(_bstr_t)valueT);

				valueT = recordset->GetCollect(
					studyDto.GetBirthDateJsonKey().c_str());
				if (valueT.vt != VT_NULL)
					CTimeHelper::ParseTime((LPCTSTR)(_bstr_t)valueT, studyDto.GetBirthDate());

				valueT = recordset->GetCollect(
					studyDto.GetStudyDateTimeJsonKey().c_str());
				if (valueT.vt != VT_NULL)
					CTimeHelper::ParseTime((LPCTSTR)(_bstr_t)valueT, studyDto.GetStudyDateTime());

				valueT = recordset->GetCollect(
					studyDto.GetReportStateJsonKey().c_str());
				if (valueT.vt == VT_BOOL)
					studyDto.SetReportState(valueT.bVal);
				else if (valueT.vt == VT_INT || valueT.vt == VT_I4 || valueT.vt == VT_I1)
					studyDto.SetReportState(valueT.intVal != 0);
				else if(valueT.vt != VT_NULL)
					studyDto.SetReportState(atoi((LPCTSTR)(_bstr_t)valueT) != 0);

				result.push_back(studyDto);

				recordset->MoveNext();
			}
		}

		catch (_com_error e)
		{
			CLogHelper::ErrorLog("查询数据库错误:[Code -- %d] %s || %s || %s",
				e.Error(), e.ErrorMessage(), e.Description(), e.Source());
		}
		catch (CException &error)
		{
			char strError[1024 * 2];
			error.GetErrorMessage(strError, sizeof(strError));

			CLogHelper::ErrorLog("查询数据库错误: %s", strError);
		}
		catch (...)
		{
			DWORD dwError = GetLastError();

			CLogHelper::ErrorLog("查询数据库错误: Error Code[%d]", dwError);
		}

		recordset->Close();
	}
	catch (...)
	{
		
	}
	
	return result;
}

void COdbcDBOperator::DeleteStudy(CStudyDto& studyDto)
{
	if (!ConnectDB())
		return;
	
	CString strCommand;

	strCommand.Format(STR_DELETE_STUDY_COMMAND,
		CStudyDto::GetDeletedJsonKey().c_str(),
		CStudyDto::GetStudyInstanceUIDJsonKey().c_str(),
		studyDto.GetStudyInstanceUID().c_str());

	GetDBConnection()->Execute(strCommand);
}
