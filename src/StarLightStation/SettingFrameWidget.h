#pragma once

#include <QWidget>
#include "ui_SettingFrameWidget.h"

#include <Foundations/Common/ToDefine.h>

#include <QIntValidator>

class SettingFrameWidget : public QWidget
{
	Q_OBJECT

public:
	SettingFrameWidget(QWidget *parent = Q_NULLPTR);
	~SettingFrameWidget();

protected:
	void Initialize();

private slots:
	void on_stationNameEdit_editingFinished();
	void on_storagePathEdit_editingFinished();
	
	void on_localServerPort_editingFinished();
	void on_localAETEdit_editingFinished();
	void on_pacsPortEdit_editingFinished();
	void on_pacsAETEdit_editingFinished();
	void on_pacsIPEdit_editingFinished();
	
	void on_databaseEntityNameEdit_editingFinished();

	void on_selectStoragePathButton_clicked();

	void on_echoPACSButton_clicked();
	
private:
	Ui::SettingFrameWidget ui;

private:
	AUTO_PROPERTY_REF_NO_SET(PortValidator, QIntValidator);
};
