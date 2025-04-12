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

#ifndef DCM_STORE_SCP_DB35B23886334A5093CCA449E8CC5E7D
#define DCM_STORE_SCP_DB35B23886334A5093CCA449E8CC5E7D

#pragma once

#include <utilities/DicomServer/DcmSCPBase.h>

#include "Foundations/DataModule/StudyDto.h"


class UTILITIESDICOMSERVER_API CDcmStoreSCP : public CDcmSCPBase
{
public:
	CDcmStoreSCP(void);
	~CDcmStoreSCP(void);

public:
	virtual OFCondition AchieveSCP(T_ASC_Association *assoc, T_DIMSE_Message *msg, T_ASC_PresentationContextID presID/*, LPVOID pParam*/);

	virtual bool IsAcceptedMsgType(T_DIMSE_Command msgType);
	virtual bool Release();

	virtual bool IsReachMyMaxThreadCount() { return m_nStaticCurClientCount >= m_nStaticMaxClientCount;}
	virtual void SetMaxThreadCount(const int iMax) { m_nStaticMaxClientCount = iMax > 0 ? iMax : 1;}
	virtual void IncreaseCurThreadCount() { InterlockedIncrement(&m_nStaticCurClientCount);}
	virtual void DecreaseCurThreadCount() { InterlockedDecrement(&m_nStaticCurClientCount);}

protected:
	static void StoreSCPCallback(void *callbackData, T_DIMSE_StoreProgress *progress, T_DIMSE_C_StoreRQ *req, char *imageFileName,
		                         DcmDataset **imageDataSet, T_DIMSE_C_StoreRSP *rsp, DcmDataset **statusDetail);

	static CStudyDto DatasetToStudyDto(DcmDataset *pDataset);
	
	// association中的对方AET
	char m_strCallingAET[128];

	CStringList m_strStudyFileList; // 接收到的图像文件保存路径

	CMapStringToString m_mapAetToIP;

	static long m_nStaticMaxClientCount;
	static long m_nStaticCurClientCount;

};
#endif