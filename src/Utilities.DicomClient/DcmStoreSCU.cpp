#include "pch.h"
#include "utilities/DicomClient/DcmStoreSCU.h"



#include <afx.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <WinSock2.h>

#include <Shlwapi.h>
#include <ImageHlp.h>

#include <dcmtk/dcmnet/assoc.h>
#include <dcmtk/dcmnet/cond.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmsr/dsrdoc.h>


#include "Foundations/Common/FileUtility.h"
#include "Foundations/Log/LogHelper.h"

void StoreSCUSubOperatorProgressCallback(void *callbackData,
                                         T_DIMSE_StoreProgress *_progress,
                                         T_DIMSE_C_StoreRQ *_request/*void *callbackData, void *_progress, void *_request*/)
{
	T_DIMSE_StoreProgress *progress = (T_DIMSE_StoreProgress*)_progress;
	T_DIMSE_C_StoreRQ *request = (T_DIMSE_C_StoreRQ*)_request;

	if(progress == NULL_SYMBAL || request == NULL_SYMBAL)
		return;

	switch(progress->state)
	{
	case DIMSE_StoreBegin:
		TRACE("\nStore SCU : Callback Begin.\n");
		break;
	case DIMSE_StoreEnd:
		TRACE("\nStore SCU : Callback End.\n");

		break;
	default:
		TRACE("\nStore SCU : Callback Processing.\n");
		break;
	}
}

CDcmStoreSCU::CDcmStoreSCU(void)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(1,1), &wsaData);

	m_assoc = NULL_SYMBAL;
	m_net = NULL_SYMBAL;
	m_param = NULL_SYMBAL;

	ZeroMemory(m_strOurAET, sizeof(m_strOurAET));
}

CDcmStoreSCU::~CDcmStoreSCU(void)
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

int CDcmStoreSCU::StoreSCU(DcmDataset *pDataset, LPCTSTR pStrOurAET, LPCTSTR pStrPeerAET, LPCTSTR pStrPeerIP, int nPeerPort)
{
	if(pDataset == NULL_SYMBAL
		|| pStrOurAET == NULL_SYMBAL
		|| pStrPeerIP == NULL_SYMBAL
		|| pStrPeerAET == NULL_SYMBAL)
		return 0;

	const char *strClassUID = NULL_SYMBAL;
	const char *strInstanceUID = NULL_SYMBAL;

	DIC_US msgID = 1;
	T_ASC_PresentationContextID presID = 1;
	T_DIMSE_C_StoreRQ storeReq;
	T_DIMSE_C_StoreRSP storeRsp;
	DcmDataset *storeDetail = NULL_SYMBAL;

	int nSuccessCount = 0;

	OFCondition cond;

	int nRet = BuildSubAssociation(pStrOurAET, pStrPeerAET, pStrPeerIP, nPeerPort);

	if(nRet < 0)
		return nRet;

	_snprintf_s(m_strOurAET, sizeof(m_strOurAET), _TRUNCATE, "%s", pStrOurAET);

	ZeroMemory(&storeRsp, sizeof(storeRsp));

	if(DIMSE_SendFile(*pDataset, msgID, &presID, &storeReq, &storeRsp, (void**)&storeDetail))
	{
		msgID++;
	}

	return msgID > 1 ? 1 : 0;
}

