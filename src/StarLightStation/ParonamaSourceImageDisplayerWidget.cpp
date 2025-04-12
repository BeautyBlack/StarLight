#include "pch.h"
#include "ParonamaSourceImageDisplayerWidget.h"

CParonamaSourceImageDisplayerWidget::CParonamaSourceImageDisplayerWidget(QWidget* parent)
	: ParonamaSourceImageDisplayerWidgetBase(parent, 0)
{
}

CParonamaSourceImageDisplayerWidget::CParonamaSourceImageDisplayerWidget(QWidget* parent,
	unsigned long long messengerOwnerId)
	: ParonamaSourceImageDisplayerWidgetBase(parent, messengerOwnerId)
{
}

void CParonamaSourceImageDisplayerWidget::Reset()
{
	ImageDisplayerWidget::Reset();
}

void CParonamaSourceImageDisplayerWidget::mouseMoveEvent(QMouseEvent* ev)
{
	ImageDisplayerWidget::mouseMoveEvent(ev);
}

void CParonamaSourceImageDisplayerWidget::mousePressEvent(QMouseEvent* ev)
{
	ImageDisplayerWidget::mousePressEvent(ev);
}

void CParonamaSourceImageDisplayerWidget::mouseReleaseEvent(QMouseEvent* ev)
{
	ImageDisplayerWidget::mouseReleaseEvent(ev);
}

//void CParonamaSourceImageDisplayerWidget::resizeEvent(QResizeEvent* ev)
//{
//	ImageDisplayerWidget::resizeEvent(ev);
//}

void CParonamaSourceImageDisplayerWidget::wheelEvent(QWheelEvent* ev)
{
	ImageDisplayerWidget::wheelEvent(ev);
}

void CParonamaSourceImageDisplayerWidget::UpdateFromViewModel()
{
	ImageDisplayerWidget::UpdateFromViewModel();
}
