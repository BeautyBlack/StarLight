#include "pch.h"
#include "NoBoundWindow.h"

NoBoundWindow::NoBoundWindow(QWidget *parent) :
	QWidget(parent)
{
	//设置无边框透明
	setWindowFlags(Qt::FramelessWindowHint);//无边框
	setAttribute(Qt::WA_TranslucentBackground);//背景透明

	m_areaMovable = geometry();
	m_bPressed = false;
}

void NoBoundWindow::mousePressEvent(QMouseEvent *e)
{
	//鼠标左键
	if (e->button() == Qt::LeftButton)
	{
		m_ptPress = e->pos();
		//qDebug() << pos() << e->pos() << m_ptPress;
		m_bPressed = m_areaMovable.contains(m_ptPress);
	}
}

void NoBoundWindow::mouseMoveEvent(QMouseEvent *e)
{
	if (m_bPressed)
	{
		//qDebug() << pos() << e->pos() << m_ptPress;
		move(pos() + e->pos() - m_ptPress);
	}
}

void NoBoundWindow::mouseReleaseEvent(QMouseEvent *)
{
	m_bPressed = false;
}

//设置鼠标按下的区域
void NoBoundWindow::setAreaMovable(const QRect rt)
{
	if (m_areaMovable != rt)
	{
		m_areaMovable = rt;
	}
}