int CDcmStoreSCU::StoreSCU(const char pSendFileList[][MAX_PATH], const int nFilesCount,
						   LPCTSTR pStrOurAET, LPCTSTR pStrPeerAET, LPCTSTR pStrPeerIP, int nPeerPort, CStringList *pFailList /* = NULL_SYMBAL */,
						   StoreSCU_Callback_Func pCallbackFunc /* = NULL_SYMBAL */, LPVOID pCallbackData /* = NULL_SYMBAL */)
{
	if(nFilesCount <= 0 || pSendFileList == NULL_SYMBAL)
		return 0;

	int nRet = BuildSubAssociation(pStrOurAET, pStrPeerAET, pStrPeerIP, nPeerPort);

	if(nRet < 0)
		return nRet;

	_snprintf_s(m_strOurAET, sizeof(m_strOurAET), _TRUNCATE, "%s", pStrOurAET);

#ifdef _DEBUG
	clock_t beginClock = clock();
	int nSuccessCount = SendFile(pSendFileList, nFilesCount, pFailList, pCallbackFunc, pCallbackData);
	clock_t endClock = clock();

	TRACE("\n Store SCU Send PACS Take %d ms, File Count - %d, Success Count - %d. \n", endClock - beginClock, nFilesCount, nSuccessCount);
#else
	int nSuccessCount = SendFile(pSendFileList, nFilesCount, pFailList, pCallbackFunc, pCallbackData);
#endif

	return nSuccessCount;
}

int CDcmStoreSCU::StoreSCU(const CStringList *pStrFileList,
						   LPCTSTR pStrOurAET, LPCTSTR pStrPeerAET, LPCTSTR pStrPeerIP, int nPeerPort, CStringList *pFailList /* = NULL_SYMBAL */,
						   StoreSCU_Callback_Func pCallbackFunc /* = NULL_SYMBAL */, LPVOID pCallbackData /* = NULL_SYMBAL */)
{
	if(pStrFileList == NULL_SYMBAL || pStrFileList->GetCount() <= 0)
		return 0;

	int nRet = 0;

	nRet = BuildSubAssociation(pStrOurAET, pStrPeerAET, pStrPeerIP, nPeerPort);

	if(nRet < 0)
		return nRet;

	_snprintf_s(m_strOurAET, sizeof(m_strOurAET), _TRUNCATE, "%s", pStrOurAET);

#ifdef _DEBUG
	clock_t beginClock = clock();
	int nSuccessCount = SendFile(pStrFileList, pFailList, pCallbackFunc, pCallbackData);
	clock_t endClock = clock();

	TRACE("\n Store SCU Send PACS Take %d ms, File Count - %d, Success Count - %d. \n", endClock - beginClock, pStrFileList->GetCount(), nSuccessCount);
#else
	int nSuccessCount = SendFile(pStrFileList, pFailList, pCallbackFunc, pCallbackData);
#endif

	return nSuccessCount;
}

bool CDcmStoreSCU::AddAllStoragePresentationContexts(void *_params)
{
	if(_params == NULL_SYMBAL)
		return false;

	T_ASC_Parameters *params = (T_ASC_Parameters *)_params;

	int i;
	int pid = 1;

	const char* transferSyntaxes[] = { NULL_SYMBAL, NULL_SYMBAL, NULL_SYMBAL, NULL_SYMBAL, NULL_SYMBAL };
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

	for (i=0; i< numberOfDcmLongSCUStorageSOPClassUIDs; i++)
	{
		if(ASC_addPresentationContext(params, pid, dcmLongSCUStorageSOPClassUIDs[i], transferSyntaxes, numTransferSyntaxes).bad())
			return false;

		pid += 2;	/* only odd presentation context id's */
	}


	return true;
}

int CDcmStoreSCU::BuildSubAssociation(LPCTSTR pStrOurAET, LPCTSTR pStrPeerAET, LPCTSTR pStrPeerIP, int nPeerPort)
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

