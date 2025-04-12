#include "stdafx.h"
#include "ViewModels/ImageFrameViewModel/ImageFrameViewModel.h"
#include "Foundations/Common/FileUtility.h"
#include "utilities/Configuration/Configuration.h"
#include "utilities/ImageModule/ModuleHelper.h"
#include "utilities/ImageModule/Service/DicomLoaderService.h"

CImageFrameViewModel CImageFrameViewModel::m_Instance;


std::shared_ptr<CStudyTabItemViewModel> CImageFrameViewModel::LoadStudy(CStudyDto& studyDto)
{
	char strStoragePath[MAX_PATH] = { 0 };

	PathCombine(strStoragePath, CConfiguration::GetInstance().GetSystemInfo().GetStoragePath().c_str(),
		studyDto.GetStudyInstanceUID().c_str());

	/*CStringList filePaths;
	CFileUtility::SearchPaths([&](const char* aFilePath) -> void
	{
		filePaths.AddTail(aFilePath);
	}, strStoragePath, "dcm");

	if (filePaths.IsEmpty())
		return NULL_SYMBAL;*/

	LoadStudy(strStoragePath);

	return GetStudyTabItem(studyDto.GetStudyInstanceUID().c_str());
}


void CImageFrameViewModel::LoadStudy(const char* strStudyUID)
{
	const auto study = CDicomLoaderService::LoadStudy(strStudyUID);

	if (!study)
		return;

	GenerateStudyTabItem(study);
}

std::shared_ptr<CStudyTabItemViewModel> CImageFrameViewModel::GetStudyTabItem(const char* strStudyUID)
{
	for (auto element : GetStudyTabItemViewModels())
	{
		if (element->IsMatchStudyUID(strStudyUID))
			return element;
	}

	return NULL_SYMBAL;
}

void CImageFrameViewModel::ReleaseStudyTabItem(const char* strStudyUID)
{
	auto itBegin = GetStudyTabItemViewModels().begin();
	const StudyTabItemViewModelsType::iterator itEnd = GetStudyTabItemViewModels().end();

	while (itBegin != itEnd)
	{
		if ((*itBegin)->IsMatchStudyUID(strStudyUID))
		{
			GetStudyTabItemViewModels().erase(itBegin);

			CDicomLoaderService::ReleaseStudy(strStudyUID);

			break;
		}

		++itBegin;
	}
}

void CImageFrameViewModel::ReleaseStudyTabItem(std::shared_ptr<CStudyTabItemViewModel> studyTab)
{
	ReleaseStudyTabItem(studyTab->GetStudy()->GetStudyInstanceUID().c_str());
}

std::shared_ptr<CStudyTabItemViewModel> CImageFrameViewModel::GenerateStudyTabItem(std::shared_ptr<CStudyModule> study)
{
	if (!study)
		return NULL_SYMBAL;

	auto tabItem = GetStudyTabItem(study->GetStudyInstanceUID().c_str());

	if (!tabItem)
	{
		// 创建新的TabItem
		tabItem = std::make_shared<CStudyTabItemViewModel>();

		tabItem->SetStudy(study);

		GetStudyTabItemViewModels().push_back(tabItem);
	}

	tabItem->ReloadStudy();

	return tabItem;
}
