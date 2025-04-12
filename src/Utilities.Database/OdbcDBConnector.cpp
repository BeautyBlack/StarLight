#include "pch.h"
#include <utilities/Database/OdbcDBConnector.h>

#include <objbase.h>

#include "Foundations/Log/LogHelper.h"

COdbcDBConnector::COdbcDBConnector()
{
	SetIsConnected(false);
}

COdbcDBConnector::~COdbcDBConnector()
{
	Close();
}

bool COdbcDBConnector::Connect()
{
	CoInitialize(NULL_SYMBAL);

	std::string strConnectCommand;

	if(GetUseLocalServer())
	{
		char strTmp[1024 * 2] = { 0 };

		_snprintf_s(strTmp, sizeof(strTmp), _TRUNCATE,
			"Provider=SQLOLEDB.1;Persist Security Info=False;User ID=%s;Password=%s;Initial Catalog=%s;Data Source=%s;",
			GetUserName().c_str(),
			GetPassword().c_str(),
			GetDefaultCatalogName().c_str(),
			GetServerAddress().c_str());

		strConnectCommand = strTmp;
	}
	else
	{
		char strTmp[1024 * 2] = { 0 };

		_snprintf_s(strTmp, sizeof(strTmp), _TRUNCATE,
			"Provider=SQLOLEDB.1;Data Source=.\\SQLEXPRESS;Initial Catalog=%s;Integrated Security=SSPI;",
			//GetServerName().c_str(),
			GetDefaultCatalogName().c_str());

		strConnectCommand = strTmp;
	}

	HRESULT hResult;

	try
	{
		hResult = GetConnection().CreateInstance("ADODB.Connection");

		if (SUCCEEDED(hResult))
		{
			GetConnection()->Open(strConnectCommand.c_str(), GetUserName().c_str(), GetPassword().c_str(), adModeUnknown);

			CLogHelper::InfoLog("连接数据库[%s]成功", GetDefaultCatalogName());

			SetIsConnected(true);
		}
	}
	catch (_com_error e)
	{
		CLogHelper::ErrorLog("连接数据库失败 - Connection[%s]", strConnectCommand.c_str());

		return false;
	}

	return true;
}

bool COdbcDBConnector::Close()
{
	if (GetIsConnected())
		GetConnection()->Close();
	
	GetConnection().Release();

	SetIsConnected(false);

	return true;
}

bool COdbcDBConnector::Execute(const char* strCommand)
{
	if(GetIsConnected())
	{
		_RecordsetPtr recordset;
		_variant_t recordsetAffected;

		try
		{
			 GetConnection()->Execute(strCommand, &recordsetAffected, adCmdText);
		}
		catch (...)
		{
			// 如果cmdText 模式报错，则切换至 Unkown模式
			GetConnection()->Execute(strCommand, &recordsetAffected, adCmdUnknown);
		}
	}

	return true;
}
