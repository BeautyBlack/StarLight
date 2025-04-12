#include "pch.h"
#include "Foundations/Common/EventGenerator.h"
#include "utilities/DicomClient/DcmStoreSCU.h"
#include "utilities/Configuration/Configuration.h"
#include "utilities/DicomClient/StoreScuBackgroundWorker.h"

CStoreScuBackgroundWorker CStoreScuBackgroundWorker::m_Instance;

CStoreScuBackgroundWorker::CStoreScuBackgroundWorker()
{
	SetWaitingThreadEvent(NULL_SYMBAL);
	SetNotifyEvent(NULL_SYMBAL);

	SetStopFlag(false);
}


CStoreScuBackgroundWorker::~CStoreScuBackgroundWorker()
{
	SetStopFlag(true);

	if(GetTaskThread().joinable())
		GetTaskThread().join();

	/*if (GetWaitingThreadEvent() != NULL_SYMBAL)
		WaitForSingleObject(GetWaitingThreadEvent(), INFINITE);*/

	CEventGenerator::ReleaseNoWatchingEvent(GetWaitingThreadEvent());
	CEventGenerator::ReleaseNoWatchingEvent(GetNotifyEvent());
}

void CStoreScuBackgroundWorker::AddTask(std::shared_ptr<CStringList> files)
{
	static bool hasInit = false;

	if( !hasInit)
	{
		CoInitialize(NULL_SYMBAL);

		InitializeCriticalSection(&GetSection());

		hasInit = true;

		SetWaitingThreadEvent(CEventGenerator::RequestNoWatchingManualEvent(true));
		SetNotifyEvent(CEventGenerator::RequestNoWatchingAutoEvent());

		m_TaskThread = std::thread(ProcessTaskThread, this);
	}

	EnterCriticalSection(&GetSection());

	GetWaitingTask().push_back(files);

	LeaveCriticalSection(&GetSection());

	SetEvent(GetNotifyEvent());
}

void CStoreScuBackgroundWorker::ProcessTaskThread(CStoreScuBackgroundWorker* pThis)
{
	if (pThis == NULL_SYMBAL)
		return;

	if (pThis->GetWaitingThreadEvent() != NULL_SYMBAL)
		ResetEvent(pThis->GetWaitingThreadEvent());

	while( !pThis->GetStopFlag())
	{
		if(WaitForSingleObject(pThis->GetNotifyEvent(), 2000) != WAIT_OBJECT_0)
			continue;

		EnterCriticalSection(&pThis->GetSection());

		TaskItemType tasks = pThis->GetWaitingTask();

		pThis->GetWaitingTask().clear();

		LeaveCriticalSection(&pThis->GetSection());

		for(auto &task : tasks)
		{
			if (pThis->GetStopFlag())
				break;

			CDcmStoreSCU scu;

			scu.StoreSCU(task.get(),
				CConfiguration::GetInstance().GetDicomInfo().GetLocalAET().c_str(),
				CConfiguration::GetInstance().GetDicomInfo().GetPeerAET().c_str(),
				CConfiguration::GetInstance().GetDicomInfo().GetPeerAddress().c_str(),
				CConfiguration::GetInstance().GetDicomInfo().GetPeerPort());
		}
	}

	if (pThis->GetWaitingThreadEvent() != NULL_SYMBAL)
		SetEvent(pThis->GetWaitingThreadEvent());
}
