#include "pch.h"

#include <utilities/DicomServer/DcmServiceOperator.h>

#include <Foundations/Common/ServiceStopFlag.h>


#include <Foundations/Log/LogHelper.h>

#include <Shlwapi.h>

#include <WinSock2.h>

#include <dcmtk/dcmdata/dcuid.h>
#include <string>

#include "utilities/DicomServer/DicomServiceProvider.h"

// This Structure serve waiting network connection thread only
// It contains the parameter to the thread handle function
typedef struct _SCT_ProcessMsgParam_
{
	T_ASC_Association *assoc;
	CDcmServiceOperator *pOperator;

	_SCT_ProcessMsgParam_()
	{
		assoc = NULL_SYMBAL;
		pOperator = NULL_SYMBAL;
	}

	_SCT_ProcessMsgParam_& operator=(const _SCT_ProcessMsgParam_ &copy)
	{
		assoc = copy.assoc;
		pOperator = copy.pOperator;

		return *this;
	}
}sctProcessMsgParam, *lpProcessMsgParam;


CDcmServiceOperator* CDcmServiceOperator::GetInstance(int port, const char *strImageRootPath,
													   const char *respAETitle, /*const char *outputDir, */bool bDetail, long PDULength,
													   int iNetWaitingTimeout, int iRequestTimeout, int iDimseTimeout,
													   int iMaxEcho, int iMaxStore, int iMaxFind, int iMaxMove, int iMaxQRSelected)
{
	static CDcmServiceOperator* pOperator= NULL_SYMBAL;

	if(pOperator == NULL_SYMBAL)
	{
			pOperator = new CDcmServiceOperator(port, strImageRootPath, respAETitle, /*outputDir, */bDetail, PDULength,
				                                iNetWaitingTimeout, iRequestTimeout, iDimseTimeout, iMaxEcho, iMaxStore, iMaxFind,
												iMaxMove, iMaxQRSelected);
	}

	return pOperator;
}

CDcmServiceOperator::CDcmServiceOperator(int port, const char *strImageRootPath,
										 const char *respAETitle, bool bDetail, long PDULength,
										 int iNetWaitingTimeout, int iRequestTimeout, int iDimseTimeout,
										 int iMaxEcho, int iMaxStore, int iMaxFind, int iMaxMove, int iMaxQRSelected)
{
	m_strRespondingAETitle.Format("%s", respAETitle);
	m_bVerbose = bDetail;
	m_maxPDULength = PDULength > 0 ? PDULength : 0xffff;

	m_port = port;

	m_netWaitingTimeout = iNetWaitingTimeout > 0 ? iNetWaitingTimeout : 30000;
	m_requestWaitingTimeout = iRequestTimeout > 0 ? iRequestTimeout : 30000;
	m_dimseWaitingTimeout = iDimseTimeout > 0 ? iDimseTimeout : 30000;

	m_maxEchoThreadCount = iMaxEcho > 0 ? iMaxEcho : 1;
	m_maxStoreThreadCount = iMaxStore > 0 ? iMaxStore : 1;

	m_maxQueryCountSelected = iMaxQRSelected > 0 ? iMaxQRSelected : 20;

	m_net = NULL_SYMBAL;

	m_strImageRootPath.Format("%s", strImageRootPath);

	m_runningThreadCount = 0;

	m_hStoreRecFileEndEvent = CreateEvent(NULL_SYMBAL, TRUE, TRUE, NULL_SYMBAL);
	ResetEvent(m_hStoreRecFileEndEvent);
}

CDcmServiceOperator::~CDcmServiceOperator()
{
	if(m_hStoreRecFileEndEvent != NULL_SYMBAL)
		CloseHandle(m_hStoreRecFileEndEvent);

	if(m_hQuitEvent != NULL_SYMBAL)
		CloseHandle(m_hQuitEvent);
}

