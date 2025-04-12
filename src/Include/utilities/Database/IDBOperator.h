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

#ifndef IDB_OPERATOR_19C0BB604145482CAA8219FA6752A028
#define IDB_OPERATOR_19C0BB604145482CAA8219FA6752A028

#pragma once

#include <Foundations/DataModule/StudyDto.h>
#include <utilities/Database/QueryFilter.h>

#include <utilities/Database/IDBConnector.h>
#include <utilities/Database/ExportFlagDefine.h>

#include <vector>

class UTILITIESDATABASE_API __declspec(novtable)  IDBOperator
{
public:
	IDBOperator();
	
	/*
	 * Description
	 * 新增\更新检查
	 * @strDate 格式 YYYY-MM-DD
	 */
	virtual void RecordStudy(CStudyDto &studyDto) = 0;

	/*
	 * Description
	 * 查询检查
	 */
	virtual std::vector<CStudyDto> QueryStudies(CQueryFilter &filter) = 0;

	/*
	 * Description
	 * 删除检查
	 * @strDate 格式 YYYY-MM-DD
	 */
	virtual void DeleteStudy(CStudyDto &studyDto) = 0;

private:
	AUTO_PROPERTY_REF(DBConnection, IDBConnector*);
};

inline IDBOperator::IDBOperator()
{
	SetDBConnection(NULL_SYMBAL);
}

#endif
