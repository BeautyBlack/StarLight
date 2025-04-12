#pragma once

#include <QWidget>

class CMinimumButton : public QWidget
{
	Q_OBJECT

public:
	CMinimumButton(QWidget *parent);
	~CMinimumButton() = default;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QBrush m_backgroundBrush;
};
