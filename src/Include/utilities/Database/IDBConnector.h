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

#pragma once

#ifndef CONNECTOR_H_8FB674706BB1442B98205F46E18925A
#define CONNECTOR_H_8FB674706BB1442B98205F46E18925A

#include <Foundations/Common/ToDefine.h>
#include <utilities/Database/ExportFlagDefine.h>

class UTILITIESDATABASE_API __declspec(novtable) IDBConnector
{
public:
	virtual ~IDBConnector() = default;

public:
	virtual bool Connect() { return false; }

	virtual bool Close() { return true; }

	virtual bool Execute(const char* strCommand) = 0;
	
private:
	/**
	 * 是否已连接数据库
	 */
	AUTO_PROPERTY(IsConnected, bool);
	
	/**
	 * 使用本地认证
	 */
	AUTO_PROPERTY(UseLocalServer, bool);

	/**
	 * 数据库实例名
	 */
	AUTO_PROPERTY_REF(ServerName, STRING_TYPE);

	/**
	 * 用户名
	 */
	AUTO_PROPERTY_REF(UserName, STRING_TYPE);

	/**
	 * 密码
	 */
	AUTO_PROPERTY_REF(Password, STRING_TYPE);

	/**
	 * 默认连接数据库
	 */
	AUTO_PROPERTY_REF(DefaultCatalogName, STRING_TYPE);

	/**
	 * 数据库服务器地址
	 */
	AUTO_PROPERTY_REF(ServerAddress, STRING_TYPE);
};

#endif

