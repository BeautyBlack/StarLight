#include "pch.h"
#include "ThemeDefine.h"

#include <QBrush>

CThemeDefine CThemeDefine::m_Instance;

CThemeDefine::CThemeDefine()
	: m_ImageInfoPen(QBrush(QColor(255,255,255)), 1)
{
	GetImageInfoFont().setPointSize(9);
	GetImageInfoForground().setRgb(255, 255, 255);
	GetAnnotationForground().setRgb(255, 255, 0);

	GetSelectedForground().setRgb(0, 0, 255);
	GetActivatedForground().setRgb(255, 255, 0);
	GetImageBackground().setRgb(0, 0, 0);

	GetAnnotationFont().setPointSize(12);
	GetAnnotationFont().setBold(true);
}

