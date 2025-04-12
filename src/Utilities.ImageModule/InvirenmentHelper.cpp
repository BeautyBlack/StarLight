#include "stdafx.h"


#include <Utilities/Messenger/Messenger.h>
#include <utilities/ImageModule/InvirenmentHelper.h>
#include <utilities/ImageModule/DcmtkJpegRegister.h>
#include "utilities/ImageModule/Modules/StudyModule.h"
#include <combaseapi.h>

CInvirenmentHelper CInvirenmentHelper::m_Instance;

CInvirenmentHelper::CInvirenmentHelper()
{
	//SetGroupId("InvirenmentHelper.ImageModule.Utilities");
}


CInvirenmentHelper::~CInvirenmentHelper()
{
	CMessenger::GetInstance().Unregist(EMessengerKey::CreateStudyModule, this);

	CoUninitialize();
}

void CInvirenmentHelper::Initialize()
{
	CoInitializeEx(NULL_SYMBAL, COINIT_MULTITHREADED);

	WSADATA winSockData;
	WSAStartup(MAKEWORD(1, 1), &winSockData);

	CDcmtkJpegRegister::GetInstance().Initialize();

	RegistMessage();
}

void CInvirenmentHelper::RegistMessage()
{
	CMessenger::GetInstance().Regist(EMessengerKey::CreateStudyModule, this,
		[](void* pParam) -> void*
	{
		const auto strJsonText = static_cast<char*>(pParam);

		if (strJsonText == NULL_SYMBAL)
			return NULL_SYMBAL;

		CStudyModule *pStudyModule = new CStudyModule;

		pStudyModule->DeserializeFrom(strJsonText);

		return pStudyModule;
	});
}
