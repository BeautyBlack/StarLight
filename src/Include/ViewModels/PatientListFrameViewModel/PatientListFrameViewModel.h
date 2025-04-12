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

#ifndef PATIENT_LIST_FRAME_VIEWMODEL_EC78B6E01F21405CB2BC548930AF82C1
#define PATIENT_LIST_FRAME_VIEWMODEL_EC78B6E01F21405CB2BC548930AF82C1

#pragma once

#include <Foundations/Common/ToDefine.h>
#include <ViewModels/PatientListFrameViewModel/PatientListTabelViewModel.h>

#include <ViewModels/PatientListFrameViewModel/patientlistframeviewmodel_global.h>

#include <Utilities/Database/QueryFilter.h>

class PATIENTLISTFRAMEVIEWMODEL_API CPatientListFrameViewModel
{
public:
	void Query();
	
private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CPatientListFrameViewModel);

	AUTO_PROPERTY_REF_NO_SET(StudyFilter, CQueryFilter);

	AUTO_PROPERTY_REF_NO_SET(PatientListTableViewModel, CPatientListTabelViewModel);
};

#endif
