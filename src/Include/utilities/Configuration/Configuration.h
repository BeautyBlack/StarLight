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

#ifndef CONFIGURATION_8AB9C0BA9A9847BABE36B1C3BECE868B
#define CONFIGURATION_8AB9C0BA9A9847BABE36B1C3BECE868B

#pragma once

#include <utilities/Configuration/SystemInfo.h>
#include <utilities/Configuration/DatabaseInfo.h>
#include <utilities/Configuration/DicomInfo.h>
#include <utilities/Configuration/ExportFlagDefine.h>
#include "utilities/Configuration/UIStyleSheetInfo.h"
#include "utilities/Configuration/VRColorSetting.h"

#include <Foundations/Common/ToDefine.h>


class UTILITIESCONFIGURATION_API CConfiguration
{
public:
	CConfiguration();
	void Save();
	void Load();

private:

	
private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CConfiguration);

	AUTO_PROPERTY_REF(DefaultConfigFilePath, STRING_TYPE);

	AUTO_PROPERTY_REF(ClientConfigFilePath, STRING_TYPE);
	
	/*
	 * 系统参数
	 */
	AUTO_PROPERTY_REF_NO_SET(SystemInfo, CSystemInfo);

	/**
	 * 数据库参数
	 */
	AUTO_PROPERTY_REF_NO_SET(DatabaseInfo, CDatabaseInfo);

	/**
	 * DICOM 参数
	 */
	AUTO_PROPERTY_REF_NO_SET(DicomInfo, CDicomInfo);

	/**
	 * 界面风格参数
	 */
	AUTO_PROPERTY_REF_NO_SET(StyleSheet, CUIStyleSheetInfo);
	
	/**
	 * VR
	 */
	AUTO_PROPERTY_REF_NO_SET(VRInfo, CVRColorSetting);
};

#ifndef UTILITIESCONFIGURATION_EXPORTS
#pragma comment(lib, "Utilities.Configuration.lib")
#endif

#endif
