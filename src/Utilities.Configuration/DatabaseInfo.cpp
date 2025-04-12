#include "pch.h"
#include <utilities/Configuration/DatabaseInfo.h>

#include <json/json.h>

CDatabaseInfo::CDatabaseInfo()
{
	SetDefaultCatalogName("StarLight");
	SetServerName("SQLEXPRESS");
}

void CDatabaseInfo::LoadFrom(void* pJsonObject)
{
	const auto pJsonRoot = static_cast<Json::Value*>(pJsonObject);

	if (pJsonRoot == NULL_SYMBAL)
		return;

	const Json::Value &referenceJsonRoot = (*pJsonRoot);

	const auto databaseRoot = referenceJsonRoot[GetDatabaseNodeJsonKey()];

	if (databaseRoot.empty())
		return;

	auto catalogName = databaseRoot[GetDefaultCatalogNameJsonKey()];
	IF_JSON_TEXT_VALUE_NOT_EMPTY(catalogName)
		SetDefaultCatalogName(catalogName.asCString());

	auto password = databaseRoot[GetPasswordJsonKey()];
	IF_JSON_TEXT_VALUE_NOT_EMPTY(password)
		SetPassword(password.asCString());
	
	auto serverAddress = databaseRoot[GetServerAddressJsonKey()];
	IF_JSON_TEXT_VALUE_NOT_EMPTY(serverAddress)
		SetServerAddress(serverAddress.asCString());

	auto serverName = databaseRoot[GetServerNameJsonKey()];
	IF_JSON_TEXT_VALUE_NOT_EMPTY(serverName)
		SetServerName(serverName.asCString());

	auto useLocalServer = databaseRoot[GetUseLocalServerJsonKey()];
	IF_JSON_VALUE_NOT_EMPTY(useLocalServer)
		SetUseLocalServer(useLocalServer.asBool());

	auto userName = databaseRoot[GetUserNameJsonKey()];
	IF_JSON_TEXT_VALUE_NOT_EMPTY(userName)
		SetUserName(userName.asCString());
}

void CDatabaseInfo::SaveTo(void* pJsonObject)
{
	const auto pJsonRoot = static_cast<Json::Value*>(pJsonObject);

	if (pJsonRoot == NULL_SYMBAL)
		return;

	Json::Value &referenceJsonRoot = (*pJsonRoot);

	auto &databaseRoot = referenceJsonRoot[GetDatabaseNodeJsonKey()];

	databaseRoot[GetDefaultCatalogNameJsonKey()] = GetDefaultCatalogName();
	databaseRoot[GetPasswordJsonKey()] = GetPassword();
	databaseRoot[GetServerAddressJsonKey()] = GetServerAddress();
	databaseRoot[GetServerNameJsonKey()] = GetServerName();
	databaseRoot[GetUseLocalServerJsonKey()] = GetUseLocalServer();
	databaseRoot[GetUserNameJsonKey()] = GetUserName();
}
