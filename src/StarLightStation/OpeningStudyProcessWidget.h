#pragma once

#include <qdialog.h>
#include <QTimer.h>
#include "ui_OpeningStudyProcessWidget.h"
#include "Foundations/Common/ToDefine.h"

class CStudyDto;
class CStudyTabItemViewModel;

class OpeningStudyProcessWidget : public QDialog
{
	Q_OBJECT

public:
	OpeningStudyProcessWidget(QWidget *parent = Q_NULLPTR);
	~OpeningStudyProcessWidget();

	void StartOpening();

private slots:
	void UpdateProcessStatus();

private:
	Ui::OpeningStudyProcessWidget ui;

	AUTO_PROPERTY_REF(StudyDto, CStudyDto*);

	AUTO_PROPERTY_REF(OpeningStudyTab, std::shared_ptr<CStudyTabItemViewModel>);

	AUTO_PROPERTY_REF_NO_SET(Timer, QTimer);

	AUTO_PROPERTY_REF(Tick, int);

	QString m_labelText[3];
};
