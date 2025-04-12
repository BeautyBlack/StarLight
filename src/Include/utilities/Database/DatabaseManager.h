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

#ifndef DATABASE_MANAGER_A17DF6A48B274CDE957A5197C01F7BBC
#define DATABASE_MANAGER_A17DF6A48B274CDE957A5197C01F7BBC

#pragma once


#include <Foundations/DataModule/StudyDto.h>

#include <Utilities/Database/QueryFilter.h>
#include <Utilities/Database/IDBOperator.h>

#include <Foundations/Common/ToDefine.h>
#include <utilities/Database/ExportFlagDefine.h>

#include <vector>

#include <atltime.h>

class UTILITIESDATABASE_API CDatabaseManager : public IDBOperator
{
public:
	CDatabaseManager();
	~CDatabaseManager();

	/*
	 * Description
	 * 初始化
	 */
	void Initialize();
	
	/*
	 * Description
	 * 新增\更新检查
	 * @strDate 格式 YYYY-MM-DD
	 */
	void RecordStudy(CStudyDto &studyDto);

	/*
	 * Description
	 * 查询检查
	 */
	std::vector<CStudyDto> QueryStudies(CQueryFilter &filter);

	/*
	 * Description
	 * 删除检查
	 * @strDate 格式 YYYY-MM-DD
	 */
	void DeleteStudy(CStudyDto &studyDto);

private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CDatabaseManager);

	AUTO_PROPERTY_REF(Operator, IDBOperator*);
	AUTO_PROPERTY_REF_NO_SET(OperatorSection, CRITICAL_SECTION);
};

#endif
