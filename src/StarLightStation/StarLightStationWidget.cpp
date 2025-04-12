#include "pch.h"
#include "StarLightStationWidget.h"

#include <QGridLayout>
#include <ui_StarLightStation.h>

#include <QTabBar>


#include "Foundations/Common/EventGenerator.h"
#include "Foundations/Common/ServiceStopFlag.h"
#include "Foundations/Common/StringUtility.h"
#include "ThemeDefine.h"
#include "ViewModels/ImageFrameViewModel/ImageFrameViewModel.h"
#include "ThemeMessageBox.h"
#include "utilities/Messenger/EventArgs/UI/MessageBox/RequestMessageBoxEventArgs.h"
#include "utilities/Messenger/EventArgs/UI/Station/StationNameChangedEventArgs.h"
#include <ViewModels/ImageFrameViewModel/Annotation/AnnotationBase.h>

#include "ViewModels/SettingFrameViewModel/SettingFrameViewModel.h"

#include <Foundations/Log/LogHelper.h>

StarLightStationWidget::StarLightStationWidget(QWidget *parent)
	: QWidget(parent)
{
	CLogHelper::InitializeLogger();
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);

	showFullScreen();

	CLogHelper::InfoLog("Start up station");

	this->ui.frameTabWidget->clear();
	
	this->ui.frameTabWidget->findChildren<QTabBar*>().at(0)->hide();

	this->ui.frameTabWidget->addTab(&GetPatientListFrame(), "病人列表");
	this->ui.frameTabWidget->addTab(&GetImageFrame(), "检查影像");
	this->ui.frameTabWidget->addTab(&GetMPRImageFrame(), "MPR");
	this->ui.frameTabWidget->addTab(&GetVRImageFrame(), "VR");
	this->ui.frameTabWidget->addTab(&GetParonamaFrame(), "牙弓线");
	this->ui.frameTabWidget->addTab(&GetSettingFrame(), "系统设置");
	
	this->ui.frameTabWidget->setCurrentIndex(0);

	this->ui.PatientListMenuLabel->SetFrameMenu(FrameMenu::PatientList);
	this->ui.StudyImageMenuLabel->SetFrameMenu(FrameMenu::StudyImage);
	this->ui.MPRMenuLabel->SetFrameMenu(FrameMenu::MPR);
	this->ui.VRMenuLabel->SetFrameMenu(FrameMenu::VR);
	this->ui.ParonamaMenuLabel->SetFrameMenu(FrameMenu::Paronama);
	this->ui.SystemSettingMenuLabel->SetFrameMenu(FrameMenu::SystemSetting);

	this->ui.PatientListMenuLabel->SetSelected(true);
	this->ui.PatientListMenuLabel->updateLabelColor();

	CLogHelper::InfoLog("Initialize Messages");
	InitializeMessages();

	CLogHelper::InfoLog("Initialize Theme");
	InitializeTheme();

	ui.stationTitleLabel->setText(SettingFrameViewModel::GetInstance().GetStationName());
}

StarLightStationWidget::~StarLightStationWidget()
{
}

void StarLightStationWidget::OnCloseButtonClicked()
{
	CServiceStopFlag::StopRunning();

	CEventGenerator::GetInstance().WaitForAllEvents();
	
	close();
}

void StarLightStationWidget::OnMinimumButtonClicked()
{
	setWindowState(Qt::WindowMinimized);
}

void StarLightStationWidget::OnMaximumButtonClicked()
{
	setWindowState(Qt::WindowMaximized);
}

void StarLightStationWidget::InitializeMessages()
{
	CMessenger::GetInstance().Regist(EMessengerKey::MainFrameMenuChanged,
		this,
		[&](void* pParam) -> void*{
			auto menu = static_cast<CMenuLabel*>(pParam);

			if (menu == NULL_SYMBAL)
				return NULL_SYMBAL;

			this->ui.frameTabWidget->setCurrentIndex(
				(int)menu->GetFrameMenu() - 1);

			return NULL_SYMBAL;
		});

	CMessenger::GetInstance().Regist(EMessengerKey::OpenStudyTab,
		this, [&](void* pParam) -> void*
	{
		ui.StudyImageMenuLabel->mousePressEvent(NULL_SYMBAL);
		
		return NULL_SYMBAL;
	});

	CMessenger::GetInstance().Regist(EMessengerKey::OpenPatientListTab,
		this, [&](void* pParam) -> void*
	{
		ui.PatientListMenuLabel->mousePressEvent(NULL_SYMBAL);

		return NULL_SYMBAL;
	});

	CMessenger::GetInstance().Regist(EMessengerKey::MprLoadImages,
		this, [&](void* pParam) -> void*
	{
		ui.MPRMenuLabel->mousePressEvent(NULL_SYMBAL);

		return NULL_SYMBAL;
	});

	CMessenger::GetInstance().Regist(EMessengerKey::VRLoadImages,
		this, [&](void* pParam) -> void*
	{
		ui.VRMenuLabel->mousePressEvent(NULL_SYMBAL);

		return NULL_SYMBAL;
	});

	CMessenger::GetInstance().Regist(EMessengerKey::ParonamaLoadImages,
		this, [&](void* pParam) -> void*
	{
		ui.ParonamaMenuLabel->mousePressEvent(NULL_SYMBAL);

		return NULL_SYMBAL;
	});

	CMessenger::GetInstance().Regist(EMessengerKey::DisplayMessageBox,
		this, [&](void* pParam) -> void*
	{
		auto pEventArgs = (CRequestMessageBoxEventArgs*)pParam;

		ThemeMessageBox messageBox(this);

		if (pEventArgs != NULL_SYMBAL)
		{
			messageBox.SetTitleText(
				pEventArgs->GetTitleText().c_str(), pEventArgs->GetTitleText().length());
			messageBox.SetMessageText(
				pEventArgs->GetMessageText().c_str(), pEventArgs->GetMessageText().length());
		}

		messageBox.Display();

		return NULL_SYMBAL;
	});


	CMessenger::GetInstance().Regist(EMessengerKey::StationNameChanged,
		this, [&](void* pParam) -> void*
	{
		auto eventArgs = (CStationNameChangedEventArgs*)pParam;
		
		ui.stationTitleLabel->setText(
			QString::fromLocal8Bit(
				eventArgs->GetStationName().c_str(),
				eventArgs->GetStationName().length()));
		
		return NULL_SYMBAL;
	});
}

void StarLightStationWidget::InitializeTheme()
{
	CAnnotationBase::SetDefaultLinePen(QPen(QBrush(
		CThemeDefine::GetInstance().GetActivatedForground()),
		1, Qt::SolidLine));

	CAnnotationBase::SetDefaultResultTextForground(
		CThemeDefine::GetInstance().GetAnnotationForground());

	CAnnotationBase::SetDefaultResultTextFont(
		CThemeDefine::GetInstance().GetAnnotationFont());
}



