#include "stdafx.h"
#include <Utilities/ImageModule/Service/DicomLoaderService.h>
#include <utilities/ImageModule/Modules/StudyModule.h>
#include "Foundations/Common/FileUtility.h"
#include "Foundations/Interfaces/MineType.h"
#include <Foundations/Common/TimeHelper.h>
#include <dcmtk/oflog/config.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dctag.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>

#include "utilities/ImageModule/ModuleHelper.h"
#include "StudyModuleLoader.h"


CDicomLoaderService::CDicomLoaderService()
{
	TRACE("Create Object : %s:%d", __FUNCTION__, __LINE__);
}


CDicomLoaderService::~CDicomLoaderService()
{
}

//std::vector<std::shared_ptr<CStudyModule>> CDicomLoaderService::LoadStudies(std::vector<STRING_TYPE>& strStudyUids)
//{
//	std::vector<STRING_TYPE> paths;
//
//	for (const auto& uid : strStudyUids)
//	{
//		CFileUtility::SearchPaths(paths, uid.c_str(), MineType::Dicom.c_str());
//	}
//
//	return LoadImageFiles(paths);
//}

std::shared_ptr<CStudyModule> CDicomLoaderService::LoadStudy(const char* strStudyPath)
{
	std::vector<STRING_TYPE> paths;

	CFileUtility::SearchPaths(paths, strStudyPath, MineType::Dicom.c_str());

	auto studyModules = LoadImageFiles(paths);

	return studyModules.empty() ? std::shared_ptr<CStudyModule>() : studyModules[0];
}

void CDicomLoaderService::ReleaseStudy(const char* strStudyUID)
{
	CStudyModuleHelper::GetInstance().RemoveItem(strStudyUID);
}


