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

#ifndef TO_DEFINE_0A0F2E10B5EE4AEE8FA391C3045B9492
#define TO_DEFINE_0A0F2E10B5EE4AEE8FA391C3045B9492

#pragma once

#include <functional>
#include <string>

#define NULL_SYMBAL nullptr
#define MAX_PATH_SIZE 1024

using STRING_TYPE = std::string;
using STRING_TYPE_REF = std::string&;
using STRING_TYPE_CONST_REF = const std::string&;

using BYTE_TYPE = unsigned char;

static const char* EMPTY_STRING = "";

static std::function<STRING_TYPE(const char*, int)> ConvertTextToQTTextFunc;

	// 32位对齐宽度
#define WIDTHSTEP(width, bitcount)	(((( (width) * (bitcount) ) + 31) & ~31) >> 3) 

	/*
 * 定义类属性
 */
#define AUTO_PROPERTY(name, type) public:                                      \
                                  void Set##name(type value) {m_##name = value;} \
                                  type Get##name() { return m_##name;}           \
								  private:                                     \
								  type m_##name;

#define AUTO_PROPERTY_REF(name, type) public:                                      \
										void Set##name(type value) {m_##name = value;} \
										type& Get##name() { return m_##name;}           \
									  private:                                     \
										type m_##name;

#define AUTO_PROPERTY_REF_NO_SET(name, type) public:                                      \
										type& Get##name() { return m_##name;}           \
									  private:                                     \
										type m_##name;

#define AUTO_STATIC_PROPERTY_REF_NO_SET(name, type) public:                                      \
	static type& Get##name() { return m_##name;}           \
									  private:                                     \
									  static type m_##name;

#define AUTO_STATIC_PROPERTY_REF(name, type) public:                                      \
	static type& Get##name() { return m_##name;}           \
	static void Set##name(type value) {m_##name = value;} \
									  private:                                     \
									  static type m_##name;

#define AUTO_PROPERTY_REF_WITH_JSON_KEY(name, type) public:                                      \
										void Set##name(type value) {m_##name = value;} \
										type& Get##name() { return m_##name;}           \
										static STRING_TYPE& Get##name##JsonKey() { static STRING_TYPE key = #name; return key;} \
									  private:                                     \
										type m_##name;


#define AUTO_PROPERTY_REF_NO_SET_WITH_JSON_KEY(name, type) public:                                      \
										type& Get##name() { return m_##name;}           \
										static STRING_TYPE& Get##name##JsonKey() { static STRING_TYPE key = #name; return key;} \
									  private:                                     \
										type m_##name;

#define AUTO_PROPERTY_REF_WITH_JSON_KEY_AND_DESC(name, type, desc) public:                                      \
										void Set##name(type value) {m_##name = value;} \
										type& Get##name() { return m_##name;}           \
										static STRING_TYPE& Get##name##JsonKey() { static STRING_TYPE key = #name; return key;} \
										static STRING_TYPE& Get##name##Desc() { static STRING_TYPE key = #desc; return key;} \
									  private:                                     \
										type m_##name;

#define AUTO_PROPERTY_REF_WITH_JSON_KEY_INDEX_AND_DESC(name, type, desc, index) public:                                      \
										void Set##name(type value) {m_##name = value;} \
										type& Get##name() { return m_##name;}           \
										static STRING_TYPE& Get##name##JsonKey() { static STRING_TYPE key = #name; return key;} \
										static STRING_TYPE& Get##name##Desc() { static STRING_TYPE key = #desc; return key;} \
										static const int name##Index = index; \
									  private:                                     \
										type m_##name;

#define AUTO_PROPERTY_REF_NO_SET_WITH_JSON_KEY_AND_DESC(name, type, desc) public:                                      \
										type& Get##name() { return m_##name;}           \
										static STRING_TYPE& Get##name##JsonKey() { static STRING_TYPE key = #name; return key;} \
										static STRING_TYPE& Get##name##Desc() { static STRING_TYPE key = #desc; return key;} \
									  private:                                     \
										type m_##name;


	/*
 * Dicom 日期格式
 */
	static const char DicomDateFormatText[] = "YYYYMMDD";
static const int DicomDateFormatTextLength = sizeof(DicomDateFormatText) - 1;

/*
 * Dicom 时间格式
 */
static const char DicomTimeFormatText[] = "HHmmss";
static const int DicomTimeFormatTextLength = sizeof(DicomTimeFormatText) - 1;

static const char* NormalDatetimeFormatText = "%d-%d-%d %d:%d:%d";

/*
* Description
* Image Position 字符串格式
*/
static const char* ImagePositionFormatText = R"(%lf\%lf\%lf)";

/*
 * Description
 * Image Orient 字符串格式
 */
static const char* ImageOrientationFormatText = R"(%lf\%lf\%lf\%lf\%lf\%lf)";

#endif
