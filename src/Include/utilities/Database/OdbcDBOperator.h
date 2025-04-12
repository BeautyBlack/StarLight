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

#ifndef ODBC_DBOPERATOR_394C347F503F40FB99756394EB18F11A
#define ODBC_DBOPERATOR_394C347F503F40FB99756394EB18F11A

#pragma once

#include <utilities/Database/IDBOperator.h>

#include <Foundations/DataModule/StudyDto.h>

#include <utilities/Database/QueryFilter.h>
#include <utilities/Database/ExportFlagDefine.h>

#include <vector>

class UTILITIESDATABASE_API COdbcDBOperator : public IDBOperator
{
public:

	COdbcDBOperator();
	
	~COdbcDBOperator();
	
	/*
	 * Description
	 * ����\���¼��
	 * @strDate ��ʽ YYYY-MM-DD
	 */
	void RecordStudy(CStudyDto &studyDto);

	/*
	 * Description
	 * ��ѯ���
	 */
	std::vector<CStudyDto> QueryStudies(CQueryFilter &filter);

	/*
	 * Description
	 * ɾ�����
	 * @strDate ��ʽ YYYY-MM-DD
	 */
	void DeleteStudy(CStudyDto &studyDto);

private:
	bool IsRecordExist(const char* strColumnName, const char* strValue);

	private:
	bool ConnectDB();

	void CloseDB();
};


#endif