bool CDcmServiceOperator::RunService()
{
	CLogHelper::InfoLog( "Start Dcm-StoreSCP .");

	m_hQuitEvent = CreateEvent(NULL_SYMBAL, TRUE, TRUE, NULL_SYMBAL);
	SetEvent(m_hQuitEvent);

	CoInitialize(NULL_SYMBAL);
	
	WSADATA winSockData;
	WSAStartup(MAKEWORD(1, 1), &winSockData);

	CLogHelper::DebugLog( "Calling ASC_initializeNetwork .");

	dcmDisableGethostbyaddr.set(OFTrue);

	// Initialize network, i.e. create an instance of T_ASC_Network
	OFCondition cond = ASC_initializeNetwork(NET_ACCEPTOR, OFstatic_cast(int, m_port), m_netWaitingTimeout, &m_net);
	if(cond.bad())
	{
		ErrorLog(cond);

		return OFFalse;
	}

	ResetEvent(m_hQuitEvent);

	ResetEvent(m_hServiceQuitEvent);

	while(cond.good() && CServiceStopFlag::IsRunning())
	{
		// Receive an association and acknowledge or reject it
		// If the association is acknowledged, offer corresponding services and invoke one or more if required
		cond = AcceptAssociation(m_net);

		if(cond == DUL_NETWORKCLOSED)
		{
			CLogHelper::InfoLog("Dicom 请求接收服务 - 接收到 DUL_NETWORKCLOSED ==》 抛弃本错误，继续服务");

			cond = EC_Normal;
		}
	}

	if(cond.bad())
		CLogHelper::ErrorLog("退出Dicom请求接收服务 - %s .", cond.text());

	while(m_runningThreadCount > 0)
		Sleep(1000);

	CLogHelper::InfoLog("Dicom 请求接收服务关闭[Running 指标 - %s] --> Set Thread Event.", (CServiceStopFlag::IsRunning() ? "true" : "false"));


	SetEvent(m_hQuitEvent);
	SetEvent(m_hServiceQuitEvent);

	return OFTrue;
}

