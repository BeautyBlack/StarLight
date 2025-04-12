#pragma once

#include <QWidget>

#include <Foundations/Common/ToDefine.h>

#include <Utilities/Messenger/Messenger.h>

#include <vector>

#include "ui_VRFrameWidget.h"
#include "VRColorSettingWidget.h"

class VRFrameWidget : public QWidget, public CMessengerOwner
{
	Q_OBJECT

public:
	VRFrameWidget(QWidget *parent = Q_NULLPTR);
	~VRFrameWidget();

protected slots:
	void on_colorSettingButton_clicked();
	
private:
	void InitializeMessenger();

private:
	Ui::VRFrameWidget ui;

	AUTO_PROPERTY_REF_NO_SET(ColorSettingWidget, VRColorSettingWidget)
};
