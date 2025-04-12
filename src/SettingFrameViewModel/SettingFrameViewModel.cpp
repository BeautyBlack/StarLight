#include "stdafx.h"

#include "utilities/Configuration/Configuration.h"
#include "ViewModels/SettingFrameViewModel/SettingFrameViewModel.h"

#include "utilities/DicomClient/DcmEchoSCU.h"

#include <QString>


#include "utilities/Messenger/EventArgs/UI/Station/StationNameChangedEventArgs.h"
#include "utilities/Messenger/Messenger.h"


SettingFrameViewModel SettingFrameViewModel::m_Instance;

SettingFrameViewModel::SettingFrameViewModel()
{
}

QString SettingFrameViewModel::GetStationName()
{
	return QString::fromLocal8Bit(
		CConfiguration::GetInstance().GetSystemInfo().GetApplicationName().c_str(),
		CConfiguration::GetInstance().GetSystemInfo().GetApplicationName().length());
}

void SettingFrameViewModel::SetStationName(const QString& name)
{
	auto bytes = name.toLocal8Bit();

	if (0 ==
		strcmp(bytes.data(),
			CConfiguration::GetInstance().GetSystemInfo()
			.GetApplicationName().c_str()))
		return;
	
	CConfiguration::GetInstance().GetSystemInfo().SetApplicationName(bytes.data());
	CConfiguration::GetInstance().Save();

	CStationNameChangedEventArgs eventArgs;

	eventArgs.SetStationName(CConfiguration::GetInstance().GetSystemInfo().GetApplicationName());
	
	CMessenger::GetInstance().SendMessage(EMessengerKey::StationNameChanged, &eventArgs);
}

QString SettingFrameViewModel::GetStoragePath()
{
	return QString::fromLocal8Bit(
		CConfiguration::GetInstance().GetSystemInfo().GetStoragePath().c_str(),
		CConfiguration::GetInstance().GetSystemInfo().GetStoragePath().length());
}

void SettingFrameViewModel::SetStoragePath(const QString path)
{
	auto bytes = path.toLocal8Bit();

	if (0 ==
		strcmp(bytes.data(),
			CConfiguration::GetInstance().GetSystemInfo()
			.GetStoragePath().c_str()))
		return;

	CConfiguration::GetInstance().GetSystemInfo().SetStoragePath(bytes.data());
	CConfiguration::GetInstance().Save();
}

QString SettingFrameViewModel::GetLocalAET()
{
	return QString::fromLocal8Bit(
		CConfiguration::GetInstance().GetDicomInfo().GetLocalAET().c_str(),
		CConfiguration::GetInstance().GetDicomInfo().GetLocalAET().length());
}

void SettingFrameViewModel::SetLocalAET(const QString& aet)
{
	auto bytes = aet.toLocal8Bit();

	if (0 ==
		strcmp(bytes.data(),
			CConfiguration::GetInstance().GetDicomInfo().GetLocalAET().c_str()))
		return;

	CConfiguration::GetInstance().GetDicomInfo().SetLocalAET(bytes.data());
	CConfiguration::GetInstance().Save();
}

QString SettingFrameViewModel::GetLocalDicomPort()
{
	CString strPort;

	strPort.Format("%d",
		CConfiguration::GetInstance().GetDicomInfo().GetServerPort());

	return QString::fromLocal8Bit(strPort.GetString(), strPort.GetLength());
}

void SettingFrameViewModel::SetLocalDicomPort(const QString& port)
{
	auto bytes = port.toLocal8Bit();

	auto portT = atoi(bytes.data());

	if (portT == CConfiguration::GetInstance().GetDicomInfo().GetServerPort())
		return;
	
	CConfiguration::GetInstance().GetDicomInfo().SetServerPort(portT);
	CConfiguration::GetInstance().Save();
}

QString SettingFrameViewModel::GetPacsAET()
{
	return QString::fromLocal8Bit(
		CConfiguration::GetInstance().GetDicomInfo().GetPeerAET().c_str(),
		CConfiguration::GetInstance().GetDicomInfo().GetPeerAET().length());
}

void SettingFrameViewModel::SetPacsAET(const QString& aet)
{
	auto bytes = aet.toLocal8Bit();

	if (0 ==
		strcmp(bytes.data(),
			CConfiguration::GetInstance().GetDicomInfo().GetPeerAET().c_str()))
		return;

	CConfiguration::GetInstance().GetDicomInfo().SetPeerAET(bytes.data());
	CConfiguration::GetInstance().Save();
}

QString SettingFrameViewModel::GetPacsPort()
{
	CString strPort;

	strPort.Format("%d",
		CConfiguration::GetInstance().GetDicomInfo().GetPeerPort());

	return QString::fromLocal8Bit(strPort.GetString(), strPort.GetLength());
}

void SettingFrameViewModel::SetPacsPort(const QString& port)
{
	auto bytes = port.toLocal8Bit();

	auto portT = atoi(bytes.data());

	if (portT == CConfiguration::GetInstance().GetDicomInfo().GetPeerPort())
		return;
	
	CConfiguration::GetInstance().GetDicomInfo().SetPeerPort(portT);
	CConfiguration::GetInstance().Save();
}

QString SettingFrameViewModel::GetPacsIP()
{
	return QString::fromLocal8Bit(
		CConfiguration::GetInstance().GetDicomInfo().GetPeerAddress().c_str(),
		CConfiguration::GetInstance().GetDicomInfo().GetPeerAddress().length());
}

void SettingFrameViewModel::SetPacsIP(const QString& ip)
{
	auto bytes = ip.toLocal8Bit();

	if (0 ==
		strcmp(bytes.data(),
			CConfiguration::GetInstance().GetDicomInfo().GetPeerAddress().c_str()))
		return;

	CConfiguration::GetInstance().GetDicomInfo().SetPeerAddress(bytes.data());
	CConfiguration::GetInstance().Save();
}

QString SettingFrameViewModel::GetDatabaseServerName()
{
	return QString::fromLocal8Bit(
		CConfiguration::GetInstance().GetDatabaseInfo().GetServerName().c_str(),
		CConfiguration::GetInstance().GetDatabaseInfo().GetServerName().length());

}

void SettingFrameViewModel::SetDatabaseServerName(const QString& name)
{
	auto bytes = name.toLocal8Bit();

	if (0 ==
		strcmp(bytes.data(),
			CConfiguration::GetInstance().GetDatabaseInfo().GetServerName().c_str()))
		return;

	CConfiguration::GetInstance().GetDatabaseInfo().SetServerName(bytes.data());
	CConfiguration::GetInstance().Save();
}

bool SettingFrameViewModel::TryEchoServer(
	const QString & localAET, const QString & aet,
	const QString & ip, const QString & port)
{
	auto localAETBytes = localAET.toLocal8Bit();
	auto aetBytes = aet.toLocal8Bit();
	auto ipBytes = ip.toLocal8Bit();
	auto portBytes = port.toLocal8Bit();
	
	CDcmEchoSCU scu;

	return scu.DoEcho(localAETBytes.data(),
		aetBytes.data(),
		ipBytes.data(),
		atoi(portBytes.data()));
}
