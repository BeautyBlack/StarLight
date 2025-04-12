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

#ifndef STATION_NAME_CHANGED_EVENT_ARGS_1A553B7BA17149C3AB9A8D7111603D27
#define STATION_NAME_CHANGED_EVENT_ARGS_1A553B7BA17149C3AB9A8D7111603D27

#pragma once

#include <utilities/Messenger/ExportFlagDefine.h>
#include <utilities/Messenger/EventArgsBase.h>

#include <Foundations/Common/ToDefine.h>

class UTILITIESMESSENGER_API CStationNameChangedEventArgs : public CEventArgsBase
{
public:
	CStationNameChangedEventArgs() = default;
	~CStationNameChangedEventArgs() = default;

private:
	AUTO_PROPERTY_REF(StationName, STRING_TYPE);
};

#endif

