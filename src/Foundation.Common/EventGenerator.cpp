#include "pch.h"
#include <Foundations/Common/EventGenerator.h>

CEventGenerator CEventGenerator::m_Instance;

CEventGenerator::CEventGenerator()
{
	CoInitialize(NULL_SYMBAL);

	InitializeCriticalSection(&GetSection());
}

CEventGenerator::~CEventGenerator()
{
	GetEventCache().clear();
	
	DeleteCriticalSection(&GetSection());

	CoUninitialize();
}

HANDLE CEventGenerator::RequestManualEvent(bool defaultState)
{
	Lock();
	
	for(auto &t : GetEventCache())
	{
		if( !t.isAutoEvent
			&& !t.isInUsed)
		{
			t.isInUsed = true;

			auto result = t.hEvent;

			Unlock();
			
			return result;
		}
	}

	auto result = CreateEvent(NULL_SYMBAL, TRUE, defaultState, NULL_SYMBAL);

	GetEventCache().push_back(
		sctEventItem(true, false, result));

	Unlock();

	return result;
}

HANDLE CEventGenerator::RequestAutoEvent(bool defaultState)
{
	Lock();
	
	for (auto &t : GetEventCache())
	{
		if (t.isAutoEvent
			&& !t.isInUsed)
		{
			t.isInUsed = true;

			auto result = t.hEvent;

			Unlock();

			return result;
		}
	}

	auto result = CreateEvent(NULL_SYMBAL, FALSE, defaultState, NULL_SYMBAL);

	GetEventCache().push_back(
		sctEventItem(true, true, result));

	Unlock();

	return result;
}

void CEventGenerator::ReleaseEvent(HANDLE hEvent)
{
	Lock();
	
	for (auto &t : GetEventCache())
	{
		// 技巧性代码 ： HANDLE地址值对比
		if(t.hEvent == hEvent)
		{
			t.isInUsed = false;

			break;
		}
	}

	Unlock();
}

HANDLE CEventGenerator::RequestNoWatchingManualEvent(bool defaultState)
{
	return CreateEvent(NULL_SYMBAL, TRUE, defaultState, NULL_SYMBAL);
}

HANDLE CEventGenerator::RequestNoWatchingAutoEvent(bool defaultState)
{
	return CreateEvent(NULL_SYMBAL, FALSE, defaultState, NULL_SYMBAL);
}

void CEventGenerator::ReleaseNoWatchingEvent(HANDLE hEvent)
{
	if (hEvent != NULL_SYMBAL)
		CloseHandle(hEvent);
}

void CEventGenerator::WaitForAllEvents()
{
	Lock();

	if(GetEventCache().size() == 0)
	{
		Unlock();

		return;
	}
	
	HANDLE *pWaitEvents = new HANDLE[GetEventCache().size()];

	int nIndex = 0;
	for (auto &t : GetEventCache())
	{
		if(!t.isAutoEvent)
			pWaitEvents[nIndex++] = t.hEvent;
	}

	Unlock();

	WaitForMultipleObjects(nIndex, pWaitEvents, TRUE, INFINITE);

	delete[] pWaitEvents;
}

void CEventGenerator::Lock()
{
	EnterCriticalSection(&GetSection());
}

void CEventGenerator::Unlock()
{
	LeaveCriticalSection(&GetSection());
}

