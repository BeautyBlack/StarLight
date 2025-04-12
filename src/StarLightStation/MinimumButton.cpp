#include "pch.h"
#include "MinimumButton.h"

#include "ThemeDefine.h"

#include <QPainter>
#include <QRect>

CMinimumButton::CMinimumButton(QWidget *parent)
	: QWidget(parent)
{
	m_backgroundBrush = QBrush(CLOSE_BUTTON_BACKGROUND);
}

void CMinimumButton::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	const auto clientRc = rect();

	painter.setRenderHint(QPainter::Antialiasing, true);

	painter.setBackground(m_backgroundBrush);
	painter.eraseRect(clientRc);

	painter.setPen(QPen(QBrush(CLOSE_BUTTON_FOREGROUND), 4));

	painter.drawLine(QPoint(clientRc.left() + 5, clientRc.bottom() - 10),
		QPoint(clientRc.right() - 5, clientRc.bottom() - 10));
}

