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

#ifndef ODBC_CONNECTOR_AE4B4C0B792041498E1ED6C6F7D876FD
#define ODBC_CONNECTOR_AE4B4C0B792041498E1ED6C6F7D876FD

#pragma once

#include <Utilities/Database/IDBConnector.h>
#include <Utilities/Database/ExportFlagDefine.h>

#include <Foundations/Common/ToDefine.h>


#define MSADO15_FILE_NAME "msado15.dll"
#import MSADO15_FILE_NAME rename("EOF","adoEOF"),rename("BOF","adoBOF")
using namespace ADODB;

class UTILITIESDATABASE_API COdbcDBConnector : public IDBConnector
{

public:
	COdbcDBConnector();
	virtual ~COdbcDBConnector();

public:
	virtual bool Connect();

	virtual bool Close();

	virtual bool Execute(const char* strCommand);

private:
	AUTO_PROPERTY_REF(Connection, _ConnectionPtr);

	//AUTO_PROPERTY_REF(IsConnection, bool);
};

#endif