OFCondition CDcmServiceOperator::AcceptAssociation(T_ASC_Network *net)
{
	if(net == NULL_SYMBAL)
	{
		CLogHelper::DebugLog( "NULL_SYMBAL parameter.");

		return EC_Normal;
	}

	char buf[BUFSIZ] = {0};

	const char* knownAbstractSyntaxes[] =
	{
		// Verification SOP Class
		UID_VerificationSOPClass,
	};

	// We prefer explicit transfer syntaxes.
	// If we are running on a Little Endian machine we prefer LittleEndianExplicitTransferSyntax to BigEndianTransferSyntax
	const char* transferSyntaxes[] = {NULL_SYMBAL, NULL_SYMBAL, NULL_SYMBAL, NULL_SYMBAL};

	if(gLocalByteOrder == EBO_LittleEndian)
	{
		transferSyntaxes[0] = UID_LittleEndianExplicitTransferSyntax;
		transferSyntaxes[1] = UID_BigEndianExplicitTransferSyntax;
	}
	else
	{
		transferSyntaxes[0] = UID_BigEndianExplicitTransferSyntax;
		transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
	}
	transferSyntaxes[2] = UID_LittleEndianImplicitTransferSyntax;

	int numTransferSyntaxes = sizeof(transferSyntaxes) / sizeof(char*) - 1;

	// Try to receive an association. Here we either want to use blocking or non-blocking,
	// depend on if the option --eostudy-timeout is set.
	T_ASC_Association *assoc;
	OFCondition cond;

	cond = ASC_receiveAssociation(net, &assoc, m_maxPDULength, NULL_SYMBAL, NULL_SYMBAL, OFFalse, DUL_NOBLOCK, m_netWaitingTimeout/*in second*/);
	
	// Errors including no association received in no-block mode
	if(cond.bad())
	{
		if(cond == DUL_NOASSOCIATIONREQUEST)
		{
			cond = CleanUp(cond,assoc);

			Sleep(100);
		}
		else
		{
			CLogHelper::ErrorLog("接收到客户端请求 - ASC_receiveAssociation失败[%s]. ", cond.text());
			cond = CleanUp(cond, assoc);
		}

		return cond;
	}
 
	if(m_bVerbose)
		printf_s("Association Received.\n");

	// Accept the Verification SOP Class if presented
	cond = ASC_acceptContextsWithPreferredTransferSyntaxes(assoc->params, knownAbstractSyntaxes, DIM_OF(knownAbstractSyntaxes), transferSyntaxes, numTransferSyntaxes);
	if(cond.bad())
	{
		if(m_bVerbose)
			ErrorLog(cond);

		return CleanUp(cond, assoc);
	}

	// The array of Storage SOP Class UIDs comes from dcuid.h
	cond = ASC_acceptContextsWithPreferredTransferSyntaxes(
		assoc->params,
		dcmAllStorageSOPClassUIDs,
		numberOfDcmAllStorageSOPClassUIDs,
		transferSyntaxes,
		numTransferSyntaxes);
	if(cond.bad())
	{
		if(m_bVerbose)
			ErrorLog(cond);

		return CleanUp(cond, assoc);
	}

	// Set our app title
	ASC_setAPTitles(assoc->params, NULL_SYMBAL, NULL_SYMBAL, m_strRespondingAETitle);

	// Acknowledge or reject this association
	cond = ASC_getApplicationContextName(assoc->params, buf, sizeof(buf));
	if(cond.bad() || StrCmp(buf, UID_StandardApplicationContext) != 0)
	{
		// reject : the application context name is not supported.
		T_ASC_RejectParameters rej = 
		{
			ASC_RESULT_REJECTEDPERMANENT,
			ASC_SOURCE_SERVICEUSER,
			ASC_REASON_SU_APPCONTEXTNAMENOTSUPPORTED
		};

		if(m_bVerbose)
			CLogHelper::InfoLog("Association Reject : bad application context name : %s, Cond[%s] .\n", buf, cond.text());

		cond = ASC_rejectAssociation(assoc, &rej);
		if(cond.bad())
		{
			if(m_bVerbose)
				ErrorLog(cond);
		}

		return CleanUp(cond, assoc);
	}
	else
	{
		cond = ASC_acknowledgeAssociation(assoc);
		if(cond.bad())
		{
			ErrorLog(cond);

			return CleanUp(cond, assoc);
		}

		if(m_bVerbose)
		{
			if(ASC_countAcceptedPresentationContexts(assoc->params) == 0)
				CLogHelper::InfoLog("Association Acknowledged - no valid presentation contexts.\n");
		}
	}
 
	lpProcessMsgParam tempParam;
	tempParam = new sctProcessMsgParam;
	tempParam->assoc = assoc;
	tempParam->pOperator = this;
	CWinThread* lpThread;

	lpThread =  AfxBeginThread(ProcessMsgThreadFunc, tempParam, THREAD_PRIORITY_ABOVE_NORMAL, 0, CREATE_SUSPENDED);

	if(lpThread != NULL_SYMBAL)
	{
		lpThread->ResumeThread();
	}

	return EC_Normal;
}

OFCondition CDcmServiceOperator::CleanUp(OFCondition &cond, T_ASC_Association *assoc)
{
	if(cond.code() == DULC_FORKEDCHILD)
		return cond;

	cond = ASC_dropSCPAssociation(assoc);
	if(cond.bad())
	{
		ErrorLog(cond);

		return cond;
	}

	cond = ASC_destroyAssociation(&assoc);
	if(cond.bad())
	{
		ErrorLog(cond);

		return cond;
	}

	return cond;
}

