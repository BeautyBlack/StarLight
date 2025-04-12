#include "pch.h"

#include "SettingFrameWidget.h"


#include <qfiledialog.h>
#include <ViewModels/SettingFrameViewModel/SettingFrameViewModel.h>



#include "utilities/Messenger/EventArgs/UI/MessageBox/RequestMessageBoxEventArgs.h"
#include "utilities/Messenger/Messenger.h"
#include "utilities/Messenger/MessengerKey.h"


SettingFrameWidget::SettingFrameWidget(QWidget *parent)
	: QWidget(parent),
      m_PortValidator(1, 65535, this)
{
	ui.setupUi(this);

	ui.localServerPort->setValidator(&GetPortValidator());
	ui.pacsPortEdit->setValidator(&GetPortValidator());

	Initialize();
}

SettingFrameWidget::~SettingFrameWidget()
{
}

void SettingFrameWidget::Initialize()
{
	ui.stationNameEdit->setText(SettingFrameViewModel::GetInstance().GetStationName());
	ui.storagePathEdit->setText(SettingFrameViewModel::GetInstance().GetStoragePath());

	ui.localServerPort->setText(SettingFrameViewModel::GetInstance().GetLocalDicomPort());
	ui.localAETEdit->setText(SettingFrameViewModel::GetInstance().GetLocalAET());
	ui.pacsPortEdit->setText(SettingFrameViewModel::GetInstance().GetPacsPort());
	ui.pacsAETEdit->setText(SettingFrameViewModel::GetInstance().GetPacsAET());
	ui.pacsIPEdit->setText(SettingFrameViewModel::GetInstance().GetPacsIP());

	ui.databaseEntityNameEdit->setText(SettingFrameViewModel::GetInstance().GetDatabaseServerName());
}

void SettingFrameWidget::on_stationNameEdit_editingFinished()
{
	SettingFrameViewModel::GetInstance().SetStationName(
		ui.stationNameEdit->text());
}

void SettingFrameWidget::on_storagePathEdit_editingFinished()
{
	SettingFrameViewModel::GetInstance().SetStoragePath(
		ui.storagePathEdit->text());
}

void SettingFrameWidget::on_localServerPort_editingFinished()
{
	SettingFrameViewModel::GetInstance().SetLocalDicomPort(
		ui.localServerPort->text());
}

void SettingFrameWidget::on_localAETEdit_editingFinished()
{
	SettingFrameViewModel::GetInstance().SetLocalAET(
		ui.localAETEdit->text());
}

void SettingFrameWidget::on_pacsPortEdit_editingFinished()
{
	SettingFrameViewModel::GetInstance().SetPacsPort(
		ui.pacsPortEdit->text());
}

void SettingFrameWidget::on_pacsAETEdit_editingFinished()
{
	SettingFrameViewModel::GetInstance().SetPacsAET(
		ui.pacsAETEdit->text());
}

void SettingFrameWidget::on_pacsIPEdit_editingFinished()
{
	SettingFrameViewModel::GetInstance().SetPacsIP(
		ui.pacsIPEdit->text());
}

void SettingFrameWidget::on_databaseEntityNameEdit_editingFinished()
{
	SettingFrameViewModel::GetInstance().SetDatabaseServerName(
		ui.databaseEntityNameEdit->text());
}

void SettingFrameWidget::on_selectStoragePathButton_clicked()
{
	static QString lastSelectPath = "/";

	CString tmpText = "选择影像存储文件夹";

	auto filePath = QFileDialog::getExistingDirectory(
		NULL_SYMBAL,
		QString::fromLocal8Bit(tmpText.GetString(), tmpText.GetLength()),
		lastSelectPath);

	if (filePath.isEmpty())
		return;

	lastSelectPath = filePath;

	ui.storagePathEdit->setText(filePath);
}

void SettingFrameWidget::on_echoPACSButton_clicked()
{
	CRequestMessageBoxEventArgs eventArgs;

	eventArgs.SetTitleText("测试连接PACS");

	if(SettingFrameViewModel::GetInstance().TryEchoServer(
		ui.localAETEdit->text(),
		ui.pacsAETEdit->text(),
		ui.pacsIPEdit->text(),
		ui.pacsPortEdit->text()))
		eventArgs.SetMessageText("连接 PACS 成功");
	else
		eventArgs.SetMessageText("连接 PACS 失败");
	
	CMessenger::GetInstance().SendMessage(EMessengerKey::DisplayMessageBox, &eventArgs);
}
