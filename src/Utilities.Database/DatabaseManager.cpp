#include "pch.h"
#include <utilities/Database/DatabaseManager.h>

#include "Foundations/Common/FileUtility.h"
#include "utilities/Messenger/Messenger.h"
#include "utilities/Configuration/Configuration.h"
#include "Foundations/Interfaces/MineType.h"
#include "utilities/Database/OdbcDBOperator.h"

#define RECORD_FOLDER_PATH_NAME "StudyJsons"

CDatabaseManager CDatabaseManager::m_Instance;


void CDatabaseManager::RecordStudy(CStudyDto &studyDto)
{
	Initialize();

	EnterCriticalSection(&GetOperatorSection());

	if (GetOperator() != NULL_SYMBAL)
		GetOperator()->RecordStudy(studyDto);
	
	LeaveCriticalSection(&GetOperatorSection());
}

std::vector<CStudyDto> CDatabaseManager::QueryStudies(CQueryFilter& filter)
{
	Initialize();

	std::vector<CStudyDto> result;
	
	EnterCriticalSection(&GetOperatorSection());

	if (GetOperator() != NULL_SYMBAL)
		result = GetOperator()->QueryStudies(filter);
	
	LeaveCriticalSection(&GetOperatorSection());

	return result;
}

void CDatabaseManager::DeleteStudy(CStudyDto& studyDto)
{
	Initialize();

	EnterCriticalSection(&GetOperatorSection());

	if (GetOperator() != NULL_SYMBAL)
		GetOperator()->DeleteStudy(studyDto);

	LeaveCriticalSection(&GetOperatorSection());
}

void CDatabaseManager::Initialize()
{
	static bool hasInit = false;

	if (hasInit)
		return;

	hasInit = true;

	CoInitialize(NULL_SYMBAL);
	
	SetOperator(new COdbcDBOperator);

	InitializeCriticalSection(&GetOperatorSection());
}

CDatabaseManager::CDatabaseManager()
{
	SetOperator(NULL_SYMBAL);
}

CDatabaseManager::~CDatabaseManager()
{
	if(GetOperator() != NULL_SYMBAL)
	{
		delete GetOperator();
		
		DeleteCriticalSection(&GetOperatorSection());
		
		SetOperator(NULL_SYMBAL);
	}
}