bool CDcmStoreSCU::DIMSE_SendFile(LPCTSTR strFilePath, const unsigned short msgID, void *_presID, 
								  void *_storeReq, void *_storeRsp, void** _storeDetail,
								  StoreSCU_Callback_Func pCallbackFunc /* = NULL_SYMBAL*/, LPVOID pCallbackData /* = NULL_SYMBAL*/)
{
	T_ASC_PresentationContextID *presID = (T_ASC_PresentationContextID *)_presID;
	T_DIMSE_C_StoreRQ *storeReq = (T_DIMSE_C_StoreRQ*)_storeReq;
	T_DIMSE_C_StoreRSP *storeRsp = (T_DIMSE_C_StoreRSP*)_storeRsp;
	DcmDataset **storeDetail = (DcmDataset**)_storeDetail;

	bool bRet = false;

	// 注意：DcmFileFormat成功loadFile后，即使调用了clear，再次通过FileFormat::Dataset获取信息时，有可能获取的是前一个File的信息
	//       maybe是老版本dcmtk的bug
	DcmFileFormat tmpFileFormat;

	if(tmpFileFormat.loadFile(strFilePath).bad())
		return bRet;

	DcmDataset *pDataset = tmpFileFormat.getDataset();

	if(pDataset == NULL_SYMBAL)
		return bRet;

	LPCTSTR strClassUID = NULL_SYMBAL;
	LPCTSTR strInstanceUID = NULL_SYMBAL;

	pDataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL_SYMBAL);

	DcmXfer fileXfer(pDataset->getOriginalXfer());

	if(fileXfer.isNotEncapsulated())
		fileXfer = EXS_LittleEndianExplicit;

	T_ASC_PresentationContext presentationContext;

	LPCTSTR strStudyUID = NULL_SYMBAL;
	if(pDataset->findAndGetString(DCM_StudyInstanceUID, strStudyUID).good())
		GetAffectedStudyUids().SetAt(strStudyUID, (LPVOID)0xFFFF);

	if(pDataset->findAndGetString(DCM_SOPClassUID, strClassUID).good()
		&& pDataset->findAndGetString(DCM_SOPInstanceUID, strInstanceUID).good())
	{
		if(fileXfer.getXfer() != EXS_Unknown)
			*presID = ASC_findAcceptedPresentationContextID(static_cast<T_ASC_Association*>(m_assoc), strClassUID, fileXfer.getXferID());
		else
			*presID = ASC_findAcceptedPresentationContextID(static_cast<T_ASC_Association*>(m_assoc), strClassUID);

		if(presID == 0)
			return bRet;

		ASC_findAcceptedPresentationContext(static_cast<T_ASC_Parameters*>(m_param), *presID, &presentationContext);
		DcmXfer netTransfer(presentationContext.acceptedTransferSyntax);

		ZeroMemory(storeReq, sizeof(T_DIMSE_C_StoreRQ));

		storeReq->DataSetType = DIMSE_DATASET_PRESENT;
		storeReq->Priority = DIMSE_PRIORITY_MEDIUM;

		storeReq->MessageID = msgID;

		if(strClassUID != NULL_SYMBAL)
			_snprintf_s(storeReq->AffectedSOPClassUID, sizeof(storeReq->AffectedSOPClassUID), _TRUNCATE, "%s", strClassUID);

		if(strInstanceUID != NULL_SYMBAL)
			_snprintf_s(storeReq->AffectedSOPInstanceUID, sizeof(storeReq->AffectedSOPInstanceUID), _TRUNCATE, "%s", strInstanceUID);

		storeReq->MoveOriginatorID = msgID;

		// 注意1：这里使用了dataset取代传递文件名
		// 区别 ：在dcmtk底层，如果dataset为NULL_SYMBAL，则会loadFile加载文件,而我们前面已经loadFile一次了，为了减少资源浪费，则传递dataset
		// 注意2：由于使用了dataset流作为参数，必须要设置callback函数。
		//        为什么：因为dcmtk底层源代码对callback函数进行判断，如果为NULL_SYMBAL，则有部分内容不读取（除非使用文件名方式)。
		OFCondition cond = DIMSE_storeUser(static_cast<T_ASC_Association*>(m_assoc), *presID, storeReq, NULL_SYMBAL, pDataset, NULL_SYMBAL, 
			NULL_SYMBAL, DIMSE_BLOCKING, 30000, storeRsp, storeDetail, NULL_SYMBAL, CFileUtility::GetFileSize(strFilePath));

		if(cond != EC_Normal)
		{
			CLogHelper::ErrorLog("Store SCU Send File Error : Condition - %s. ", cond.text());

			TRACE("\nStore SCU Send File Error : Condition - %s. \n", cond.text());
		}
		else
		{
			CLogHelper::DebugLog("Store SCU Send File Success. ");

			bRet = true;
		}

		if(pCallbackFunc != NULL_SYMBAL)
			pCallbackFunc(pCallbackData, 0x22/*SCC_STORE_ONEFILE_FINISHED*/, storeRsp);
	}

	return bRet;
}

