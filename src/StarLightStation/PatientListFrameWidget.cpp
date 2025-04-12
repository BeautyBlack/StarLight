#include "pch.h"
#include "PatientListFrameWidget.h"

#include "ImportImagesProcessWidget.h"

#include <ViewModels/PatientListFrameViewModel/PatientListFrameViewModel.h>

#include <vector>
#include <QPair>
#include "Foundations/Common/FileUtility.h"
#include "utilities/Configuration/Configuration.h"
#include "utilities/DicomClient/DcmStoreSCU.h"
#include "utilities/DicomClient/StoreScuBackgroundWorker.h"
#include "ViewModels/ImageFrameViewModel/ImageFrameViewModel.h"
#include "utilities/Messenger/EventArgs/UI/MessageBox/RequestMessageBoxEventArgs.h"

#include "utilities/Messenger/Messenger.h"
#include "ThemeDefine.h"
#include <qfiledialog.h>
#include "OpeningStudyProcessWidget.h"


std::vector<QPair<StudyReportState, CString>> PatientListFrameReportStates
= { QPair<StudyReportState, CString>(StudyReportState::NoReport, "未写报告"),
	QPair<StudyReportState, CString>(StudyReportState::Reported, "已写报告")};

PatientListFrameWidget::PatientListFrameWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.patientTableView->setModel(
		&CPatientListFrameViewModel::GetInstance()
		.GetPatientListTableViewModel());
	
	const auto &widthMap = CPatientListTabelViewModel::GetSuggestColumnWidths();

	for (const auto& element : widthMap)
	{
		if (element.second > 0)
			ui.patientTableView->setColumnWidth(element.first, element.second);
	}

	ui.patientTableView->horizontalHeader()->setMinimumHeight(35);
	ui.patientTableView->horizontalHeader()->setStretchLastSection(true);
	ui.patientTableView->horizontalHeader()->setFont(
		CThemeDefine::GetInstance().GetImageInfoFont());

	OnSelectTodayStudies();

	auto nComboIndex = 0;
	for(auto itBegin = PatientListFrameReportStates.begin(); itBegin != PatientListFrameReportStates.end(); ++itBegin)
	{
		CString &text = (*itBegin).second;

		ui.reportStatusFilterComboBox->addItem(QString::fromLocal8Bit(text.GetString(), text.GetLength()));
		ui.reportStatusFilterComboBox->setItemData(nComboIndex, (int)(*itBegin).first);
	}

	// Connects
	connect(ui.studyDateTodayFilterRadioButton,
		SIGNAL(clicked()),
		this,
		SLOT(OnSelectTodayStudies()));

	connect(ui.studyDateThisMonthFilterRadioButton,
		SIGNAL(clicked()),
		this,
		SLOT(OnSelectThisMonthStudies()));

	connect(ui.queryButton,
		SIGNAL(clicked()),
		this,
		SLOT(OnQueryStudies()));

	connect(ui.sendPacsButton,
		SIGNAL(clicked()),
		this,
		SLOT(OnSendToPacs()));

	connect(ui.importFilesButton,
		SIGNAL(clicked()),
		this,
		SLOT(OnImportFiles()));
	
	connect(ui.exportStudyButton,
		SIGNAL(clicked()),
		this,
		SLOT(OnExportFiles()));

	connect(ui.burnStudyButton,
		SIGNAL(clicked()),
		this,
		SLOT(OnBurnStudies()));

	connect(ui.viewStudyButton,
		SIGNAL(clicked()),
		this,
		SLOT(OnDisplayStudy()));

	connect(ui.patientTableView,
		SIGNAL(doubleClicked(const QModelIndex &)),
		this,
		SLOT(OnDoubleClickedPatientTable(const QModelIndex &)));

	connect(ui.viewReportButton,
		SIGNAL(clicked()),
		this,
		SLOT(OnDisplayReport()));
}

void PatientListFrameWidget::OnSelectTodayStudies()
{
	ui.studyDateFromFilterDateEdit->setDate(QDateTime::currentDateTime().date());
	ui.studyDateToFilterDateEdit->setDate(QDateTime::currentDateTime().date());
}

void PatientListFrameWidget::OnSelectThisMonthStudies()
{
	const auto today = QDateTime::currentDateTime().date();
	
	ui.studyDateFromFilterDateEdit->setDate(
		QDate(today.year(), today.month(), 1));
	
	ui.studyDateToFilterDateEdit->setDate(
		QDate(today.year(), today.month(), today.daysInMonth()));
}

void PatientListFrameWidget::OnQueryStudies()
{
	UpdateQueryFilter();

	CPatientListFrameViewModel::GetInstance().Query();

	//ui.patientTableView->update();
}

