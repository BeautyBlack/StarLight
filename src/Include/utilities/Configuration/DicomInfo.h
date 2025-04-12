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

#ifndef DICOM_INFO_5F73FFC092E14F3DB94D7CFD708EE82B
#define DICOM_INFO_5F73FFC092E14F3DB94D7CFD708EE82B

#pragma once

#include <Foundations/Common/ToDefine.h>
#include <utilities/Configuration/ExportFlagDefine.h>

class UTILITIESCONFIGURATION_API CDicomInfo
{
public:
	CDicomInfo();
	
	/*
	 * Description
	 * 加载配置文件
	 * @pJsonObject Json::Value*
	 */
	void LoadFrom(void* pJsonObject);

	void SaveTo(void* pJsonObject);

private:
	AUTO_PROPERTY_REF_WITH_JSON_KEY(DicomNode, STRING_TYPE);

	//////////////  本地服务   ////////////////////
	/**
	 * 服务端口
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(ServerPort, int);

	/**
	 * LOCAL AET
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(LocalAET, STRING_TYPE);



	//////////////  第三方PACS   ////////////////////
	/**
	 * PACS AET
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(PeerAET, STRING_TYPE);

	/**
	 * PACS 端口
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(PeerPort, int);

	/**
	 * PACS 地址
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(PeerAddress, STRING_TYPE);
};

#endif
