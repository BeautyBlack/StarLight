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

#pragma once

#include <Foundations/Common/ToDefine.h>
#include <map>
#include <memory>
#include <unordered_map>

#include <vector>
#include <Utilities/ImageModule/Modules/StudyModule.h>

#include <utilities/ImageModule/ExportFlagDefine.h>

template<class T>
class UTILITIESIMAGEMODULE_API CModuleHelper
{
public:
	virtual void AddItem(STRING_TYPE& strKey, std::shared_ptr<T> item);
	
	virtual void AddItem(const char* strKey, std::shared_ptr<T> item);
	
	virtual void RemoveItem(STRING_TYPE& strKey);
	virtual void RemoveItem(const char *strKey);

	std::shared_ptr<T>& operator[](STRING_TYPE& strKey);
	
	std::shared_ptr<T>& operator[](int nIndex) = delete;
	
private:
	using MAP_TYPE = std::unordered_map<STRING_TYPE, std::shared_ptr<T>>;
	
	AUTO_PROPERTY_REF(CacheMap, MAP_TYPE);
};

class UTILITIESIMAGEMODULE_API CStudyModuleHelper : public CModuleHelper<CStudyModule>
{
public:

	void AddItem(STRING_TYPE& strKey, std::shared_ptr<CStudyModule> item) override;

	void AddItem(const char* strKey, std::shared_ptr<CStudyModule> item) override;

	
	/*
	 * Description
	 * 获取Study
	 */
	std::shared_ptr<CStudyModule> GetStudy(STRING_TYPE& studyUid);
	
	/*
	 * Description
	 * 获取Study
	 */
	std::shared_ptr<CStudyModule> GetStudy(const char* studyUid);

	/*
	 * Description
	 * 释放Study
	 */
	void RemoveItem(STRING_TYPE& studyUid) override;
	void RemoveItem(const char *strKey) override;

private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CStudyModuleHelper);

	using REFERENCE_COUNT_MAP_TYPE = std::unordered_map<STRING_TYPE, int>;
	AUTO_PROPERTY_REF(CacheReferenceCoutMap, REFERENCE_COUNT_MAP_TYPE);
};


