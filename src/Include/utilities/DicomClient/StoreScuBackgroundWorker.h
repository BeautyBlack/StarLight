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

#ifndef STORE_SCU_BACKGOUND_BB7E6295E5E549BC802040F0F273EFB4
#define STORE_SCU_BACKGOUND_BB7E6295E5E549BC802040F0F273EFB4

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <utilities/DicomClient/ExportFlagDefine.h>

#include <vector>
#include <thread>
#include <thread>

class UTILITIESDICOMCLIENT_API CStoreScuBackgroundWorker
{
public:
	CStoreScuBackgroundWorker();
	~CStoreScuBackgroundWorker();

	void AddTask(std::shared_ptr<CStringList> files);

private:
	static void ProcessTaskThread(CStoreScuBackgroundWorker*);

private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CStoreScuBackgroundWorker);

	AUTO_PROPERTY_REF_NO_SET(Section, CRITICAL_SECTION);

	using TaskItemType = std::vector<std::shared_ptr<CStringList>>;

	AUTO_PROPERTY_REF_NO_SET(WaitingTask, TaskItemType);

	AUTO_PROPERTY_REF_NO_SET(TaskThread, std::thread);

	AUTO_PROPERTY_REF(NotifyEvent, HANDLE);

	AUTO_PROPERTY_REF(WaitingThreadEvent, HANDLE);

	AUTO_PROPERTY_REF(StopFlag, bool);
};

#endif

