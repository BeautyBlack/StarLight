#include "pch.h"
#include "utilities/DicomClient/DcmSCUBase.h"


#include <WinSock2.h>

#include <Shlwapi.h>
#include <ImageHlp.h>

#include <dcmtk/dcmnet/assoc.h>
#include <dcmtk/dcmnet/cond.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmsr/dsrdoc.h>

#include "Foundations/Log/LogHelper.h"

CDcmSCUBase::CDcmSCUBase(void)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(1,1), &wsaData);

	m_assoc = NULL;
	m_net = NULL;
	m_param = NULL;

	SetHasBuildAssociation(false);
}

CDcmSCUBase::~CDcmSCUBase(void)
{
	if(m_assoc != NULL)
	{
		ASC_releaseAssociation(static_cast<T_ASC_Association*>(m_assoc));
		ASC_dropAssociation(static_cast<T_ASC_Association*>(m_assoc));
		ASC_destroyAssociation((T_ASC_Association**)(&m_assoc));
		m_assoc = NULL;

		CLogHelper::InfoLog("Store SCU Release Association.");
	}

	// 注意：@m_param不需要手动delete, @m_assoc释放时会释放@m_param

	if(m_net != NULL)
	{
		ASC_dropNetwork((T_ASC_Network**)(&m_net));
		m_net = NULL;
	}

	WSACleanup();
}

bool CDcmSCUBase::Process(CDcmParam &param)
{
	if(GetMyAET().IsEmpty() || GetPeerAET().IsEmpty() || GetPeerIP().IsEmpty() || GetPeerPort() <= 0)
		return false;

	const char *strClassUID = NULL;
	const char *strInstanceUID = NULL;

	DIC_US msgID = 1;
	T_ASC_PresentationContextID presID = 1;
	DcmDataset *storeDetail = NULL;

	int nSuccessCount = 0;

	if( !GetHasBuildAssociation())
	{
		OFCondition cond;

		int nRet = BuildSubAssociation();

		if(nRet < 0)
			return false;

		SetHasBuildAssociation(true);
	}

	return ProcessInternal(param);
}

bool CDcmSCUBase::AddPresentationContexts(void *_params)
{
	if(_params == NULL)
		return false;

	T_ASC_Parameters *params = (T_ASC_Parameters *)_params;

	int i;
	int pid = 1;

	const char* transferSyntaxes[] = { NULL, NULL, NULL, NULL, NULL };
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

	transferSyntaxes[3] = UID_JPEGProcess14SV1TransferSyntax;
	numTransferSyntaxes = 4;

	for (i=0; i< m_nAbstractSyntaxCount; i++)
	{
		if(ASC_addPresentationContext(params, pid, m_pAbstractSyntaxes[i], transferSyntaxes, numTransferSyntaxes).bad())
			return false;

		pid += 2;	/* only odd presentation context id's */
	}


	return true;
}

int CDcmSCUBase::BuildSubAssociation()
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

		m_net = NULL;
		return -2;
	}

	gethostname(localHostName, sizeof(localHostName)-1);

	_snprintf_s(dstHostNamePlusPort, sizeof(dstHostNamePlusPort), _TRUNCATE, "%s:%d", GetPeerIP().GetString(), GetPeerPort());

	ASC_setPresentationAddresses(static_cast<T_ASC_Parameters*>(m_param), localHostName, dstHostNamePlusPort);
	ASC_setAPTitles(static_cast<T_ASC_Parameters*>(m_param), GetMyAET(), GetPeerAET(), NULL);
	ASC_setTransportLayerType(static_cast<T_ASC_Parameters*>(m_param), false);

	if( !AddPresentationContexts(static_cast<T_ASC_Parameters*>(m_param)))
	{
		ASC_destroyAssociationParameters((T_ASC_Parameters**)(&m_param));
		m_param = NULL;

		ASC_dropNetwork((T_ASC_Network**)(&m_net));
		m_net = NULL;

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

			CLogHelper::ErrorLog("C-Store Request : Sub Association Rejected - %s. ", strStream.str().c_str());

			OutputDebugString("\nC-Store Request : Sub Association Rejected - ");
			OutputDebugString(strStream.str().c_str());
		}
		else
		{
			CLogHelper::ErrorLog("C-Store Request : Sub Association Failed; Codition Detail - %s. ", cond.text());

			OutputDebugString("\nC-Store Request : Sub Association Failed; Codition Detail - ");
			OutputDebugString(cond.text());
		}

		ASC_abortAssociation(static_cast<T_ASC_Association*>(m_assoc));
		ASC_dropAssociation(static_cast<T_ASC_Association*>(m_assoc));
		ASC_destroyAssociation((T_ASC_Association**)(&m_assoc));
		m_assoc = NULL;

		m_param = NULL;

		ASC_dropNetwork((T_ASC_Network**)(&m_net));
		m_net = NULL;

		return -4;
	}

	// No Presentation Contexts Accepted by SCP
	if(ASC_countAcceptedPresentationContexts(static_cast<T_ASC_Parameters*>(m_param)) == 0)
		return -5;

	return 0;
}
