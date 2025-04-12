#pragma once

#include <QDialog>
#include "ui_VRColorSettingWidget.h"
#include "utilities/Configuration/VRColorSetting.h"
#include "utilities/Messenger/Messenger.h"

class VRColorSettingWidget : public QDialog, public CMessengerOwner
{
	Q_OBJECT

public:
	VRColorSettingWidget(QWidget *parent = Q_NULLPTR);
	~VRColorSettingWidget();

	void LoadSetting();

private slots:
	void on_addColorButton_clicked();
	void on_addOpacityButton_clicked();
	void on_applyButton_clicked();
	void on_saveButton_clicked();
	void on_clearOpacityButton_clicked();
	void on_clearColorButton_clicked();

	void UpdateDisplay();

	void OnBtnClicked(void);
	void OnOpacityBtnClicked(void);
	
private:
	Ui::VRColorSettingWidget ui;

	AUTO_PROPERTY_REF(Setting, CVRColorSetting);

	QPushButton m_colorButton;
	QPushButton m_opcaityButton;
};
