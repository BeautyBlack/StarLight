#include "stdafx.h"
#include <ViewModels/PatientListFrameViewModel/PatientListFrameViewModel.h>
#include "utilities/Database/DatabaseManager.h"

CPatientListFrameViewModel CPatientListFrameViewModel::m_Instance;

void CPatientListFrameViewModel::Query()
{
	auto studies = CDatabaseManager::GetInstance().QueryStudies(GetStudyFilter());

	GetPatientListTableViewModel().UpdatePatients(studies);
}
