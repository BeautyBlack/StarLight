#pragma once

#include <QWidget>
#include <QMouseEvent>

//#include "ui_NoBoundWindow.h"

class NoBoundWindow : public QWidget
{
	//Q_OBJECT

public:
	NoBoundWindow(QWidget *parent = Q_NULLPTR);
	~NoBoundWindow() = default;


	//������갴�¿��ƶ����ڵ��������Ӵ����б������ø�����
	void setAreaMovable(const QRect rt);

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

private:
	QRect m_areaMovable;//���ƶ����ڵ��������ֻ���ڸ������²����ƶ�����
	bool m_bPressed;//��갴�±�־���������Ҽ���
	QPoint m_ptPress;//��갴�µĳ�ʼλ��
	
};
