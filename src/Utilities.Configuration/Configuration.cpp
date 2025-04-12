#include "pch.h"
#include <utilities/Configuration/Configuration.h>

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <json/json.h>

#include "Foundations/Common/FileUtility.h"

CConfiguration CConfiguration::m_Instance;

CConfiguration::CConfiguration()
{
	TRACE("Create Object : %s:%d", __FUNCTION__, __LINE__);
	
	char strPath[MAX_PATH_SIZE] = { 0 };

	GetModuleFileName(NULL_SYMBAL, strPath, sizeof(strPath));
	PathRemoveFileSpec(strPath);
	PathRemoveFileSpec(strPath);

	PathAppend(strPath, "Config\\System.json");

	SetDefaultConfigFilePath(strPath);
	
	Load();
}

void CConfiguration::Load()
{
	auto nFileSize = 0;
	auto textBuffer = CFileUtility::ReadWholeFile(GetDefaultConfigFilePath().c_str(), nFileSize);

	if (textBuffer == NULL_SYMBAL)
		return;
	
	Json::Value jsonRoot;
	Json::CharReaderBuilder jsonReaderBuilder;
	Json::String errText;

	auto jsonReader = std::unique_ptr<Json::CharReader>(jsonReaderBuilder.newCharReader());

	if (jsonReader->parse(textBuffer, textBuffer + nFileSize, &jsonRoot, &errText))
	{
		GetSystemInfo().LoadFrom(&jsonRoot);
		GetDatabaseInfo().LoadFrom(&jsonRoot);
		GetDicomInfo().LoadFrom(&jsonRoot);
		GetStyleSheet().LoadFrom(&jsonRoot);
		GetVRInfo().LoadFrom(&jsonRoot);
	}

	delete[] textBuffer;
}

void CConfiguration::Save()
{
	Json::Value jsonRoot;

	GetSystemInfo().SaveTo(&jsonRoot);
	GetDatabaseInfo().SaveTo(&jsonRoot);
	GetDicomInfo().SaveTo(&jsonRoot);
	GetStyleSheet().SaveTo(&jsonRoot);
	GetVRInfo().SaveTo(&jsonRoot);

	auto jsonText = jsonRoot.toStyledString();

	CFileUtility::RecordToFile(jsonText.c_str(), jsonText.length(), GetDefaultConfigFilePath().c_str());
}



