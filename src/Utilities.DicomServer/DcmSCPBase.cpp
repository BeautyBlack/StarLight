#include "pch.h"
#include "utilities/DicomServer/DcmSCPBase.h"

#include <Foundations/Common/ServiceStopFlag.h>

#include <dcmtk/dcmdata/dcelem.h>


//char CDcmSCPBase::m_strMyAET[MAX_PATH] = { 0 };
int CDcmSCPBase::m_netWaitingTimeout = 30000;
int CDcmSCPBase::m_requestWaitingTimeout = 30000;
int CDcmSCPBase::m_dimseWaitingTimeout = 30000;

CDcmSCPBase::CDcmSCPBase()
{
	m_myType = DCM_SCP_TYPE::eUnkown;

	m_curThreadCount = 0;
	m_maxThreadCount = 1;

	m_pSupportDcmTagMap = NULL_SYMBAL;

	m_bStopFlag = NULL_SYMBAL;

	IncreaseCurThreadCount();
}

CDcmSCPBase::~CDcmSCPBase()
{
	DecreaseCurThreadCount();
}

OFCondition CDcmSCPBase::ProcessCommands(T_ASC_Association *assoc, T_DIMSE_Message &msg, T_ASC_PresentationContextID &presID)
{
	if (assoc == NULL_SYMBAL)
		return EC_IllegalParameter;

	OFCondition cond = EC_Normal;
	DcmDataset *statusDetail = NULL_SYMBAL;

	int nWaitingLoopo = 0;

	do
	{
		if (statusDetail != NULL_SYMBAL)
		{
#ifdef _DEBUG
			printf_s("Extra Status Detail : \n");

			statusDetail->print(COUT);
#endif
			delete statusDetail;
		}

		if (CServiceStopFlag::IsRunning() == false)
			return DIMSE_OUTOFRESOURCES;

		if (cond == DIMSE_NODATAAVAILABLE)
		{
			nWaitingLoopo++;
		}
		else if (cond == EC_Normal)
		{
			if (IsReachMyMaxThreadCount())
				return DIMSE_OUTOFRESOURCES;

			cond = AchieveSCP(assoc, &msg, presID);

			if (cond.bad())
				return DIMSE_OUTOFRESOURCES;

			nWaitingLoopo = 0;
		}

		if (nWaitingLoopo >= 60)
		{
			return DIMSE_OUTOFRESOURCES;
		}

		cond = DIMSE_receiveCommand(assoc, DIMSE_NONBLOCKING, 2, &presID, &msg, &statusDetail);
	} while (cond == EC_Normal || cond == DIMSE_NODATAAVAILABLE);

	return cond;
}

