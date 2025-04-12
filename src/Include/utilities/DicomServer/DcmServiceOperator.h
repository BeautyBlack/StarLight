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

#ifndef DCM_SERVICE_OPERATOR_8E66BFD2821D4F0FAB149647E7A9B821
#define DCM_SERVICE_OPERATOR_8E66BFD2821D4F0FAB149647E7A9B821

#pragma once


#include <utilities/DicomServer/DcmSCPBase.h>

#include <dcmtk/dcmdata/dcelem.h>

#include <vector>

UINT ProcessMsgThreadFunc(LPVOID pParam);

class UTILITIESDICOMSERVER_API CDcmServiceOperator
{
public:
	~CDcmServiceOperator();

	friend UINT ProcessMsgThreadFunc(LPVOID pParam);

public:
	// Not allow users create [CDcmServiceOperator] itself.
	// [GetInstance] will return an object -- the only one object. This Object should be deleted when no one will call it anymore.
	static CDcmServiceOperator* GetInstance(int port = 9512, const char *strImageRootPath = "",
											const char *respAETitle = "StarLight", bool bDetail = true, long PDULength = 65535,
											int iNetWaitingTimeout = 3, int iRequestTimeout = 3, int iDimseTimeout = 3,
											int iMaxEcho = 10, int iMaxStore = 10, int iMaxFind = 10, int iMaxMove = 10, int iMaxQRSelected = 10);
	virtual bool RunService();
	virtual bool StopService();

	bool GetbVerbose() { return m_bVerbose;}

	// Just close and drop association
	OFCondition CleanUp(OFCondition &cond, T_ASC_Association *assoc);

	// Receive DIMSE From Association and Process it
	OFCondition ProcessCommands(T_ASC_Association *assoc/*, LPVOID pParam*/);

	void SetServiceQuitEvent(HANDLE &hEvent) { m_hServiceQuitEvent = hEvent;}

private:
	CDcmServiceOperator(int port, const char *strImageRootPath,
		const char *respAETitle, /*const char *outputDir, */bool bDetail, long PDULength,
		int iNetWaitingTimeout, int iRequestTimeout, int iDimseTimeout,
		int iMaxEcho, int iMaxStore, int iMaxFind, int iMaxMove, int iMaxQRSelected);
	CDcmServiceOperator();

	// Waiting Peer Association And Create a new Thread to Process this Association
	OFCondition AcceptAssociation(T_ASC_Network *net);

	bool SCPTypeExist(DCM_SCP_TYPE myType);

	void ErrorLog(OFCondition &cond);

	void IncreaseThreadCount()
	{
		InterlockedIncrement(&m_runningThreadCount);
	}

	void DecreaseThreadCount()
	{
		InterlockedDecrement(&m_runningThreadCount);
	}

private:
	T_ASC_Network *m_net;

	int m_port;
	long m_maxPDULength;
	long m_netWaitingTimeout;
	long m_requestWaitingTimeout;
	long m_dimseWaitingTimeout;

	int m_maxEchoThreadCount;
	int m_maxStoreThreadCount;

	int m_maxQueryCountSelected;

	CString m_strRespondingAETitle;
	CString m_strOutputDir;

	CString m_strImageRootPath;

	OFBool m_bVerbose;

	long m_runningThreadCount; // Record how many thread has been create and that thread does not finish

	std::vector<CString> m_ProcessingAET; // Record AET Of Processing Peer AE, Use For Limit Processing Count Of A Peer AE
	std::vector<CString> m_ConnectedAET; // Record AET Of Connecting Peer AE, Use For Limit Connection Of Peer AEs With Same AET

	HANDLE m_hQuitEvent;

	HANDLE m_hStoreRecFileEndEvent;

	HANDLE m_hServiceQuitEvent;
};

#ifndef UTILITIESDICOMSERVER_EXPORTS
#pragma comment(lib, "Utilities.DicomServer.lib")
#endif

#endif

