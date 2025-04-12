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

#ifndef DICOM_SERVICE_PROVIDER_200A82C7A75E487FBE99B8DD23CC8AA9
#define DICOM_SERVICE_PROVIDER_200A82C7A75E487FBE99B8DD23CC8AA9

#include <dcmtk/dcmnet/dimse.h>

#include <utilities/DicomServer/DcmSCPBase.h>

#pragma once

class CDicomServiceProvider
{
public:
	CDicomServiceProvider();

	CDcmSCPBase* CreateSCP(T_DIMSE_Message &msg/*T_DIMSE_Command command*/);
	void ReleaseSCP(CDcmSCPBase *pScp);

	static CDicomServiceProvider& GetInstance() { return m_dcmProvider;}

	void EnableCEcho() { m_bCEchoEnable = true;}
	void EnableCStore() { m_bCStoreEnable = true;}

private:
	static CDicomServiceProvider m_dcmProvider;

	bool m_bCEchoEnable;
	bool m_bCStoreEnable;
};

#endif