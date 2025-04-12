#include "pch.h"
#include "LoginWindow.h"

//#include <NoBoundWindow.h>

LoginWindow::LoginWindow(QWidget *parent)
	: QWidget(parent), m_closeButton(this)
{
	//ui.setupUi(this);

	m_closeButton.resize(35, 35);

	connect(&m_closeButton, SIGNAL(mousePressEvent()), this, SLOT(close()));

}

void LoginWindow::resizeEvent(QResizeEvent* event)
{
	const auto clientRc = rect();

	m_closeButton.move(clientRc.right() - 10 - 35, clientRc.top() + 10);

	//setAreaMovable(QRect(0, 0, clientRc.width(), 50));
}

