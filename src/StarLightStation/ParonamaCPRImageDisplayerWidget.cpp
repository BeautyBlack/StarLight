#include "pch.h"
#include "ParonamaCPRImageDisplayerWidget.h"

CParonamaCPRImageDisplayerWidget::CParonamaCPRImageDisplayerWidget(QWidget* parent)
	:ParonamaCPRImageDisplayerWidgetBase(parent, 0)
{
}

CParonamaCPRImageDisplayerWidget::CParonamaCPRImageDisplayerWidget(QWidget* parent, MESSAGE_GROUP_ID_TYPE messengerOwnerId)
	: ParonamaCPRImageDisplayerWidgetBase(parent, messengerOwnerId)
{
}

void CParonamaCPRImageDisplayerWidget::Reset()
{
	ImageDisplayerWidget::Reset();
}

//void CParonamaCPRImageDisplayerWidget::resizeEvent(QResizeEvent* event)
//{
//}

void CParonamaCPRImageDisplayerWidget::UpdateFromViewModel()
{
	ImageDisplayerWidget::UpdateFromViewModel();
}

void CParonamaCPRImageDisplayerWidget::SwitchToNextSlice(bool isForward)
{
}
