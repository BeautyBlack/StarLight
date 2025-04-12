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

#ifndef QUERY_FILTER_9F1922D4778B4741B964DEC80AAFD020
#define QUERY_FILTER_9F1922D4778B4741B964DEC80AAFD020

#pragma once

#include <Foundations/Common/ToDefine.h>
#include <Foundations/Interfaces/StudyReportState.h>

#include <utilities/Database/ExportFlagDefine.h>

#include <atltime.h>

/**
* ��������
*/
class UTILITIESDATABASE_API CQueryFilter
{
public:
	CQueryFilter();

private:
	/**
	 * ��������
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(PatientName, STRING_TYPE);

	/**
	 * ����
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(AccessionNum, STRING_TYPE);

	/**
	 * ����״̬
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(StudyReportState, StudyReportState);
	
	/**
	 * �������
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(DateFrom, CTime);

	/**
	 * �������
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(DateTo, CTime);


	/**
	 * �������[��Ч]
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(StudyDate, STRING_TYPE);
};

#endif
