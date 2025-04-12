#include "stdafx.h"
#include <Utilities/ImageModule/ModuleHelper.h>

CStudyModuleHelper CStudyModuleHelper::m_Instance;

template <class T>
void CModuleHelper<T>::AddItem(STRING_TYPE& strKey, std::shared_ptr<T> item)
{
	AddItem(strKey.c_str(), item);
}

template <class T>
void CModuleHelper<T>::AddItem(const char* strKey, std::shared_ptr<T> item)
{
	GetCacheMap().insert_or_assign(strKey, std::move(item));
}

template <class T>
void CModuleHelper<T>::RemoveItem(STRING_TYPE& strKey)
{
	GetCacheMap().erase(strKey);
}

template <class T>
void CModuleHelper<T>::RemoveItem(const char *strKey)
{
	STRING_TYPE t = strKey;

	RemoveItem(t);
}

template <class T>
std::shared_ptr<T>& CModuleHelper<T>::operator[](STRING_TYPE& strKey)
{
	return GetCacheMap()[strKey];
}

void CStudyModuleHelper::AddItem(STRING_TYPE& strKey, std::shared_ptr<CStudyModule> item)
{
	AddItem(strKey.c_str(), item);
}

void CStudyModuleHelper::AddItem(const char* strKey, std::shared_ptr<CStudyModule> item)
{
	CModuleHelper<CStudyModule>::AddItem(strKey, item);
	GetCacheReferenceCoutMap().insert_or_assign(strKey, 1);
}

std::shared_ptr<CStudyModule> CStudyModuleHelper::GetStudy(STRING_TYPE& studyUid)
{
	return GetStudy(studyUid.c_str());
}

std::shared_ptr<CStudyModule> CStudyModuleHelper::GetStudy(const char* studyUid)
{
	if (GetCacheMap().find(studyUid) == GetCacheMap().end())
		return std::shared_ptr<CStudyModule>(NULL_SYMBAL);
	
	auto studyModule = GetCacheMap()[studyUid];

	const auto nCount = GetCacheReferenceCoutMap()[studyUid];

	GetCacheReferenceCoutMap()[studyUid] = nCount + 1;
	
	return studyModule;
}

void CStudyModuleHelper::RemoveItem(STRING_TYPE& studyUid)
{
	if (GetCacheMap().find(studyUid) == GetCacheMap().end())
		return ;

	const auto nCount = GetCacheReferenceCoutMap()[studyUid];

	if(nCount <= 1)
	{
		GetCacheReferenceCoutMap().erase(studyUid);

		CModuleHelper<CStudyModule>::RemoveItem(studyUid);
	}
	else
	{
		GetCacheReferenceCoutMap()[studyUid] = nCount - 1;
	}
}

void CStudyModuleHelper::RemoveItem(const char* strKey)
{
	STRING_TYPE t = strKey;

	RemoveItem(t);
}