// This function receives DIMSE commands over the network connection,
// and handles these commands correspondingly.
//
// Parameters:
//   assoc -- [in] The association (network connection to another DICOM application).
OFCondition CDcmServiceOperator::ProcessCommands(T_ASC_Association *assoc/*, LPVOID pParam*/)
{
	OFCondition cond = EC_Normal;
	T_DIMSE_Message msg;
	T_ASC_PresentationContextID presID = 0;
	DcmDataset *statusDetail = NULL_SYMBAL;

	DCM_SCP_TYPE eScpType = DCM_SCP_TYPE::eUnkown;

	// start a loop to be able to receive more than one DIMSE command
	while(cond == EC_Normal || cond == DIMSE_NODATAAVAILABLE)
	{
		// Receive a DIMSE command over the network
		cond = DIMSE_receiveCommand(assoc, DIMSE_NONBLOCKING, 2, &presID, &msg, &statusDetail);

		if( !CServiceStopFlag::IsRunning())
			return DIMSE_OUTOFRESOURCES;

		// If the command which was received has extra status
		// detail information, dump this information
		if(statusDetail != NULL_SYMBAL)
		{
			printf_s("Extra Status Detail : \n");

			statusDetail->print(COUT);
			delete statusDetail;
		}

		// Two Reason For [cond.bad()] is True:
		// 1. Peer AE or Local AE Close/Abort network connection
		// 2. Receive Assoc Command Error
		if (cond.bad())
			continue;

		if(cond == EC_Normal)
		{
			CDcmSCPBase *pXScp = CDicomServiceProvider::GetInstance().CreateSCP(msg);

			if(pXScp == NULL_SYMBAL)
				return DIMSE_OUTOFRESOURCES;

			pXScp->SetMyAET(m_strRespondingAETitle);
			pXScp->SetImageRootPath(m_strImageRootPath.GetString());

			pXScp->InitialDBOperation();

			cond = pXScp->ProcessCommands(assoc, msg, presID);

			pXScp->EndAchieveSCP();
			pXScp->ReleaseDBOperation();

			DCM_SCP_TYPE scpType = pXScp->GetSCPType();

			CDicomServiceProvider::GetInstance().ReleaseSCP(pXScp);

			if(scpType == DCM_SCP_TYPE::eEchoSCP)
				continue;

			return cond;
		}
	}

	return cond;
}

bool CDcmServiceOperator::StopService()
{
	HANDLE hQuit[1] = {0};

	hQuit[0] = m_hQuitEvent;

	WaitForMultipleObjects(_countof(hQuit), hQuit, TRUE, INFINITE);

	CLogHelper::DebugLog( "Calling ASC_dropNetwork .");

	OFCondition cond = ASC_dropNetwork(&m_net);
	if(cond.bad())
	{
		ErrorLog(cond);

		WSACleanup();

		return OFFalse;
	}

	WSACleanup();

	CLogHelper::InfoLog( "End Dcm-StoreSCP .");

	return true;
}

UINT ProcessMsgThreadFunc(LPVOID pParam)
{
	if(pParam == NULL_SYMBAL)
	{
		return 0;
	}

	lpProcessMsgParam waitParam = static_cast<lpProcessMsgParam>(pParam);
	CDcmServiceOperator *lpOperator = waitParam->pOperator;

	T_ASC_Association *assoc = waitParam->assoc;

	delete waitParam;
	waitParam = NULL_SYMBAL;

	if(assoc == NULL_SYMBAL)
	{
		return 0;
	}

	lpOperator->IncreaseThreadCount();

	OFCondition cond;

	cond = lpOperator->ProcessCommands(assoc);

	if(cond == DUL_PEERREQUESTEDRELEASE)
	{
		if(lpOperator->GetbVerbose())
			printf_s("Association Release.\n");

		cond = ASC_acknowledgeRelease(assoc);

		CLogHelper::DebugLog("Association Release .");
	}
	else if(cond == DUL_PEERABORTEDASSOCIATION)
	{
		if(lpOperator->GetbVerbose())
			printf_s("Association Aborted.\n");

		cond = ASC_abortAssociation(assoc);

		CLogHelper::DebugLog("Association Aborted .");
	}
	else
	{
		fprintf_s(stderr, "DIMSE Failure (aborting association).\n");

		// some kind of error so abort the association
		cond = ASC_abortAssociation(assoc);

		CLogHelper::DebugLog("Association Aborted By unkown Reason .");
	}

	lpOperator->CleanUp(cond, assoc);
	printf_s("Thread Exist .\n");

	lpOperator->DecreaseThreadCount();

	return 0;
}

void CDcmServiceOperator::ErrorLog(OFCondition &cond)
{
	CLogHelper::ErrorLog( "%s .", cond.text());
}

