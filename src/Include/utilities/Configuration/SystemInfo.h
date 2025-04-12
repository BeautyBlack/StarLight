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

#ifndef SYSTEM_INFO_A39FC5E22F6E4608B4134924B80B35F9
#define SYSTEM_INFO_A39FC5E22F6E4608B4134924B80B35F9

#pragma once

#include <Foundations/Common/ToDefine.h>
#include <utilities/Configuration/ExportFlagDefine.h>

#include <string>

class UTILITIESCONFIGURATION_API CSystemInfo
{
public:
	CSystemInfo();
	
	/*
	 * Description
	 * 加载配置文件
	 * @pJsonObject Json::Value*
	 */
	void LoadFrom(void* pJsonObject);

	void SaveTo(void* pJsonObject);

private:
	AUTO_PROPERTY_REF_WITH_JSON_KEY(SystemNode, STRING_TYPE);

	/*
	 * 工作站名称
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(ApplicationName, STRING_TYPE);

	/*
	 * 存储路径
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(StoragePath, STRING_TYPE);

	/**
	 * 屏幕宽度
	 */
	AUTO_PROPERTY(ScreenWidth, int);

	/**
	 * 屏幕高度
	 */
	AUTO_PROPERTY(ScreenHeight, int);
};

#endif
