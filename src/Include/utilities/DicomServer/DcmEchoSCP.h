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

#ifndef DCM_ECHO_SCP_9527CE9CAB23453AA93E2140F4B56D04
#define DCM_ECHO_SCP_9527CE9CAB23453AA93E2140F4B56D04

#pragma once

#include <utilities/DicomServer/DcmSCPBase.h>

class UTILITIESDICOMSERVER_API CDcmEchoSCP : public CDcmSCPBase
{
public:
	CDcmEchoSCP(void);
	~CDcmEchoSCP(void);

public:
	virtual OFCondition AchieveSCP(T_ASC_Association *assoc, T_DIMSE_Message *msg, T_ASC_PresentationContextID presID/*, LPVOID pParam*/);
	virtual bool IsAcceptedMsgType(T_DIMSE_Command msgType);
	virtual bool Release();

	// 处理DICOM 消息
	//virtual OFCondition ProcessCommands(T_ASC_Association *assoc, T_DIMSE_Message &msg, T_ASC_PresentationContextID &presID);

};

#endif