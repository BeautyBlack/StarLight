#include "pch.h"
#include "utilities/Configuration/VRColorSetting.h"

CVRColorSetting::CVRColorSetting()
{
	SetAmbient(0.5);
	SetDiffuse(0.9);
	SetSpecular(0.2);
	SetSpecularPower(10);

	double colorTrans[16 * 4] = { 0 }, opacityTrans[16 * 2] = { 0 };

	double colorFormer[] = {
	-500,0,0,0,1112,100,20,8,1716,234,139,16,2272,255,230,179,2925,255,255,230,3962,255,255,240
	};
	double opacityFormer[] = {
	0,0,480,0,1247,6,1500,0,2037,10,2597,94,3538,160,4074,175
	};
	const auto offset = 0;// 1024;

	for (int i = 0; i < sizeof(colorFormer) / sizeof(double) / 4; i++)
	{
		colorTrans[i * 4] = colorFormer[i * 4] - offset;
		colorTrans[i * 4 + 1] = (colorFormer[i * 4 + 1]);
		colorTrans[i * 4 + 2] = (colorFormer[i * 4 + 2]);
		colorTrans[i * 4 + 3] = (colorFormer[i * 4 + 3]);
	}
	
	for (int i = 0; i < sizeof(opacityFormer) / sizeof(double) / 2; i++)
	{
		opacityTrans[i * 2] = opacityFormer[i * 2] - offset;
		opacityTrans[2 * i + 1] = opacityFormer[i * 2 + 1];
	}

	for (int i = 0; i < 6; i++)
	{
		sctOpacity opacity;

		opacity.ctValue = opacityTrans[i * 2];
		opacity.opacity = opacityTrans[2 * i + 1];

		GetOpacities().push_back(opacity);
	}

	for (int i = 0; i < 6; i++)
	{
		sctColor color;

		color.ctValue = colorTrans[i * 4];

		color.r = colorTrans[i * 4 + 1];
		color.g = colorTrans[i * 4 + 2];
		color.b = colorTrans[i * 4 + 3];

		GetColors().push_back(color);
	}
}

void CVRColorSetting::LoadFrom(void* pJsonObject)
{
	const auto pJsonRoot = static_cast<Json::Value*>(pJsonObject);

	if (pJsonRoot == NULL_SYMBAL)
		return;

	const Json::Value &referenceJsonRoot = (*pJsonRoot);

	const auto vrRoot = referenceJsonRoot[GetVRColorSettingNodeJsonKey()];

	if (vrRoot.empty())
		return;

	auto ambient = vrRoot[GetAmbientJsonKey()];
	if (!ambient.empty())
		SetAmbient(ambient.asDouble());

	auto diffuse = vrRoot[GetDiffuseJsonKey()];
	if ( !diffuse.empty())
		SetDiffuse(diffuse.asDouble());

	auto spacular = vrRoot[GetSpecularJsonKey()];
	if ( !spacular.empty())
		SetSpecular(spacular.asDouble());

	auto specularPower = vrRoot[GetSpecularPowerJsonKey()];
	if ( !specularPower.empty())
		SetSpecularPower(specularPower.asDouble());

	auto colors = vrRoot[GetColorsJsonKey()];

	if( !colors.empty())
	{
		auto size = colors.size();

		GetColors().clear();
		
		for(int i = 0; i < size; i++)
		{
			Json::Value item = colors[i];

			sctColor color;

			color.ctValue = item["ctValue"].asDouble();
			color.r = item["r"].asInt();
			color.g = item["g"].asInt();
			color.b = item["b"].asInt();

			GetColors().push_back(color);
		}
	}

	auto opacity = vrRoot[GetOpacitiesJsonKey()];

	if (!opacity.empty())
	{
		auto size = opacity.size();

		GetOpacities().clear();

		for (int i = 0; i < size; i++)
		{
			Json::Value item = opacity[i];

			sctOpacity opacity;

			opacity.ctValue = item["ctValue"].asDouble();
			opacity.opacity = item["opacity"].asInt();

			GetOpacities().push_back(opacity);
		}
	}
	
}

void CVRColorSetting::SaveTo(void* pJsonObject)
{
	const auto pJsonRoot = static_cast<Json::Value*>(pJsonObject);

	if (pJsonRoot == NULL_SYMBAL)
		return;

	Json::Value &referenceJsonRoot = (*pJsonRoot);

	auto &vrRoot = referenceJsonRoot[GetVRColorSettingNodeJsonKey()];

	vrRoot[GetAmbientJsonKey()] = GetAmbient();
	vrRoot[GetDiffuseJsonKey()] = GetDiffuse();
	vrRoot[GetSpecularJsonKey()] = GetSpecular();
	vrRoot[GetSpecularPowerJsonKey()] = GetSpecularPower();

	int nIndex = 0;

	Json::Value colorArray;
	
	for(auto &it : GetColors())
	{
		Json::Value item;

		item["ctValue"] = it.ctValue;
		item["r"] = it.r;
		item["g"] = it.g;
		item["b"] = it.b;

		colorArray.insert(nIndex++, item);
	}
	vrRoot[GetColorsJsonKey()] = colorArray;

	nIndex = 0;
	Json::Value opacityArray;
	for (auto &it : GetOpacities())
	{
		Json::Value item;

		item["ctValue"] = it.ctValue;
		item["opacity"] = it.opacity;

		opacityArray.insert(nIndex++, item);
	}
	
	vrRoot[GetOpacitiesJsonKey()] = opacityArray;
}

CVRColorSetting& CVRColorSetting::operator=(const CVRColorSetting& copySrc)
{
	SetAmbient(copySrc.m_Ambient);
	SetDiffuse(copySrc.m_Diffuse);
	SetSpecular(copySrc.m_Specular);
	SetSpecularPower(copySrc.m_SpecularPower);

	SetColors(copySrc.m_Colors);
	SetOpacities(copySrc.m_Opacities);
	
	return *this;
}

