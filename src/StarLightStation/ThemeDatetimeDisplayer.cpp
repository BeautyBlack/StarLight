#include "pch.h"
#include "ThemeDatetimeDisplayer.h"

#include <QDateTime>

#include "utilities/Configuration/Configuration.h"

ThemeDatetimeDisplayer::ThemeDatetimeDisplayer(QWidget *parent)
	: QLabel(parent)
{
	GetTimer().setParent(this);

	connect(&GetTimer(), SIGNAL(timeout()), this, SLOT(UpdateDatetime()));

	GetTimer().start(1000);

	/*setStyleSheet(CConfiguration::GetInstance()
		.GetStyleSheet()
		.GetDefaultButtonDisableStyleSheet().c_str());*/
}

void ThemeDatetimeDisplayer::UpdateDatetime()
{
	const auto now = QDateTime::currentDateTime();

	QString timeText = now.toString("yyyy-MM-dd");;

	timeText.append("\n");
	timeText.append(now.toString("hh:mm:ss ddd"));

	setText(timeText);
}
