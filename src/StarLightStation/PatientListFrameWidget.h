#pragma once

#include <QWidget>
#include "ui_PatientListFrameWidget.h"

class PatientListFrameWidget : public QWidget
{
	Q_OBJECT

public:
	PatientListFrameWidget(QWidget *parent = Q_NULLPTR);
	~PatientListFrameWidget() = default;

protected slots:
	// ѡ���ռ��
	void OnSelectTodayStudies();

	// ѡ���¼��
	void OnSelectThisMonthStudies();

	// ��ѯ
	void OnQueryStudies();

	// Ӱ����PACS
	void OnSendToPacs();

	// ����Ӱ���ļ�
	void OnImportFiles();

	// ����Ӱ���ļ�
	void OnExportFiles();

	// ��¼����
	void OnBurnStudies();

	// �鿴Ӱ��
	void OnDisplayStudy();

	// �鿴����
	void OnDisplayReport();

	void OnDoubleClickedPatientTable(const QModelIndex &index);

private:
	void UpdateQueryFilter();
	
private:
	Ui::PatientListFrameWidget ui;

};
