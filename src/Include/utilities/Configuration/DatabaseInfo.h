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

#ifndef DATABASE_INFO_DA827BAB3A614FD4A87B881DE32EECD0
#define DATABASE_INFO_DA827BAB3A614FD4A87B881DE32EECD0

#pragma once

#include <utilities/Configuration/ExportFlagDefine.h>

#include <Foundations/Common/ToDefine.h>

class UTILITIESCONFIGURATION_API CDatabaseInfo
{
public:
	CDatabaseInfo();

	/*
	 * Description
	 * ���������ļ�
	 * @pJsonObject Json::Value*
	 */
	void LoadFrom(void* pJsonObject);

	void SaveTo(void* pJsonObject);
	
private:

	/**
	 * ���ݿ�ڵ�
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(DatabaseNode, STRING_TYPE);
	
	/**
	 * ʹ�ñ�����֤
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(UseLocalServer, bool);

	/**
	 * ���ݿ�ʵ����
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(ServerName, STRING_TYPE);

	/**
	 * �û���
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(UserName, STRING_TYPE);

	/**
	 * ����
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(Password, STRING_TYPE);

	/**
	 * Ĭ���������ݿ�
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(DefaultCatalogName, STRING_TYPE);

	/**
	 * ���ݿ��������ַ
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(ServerAddress, STRING_TYPE);
};

#endif
