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

#ifndef SETTING_FRAME_VIEWMODEL_E630AF319F15491CA8A273F68081C8CB
#define SETTING_FRAME_VIEWMODEL_E630AF319F15491CA8A273F68081C8CB

#pragma once

#include <Foundations/Common/ToDefine.h>

#include "settingframeviewmodel_global.h"

#include <QString>

class SETTINGFRAMEVIEWMODEL_API SettingFrameViewModel
{
public:
    SettingFrameViewModel();

	QString GetStationName();
	void SetStationName(const QString& name);

	QString GetStoragePath();
	void SetStoragePath(const QString path);

	QString GetLocalAET();
	void SetLocalAET(const QString& aet);

	QString GetLocalDicomPort();
	void SetLocalDicomPort(const QString& nPort);

	QString GetPacsAET();
	void SetPacsAET(const QString& aet);

	QString GetPacsPort();
	void SetPacsPort(const QString& port);
	
	QString GetPacsIP();
	void SetPacsIP(const QString& aet);

	QString GetDatabaseServerName();
	void SetDatabaseServerName(const QString& name);
	
	bool TryEchoServer(const QString &localAET, const QString &aet, const QString &ip, const QString &port);

private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, SettingFrameViewModel);

};

#endif
