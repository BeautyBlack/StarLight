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
	// 选择本日检查
	void OnSelectTodayStudies();

	// 选择本月检查
	void OnSelectThisMonthStudies();

	// 查询
	void OnQueryStudies();

	// 影像发送PACS
	void OnSendToPacs();

	// 导入影像文件
	void OnImportFiles();

	// 导出影像文件
	void OnExportFiles();

	// 刻录光盘
	void OnBurnStudies();

	// 查看影像
	void OnDisplayStudy();

	// 查看报告
	void OnDisplayReport();

	void OnDoubleClickedPatientTable(const QModelIndex &index);

private:
	void UpdateQueryFilter();
	
private:
	Ui::PatientListFrameWidget ui;

};
