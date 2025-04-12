#include "pch.h"
#include <utilities/DicomServer/DicomServiceProvider.h>

#include <utilities/DicomServer/DcmSCPBase.h>
#include <utilities/DicomServer/DcmStoreSCP.h>
#include <utilities/DicomServer/DcmEchoSCP.h>

#include <dcmtk/dcmnet/dimse.h>

CDicomServiceProvider CDicomServiceProvider::m_dcmProvider;

CDicomServiceProvider::CDicomServiceProvider()
{
	m_bCEchoEnable = true;
	m_bCStoreEnable = true;
}

CDcmSCPBase* CDicomServiceProvider::CreateSCP(T_DIMSE_Message &msg/*T_DIMSE_Command command*/)
{
	CDcmSCPBase *pRet = NULL;

	switch(msg.CommandField)
	{
	case DIMSE_C_STORE_RQ:
		pRet = new CDcmStoreSCP;
		pRet->SetMaxThreadCount(10);
		break;

	case DIMSE_C_ECHO_RQ:
		pRet = new CDcmEchoSCP;
		pRet->SetMaxThreadCount(10);
		break;
	}

	return pRet;
}

void CDicomServiceProvider::ReleaseSCP(CDcmSCPBase *pSCP)
{
	if(pSCP != NULL_SYMBAL)
		delete pSCP;
}
