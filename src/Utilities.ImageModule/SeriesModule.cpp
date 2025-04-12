#include "stdafx.h"
#include <Utilities/ImageModule/Modules/SeriesModule.h>

CSeriesModule::CSeriesModule()
{
	SetSeriesNumber(1);
	SetModality(Modality::UnkownModality);
	ZeroMemory(&GetSeriesDateTime(), sizeof(SYSTEMTIME));
}


CSeriesModule::~CSeriesModule()
{
}

std::shared_ptr<CImageInstanceModule> CSeriesModule::GetInstance(const char* strInstanceUID)
{
	InstanceArrayType::iterator itBegin = GetInstances().begin();
	InstanceArrayType::iterator itEnd = GetInstances().end();

	while (itBegin != itEnd)
	{
		if ((*itBegin)->GetInstanceUID().compare(
			strInstanceUID) == 0)
			return (*itBegin);

		++itBegin;
	}

	return NULL_SYMBAL;
}

void CSeriesModule::AddInstanceModule(std::shared_ptr<CImageInstanceModule> instanceModule)
{
	InstanceArrayType::iterator itBegin = GetInstances().begin();
	InstanceArrayType::iterator itEnd = GetInstances().end();

	while (itBegin != itEnd)
	{
		if ((*itBegin)->GetInstanceNumber() >= instanceModule->GetInstanceNumber())
			break;

		++itBegin;
	}

	GetInstances().insert(itBegin, instanceModule);
}
