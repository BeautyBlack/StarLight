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


#ifndef DICOM_RECORD_SERVICE_8A61BED3151E4B28B9544F048A99BDA8
#define DICOM_RECORD_SERVICE_8A61BED3151E4B28B9544F048A99BDA8

#pragma once

#include <Foundations/Interfaces/ServiceBase.h>

#include <Foundations/Common/ToDefine.h>


#ifdef UTILITIESIMAGEMODULE_EXPORTS
#define UTILITIESIMAGEMODULE_API __declspec(dllexport)
#else
#define UTILITIESIMAGEMODULE_API __declspec(dllimport)
#endif

class UTILITIESIMAGEMODULE_API CDicomRecordService : public CServiceBase
{
public:
	CDicomRecordService();
	~CDicomRecordService();


	/*
	 * Description
	 * 初始化
	 * @pRequest [CServiceRequestBase*] 初始化参数
	 * @return [bool] true : 成功; false : 失败
	 */
	bool Initialize(std::shared_ptr<CServiceRequest> request) override;

	/*
	 * Description
	 * 启动Service
	 */
	bool Start() override;

	/*
	 * Description
	 * 停止Service, 释放资源
	 */
	bool Release() override;

	///////////  Actions   /////////////////

	/*
	 * Description
	 * 处理
	 */
	bool Process() override;

	/*
	 * Description
	 * 重置
	 */
	bool Reset() override;
};

#endif
