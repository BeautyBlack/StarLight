#include "pch.h"
#include "utilities/Configuration/UIStyleSheetInfo.h"

CUIStyleSheetInfo::CUIStyleSheetInfo()
{
	SetDefaultButtonEnableStyleSheet("QPushButton {" \
		"height: 40px; width: 150px;" \
		"background-color: rgb(30, 144, 255);" \
		"color: rgb(255, 255, 255);" \
		"border-radius:5px;}" \

		"QPushButton:hover {" \
		"height: 40px; width: 150px;" \
		"background-color: rgb(135, 206, 250);" \
		"color: rgb(47, 79, 79);" \
		"border-radius:5px;}");

	SetDefaultButtonDisableStyleSheet("QPushButton {" \
		"height: 40px; width: 150px;" \
		"background-color: rgb(160, 160, 160);" \
		"color: rgb(255, 255, 255);" \
		"border-radius:5px;}");

	SetDefaultDatetimeLabelStyleSheet("QLabel{color: rgb(30, 144, 255);}");
}

void CUIStyleSheetInfo::LoadFrom(void* pJsonObject)
{
	const auto pJsonRoot = static_cast<Json::Value*>(pJsonObject);

	if (pJsonRoot == NULL_SYMBAL)
		return;

	const Json::Value &referenceJsonRoot = (*pJsonRoot);

	const auto styleSheet = referenceJsonRoot[GetStyleSheetNodeJsonKey()];

	if (styleSheet.empty())
		return;

	const auto defaultStyleSheet = styleSheet[GetDefaultStyleSheetJsonKey()];

	if( !defaultStyleSheet.empty())
		SetDefaultStyleSheet(defaultStyleSheet.asCString());
}

void CUIStyleSheetInfo::SaveTo(void* pJsonObject)
{
	const auto pJsonRoot = static_cast<Json::Value*>(pJsonObject);

	if (pJsonRoot == NULL_SYMBAL)
		return;

	Json::Value &referenceJsonRoot = (*pJsonRoot);

	auto &styleSheet = referenceJsonRoot[GetStyleSheetNodeJsonKey()];

	styleSheet[GetDefaultStyleSheetJsonKey()] = GetDefaultStyleSheet();
}
