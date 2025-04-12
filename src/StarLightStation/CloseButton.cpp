#include "pch.h"
#include "CloseButton.h"

#include "ThemeDefine.h"

#include <QPainter>
#include <QRect>


CCloseButton::CCloseButton(QWidget *parent)
	: QWidget(parent)
{
	m_backgroundBrush = QBrush(CLOSE_BUTTON_BACKGROUND);
}

void CCloseButton::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	const auto clientRc = rect();
	
	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing, true);
	
	painter.setBackground(m_backgroundBrush);
	painter.eraseRect(clientRc);

	painter.setPen(QPen(QBrush(CLOSE_BUTTON_FOREGROUND), 2));
	
	painter.drawLine(
		QPointF(clientRc.left() + 3, clientRc.top() + 3), 
		QPointF(clientRc.right() - 3, clientRc.bottom() - 3));

	painter.drawLine(
		QPointF(clientRc.left() + 3, clientRc.bottom() - 3),
		QPointF(clientRc.right() - 3, clientRc.top() + 3));
}
