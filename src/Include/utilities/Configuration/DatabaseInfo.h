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
	 * 加载配置文件
	 * @pJsonObject Json::Value*
	 */
	void LoadFrom(void* pJsonObject);

	void SaveTo(void* pJsonObject);
	
private:

	/**
	 * 数据库节点
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(DatabaseNode, STRING_TYPE);
	
	/**
	 * 使用本地认证
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(UseLocalServer, bool);

	/**
	 * 数据库实例名
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(ServerName, STRING_TYPE);

	/**
	 * 用户名
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(UserName, STRING_TYPE);

	/**
	 * 密码
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(Password, STRING_TYPE);

	/**
	 * 默认连接数据库
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(DefaultCatalogName, STRING_TYPE);

	/**
	 * 数据库服务器地址
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(ServerAddress, STRING_TYPE);
};

#endif
