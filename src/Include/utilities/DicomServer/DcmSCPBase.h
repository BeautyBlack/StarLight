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

#ifndef DCM_SCP_BASE_5A059C3DD5414EBAA3825E4A43BA289C
#define DCM_SCP_BASE_5A059C3DD5414EBAA3825E4A43BA289C


#pragma once

#include <Foundations/Common/ToDefine.h>

#include <utilities/DicomServer/ExportFlagDefine.h>

#include <dcmtk/dcmnet/assoc.h>
#include <dcmtk/dcmnet/dimse.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/ofstd/ofcond.h>

#include <afxtempl.h>

enum class UTILITIESDICOMSERVER_API DCM_SCP_TYPE
{
	// 不可用
	eUnkown = 0,

	// C-Echo SCP
	eEchoSCP = 1,

	// C-Store SCP
	eStoreSCP = 2,

	// 不可用
	eAllSCP = 255
};


class UTILITIESDICOMSERVER_API __declspec(novtable) CDcmSCPBase
{
public:
	CDcmSCPBase();
	virtual ~CDcmSCPBase();

public:
	// [GetInstance] will return the only one [CDcmSCPBase*],according to the certain type.
	// Note : Do Not Try To Delete This Pointer Unless No Other Comment Will Call it.
	//static CDcmSCPBase* GetInstance(DCM_SCP_TYPE eMyType);
	virtual OFCondition AchieveSCP(T_ASC_Association *assoc, T_DIMSE_Message *msg, T_ASC_PresentationContextID presID/*, LPVOID pParam*/) = 0;
	virtual void EndAchieveSCP() {}

	virtual bool IsAcceptedMsgType(T_DIMSE_Command msgType) = 0;
	virtual bool Release() = 0;
	virtual void InitialDBOperation() {}
	virtual void ReleaseDBOperation() {}

	virtual DCM_SCP_TYPE GetSCPType() const { return m_myType; }

	void SetStopFlag(OFBool *bStopFlag) { m_bStopFlag = bStopFlag; }

	void SetMyAET(LPCTSTR strAET) { _snprintf_s(m_strMyAET, sizeof(m_strMyAET), _TRUNCATE, "%s", strAET); }
	const char* GetMyAET() { return m_strMyAET; }

	virtual bool IsReachMyMaxThreadCount() { return m_curThreadCount >= m_maxThreadCount; }
	virtual void SetMaxThreadCount(const int iMax) { m_maxThreadCount = iMax > 0 ? iMax : 1; }
	virtual void IncreaseCurThreadCount() { InterlockedIncrement(&m_curThreadCount); }
	virtual void DecreaseCurThreadCount() { InterlockedDecrement(&m_curThreadCount); }

	static void SetNetSocketWaitingTimeout(const int lTimeout) { m_netWaitingTimeout = lTimeout > 0 ? lTimeout : 30000; }
	static int GetNetSocketWaitingTimeout() { return m_netWaitingTimeout; }

	// 处理DICOM 消息
	virtual OFCondition ProcessCommands(T_ASC_Association *assoc, T_DIMSE_Message &msg, T_ASC_PresentationContextID &presID);

protected:
	DCM_SCP_TYPE m_myType;
	char m_strMyAET[MAX_PATH];

	long m_curThreadCount;
	long m_maxThreadCount;

	static int m_netWaitingTimeout; // Net Socket Waiting Timeout
	static int m_requestWaitingTimeout; // DCMTK Association Request Waiting Timeout
	static int m_dimseWaitingTimeout; // DCMTK DIMSE Waiting Timeout

	typedef CMap<Uint32, Uint32, LPCTSTR, LPCTSTR>* DcmKeyMapPtr;
	DcmKeyMapPtr m_pSupportDcmTagMap;

	OFBool *m_bStopFlag;

	AUTO_PROPERTY_REF(ImageRootPath, CString);
};


#endif 
