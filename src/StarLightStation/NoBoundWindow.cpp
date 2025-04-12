#include "pch.h"
#include "NoBoundWindow.h"

NoBoundWindow::NoBoundWindow(QWidget *parent) :
	QWidget(parent)
{
	//�����ޱ߿�͸��
	setWindowFlags(Qt::FramelessWindowHint);//�ޱ߿�
	setAttribute(Qt::WA_TranslucentBackground);//����͸��

	m_areaMovable = geometry();
	m_bPressed = false;
}

void NoBoundWindow::mousePressEvent(QMouseEvent *e)
{
	//������
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

//������갴�µ�����
void NoBoundWindow::setAreaMovable(const QRect rt)
{
	if (m_areaMovable != rt)
	{
		m_areaMovable = rt;
	}
}
