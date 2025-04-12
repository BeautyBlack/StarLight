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
	 * ���������ļ�
	 * @pJsonObject Json::Value*
	 */
	void LoadFrom(void* pJsonObject);

	void SaveTo(void* pJsonObject);

private:
	AUTO_PROPERTY_REF_WITH_JSON_KEY(SystemNode, STRING_TYPE);

	/*
	 * ����վ����
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(ApplicationName, STRING_TYPE);

	/*
	 * �洢·��
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(StoragePath, STRING_TYPE);

	/**
	 * ��Ļ���
	 */
	AUTO_PROPERTY(ScreenWidth, int);

	/**
	 * ��Ļ�߶�
	 */
	AUTO_PROPERTY(ScreenHeight, int);
};

#endif
