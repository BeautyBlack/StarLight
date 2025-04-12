/*
 *****************************************************************
 *                 C O P Y R I G H T  (c) 2020
 *                    All Rights Reserved
 *****************************************************************
 *
 *       THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF
 *                      Zhang Jiaqi.
 *      The copyright notice above does not evidence any
 *     actual or intended publication of such source code.
 *
 *****************************************************************
 *
 *  Author: Zhang Jiaqi
 *
 */

#ifndef EVENT_GENERATOR_76ABC7BBD7C04DDD8C5E68E1A87D4D02
#define EVENT_GENERATOR_76ABC7BBD7C04DDD8C5E68E1A87D4D02

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <unordered_set>

class CEventGenerator
{
public:
	CEventGenerator();
	~CEventGenerator();
	
public:
	// 申请此Event会在本实例析构时等待Event
	HANDLE RequestManualEvent(bool defaultState = false);

	HANDLE RequestAutoEvent(bool defaultState = false);

	void ReleaseEvent(HANDLE hEvent);

	// 申请不受本实例监控的Event
	static HANDLE RequestNoWatchingManualEvent(bool defaultState = false);
	static HANDLE RequestNoWatchingAutoEvent(bool defaultState = false);
	static void ReleaseNoWatchingEvent(HANDLE hEvent);

	/**
	 * 等待所有的手动event
	 */
	void WaitForAllEvents();

private:
	void Lock();
	void Unlock();

private:
	struct sctEventItem
	{
		bool isInUsed;
		bool isAutoEvent;
		HANDLE hEvent;
		int nReferCount;

		sctEventItem()
		{
			isInUsed = false;
			isAutoEvent = false;
			hEvent = NULL_SYMBAL;
			nReferCount = 0;
		}
		
		sctEventItem(bool isInUsedT, bool isAuto, HANDLE eventHandle)
		{
			isInUsed = isInUsedT;
			isAutoEvent = isAuto;
			hEvent = eventHandle;
			nReferCount = 0;
		}

		sctEventItem(const sctEventItem& copySrc)
		{
			isInUsed = copySrc.isInUsed;
			isAutoEvent = copySrc.isAutoEvent;
			hEvent = copySrc.hEvent;
			nReferCount = copySrc.nReferCount + 1;

			auto constSrcPtr = &copySrc;

			auto srcPtr = const_cast<sctEventItem*>(constSrcPtr);
			
			srcPtr->nReferCount = nReferCount;
		}

		~sctEventItem()
		{
			--nReferCount;
			
			if (hEvent != NULL_SYMBAL
				&& nReferCount <= 0)
				CloseHandle(hEvent);

			hEvent = NULL_SYMBAL;
		}
	};
	
private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CEventGenerator);

	AUTO_PROPERTY_REF_NO_SET(EventCache, std::vector<sctEventItem>);

	AUTO_PROPERTY_REF(Section, CRITICAL_SECTION);

	AUTO_PROPERTY_REF(HasInitialized, bool);
};

#endif
