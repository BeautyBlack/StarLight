#include "pch.h"
#include <utilities/DicomServer/DcmEchoSCP.h>
#include <Foundations/Log/LogHelper.h>
#include <Foundations/Common/ServiceStopFlag.h>

CDcmEchoSCP::CDcmEchoSCP(void)
{
	m_myType = DCM_SCP_TYPE::eEchoSCP;
}

CDcmEchoSCP::~CDcmEchoSCP(void)
{
	CDcmSCPBase::~CDcmSCPBase();
}

bool CDcmEchoSCP::IsAcceptedMsgType(T_DIMSE_Command msgType)
{
	if(msgType == DIMSE_C_ECHO_RQ)
		return true;

	return false;
}

OFCondition CDcmEchoSCP::AchieveSCP(T_ASC_Association *assoc, T_DIMSE_Message *msg, T_ASC_PresentationContextID presID/*, LPVOID pParam*/)
{
	CLogHelper::HeadLog( __FUNCTION__);

	if(assoc == NULL_SYMBAL || msg == NULL_SYMBAL)
		return EC_IllegalParameter;

#ifdef PRINT_INFO_TO_CONSOLE
	printf_s("Received Echo Request : ");
	DIMSE_printCEchoRQ(stdout, &msg->msg.CEchoRQ);
#endif

	OFCondition cond = DIMSE_sendEchoResponse(assoc, presID, &msg->msg.CEchoRQ, STATUS_Success, NULL_SYMBAL);
	if(cond.bad())
	{
#ifdef PRINT_INFO_TO_CONSOLE
		fprintf_s(stderr, "Echo SCP Failed :\n");
#endif

		CLogHelper::ErrorLog( "Echo SCP Failed : %s .", cond.text());
	}
	else
	{
#ifdef PRINT_INFO_TO_CONSOLE
		printf_s("Echo SCP Success .\n");
#endif

		CLogHelper::DebugLog( "Echo SCP Success .");
	}

	return cond;
}

bool CDcmEchoSCP::Release()
{
	CLogHelper::HeadLog( __FUNCTION__);

	return true;
}


//OFCondition CDcmEchoSCP::ProcessCommands(T_ASC_Association *assoc, T_DIMSE_Message &msg, T_ASC_PresentationContextID &presID)
//{
//	if(assoc == NULL_SYMBAL)
//		return EC_IllegalParameter;
//
//	OFCondition cond = EC_Normal;
//	DcmDataset *statusDetail = NULL_SYMBAL;
//
//	int nWaitingLoopo = 0;
//
//	if(statusDetail != NULL_SYMBAL)
//	{
//#ifdef _DEBUG
//		printf_s("Extra Status Detail : \n");
//
//		statusDetail->print(COUT);
//#endif
//		delete statusDetail;
//	}
//
//	if(CServiceStopFlag::IsRunning() == false)
//		return DIMSE_OUTOFRESOURCES;
//
//	if(cond == DIMSE_NODATAAVAILABLE)
//	{
//		nWaitingLoopo++;
//	}
//	else if(cond == EC_Normal)
//	{
//		if(IsReachMyMaxThreadCount())
//			return DIMSE_OUTOFRESOURCES;
//
//		cond = AchieveSCP(assoc, &msg, presID);
//
//		if(cond.bad())
//			return DIMSE_OUTOFRESOURCES;
//
//		nWaitingLoopo = 0;
//	}
//
//	if(nWaitingLoopo >= 60)
//	{
//		return DIMSE_OUTOFRESOURCES;
//	}
//
//	return cond;
//}