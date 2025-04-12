#include "pch.h"
#include "StarLightStation.h"
#include <QtWidgets/QApplication>

#include "StarLightStationWidget.h"
#include "utilities/Configuration/Configuration.h"

#include <QDesktopWidget>

#include "ThemeDefine.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	//QTextCodec *codec = QTextCodec::codecForName("GBK");

	//QTextCodec::setCodecForTr(codec);

	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
	//QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	//

	ConvertTextToQTTextFunc = [](const char* srcText, int nLen)
	-> STRING_TYPE
	{
		return QString::fromLocal8Bit(srcText, nLen).toStdString();
	};

    const auto styleSheet = CConfiguration::GetInstance().GetStyleSheet().GetDefaultStyleSheet();

	if (!styleSheet.empty())
		;// a.setStyleSheet(styleSheet.c_str());

	QDesktopWidget* pDesktopWidget = QApplication::desktop();

    const auto screenRect = QApplication::desktop()->screenGeometry();

	CConfiguration::GetInstance()
		.GetSystemInfo().SetScreenWidth(screenRect.width());
	
	CConfiguration::GetInstance()
		.GetSystemInfo().SetScreenHeight(screenRect.height());

	QFont font;
	font.setPointSizeF(9);

	CThemeDefine::GetInstance().SetImageInfoFont(font);
	
	StarLightStationWidget w;
    w.show();
    return a.exec();
}