std::vector<std::shared_ptr<CStudyModule>> CDicomLoaderService::LoadImageFiles(std::vector<STRING_TYPE>& filePaths)
{
	std::map<STRING_TYPE, std::shared_ptr<CStudyModule>> result;

	for (const auto& aPath : filePaths)
	{
		DcmFileFormat dcmFileHandle;

		if(dcmFileHandle.loadFile(OFFilename(aPath.c_str())).bad())
			continue;

		auto dataset = dcmFileHandle.getDataset();

		dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL_SYMBAL);

		LPCTSTR strStudyUid = NULL_SYMBAL;

		if(dataset->findAndGetString(DCM_StudyInstanceUID, strStudyUid).bad()
			|| strStudyUid == NULL_SYMBAL
			|| strStudyUid[0] == '\0')
			continue;

		auto itRefStudyModule = result.find(strStudyUid);
		std::shared_ptr<CStudyModule> studyModule;

		LPCTSTR strValue = NULL_SYMBAL;

		UINT16 nValue16 = 0;

		// Study
		if (itRefStudyModule == result.end())
		{
			studyModule = CStudyModuleHelper::GetInstance().GetStudy((const char*)strStudyUid);

			if (!studyModule)
			{
				studyModule = std::make_shared<CStudyModule>();

				CStudyModuleHelper::GetInstance().AddItem(strStudyUid, studyModule);

				studyModule->SetStudyInstanceUID(strStudyUid);

				CStudyModuleLoader::Load(studyModule.get(), dataset);

				result.insert_or_assign(strStudyUid, studyModule);
			}

		}
		else
			studyModule = itRefStudyModule->second;
		
		// Series
		strValue = NULL_SYMBAL;
		dataset->findAndGetString(DCM_SeriesInstanceUID, strValue);

		if (strValue == NULL_SYMBAL || strValue[0] == '\0')
			continue;

		auto seriesModule = studyModule->GetSeries(strValue);
		
		if( !seriesModule)
		{
			seriesModule = std::make_shared<CSeriesModule>();

			seriesModule->SetSeriesInstanceUID(strValue);

			seriesModule->SetReferenceStudy(studyModule);

			strValue = NULL_SYMBAL;
			if (dataset->findAndGetString(DCM_BodyPartExamined, strValue).good()
				&& strValue != NULL_SYMBAL)
			{
				seriesModule->SetBodyPart(strValue);

				CString strBodyParts = studyModule->GetBodyParts().c_str();

				if (strBodyParts.Find(strValue) < 0)
				{
					strBodyParts.AppendFormat("/%s", strValue);

					strBodyParts.Trim('/');

					studyModule->SetBodyParts(strBodyParts.GetString());
				}
			}

			strValue = NULL_SYMBAL;
			if (dataset->findAndGetString(DCM_SeriesNumber, strValue).good()
				&& strValue != NULL_SYMBAL)
				seriesModule->SetSeriesNumber(atoi(strValue));

			strValue = NULL_SYMBAL;
			if (dataset->findAndGetString(DCM_SeriesDescription, strValue).good()
				&& strValue != NULL_SYMBAL)
				seriesModule->SetSeriesDescription(strValue);

			LPCTSTR strDate = NULL_SYMBAL, strTime = NULL_SYMBAL;
			if (dataset->findAndGetString(DCM_SeriesDate, strDate).good()
				&& strValue != NULL_SYMBAL)
			{
				dataset->findAndGetString(DCM_SeriesTime, strTime);

				CTimeHelper::ParseDicomDateTime(strDate, strTime, seriesModule->GetSeriesDateTime());
			}
			
			strValue = NULL_SYMBAL;
			if (dataset->findAndGetString(DCM_Modality, strValue).good()
				&& strValue != NULL_SYMBAL)
			{
				seriesModule->SetModality(ParseTextToModality(strValue));
				
				CString strModalities = studyModule->GetModalitys().c_str();

				if (strModalities.Find(strValue) < 0)
				{
					strModalities.AppendFormat("/%s", strValue);

					strModalities.Trim('/');

					studyModule->SetModalitys(strModalities.GetString());
				}
			}

			studyModule->AddSeriesModule(seriesModule);

		}

		// Instance & Image Plan
		strValue = NULL_SYMBAL;
		dataset->findAndGetString(DCM_SOPInstanceUID, strValue);

		if (strValue == NULL_SYMBAL || strValue[0] == '\0')
			continue;
		
		auto instance = seriesModule->GetInstance(strValue);
		
		if( !instance)
		{
			std::shared_ptr<CImageInstanceModule> instanceModule
				= std::make_shared<CImageInstanceModule>();
			
			std::shared_ptr<CImagePlanModule> imagePlanModule
				= std::make_shared<CImagePlanModule>();

			imagePlanModule->SetReferenceImageInstance(instanceModule);
			instanceModule->SetImagePlan(imagePlanModule);

			instanceModule->SetReferenceSeries(seriesModule);

			instanceModule->SetInstanceUID(strValue);
			
			// Image Instance
			strValue = NULL_SYMBAL;
			if (dataset->findAndGetString(DCM_InstanceNumber, strValue).good()
				&& strValue != NULL_SYMBAL)
				instanceModule->SetInstanceNumber(atoi(strValue));

			instanceModule->SetFilePath(aPath);

			seriesModule->AddInstanceModule(instanceModule);

			// Image Plan
			nValue16 = 0;
			if (dataset->findAndGetUint16(DCM_Rows, nValue16).good())
				imagePlanModule->SetHeight(nValue16);

			nValue16 = 0;
			if (dataset->findAndGetUint16(DCM_Columns, nValue16).good())
				imagePlanModule->SetWidth(nValue16);

			OFString strDcmValue;
			if(dataset->findAndGetOFStringArray(DCM_PixelSpacing, strDcmValue).good()
				&& strDcmValue.length() > 3)
			{
				sscanf_s(strDcmValue.c_str(), "%lf\\%lf",
					&imagePlanModule->GetPixelSpacingX(),
					&imagePlanModule->GetPixelSpacingY());
			}

			strValue = NULL_SYMBAL;
			if (dataset->findAndGetString(DCM_SpacingBetweenSlices, strValue).good()
				&& strValue != NULL_SYMBAL)
				imagePlanModule->SetSpacingBetweenSlice(atof(strValue));

			strValue = NULL_SYMBAL;
			if (dataset->findAndGetString(DCM_SliceThickness, strValue).good()
				&& strValue != NULL_SYMBAL)
				imagePlanModule->SetSliceThickness(atof(strValue));

			nValue16 = 0;
			if (dataset->findAndGetUint16(DCM_NumberOfFrames, nValue16).good()
				&& strValue != NULL_SYMBAL)
				imagePlanModule->SetFrameCount(nValue16);

			nValue16 = 0;
			if (dataset->findAndGetUint16(DCM_PixelRepresentation, nValue16).good())
				imagePlanModule->SetIsSigned(nValue16 > 0);

			nValue16 = 0;
			if (dataset->findAndGetUint16(DCM_BitsStored, nValue16).good())
				imagePlanModule->SetIsSampleWord(nValue16 > 8);
			

			strValue = NULL_SYMBAL;
			if (dataset->findAndGetString(DCM_PhotometricInterpretation, strValue).good()
				&& strValue != NULL_SYMBAL)
				imagePlanModule->SetPhotometricInterpretation(
					ParseTextToPhotometricInterpretation(strValue));

			strValue = NULL_SYMBAL;
			if (dataset->findAndGetString(DCM_RescaleIntercept, strValue).good()
				&& strValue != NULL_SYMBAL)
				imagePlanModule->SetIntercept(atof(strValue));

			strValue = NULL_SYMBAL;
			if (dataset->findAndGetString(DCM_RescaleSlope, strValue).good()
				&& strValue != NULL_SYMBAL)
				imagePlanModule->SetSlope(atof(strValue));

			strDcmValue.clear();
			if(dataset->findAndGetOFStringArray(DCM_ImagePositionPatient, strDcmValue).good()
				&& strDcmValue.length() > 3)
			{
				sscanf_s(strDcmValue.c_str(), ImagePositionFormatText,
					&imagePlanModule->GetPosition().x,
					&imagePlanModule->GetPosition().y,
					&imagePlanModule->GetPosition().z);
			}

			strDcmValue.clear();
			if(dataset->findAndGetOFStringArray(DCM_ImageOrientationPatient, strDcmValue).good()
				&& strDcmValue.length() > 3)
			{
				sscanf_s(strDcmValue.c_str(), ImageOrientationFormatText,
					&imagePlanModule->GetOrientationX().x,
					&imagePlanModule->GetOrientationX().y,
					&imagePlanModule->GetOrientationX().z,
					&imagePlanModule->GetOrientationY().x,
					&imagePlanModule->GetOrientationY().y,
					&imagePlanModule->GetOrientationY().z);
			}

			strDcmValue.clear();
			if (dataset->findAndGetOFStringArray(DCM_WindowCenter, strDcmValue).good()
				&& strDcmValue.length() > 0)
				imagePlanModule->SetWindowCenter(atof(strDcmValue.c_str()));

			strDcmValue.clear();
			if (dataset->findAndGetOFStringArray(DCM_WindowWidth, strDcmValue).good()
				&& strDcmValue.length() > 0)
				imagePlanModule->SetWindowWidth(atof(strDcmValue.c_str()));

			imagePlanModule->SetOneFramePixelSize(imagePlanModule->GetWidth() * imagePlanModule->GetHeight());
			imagePlanModule->SetOneFramePixelSizeInBytes(
				imagePlanModule->GetIsSampleWord()
				? 2 * imagePlanModule->GetOneFramePixelSize()
				: imagePlanModule->GetOneFramePixelSize());

			const UINT8 *pImageData = NULL_SYMBAL;
			if(dataset->findAndGetUint8Array(DCM_PixelData, pImageData).good()
				&& pImageData != NULL_SYMBAL)
			{
				auto pixelSize = imagePlanModule->GetOneFramePixelSizeInBytes() * imagePlanModule->GetFrameCount();

				imagePlanModule->SetAllFramePixels(std::shared_ptr<unsigned char>(new unsigned char[pixelSize], std::default_delete<unsigned char[]>()));

				memcpy_s(imagePlanModule->GetAllFramePixels().get(), pixelSize, pImageData, pixelSize);

				imagePlanModule->SetFrameIndex(CImagePlanModule::DefaultFrameIndex);
			}
		}

		// Combine Modality
		/*CString strModalities, strBodyParts;

		for(CStudyModule::Series_Array_Type::iterator itBegin = studyModule->GetSeries().begin();
			itBegin != studyModule->GetSeries().end();
			++itBegin)
		{
			auto modality = ModalityText((*itBegin)->GetModality());
			if (strModalities.Find(modality) < 0)
			{
				strModalities.Append("/");
				strModalities.Append(modality);
			}

			if(strBodyParts.Find((*itBegin)->GetBodyPart().c_str()) < 0)
			{
				strBodyParts.Append("/");
				strBodyParts.Append((*itBegin)->GetBodyPart().c_str());
			}
		}

		studyModule->SetModalitys(strModalities.GetString());
		studyModule->SetBodyParts(strBodyParts.GetString());*/
	}

	std::vector<std::shared_ptr<CStudyModule>> studyModules;

	for(decltype(result)::iterator itBegin = result.begin();
		itBegin != result.end();
		++itBegin)
	{
		studyModules.push_back(itBegin->second);
	}

	return studyModules;
}

