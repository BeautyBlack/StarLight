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


#ifndef DCM_SCU_BASE_8CB34C9F526F4718834AFDA843946BC4
#define DCM_SCU_BASE_8CB34C9F526F4718834AFDA843946BC4

#pragma once

#include <utilities/DicomClient/DcmParam.h>

#include <utilities/DicomClient/ExportFlagDefine.h>

class UTILITIESDICOMCLIENT_API __declspec(novtable) CDcmSCUBase
{
public:
	bool Process(CDcmParam &param);

	virtual ~CDcmSCUBase();

protected:
	CDcmSCUBase();

private:
	int BuildSubAssociation();
	bool AddPresentationContexts(void *_params);

	virtual bool ProcessInternal(CDcmParam &param) = 0;

protected:
	AUTO_PROPERTY_REF(MyAET, CString);
	AUTO_PROPERTY_REF(PeerAET, CString);
	AUTO_PROPERTY_REF(PeerIP, CString);
	AUTO_PROPERTY_REF(PeerPort, int);

	AUTO_PROPERTY_REF(HasBuildAssociation, bool);

protected:
	void *m_net; // T_ASC_Network
	void *m_param; // T_ASC_Param
	void *m_assoc; // T_ASC_Association

	char** m_pAbstractSyntaxes;
	int m_nAbstractSyntaxCount;
};



#ifndef UTILITIESDICOMCLIENT_EXPORTS
#pragma comment(lib,"Utilities.DicomClient.lib")
#endif

#endif
