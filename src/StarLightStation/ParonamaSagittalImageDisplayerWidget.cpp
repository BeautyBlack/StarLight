#include "pch.h"
#include "ParonamaSagittalImageDisplayerWidget.h"

CParonamaSagittalImageDisplayerWidget::CParonamaSagittalImageDisplayerWidget(QWidget* parent)
	: ParonamaSagittalImageDisplayerWidgetBase(parent, 0)
{
}

CParonamaSagittalImageDisplayerWidget::CParonamaSagittalImageDisplayerWidget(QWidget* parent,
	unsigned long long messengerOwnerId)
	: ParonamaSagittalImageDisplayerWidgetBase(parent, messengerOwnerId)
{
}

void CParonamaSagittalImageDisplayerWidget::Reset()
{
}

//void CParonamaSagittalImageDisplayerWidget::resizeEvent(QResizeEvent* event)
//{
//}

void CParonamaSagittalImageDisplayerWidget::wheelEvent(QWheelEvent* event)
{
}

void CParonamaSagittalImageDisplayerWidget::UpdateFromViewModel()
{
}

void CParonamaSagittalImageDisplayerWidget::SwitchToNextSlice(bool isForward)
{
}
