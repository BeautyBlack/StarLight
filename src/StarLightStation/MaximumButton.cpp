#include "pch.h"
#include "MaximumButton.h"

#include "ThemeDefine.h"

#include <QPainter>
#include <QRect>

CMaximumButton::CMaximumButton(QWidget *parent)
	: QWidget(parent)
{
	m_backgroundBrush = QBrush(CLOSE_BUTTON_BACKGROUND);
}

void CMaximumButton::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter painter(this);

	const auto clientRc = rect();
	
	painter.setRenderHint(QPainter::Antialiasing, true);

	painter.setBackground(m_backgroundBrush);
	painter.eraseRect(clientRc);

	painter.drawRect(QRect(clientRc.left() + 4, clientRc.top() + 4,
		clientRc.width() - 8, clientRc.height() - 8));
}

