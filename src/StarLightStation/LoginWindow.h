#pragma once

#include "CloseButton.h"
//#include "NoBoundWindow.h"
#include "ui_LoginWindow.h"

#include <QWidget>
#include <QMouseEvent>

class LoginWindow : public QWidget
{
	Q_OBJECT

public:
	LoginWindow(QWidget *parent = Q_NULLPTR);
	~LoginWindow() = default;

protected:
	void resizeEvent(QResizeEvent* event) override;
	//void showEvent(QShowEvent* event) override;

private:
	Ui::LoginWindow ui;

private:
	CCloseButton m_closeButton;
};
