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

#ifndef DCM_STORE_SCU_F621C4378E34451DB01417021471B7AB
#define DCM_STORE_SCU_F621C4378E34451DB01417021471B7AB

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <utilities/DicomClient/ExportFlagDefine.h>

class DcmDataset;

typedef void (__cdecl *StoreSCU_Callback_Func)(void *, int , void *);

// 用于完成 StoreSCU功能，暂时位于ScanFrame
// 注意：在ScanFrame的使用中，由于MutualData已经定义了DIC_UI等类型，而该类型本应该是dcmtk定义的
//       因此不可以在 thisClass中写入#include<dcmtk.h>，会导致类型重定义错误
class UTILITIESDICOMCLIENT_API CDcmStoreSCU
{
public:
	CDcmStoreSCU(void);
	~CDcmStoreSCU(void);

	// Description
	// @return
	//  >1 : Success Send File Count
	//  0 : Success
	// -1 : 网络初始化失败
	// -2 : 创建Association Parameter失败
	// -3 : 添加Presentation Contexts失败
	// -4 : Request Association 失败
	// -5 : No Presentation Contexts Accepted by SCP
	// @pCallbackFunc : 用途类似老版本StoreSCP Callback Function
	int StoreSCU(const CStringList *pStrFileList, LPCTSTR pStrOurAET, LPCTSTR pStrPeerAET, LPCTSTR pStrPeerIP, int nPeerPort, CStringList *pFailList = NULL, StoreSCU_Callback_Func pCallbackFunc = NULL, LPVOID pCallbackData = NULL);
	int StoreSCU(const char pSendFileList[][MAX_PATH], const int nFilesCount, LPCTSTR pStrOurAET, LPCTSTR pStrPeerAET, LPCTSTR pStrPeerIP, int nPeerPort, CStringList *pFailList = NULL, StoreSCU_Callback_Func pCallbackFunc = NULL, LPVOID pCallbackData = NULL);

	int StoreSCU(DcmDataset *pDataset, LPCTSTR pStrOurAET, LPCTSTR pStrPeerAET, LPCTSTR pStrPeerIP, int nPeerPort);

protected:
	int BuildSubAssociation(LPCTSTR pStrOurAET, LPCTSTR pStrPeerAET, LPCTSTR pStrPeerIP, int nPeerPort);

	int SendFile(const CStringList *pStrFileList, CStringList *pFailList = NULL, StoreSCU_Callback_Func pCallbackFunc = NULL, LPVOID pCallbackData = NULL);
	int SendFile(const char pSendFileList[][MAX_PATH], const int nFilesCount, CStringList *pFailList = NULL, StoreSCU_Callback_Func pCallbackFunc = NULL, LPVOID pCallbackData = NULL);

	bool AddAllStoragePresentationContexts(void *_params);

	bool DIMSE_SendFile(
		LPCTSTR strFilePath,
		const unsigned short msgID,
		void* _presID,
		void* _storeReq,
		void* _storeRsp,
		void** _storeDetail,
		StoreSCU_Callback_Func pCallbackFunc = NULL,
		LPVOID pCallbackData = NULL);

	bool DIMSE_SendFile(
		DcmDataset &dataset,
		const unsigned short msgID,
		void* _presID,
		void* _storeReq,
		void* _storeRsp,
		void** _storeDetail);

private:
	char m_strOurAET[64];

	void *m_net; // T_ASC_Network
	void *m_param; // T_ASC_Param
	void *m_assoc; // T_ASC_Association

	AUTO_PROPERTY_REF_NO_SET(AffectedStudyUids, CMapStringToPtr);
};

#endif
