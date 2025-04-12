#include "pch.h"
#include <utilities/DicomServer/DcmStoreSCP.h>
#include <Foundations/Log/LogHelper.h>
#include <Foundations/Common/ServiceStopFlag.h>

#include <utilities/ImageModule/Service/DicomFileImporter.h>

#include <json/json.h>

#include <ImageHlp.h>
#include <Shlwapi.h>

//use for MakeSureDirectoryPathExists
#pragma comment(lib, "dbghelp.lib")

#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmsr/dsrdoc.h>
#include <dcmtk/dcmdata/dcdeftag.h>

//#include "utilities/Configuration/Configuration.h"

#include <ImageHlp.h>


#include "Foundations/Common/StringUtility.h"
#include "Foundations/Common/TimeHelper.h"
#include "utilities/Database/DatabaseManager.h"
#pragma comment(lib, "ImageHlp.lib")

struct sctReservedInfo
{
	char strStudyInstUID[128];
	char strStudyID[64];
	char strStudyDate[32];
	char strStudyTime[32];
	char strAccNum[64];
	char strStudyDesc[64];
	char strBodypart[128];
	char strModality[128];
	char strPatName[64];
	char strPatID[64];
	char strPatSex[64];
	char strPatAge[64];
	char strBirthData[32];

	bool bValid;
	bool bUTF8;
};

struct sctStoreCallbackData
{
	DcmFileFormat* dcmff;
	T_ASC_Association* assoc;

	CDcmStoreSCP *pStoreSCP;

	sctStoreCallbackData()
	{
		dcmff = NULL_SYMBAL;
		assoc = NULL_SYMBAL;

		pStoreSCP = NULL_SYMBAL;
	}
};



long CDcmStoreSCP::m_nStaticMaxClientCount = 10;
long CDcmStoreSCP::m_nStaticCurClientCount = 0;

CDcmStoreSCP::CDcmStoreSCP(void)
{
	m_myType = DCM_SCP_TYPE::eStoreSCP;

	ZeroMemory(m_strCallingAET, sizeof(m_strCallingAET));

}

CDcmStoreSCP::~CDcmStoreSCP(void)
{
	//CDcmSCPBase::~CDcmSCPBase();
}

OFCondition CDcmStoreSCP::AchieveSCP(T_ASC_Association *assoc, T_DIMSE_Message *msg, T_ASC_PresentationContextID presID/*, LPVOID pParam*/)
{
	CLogHelper::HeadLog(__FUNCTION__);

	OFCondition cond = EC_Normal;
	T_DIMSE_C_StoreRQ *req;

	// assign the actual information of the C-STORE_RQ command to a local variable
	req = &msg->msg.CStoreRQ;

#ifdef PRINT_INFO_TO_CONSOLE
	printf_s("Received Store Request : ");
	DIMSE_printCStoreRQ(stdout, req);
#endif

	if(m_strCallingAET[0] == 0)
		_snprintf_s(m_strCallingAET, sizeof(m_strCallingAET), _TRUNCATE, "%s", assoc->params->DULparams.callingAPTitle);

	// intialize some variables
	DcmFileFormat *pDcmfile = new DcmFileFormat;

	sctStoreCallbackData callbackData;

	callbackData.assoc = assoc;
	callbackData.dcmff = pDcmfile;
	callbackData.pStoreSCP = this;

	DcmDataset *dset = pDcmfile->getDataset();

	// if opt_bitPreserving is set, the user requires that the data shall be written exactly as it was received.
	// Depending on this option, function DIMSE_storeProvider must be called with certain parameters.
	//m_staticLogger = m_myLogger;
	cond = DIMSE_storeProvider(assoc, presID, req, NULL_SYMBAL/*imageFileName*/, OFTrue, /*NULL_SYMBAL*/&dset, StoreSCPCallback, /*pCbParam*/&callbackData, DIMSE_BLOCKING, 0);

	// If some error occured, dump corresponding information and remove the outfile if necessary
	if(cond.bad())
	{
#ifdef PRINT_INFO_TO_CONSOLE
		fprintf_s(stderr, "Store SCP : Store SCP Failed .\n");
#endif
		CLogHelper::ErrorLog( "Store SCP : Store SCP Failed - %s .", cond.text());
	}

	return cond;
}

