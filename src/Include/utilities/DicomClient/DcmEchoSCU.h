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

#ifndef DCM_ECHO_SCU_BF2C6C5EF4D84FEBA0D930EB4C39F551
#define DCM_ECHO_SCU_BF2C6C5EF4D84FEBA0D930EB4C39F551

#pragma once

#include <utilities/DicomClient/ExportFlagDefine.h>

class UTILITIESDICOMCLIENT_API CDcmEchoSCU
{
public:
	CDcmEchoSCU(void);
	~CDcmEchoSCU(void);

	bool DoEcho(LPCTSTR pStrOurAET, LPCTSTR pStrPeerAET, LPCTSTR pStrPeerIP, int nPeerPort);

protected:
	int BuildSubAssociation(LPCTSTR pStrOurAET, LPCTSTR pStrPeerAET, LPCTSTR pStrPeerIP, int nPeerPort);
	bool AddAllStoragePresentationContexts(void *_params);

private:
	char m_strOurAET[64];

	void *m_net; // T_ASC_Network
	void *m_param; // T_ASC_Param
	void *m_assoc; // T_ASC_Association
};

#endif

