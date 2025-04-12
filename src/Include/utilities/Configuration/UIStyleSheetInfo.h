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

#ifndef UI_STYLE_SHEET_INFO_AA0F66399EB644E2A955B8C4AE59983D
#define UI_STYLE_SHEET_INFO_AA0F66399EB644E2A955B8C4AE59983D

#pragma once


#include <Foundations/Common/ToDefine.h>
#include <utilities/Configuration/ExportFlagDefine.h>

class UTILITIESCONFIGURATION_API CUIStyleSheetInfo
{
public:
	CUIStyleSheetInfo();
	
	/*
	 * Description
	 * º”‘ÿ≈‰÷√Œƒº˛
	 * @pJsonObject Json::Value*
	 */
	void LoadFrom(void* pJsonObject);

	void SaveTo(void* pJsonObject);

private:
	AUTO_PROPERTY_REF_NO_SET_WITH_JSON_KEY(StyleSheetNode, STRING_TYPE);
	
	AUTO_PROPERTY_REF_WITH_JSON_KEY(DefaultStyleSheet, STRING_TYPE);

	AUTO_PROPERTY_REF_WITH_JSON_KEY(DefaultButtonEnableStyleSheet, STRING_TYPE);

	AUTO_PROPERTY_REF_WITH_JSON_KEY(DefaultButtonDisableStyleSheet, STRING_TYPE);

	AUTO_PROPERTY_REF_WITH_JSON_KEY(DefaultDatetimeLabelStyleSheet, STRING_TYPE);

};

#endif