bool CDcmStoreSCP::IsAcceptedMsgType(T_DIMSE_Command msgType)
{
	if(msgType == DIMSE_C_STORE_RQ)
		return true;

	return false;
}

bool CDcmStoreSCP::Release()
{
	CLogHelper::HeadLog( __FUNCTION__);

	return true;
}

// This function is used to indicate progress when storescp receives instance data over the network.
// On the final call to this function (identified by progress->state == DIMSE_StoreEnd)
// This function will store the data set which was received over the network to a file.
// Earlier calls to this funciton will simply cause some information to be dumped to stdout.
//
// Parameters:
//    callbackData  -  [in] data for this callback function
//    progress      -  [in] The State of progress. (identifies if this is the initial or final call to this function,
//                          or a call in between these two calls.
//    req           -  [in] The original store request message.
//    imageFileName -  [in] The path to and name of the file the information shall be written to.
//    imageDataSet  -  [in] The data set which shall be stored in the image path
//    rsp           -  [inout] The C-Store-RSP message (will be sent after the call to this function)
//    statusDetail  -  [inout] This variable can be used to capture detailed information with regard to the status information
//                          which is captured in the status element(0000,0900).
//                          Note that this function does specify any such information, the pointer will be set to NULL_SYMBAL.
void CDcmStoreSCP::StoreSCPCallback(void *callbackData, T_DIMSE_StoreProgress *progress, T_DIMSE_C_StoreRQ *req, char *imageFileName, DcmDataset **imageDataSet, T_DIMSE_C_StoreRSP *rsp, DcmDataset **statusDetail)
{
	DIC_UI sopClass;
	char sopInstance[128] = {0};

	sctStoreCallbackData *cbData = OFstatic_cast(sctStoreCallbackData*, callbackData);

	CDcmStoreSCP *pParentSCP = cbData->pStoreSCP;

	// dump some information if required (depending on the progress state) opt_bose info
	switch(progress->state)
	{
	case DIMSE_StoreBegin:
#ifdef PRINT_INFO_TO_CONSOLE
		printf_s("Recv Begin ");
#endif

		CLogHelper::DebugLog("Recv : ");
		break;

	case DIMSE_StoreEnd:
#ifdef PRINT_INFO_TO_CONSOLE
		printf_s("Recv End \n");
#endif

		CLogHelper::DebugLog("Receive End .");

		break;

	default:
#ifdef PRINT_INFO_TO_CONSOLE
		putchar('.');
#endif
		break;
	}

#ifdef PRINT_INFO_TO_CONSOLE
	fflush(stdout);
#endif

	// If this is the final call of this function, save the data which was received to a file
	// (note that we could also save the image somewhere else, put it in database, etc.)
	if(progress->state == DIMSE_StoreEnd)
	{
		// do not send status detail information
		*statusDetail = NULL_SYMBAL;

		// remember callback data

		// Concerning the following line : an appropriate status code is already set in the resp structure,
		// It need not be success. For example, if the caller has already detected an out of resources problem
		// Then the status will reflect this. The callback function is still called to allow cleanup.

		// we want to write the received information to a file only if this information
		// is present and the options opt_bitPreserving and opt_ignore are not set
		if(imageDataSet != NULL_SYMBAL && *imageDataSet != NULL_SYMBAL)
		{
			CDicomFileImporter::ImportFile(cbData->dcmff);

			/*auto pDataset = cbData->dcmff->getDataset();
			
			auto studyDto = DatasetToStudyDto(pDataset);

			char strDcmPath[MAX_PATH] = {0};

			PathCombine(strDcmPath,
				pParentSCP->GetImageRootPath().GetString(),
				studyDto.GetStudyInstanceUID().c_str());

			LPCTSTR strInstanceUID = NULL_SYMBAL;
			pDataset->findAndGetString(DCM_SOPInstanceUID, strInstanceUID);

			CString strFileName;

			strFileName.Format("%s.dcm", strInstanceUID);
			
			PathAppend(strDcmPath, strFileName);

			MakeSureDirectoryPathExists(strDcmPath);
			
			cbData->dcmff->saveFile(strDcmPath, EXS_LittleEndianExplicit, EET_ExplicitLength, EGL_withGL);

			CDatabaseManager::GetInstance().RecordStudy(studyDto);*/

			
			//CStorageJobQueueMgr::GetInstance().AddJob(cbData->dcmff, pParentSCP->m_strCallingAET);

			//// 30 - 按照20MB一张影像， 30张为600MB，还算是在可自由申请内存限制内。也可以设为50,。
			//if(CStorageJobQueueMgr::GetInstance().GetCurrentWaitingFiles() > 40)
			//	Sleep(2000);
		}
	}
}