int CDcmStoreSCU::SendFile(const char pSendFileList[][MAX_PATH], const int nFilesCount, CStringList *pFailList /* = NULL_SYMBAL */,
						   StoreSCU_Callback_Func pCallbackFunc /* = NULL_SYMBAL */, LPVOID pCallbackData /* = NULL_SYMBAL */)
{
	if(m_assoc == NULL_SYMBAL)
		return -1;

	if(pSendFileList == NULL_SYMBAL || nFilesCount <= 0)
		return 0;

	const char *strClassUID = NULL_SYMBAL;
	const char *strInstanceUID = NULL_SYMBAL;

	DIC_US msgID = 1;
	T_ASC_PresentationContextID presID = 1;
	T_DIMSE_C_StoreRQ storeReq;
	T_DIMSE_C_StoreRSP storeRsp;
	DcmDataset *storeDetail = NULL_SYMBAL;

	int nSuccessCount = 0;

	OFCondition cond;

	for(int i = 0; i < nFilesCount; i++)
	{
		if( !PathFileExists(pSendFileList[i]))
			continue;

		ZeroMemory(&storeRsp, sizeof(storeRsp));

		if(DIMSE_SendFile(pSendFileList[i], msgID, &presID, &storeReq, &storeRsp, (void**)&storeDetail), pCallbackFunc, pCallbackData)
		{
			msgID++;
		}
		else
		{
			if(pFailList != NULL_SYMBAL)
				pFailList->AddTail(pSendFileList[i]);
		}
	}

	return nSuccessCount;
}

int CDcmStoreSCU::SendFile(const CStringList *pStrFileList, CStringList *pFailList /* = NULL_SYMBAL */,
						   StoreSCU_Callback_Func pCallbackFunc /* = NULL_SYMBAL */, LPVOID pCallbackData /* = NULL_SYMBAL */)
{
	if(m_assoc == NULL_SYMBAL)
		return -1;

	const char *strClassUID = NULL_SYMBAL;
	const char *strInstanceUID = NULL_SYMBAL;

	DIC_US msgID = 1;
	T_ASC_PresentationContextID presID = 1;
	T_DIMSE_C_StoreRQ storeReq;
	T_DIMSE_C_StoreRSP storeRsp;
	DcmDataset *storeDetail = NULL_SYMBAL;

	int nSuccessCount = 0;

	OFCondition cond;


	for(POSITION pos = pStrFileList->GetHeadPosition(); pos != NULL_SYMBAL; pStrFileList->GetNext(pos))
	{
		if( !PathFileExists(pStrFileList->GetAt(pos)))
			continue;

		ZeroMemory(&storeRsp, sizeof(storeRsp));

		if(DIMSE_SendFile(pStrFileList->GetAt(pos), msgID, &presID, &storeReq, &storeRsp, (void**)&storeDetail, pCallbackFunc, pCallbackData))
		{
			nSuccessCount++;
		}
		else
		{
			if(pFailList != NULL_SYMBAL)
				pFailList->AddTail(pStrFileList->GetAt(pos));
		}

		CLogHelper::DebugLog("Store SCU - 发送文件[%d]", msgID);

		msgID++;
	}

	CLogHelper::DebugLog("Store SCU - 发送文件共[%d]， 成功 [%d]", pStrFileList->GetCount(), nSuccessCount);

	return nSuccessCount;
}


