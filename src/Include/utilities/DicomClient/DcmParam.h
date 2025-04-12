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

#ifndef DCM_PARAM_5CB0D8EAF09A4599BCEFD92C4B704779
#define DCM_PARAM_5CB0D8EAF09A4599BCEFD92C4B704779

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <utilities/DicomClient/ExportFlagDefine.h>

class DcmDataset;

class UTILITIESDICOMCLIENT_API CDcmParam
{
public:
	CDcmParam(){}
	CDcmParam(const CDcmParam &srcCopy);
	CDcmParam& operator=(const CDcmParam &srcCopy);

	static CDcmParam Load(DcmDataset *pDataset);

	void CopyTo(DcmDataset *pDataset);

private:
	void DeepCopy(const CDcmParam &srcCopy);

private:
	// param可以有很多，等以后用到再加其他的
	AUTO_PROPERTY_REF(StudyInstanceUID, CString);
	AUTO_PROPERTY_REF(Modality, CString);
	AUTO_PROPERTY_REF(StudyDate, CString);
	AUTO_PROPERTY_REF(PatientID, CString);
	AUTO_PROPERTY_REF(AccessionNum, CString);
	AUTO_PROPERTY_REF(StudyID, CString);
	AUTO_PROPERTY_REF(PatientName, CString);
};

#ifndef UTILITIESDICOMCLIENT_EXPORTS
#pragma comment(lib,"Utilities.DicomClient.lib")
#endif

#endif