CStudyDto CDcmStoreSCP::DatasetToStudyDto(DcmDataset* pDataset)
{
	CStudyDto studyDto;

	LPCTSTR strTemp = NULL_SYMBAL;

	LPCTSTR strCharset = NULL_SYMBAL;
	pDataset->findAndGetString(DCM_SpecificCharacterSet, strCharset);

	bool isUTF8 = false;
	
	if (strCharset != NULL_SYMBAL
		&& strstr(strCharset, "192"))
		isUTF8 = true;

	if (pDataset->findAndGetString(DCM_StudyInstanceUID, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetStudyInstanceUID(strTemp);

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_AccessionNumber, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetAccessionNumber(strTemp);

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_PatientAge, strTemp).good()
		&& strTemp != NULL_SYMBAL)
	{
		const int nLen = strlen(strTemp);
		
		studyDto.SetAge(atoi(strTemp));

		if (strTemp[nLen - 1] == 'Y')
			studyDto.SetAgeUnit(AgeUnit::Year);
		else if (strTemp[nLen - 1] == 'M')
			studyDto.SetAgeUnit(AgeUnit::Month);
		else if (strTemp[nLen - 1] == 'W')
			studyDto.SetAgeUnit(AgeUnit::Week);
		else if (strTemp[nLen - 1] == 'D')
			studyDto.SetAgeUnit(AgeUnit::Day);
	}
	
	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_PatientBirthDate, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		CTimeHelper::ParseDicomDateTime(strTemp, NULL_SYMBAL, studyDto.GetBirthDate());

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_BodyPartExamined, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetBodyParts(
			isUTF8 ? CStringUtility::ConvertUTF8ToGB(strTemp).GetString() : strTemp);

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_Modality, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetModalitys(strTemp);

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_PatientID, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetPatientId(strTemp);

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_PatientName, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetPatientName(
			isUTF8 ? CStringUtility::ConvertUTF8ToGB(strTemp).GetString() : strTemp);

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_PatientSex, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetSex(ParseTextToSex(strTemp));

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_StudyDate, strTemp).good()
		&& strTemp != NULL_SYMBAL)
	{
		LPCTSTR strTime = NULL_SYMBAL;
		pDataset->findAndGetString(DCM_StudyTime, strTime);

		CTimeHelper::ParseDicomDateTime(strTemp, strTime, studyDto.GetStudyDateTime());
	}

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_StudyDescription, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetStudyDescription(
			isUTF8 ? CStringUtility::ConvertUTF8ToGB(strTemp).GetString() : strTemp);

	strTemp = NULL_SYMBAL;
	if (pDataset->findAndGetString(DCM_StudyID, strTemp).good()
		&& strTemp != NULL_SYMBAL)
		studyDto.SetStudyId(strTemp);
	
	return studyDto;
}


