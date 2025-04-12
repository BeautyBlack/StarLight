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

#ifndef STUDY_MODULE_LOADER_FE43DD74E7BD4DCA857651762F7DD562
#define STUDY_MODULE_LOADER_FE43DD74E7BD4DCA857651762F7DD562

#pragma once

#include <Foundations/Interfaces/AgeUnit.h>

class CStudyModule;
class DcmDataset;

class CStudyModuleLoader
{
public:
	static void Load(CStudyModule *studyModule, DcmDataset *dataset);

private:
	static void ParseAgeText(LPCTSTR strAge, int &nAge, AgeUnit &ageUnit);
};

#endif

