// SystemServer.StarLightServer.cpp : Implementation of WinMain


#include "pch.h"

//#include <winsvc.h>

#include "Foundations/Common/ServiceStopFlag.h"
#include "Foundations/Log/LogHelper.h"
#include "framework.h"
#include "resource.h"
#include "SystemServerStarLightServer_i.h"

//#include <atlbase.h>
//using namespace ATL;

#include <stdio.h>


#include "utilities/Configuration/Configuration.h"
#include "utilities/DicomServer/DcmServiceOperator.h"

class CSystemServerStarLightServerModule
	: public ATL::CAtlServiceModuleT< CSystemServerStarLightServerModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_SystemServerStarLightServerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SYSTEMSERVERSTARLIGHTSERVER, "{41f8efb4-8d83-4495-b543-c6fb490c5d1b}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for your service
		// Suggested - PKT Level Authentication,
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY
		// and an appropriate non-null Security Descriptor.

		return S_OK;
	}

public:
	HRESULT RegisterAppId(bool bService = false); // Begin Initial Win Server
	HRESULT PreMessageLoop(int nShowCmd); // Begin Setup Win Server Msg loop
	HRESULT PostMessageLoop();

	static UINT StartWorkingThread(LPVOID lpParam);
	
	HANDLE m_startWorkingEvent;

	CDcmServiceOperator *m_operator;
};

CSystemServerStarLightServerModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}


HRESULT CSystemServerStarLightServerModule::RegisterAppId(bool bService) throw()
{
	HRESULT hr = S_OK;

	char strConfigPath[MAX_PATH] = { 0 };
	char strDesc[128] = { 0 };
	char strName[64] = { 0 };
	char strSession[64] = { 0 };

	char strRunningServiceDir[512] = { 0 };

	SERVICE_DESCRIPTION Description;
	TCHAR szDescription[128] = { 0 };

	ZeroMemory(&Description, sizeof(Description));

	lstrcpy(szDescription, _T("StarLight Service"));
	Description.lpDescription = szDescription;

	CLogHelper::InitializeLogger("StarLight");

	if (strlen(strName) > 0 && strcmp(strName, m_szServiceName) != 0)
	{
		_snprintf_s(m_szServiceName, sizeof(m_szServiceName), _TRUNCATE, "%s", strName);

		if (strlen(strDesc) > 0)
			_snprintf_s(szDescription, sizeof(szDescription), _TRUNCATE, "%s", strDesc);
	}

	BOOL res = __super::RegisterAppId(bService);

	if (bService)
	{
		CLogHelper::InfoLog("%s - Service Name[%s],   Service Description[%s]. ", __FUNCTION__, m_szServiceName, szDescription);
		CLogHelper::InfoLog("%s - Uninstall Service. ", __FUNCTION__);
		Uninstall();

		CLogHelper::InfoLog("%s - Install Service. ", __FUNCTION__);
		Install();

		if (IsInstalled())
		{
			CLogHelper::InfoLog("%s - Modify Service Configuration. New Service Name - %s. ", __FUNCTION__, m_szServiceName);

			SC_HANDLE hSCM = ::OpenSCManagerW(NULL_SYMBAL, NULL_SYMBAL, SERVICE_CHANGE_CONFIG);
			SC_HANDLE hService = NULL_SYMBAL;

			if (hSCM == NULL_SYMBAL)
				hr = AtlHresultFromLastError();
			else
			{
				hService = ::OpenService(hSCM, m_szServiceName, SERVICE_CHANGE_CONFIG);

				if (hService != NULL_SYMBAL)
				{
					::ChangeServiceConfig(hService, SERVICE_NO_CHANGE, SERVICE_AUTO_START,
						0, NULL_SYMBAL, NULL_SYMBAL, NULL_SYMBAL, NULL_SYMBAL, NULL_SYMBAL, NULL_SYMBAL, m_szServiceName);

					::ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &Description);
					::CloseServiceHandle(hService);
				}
				else
				{
					hr = AtlHresultFromLastError();
				}

				::CloseServiceHandle(hSCM);
			}
		}
		else
		{
			CLogHelper::InfoLog("%s - Install Service Failed. ", __FUNCTION__);
		}
	}

	return hr;
}


HRESULT CSystemServerStarLightServerModule::PreMessageLoop(int nShowCmd) throw()
{
	HRESULT hr = __super::PreMessageLoop(nShowCmd);

	if (hr == S_FALSE)
		hr = S_OK;

	//GetCurrentDirectory(sizeof(m_strDefaultServiceDir), m_strDefaultServiceDir);

	if (SUCCEEDED(hr))
	{
		m_operator = NULL_SYMBAL;
		
		char strRunningServiceDir[MAX_PATH] = { 0 };

		GetModuleFileName(NULL_SYMBAL, strRunningServiceDir, sizeof(strRunningServiceDir));
		PathRemoveFileSpec(strRunningServiceDir);

		SetCurrentDirectory(strRunningServiceDir);

		::CoInitialize(NULL_SYMBAL);

		//char strConfigPath[MAX_PATH] = { 0 };
		////char strAppDataDir[MAX_PATH] = {0};

		////PathCombine(strAppDataDir, strRunningServiceDir, "..\\AppData");
		//PathCombine(strConfigPath, strRunningServiceDir, CONFIG_RELATE_FILE_PATH);

		//if (!CConfigMgr::GetInstance().Initialize(strConfigPath))
		//	return S_OK;

		CLogHelper::InitializeLogger("StarLight");

		CLogHelper::InfoLog("初始化日志");

		m_startWorkingEvent = CreateEvent(NULL_SYMBAL, TRUE, TRUE, NULL_SYMBAL);

		AfxBeginThread(StartWorkingThread, this);

		CLogHelper::InfoLog("开启工作线程");
	}

	CLogHelper::InfoLog("初始化完成");

	return S_OK;
}

UINT CSystemServerStarLightServerModule::StartWorkingThread(LPVOID lpParam)
{
	CSystemServerStarLightServerModule *pThis = (CSystemServerStarLightServerModule*)lpParam;

	ResetEvent(pThis->m_startWorkingEvent);

#ifndef _DEBUG
	for (int i = 0; i < 30 && CServiceStopFlag::IsRunning(); i++)
		Sleep(2000);
#endif

	if (!CServiceStopFlag::IsRunning())
	{
		SetEvent(pThis->m_startWorkingEvent);
		return 0;
	}

	auto dcmServerOperator = CDcmServiceOperator::GetInstance(
		CConfiguration::GetInstance().GetDicomInfo().GetServerPort(),
		CConfiguration::GetInstance().GetSystemInfo().GetStoragePath().c_str(),
		CConfiguration::GetInstance().GetDicomInfo().GetLocalAET().c_str()
	);

	if (dcmServerOperator != NULL_SYMBAL)
		dcmServerOperator->RunService();
	
	SetEvent(pThis->m_startWorkingEvent);

	return 0;
}

HRESULT CSystemServerStarLightServerModule::PostMessageLoop()
{
	CServiceStopFlag::StopRunning();

	CLogHelper::InfoLog("关闭Hubit子处理程序.");

	WaitForSingleObject(m_startWorkingEvent, INFINITE);
	CloseHandle(m_startWorkingEvent);

	return __super::PostMessageLoop();
}

