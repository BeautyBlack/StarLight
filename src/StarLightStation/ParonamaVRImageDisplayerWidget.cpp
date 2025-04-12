#include "pch.h"
#include "ParonamaVRImageDisplayerWidget.h"

CParonamaVRImageDisplayerWidget::CParonamaVRImageDisplayerWidget(QWidget* parent)
	: ParonamaVRImageDisplayerWidgetBase(parent, 0)
{
}

CParonamaVRImageDisplayerWidget::CParonamaVRImageDisplayerWidget(QWidget* parent, unsigned long long messengerOwnerId)
	: ParonamaVRImageDisplayerWidgetBase(parent, messengerOwnerId)
{
}

void CParonamaVRImageDisplayerWidget::Reset()
{
}
