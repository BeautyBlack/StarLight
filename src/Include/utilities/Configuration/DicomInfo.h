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
	 * ���������ļ�
	 * @pJsonObject Json::Value*
	 */
	void LoadFrom(void* pJsonObject);

	void SaveTo(void* pJsonObject);

private:
	AUTO_PROPERTY_REF_WITH_JSON_KEY(DicomNode, STRING_TYPE);

	//////////////  ���ط���   ////////////////////
	/**
	 * ����˿�
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(ServerPort, int);

	/**
	 * LOCAL AET
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(LocalAET, STRING_TYPE);



	//////////////  ������PACS   ////////////////////
	/**
	 * PACS AET
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(PeerAET, STRING_TYPE);

	/**
	 * PACS �˿�
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(PeerPort, int);

	/**
	 * PACS ��ַ
	 */
	AUTO_PROPERTY_REF_WITH_JSON_KEY(PeerAddress, STRING_TYPE);
};

#endif
