#pragma once

#include <QWidget>
#include "ui_StarLightStationWidget.h"

#include "PatientListFrameWidget.h"
#include "ImageFrameWidget.h"
#include "SettingFrameWidget.h"
#include "MPRFrameWidget.h"
#include "VRFrameWidget.h"
#include "ParonamaFrameWidget.h"

#include <QPushButton>

class StarLightStationWidget : public QWidget, public CMessengerOwner
{
	Q_OBJECT

public:
	StarLightStationWidget(QWidget *parent = Q_NULLPTR);
	~StarLightStationWidget();

private slots:
	void OnCloseButtonClicked();
	void OnMinimumButtonClicked();
	void OnMaximumButtonClicked();

private:

	void InitializeMessages();
	void InitializeTheme();
	
private:
	Ui::StarLightStationWidget ui;

	AUTO_PROPERTY_REF_NO_SET(PatientListFrame, PatientListFrameWidget);
	AUTO_PROPERTY_REF_NO_SET(ImageFrame, ImageFrameWidget);
	AUTO_PROPERTY_REF_NO_SET(SettingFrame, SettingFrameWidget);
	AUTO_PROPERTY_REF_NO_SET(MPRImageFrame, MPRFrameWidget);
	AUTO_PROPERTY_REF_NO_SET(VRImageFrame, VRFrameWidget);
	AUTO_PROPERTY_REF_NO_SET(ParonamaFrame, ParonamaFrameWidget);
};
