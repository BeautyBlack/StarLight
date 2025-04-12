#include "pch.h"
#include "utilities/DicomClient/DcmEchoSCU.h"


#include <Foundations/Log/LogHelper.h>

#include <WinSock2.h>

#include <dcmtk/dcmnet/assoc.h>
#include <dcmtk/dcmnet/cond.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmsr/dsrdoc.h>

CDcmEchoSCU::CDcmEchoSCU(void)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(1,1), &wsaData);

	m_assoc = NULL_SYMBAL;
	m_net = NULL_SYMBAL;
	m_param = NULL_SYMBAL;

	ZeroMemory(m_strOurAET, sizeof(m_strOurAET));
}

CDcmEchoSCU::~CDcmEchoSCU(void)
{
	if(m_assoc != NULL_SYMBAL)
	{
		ASC_releaseAssociation(static_cast<T_ASC_Association*>(m_assoc));
		ASC_dropAssociation(static_cast<T_ASC_Association*>(m_assoc));
		ASC_destroyAssociation((T_ASC_Association**)(&m_assoc));
		m_assoc = NULL_SYMBAL;

		CLogHelper::InfoLog("Store SCU Release Association.");
	}

	// 注意：@m_param不需要手动delete, @m_assoc释放时会释放@m_param

	if(m_net != NULL_SYMBAL)
	{
		ASC_dropNetwork((T_ASC_Network**)(&m_net));
		m_net = NULL_SYMBAL;
	}

	WSACleanup();
}

bool CDcmEchoSCU::DoEcho(LPCTSTR pStrOurAET, LPCTSTR pStrPeerAET, LPCTSTR pStrPeerIP, int nPeerPort)
{
	if(pStrOurAET == NULL_SYMBAL || pStrPeerAET == NULL_SYMBAL || pStrPeerIP == NULL_SYMBAL || nPeerPort == 0)
		return false;

	if(BuildSubAssociation(pStrOurAET, pStrPeerAET, pStrPeerIP, nPeerPort) < 0)
		return false;

	DIC_US msgId = static_cast<T_ASC_Association*>(m_assoc)->nextMsgID++;
	DIC_US status;
	DcmDataset *statusDetail = NULL_SYMBAL;

	CLogHelper::InfoLog("Sending Echo Request: MsgID[%d] ", msgId);

	OFCondition cond = DIMSE_echoUser(static_cast<T_ASC_Association*>(m_assoc), msgId, DIMSE_BLOCKING, 2, &status, &statusDetail);

	if (cond.bad())
		CLogHelper::ErrorLog("发送Echo失败 - %s", cond.text());

	if (statusDetail != NULL_SYMBAL)
		delete statusDetail;

	CLogHelper::InfoLog("Echo - AET[%s], PeerAET[%s], PeerIP[%s], Port[%d] : %s",
		pStrOurAET, pStrPeerAET, pStrPeerIP, nPeerPort,
		cond.good() ? "成功" : "失败");

	return cond.good();
}

int CDcmEchoSCU::BuildSubAssociation(LPCTSTR pStrOurAET, LPCTSTR pStrPeerAET, LPCTSTR pStrPeerIP, int nPeerPort)
{
	DIC_NODENAME localHostName;
	DIC_NODENAME dstHostNamePlusPort;

	OFCondition cond = ASC_initializeNetwork(NET_REQUESTOR, OFstatic_cast(int, 0), 30000, (T_ASC_Network**)(&m_net));
	if(cond.bad())
		return -1;

	cond = ASC_createAssociationParameters((T_ASC_Parameters**)(&m_param), ASC_DEFAULTMAXPDU);
	if(cond.bad())
	{
		ASC_dropNetwork((T_ASC_Network**)(&m_net));

		m_net = NULL_SYMBAL;
		return -2;
	}

	gethostname(localHostName, sizeof(localHostName)-1);

	_snprintf_s(dstHostNamePlusPort, sizeof(dstHostNamePlusPort), _TRUNCATE, "%s:%d", pStrPeerIP, nPeerPort);

	ASC_setPresentationAddresses(static_cast<T_ASC_Parameters*>(m_param), localHostName, dstHostNamePlusPort);
	ASC_setAPTitles(static_cast<T_ASC_Parameters*>(m_param), pStrOurAET, pStrPeerAET, NULL_SYMBAL);
	ASC_setTransportLayerType(static_cast<T_ASC_Parameters*>(m_param), false);

	if( !AddAllStoragePresentationContexts(static_cast<T_ASC_Parameters*>(m_param)))
	{
		ASC_destroyAssociationParameters((T_ASC_Parameters**)(&m_param));
		m_param = NULL_SYMBAL;

		ASC_dropNetwork((T_ASC_Network**)(&m_net));
		m_net = NULL_SYMBAL;

		return -3;
	}

	cond = ASC_requestAssociation(static_cast<T_ASC_Network*>(m_net), static_cast<T_ASC_Parameters*>(m_param), (T_ASC_Association**)(&m_assoc));
	if( !cond.good())
	{
		if(cond == DUL_ASSOCIATIONREJECTED)
		{
			T_ASC_RejectParameters rej;
			std::ostringstream strStream;

			ASC_getRejectParameters(static_cast<T_ASC_Parameters*>(m_param), &rej);

			ASC_printRejectParameters(strStream, &rej);

#ifdef PRINT_INFO_TO_CONSOLE
			printf_s("%s \n", strStream.str());
#endif

			CLogHelper::ErrorLog("C-Echo Request : Sub Association Rejected - %s. ", strStream.str().c_str());

			OutputDebugString("\nC-Echo Request : Sub Association Rejected - ");
			OutputDebugString(strStream.str().c_str());
		}
		else
		{
			CLogHelper::ErrorLog("C-Echo Request : Sub Association Failed; Codition Detail - %s. ", cond.text());

			OutputDebugString("\nC-Echo Request : Sub Association Failed; Codition Detail - ");
			OutputDebugString(cond.text());
		}

		ASC_abortAssociation(static_cast<T_ASC_Association*>(m_assoc));
		ASC_dropAssociation(static_cast<T_ASC_Association*>(m_assoc));
		ASC_destroyAssociation((T_ASC_Association**)(&m_assoc));
		m_assoc = NULL_SYMBAL;

		m_param = NULL_SYMBAL;

		ASC_dropNetwork((T_ASC_Network**)(&m_net));
		m_net = NULL_SYMBAL;

		return -4;
	}

	// No Presentation Contexts Accepted by SCP
	if(ASC_countAcceptedPresentationContexts(static_cast<T_ASC_Parameters*>(m_param)) == 0)
		return -5;

	return 0;
}


bool CDcmEchoSCU::AddAllStoragePresentationContexts(void *_params)
{
	if(_params == NULL_SYMBAL)
		return false;

	T_ASC_Parameters *params = (T_ASC_Parameters *)_params;

	int pid = 1;

	const char* transferSyntaxes[] = { NULL_SYMBAL, NULL_SYMBAL, NULL_SYMBAL, NULL_SYMBAL };
	int numTransferSyntaxes = 0;

	if (gLocalByteOrder == EBO_LittleEndian)  /* defined in dcxfer.h */
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
	numTransferSyntaxes = 3;

	ASC_addPresentationContext(params, pid, UID_VerificationSOPClass, transferSyntaxes, numTransferSyntaxes);

	return true;
}

