#pragma once

#include <QtWidgets/QMainWindow>

#include <QPushButton>
#include "LoginWindow.h"
#include "ui_StarLightStation.h"

class StarLightStation : public QMainWindow
{
    Q_OBJECT

public:
    StarLightStation(QWidget *parent = Q_NULLPTR);

	void showEvent(QShowEvent* event) override;

protected:

	void CreateGridGroupBox();
	
private:
    Ui::StarLightStationClass ui;

	//LoginWindow m_loginWindow;

	QPushButton TestBtn1, TestBtn2, TestBtn3, TestBtn4;
};
