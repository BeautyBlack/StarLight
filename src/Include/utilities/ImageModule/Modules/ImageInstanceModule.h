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

#ifndef IMAGE_INSTANCE_A4FBD7452BF7497BB53890BD4DF93201
#define IMAGE_INSTANCE_A4FBD7452BF7497BB53890BD4DF93201

#pragma once

#include <Utilities/ImageModule/Modules/ImagePlanModule.h>

#include <Foundations/Common/ToDefine.h>

#include <string>

class CSeriesModule;


#include <utilities/ImageModule/ExportFlagDefine.h>

class UTILITIESIMAGEMODULE_API CImageInstanceModule
{
public:
	CImageInstanceModule();
	~CImageInstanceModule();

private:
	/*
	 * Reference Series ¹éÊôSeries
	 */
	AUTO_PROPERTY(ReferenceSeries, std::weak_ptr<CSeriesModule>);

	/*
	 * SOP Instance UID Instance Î¨Ò»Âë
	 */
	AUTO_PROPERTY_REF(InstanceUID, STRING_TYPE);

	/*
	 * Instance Number Instance±àºÅ
	 */
	AUTO_PROPERTY(InstanceNumber, int);

	/*
	 * File Path ÎÄ¼þÂ·¾¶
	 */
	AUTO_PROPERTY_REF(FilePath, STRING_TYPE);

	AUTO_PROPERTY(ImagePlan, std::shared_ptr<CImagePlanModule>);
};

#endif

