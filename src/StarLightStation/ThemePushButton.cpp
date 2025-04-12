#include "pch.h"
#include "ThemePushButton.h"

#include "utilities/Configuration/Configuration.h"

ThemePushButton::ThemePushButton(QWidget *parent)
	: QPushButton(parent)
{
	SetIsEnable(true);
}

ThemePushButton::~ThemePushButton()
{
}

void ThemePushButton::SetIsEnable(bool isEnable)
{
	setEnabled(isEnable);

	setStyleSheet(isEnable
		? CConfiguration::GetInstance().GetStyleSheet().GetDefaultButtonEnableStyleSheet().c_str()
		: CConfiguration::GetInstance().GetStyleSheet().GetDefaultButtonDisableStyleSheet().c_str());
}
