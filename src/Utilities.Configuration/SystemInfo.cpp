#include "pch.h"
#include <utilities/Configuration/SystemInfo.h>

#include "Foundations/Common/FileUtility.h"

#include <json/json.h>

#include "Foundations/Common/StringUtility.h"


CSystemInfo::CSystemInfo()
{
	SetScreenHeight(0);
	SetScreenWidth(0);
}

void CSystemInfo::LoadFrom(void* pJsonObject)
{
	const auto pJsonRoot = static_cast<Json::Value*>(pJsonObject);

	if (pJsonRoot == NULL_SYMBAL)
		return;

	const Json::Value &referenceJsonRoot = (*pJsonRoot);

	const auto systemRoot = referenceJsonRoot[GetSystemNodeJsonKey()];

	if (systemRoot.empty())
		return;

	auto applicationName = systemRoot[GetApplicationNameJsonKey()];

	if (!applicationName.empty())
		SetApplicationName(CStringUtility::ConvertUTF8ToGB(applicationName.asCString()).GetString());

	auto storagePath = systemRoot[GetStoragePathJsonKey()];

	if (!storagePath.empty())
		SetStoragePath(CStringUtility::ConvertUTF8ToGB(storagePath.asCString()).GetString());
}

void CSystemInfo::SaveTo(void* pJsonObject)
{
	const auto pJsonRoot = static_cast<Json::Value*>(pJsonObject);

	if (pJsonRoot == NULL_SYMBAL)
		return;

	Json::Value &referenceJsonRoot = (*pJsonRoot);

	auto &systemRoot = referenceJsonRoot[GetSystemNodeJsonKey()];

	auto nameUtf8 = CStringUtility::ConvertGBToUTF8(GetApplicationName().c_str(), GetApplicationName().length());
	
	systemRoot[GetApplicationNameJsonKey()] = nameUtf8.GetString();
	
	auto pathUtf8 = CStringUtility::ConvertGBToUTF8(GetStoragePath().c_str(), GetStoragePath().length());

	systemRoot[GetStoragePathJsonKey()] = pathUtf8.GetString();
}


