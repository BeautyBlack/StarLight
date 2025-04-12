#include "pch.h"
#include "OpeningStudyProcessWidget.h"
#include <thread>
#include <QTime>
#include "ViewModels/ImageFrameViewModel/ImageFrameViewModel.h"

OpeningStudyProcessWidget::OpeningStudyProcessWidget(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

	SetStudyDto(NULL_SYMBAL);

	GetTimer().setParent(this);

	connect(&GetTimer(), SIGNAL(timeout()), this, SLOT(UpdateProcessStatus()));

	GetTimer().start(200);

	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

	m_labelText[0] = QString::fromLocal8Bit("正在打开影像");
	m_labelText[1] = QString::fromLocal8Bit("正在打开影像...");
	m_labelText[2] = QString::fromLocal8Bit("正在打开影像......");

	SetTick(0);
}

OpeningStudyProcessWidget::~OpeningStudyProcessWidget()
{
}

void OpeningStudyProcessWidget::StartOpening()
{
	std::thread processThread([&]()->void {
		const auto studyTab = CImageFrameViewModel::GetInstance().LoadStudy(*GetStudyDto());

		if (studyTab)
			SetOpeningStudyTab(studyTab);

		QMetaObject::invokeMethod(this, "close");
	});

	processThread.detach();

	exec();
}

void OpeningStudyProcessWidget::UpdateProcessStatus()
{
	auto step = qrand() % 5;

	if (qrand() % 2 == 0)
		++step;

	ui.openingStudyProgressBar->setValue(min(99,
		ui.openingStudyProgressBar->value() + step));

	ui.openingStudyLable->setText(m_labelText[GetTick() % 3]);

	SetTick(GetTick() + 1);
}
