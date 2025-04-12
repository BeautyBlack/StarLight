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

#ifndef VR_COLOR_SETTING_2E73600F28DF4FF68515EDE634048B4E
#define VR_COLOR_SETTING_2E73600F28DF4FF68515EDE634048B4E

#pragma once


#include <Foundations/Common/ToDefine.h>
#include <utilities/Configuration/ExportFlagDefine.h>

#include <string>
#include <vector>

struct UTILITIESCONFIGURATION_API sctColor
{
	double ctValue;
	int r;
	int b;
	int g;

	sctColor()
	{
		ctValue = r = b = g = 0;
	}
};

struct UTILITIESCONFIGURATION_API sctOpacity
{
	double ctValue;
	int opacity;

	sctOpacity()
	{
		ctValue = opacity = 0;
	}
};

class UTILITIESCONFIGURATION_API CVRColorSetting
{
public:
	CVRColorSetting();

	/*
	 * Description
	 * º”‘ÿ≈‰÷√Œƒº˛
	 * @pJsonObject Json::Value*
	 */
	void LoadFrom(void* pJsonObject);

	void SaveTo(void* pJsonObject);

	CVRColorSetting& operator=(const CVRColorSetting &copySrc);

private:
	AUTO_PROPERTY_REF_WITH_JSON_KEY(VRColorSettingNode, STRING_TYPE);
	
	AUTO_PROPERTY_REF_WITH_JSON_KEY(Ambient, double);
	AUTO_PROPERTY_REF_WITH_JSON_KEY(Diffuse, double);
	AUTO_PROPERTY_REF_WITH_JSON_KEY(Specular, double);
	AUTO_PROPERTY_REF_WITH_JSON_KEY(SpecularPower, double);

	AUTO_PROPERTY_REF_WITH_JSON_KEY(Colors, std::vector<sctColor>);
	AUTO_PROPERTY_REF_WITH_JSON_KEY(Opacities, std::vector <sctOpacity>);
};

#endif

