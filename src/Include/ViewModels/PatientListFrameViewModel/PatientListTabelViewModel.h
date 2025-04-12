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

#ifndef PATIENT_LIST_TABLE_VIEWMODEL_D77DE99719F24AFDBEBC8D1F66E19D85
#define PATIENT_LIST_TABLE_VIEWMODEL_D77DE99719F24AFDBEBC8D1F66E19D85

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <qabstractitemmodel.h>

#include <ViewModels/PatientListFrameViewModel/patientlistframeviewmodel_global.h>

#include "Foundations/DataModule/StudyDto.h"

#include <vector>

class PATIENTLISTFRAMEVIEWMODEL_API CPatientListTabelViewModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	CPatientListTabelViewModel(QObject *pParent = NULL_SYMBAL);

	/**
	 * @brief rowCount 虚函数，重写
	 * @param parent
	 * @return  返回table的行
	 */
	Q_INVOKABLE virtual int rowCount(const QModelIndex &parent) const;
	
	/**
	 * @brief columnCount 虚函数，重写
	 * @param parent
	 * @return  返回table的列
	 */
	Q_INVOKABLE virtual int columnCount(const QModelIndex &parent) const;
	
	/**
	 * @brief data  虚函数，重写
	 * @param index
	 * @param role  控制每个Index的角色
	 * @return 返回每个Index的值
	 */
	Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	/**
	 * @brief headerData    虚函数，重写
	 * @param section       header的索引，依次递增
	 * @param orientation   横向头/纵向头
	 * @param role          每个headIndex的角色
	 * @return              返回每个headIndex的值
	 */
	Q_INVOKABLE virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public:
	static const std::map<int, int>& GetSuggestColumnWidths();

	void UpdatePatients(std::vector<CStudyDto> &studyDtos);

	CStudyDto& GetStudy(int nIndex);

private:
	AUTO_PROPERTY_REF_NO_SET(Studies, std::vector<CStudyDto>);
};



#endif