void PatientListFrameWidget::OnSendToPacs()
{
	auto selections = ui.patientTableView->selectionModel();

	QModelIndexList selected = selections->selectedIndexes();

	foreach(auto index, selected)
	{
		auto &studyDto = CPatientListFrameViewModel::GetInstance().GetPatientListTableViewModel().GetStudy(index.row());

		if(studyDto.GetStudyInstanceUID().empty())
			continue;

		auto pStrFiles = std::make_shared<CStringList>();
		char strStudyPath[MAX_PATH] = { 0 };

		PathCombine(strStudyPath,
			CConfiguration::GetInstance().GetSystemInfo().GetStoragePath().c_str(),
			studyDto.GetStudyInstanceUID().c_str());

		if(PathFileExists(strStudyPath))
			continue;

		CFileUtility::SearchPaths([&](STRING_TYPE aFile) -> void
		{
			pStrFiles->AddTail(aFile.c_str());
		}, strStudyPath, "dcm");

		CStoreScuBackgroundWorker::GetInstance().AddTask(pStrFiles);
	}
}

void PatientListFrameWidget::OnImportFiles()
{
	static QString lastSelectPath = "/";

	CString tmpText = "选择导入影像文件夹";

	auto filePath = QFileDialog::getExistingDirectory(
		NULL_SYMBAL,
		QString::fromLocal8Bit(tmpText.GetString(), tmpText.GetLength()),
		lastSelectPath);

	if (filePath.isEmpty())
		return;

	lastSelectPath = filePath;

	ImportImagesProcessWidget importWidget;

	importWidget.SetImportingPath(filePath.toLocal8Bit().data());

	importWidget.StartImport();
}

void PatientListFrameWidget::OnExportFiles()
{
	// todo
}

void PatientListFrameWidget::OnBurnStudies()
{
	// todo
}

void PatientListFrameWidget::OnDisplayStudy()
{
	auto selections = ui.patientTableView->selectionModel();

	QModelIndexList selected = selections->selectedRows();

	foreach(auto index, selected)
	{
		auto &studyDto = CPatientListFrameViewModel::GetInstance().GetPatientListTableViewModel().GetStudy(index.row());

		if (studyDto.GetStudyInstanceUID().empty())
			continue;

		const auto studyTab = CImageFrameViewModel::GetInstance().LoadStudy(studyDto);

		CMessenger::GetInstance().SendMessage(EMessengerKey::OpenStudyTab,
			const_cast<std::shared_ptr<CStudyTabItemViewModel>*>(&studyTab));
	}
}

void PatientListFrameWidget::OnDisplayReport()
{
	// todo
}

void PatientListFrameWidget::OnDoubleClickedPatientTable(const QModelIndex& index)
{
	auto &studyDto =
		CPatientListFrameViewModel::GetInstance()
	.GetPatientListTableViewModel().GetStudy(index.row());

	if (studyDto.GetStudyInstanceUID().empty())
		return;

	OpeningStudyProcessWidget widget;

	widget.SetStudyDto(&studyDto);

	widget.StartOpening();

	if( !widget.GetOpeningStudyTab())
	{
		CRequestMessageBoxEventArgs eventArgs;

		eventArgs.SetTitleText("打开影像");
		eventArgs.SetMessageText("打开影像失败");

		CMessenger::GetInstance().SendMessage(EMessengerKey::DisplayMessageBox, &eventArgs);
	}
	else
	{
		CMessenger::GetInstance().SendMessage(EMessengerKey::OpenStudyTab,
			const_cast<std::shared_ptr<CStudyTabItemViewModel>*>(&widget.GetOpeningStudyTab()));
	}
}

void PatientListFrameWidget::UpdateQueryFilter()
{
	auto &viewModel = CPatientListFrameViewModel::GetInstance();

	viewModel.GetStudyFilter().SetPatientName(
		ui.nameFilterEdit->text().toStdString().c_str());

	viewModel.GetStudyFilter().SetAccessionNum(
		ui.accessionNumFilterEdit->text().toStdString().c_str());

	auto reportStateIndex = ui.reportStatusFilterComboBox->currentIndex();

	viewModel.GetStudyFilter().SetStudyReportState(
		PatientListFrameReportStates[reportStateIndex].first);

	int nYearFrom = 0, nYearTo = 0,
		nMonthFrom = 0, nMonthTo = 0,
		nDayFrom = 0, nDayTo = 0;

	ui.studyDateFromFilterDateEdit->date().getDate(
		&nYearFrom, &nMonthFrom, &nDayFrom);
	
	ui.studyDateToFilterDateEdit->date().getDate(
		&nYearTo, &nMonthTo, &nDayTo);
	
	viewModel.GetStudyFilter().SetDateFrom(
		CTime(nYearFrom, nMonthFrom, nDayFrom, 
			0, 0, 0, 0));

	viewModel.GetStudyFilter().SetDateTo(
		CTime(nYearTo, nMonthTo, nDayTo,
			0, 0, 0, 0));
}