bool CDcmStoreSCU::DIMSE_SendFile(DcmDataset &dataset, const unsigned short msgID, void *_presID, 
								  void *_storeReq, void *_storeRsp, void** _storeDetail)
{
	T_ASC_PresentationContextID *presID = (T_ASC_PresentationContextID *)_presID;
	T_DIMSE_C_StoreRQ *storeReq = (T_DIMSE_C_StoreRQ*)_storeReq;
	T_DIMSE_C_StoreRSP *storeRsp = (T_DIMSE_C_StoreRSP*)_storeRsp;
	DcmDataset **storeDetail = (DcmDataset**)_storeDetail;

	bool bRet = false;

	LPCTSTR strClassUID = NULL_SYMBAL;
	LPCTSTR strInstanceUID = NULL_SYMBAL;

	dataset.chooseRepresentation(EXS_LittleEndianExplicit, NULL_SYMBAL);

	DcmXfer fileXfer(dataset.getOriginalXfer());

	if(fileXfer.isNotEncapsulated())
		fileXfer = EXS_LittleEndianExplicit;

	T_ASC_PresentationContext presentationContext;

	LPCTSTR strStudyUID = NULL_SYMBAL;
	if(dataset.findAndGetString(DCM_StudyInstanceUID, strStudyUID).good())
		GetAffectedStudyUids().SetAt(strStudyUID, (LPVOID)0xFFFF);

	if(dataset.findAndGetString(DCM_SOPClassUID, strClassUID).good()
		&& dataset.findAndGetString(DCM_SOPInstanceUID, strInstanceUID).good())
	{
		if(fileXfer.getXfer() != EXS_Unknown)
			*presID = ASC_findAcceptedPresentationContextID(static_cast<T_ASC_Association*>(m_assoc), strClassUID, fileXfer.getXferID());
		else
			*presID = ASC_findAcceptedPresentationContextID(static_cast<T_ASC_Association*>(m_assoc), strClassUID);

		if(presID == 0)
			return bRet;

		ASC_findAcceptedPresentationContext(static_cast<T_ASC_Parameters*>(m_param), *presID, &presentationContext);
		DcmXfer netTransfer(presentationContext.acceptedTransferSyntax);

		ZeroMemory(storeReq, sizeof(T_DIMSE_C_StoreRQ));

		storeReq->DataSetType = DIMSE_DATASET_PRESENT;
		storeReq->Priority = DIMSE_PRIORITY_MEDIUM;

		storeReq->MessageID = msgID;

		if(strClassUID != NULL_SYMBAL)
			_snprintf_s(storeReq->AffectedSOPClassUID, sizeof(storeReq->AffectedSOPClassUID), _TRUNCATE, "%s", strClassUID);

		if(strInstanceUID != NULL_SYMBAL)
			_snprintf_s(storeReq->AffectedSOPInstanceUID, sizeof(storeReq->AffectedSOPInstanceUID), _TRUNCATE, "%s", strInstanceUID);

		storeReq->MoveOriginatorID = msgID;

		// 注意1：这里使用了dataset取代传递文件名
		// 区别 ：在dcmtk底层，如果dataset为NULL_SYMBAL，则会loadFile加载文件,而我们前面已经loadFile一次了，为了减少资源浪费，则传递dataset
		// 注意2：由于使用了dataset流作为参数，必须要设置callback函数。
		//        为什么：因为dcmtk底层源代码对callback函数进行判断，如果为NULL_SYMBAL，则有部分内容不读取（除非使用文件名方式)。
		OFCondition cond = DIMSE_storeUser(static_cast<T_ASC_Association*>(m_assoc), *presID, storeReq, NULL_SYMBAL, &dataset, NULL_SYMBAL/*StoreSCUSubOperatorProgressCallback*/, 
			NULL_SYMBAL, DIMSE_BLOCKING, 30000, storeRsp, storeDetail);

		if(cond != EC_Normal)
		{
			CLogHelper::ErrorLog("Store SCU Send File Error : Condition - %s. ", cond.text());

			TRACE("\nStore SCU Send File Error : Condition - %s. \n", cond.text());
		}
		else
		{
			CLogHelper::DebugLog("Store SCU Send File Success. ");

			bRet = true;
		}
	}

	return bRet;
}
