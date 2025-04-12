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

#ifndef PARONAMA_SERVICE_27716B7544024B508A2A113FC3F9FB2F
#define PARONAMA_SERVICE_27716B7544024B508A2A113FC3F9FB2F

#pragma once


#include "Foundations/Interfaces/ServiceBase.h"
#include "Services/Paronama/ParonamaResult.h"
#include "utilities/Messenger/Messenger.h"

#include "Services/Paronama/ExportFlagDefine.h"
#include "Services/Paronama/ParonamaParameter.h"

#include <vtk-9.0/vtkImageData.h>
#include <vtk-9.0/vtkMatrix4x4.h>

class PARONAMA_SERVICEMPR_API CParonamaService : public CServiceBase, public CMessengerOwner
{
public:
	CParonamaService();
	~CParonamaService();

	/*
	 * Description
	 * 初始化
	 * @request [CServiceRequest&] 影像参数
	 * @return [bool] true : 成功; false : 失败
	 */
	virtual bool Initialize(std::shared_ptr<CServiceRequest> request);

	/*
	 * Description
	 * 启动Service
	 */
	virtual bool Start();

	/*
	 * Description
	 * 停止Service, 释放资源
	 */
	virtual bool Release();

	///////////  Actions   /////////////////

	/*
	 * Description
	 * 处理图像
	 */
	virtual bool Process();

	/*
	 * Description
	 * 重置
	 */
	virtual bool Reset();

private:
	void LoadImages();

	std::shared_ptr<CParonamaResult> GetResult(ImagePlanDirection direction);
	
	/**
	 * 创建体数据
	 */
	virtual void CreateVolume();

	void ResetActionParameters();
	
	void Reconstruct(ImagePlanDirection direction);

	void ReconstructSaggital();
	void ReconstructTransverse();
	void ReconstructCoronal();
	void ReconstructParonama();
	
	void ComputeScoutLine();

	void ComputeScoutLine(
		std::shared_ptr<CParonamaResult> mprResult,
		CParonamaParameter &paramItem);

	/*void ComputeScoutLine();

	std::shared_ptr<CParonamaResult> GetMprResult(ImagePlanDirection direction);

	sctPoint3D ProjectToImagePosition(double volumePosition[3]);
	sctPoint3D ProjectToImagePosition(const sctPoint3D &volumePosition);

	sctPoint3D ProjectToVtkVolumePosition(double imagePosition[3]);
	sctPoint3D ProjectToVtkVolumePosition(sctPoint3D &imagePosition);
*/
private:

	AUTO_PROPERTY_REF(Study, std::shared_ptr<CStudyModule>);

	AUTO_PROPERTY_REF(VtkImageData, std::shared_ptr<vtkImageData>);

	AUTO_PROPERTY_REF(RequestParam, std::shared_ptr<CServiceRequest>);

	std::map<ImagePlanDirection, std::function<void(void)>> m_reconstructFuns;
	
	double m_firstImageMatrix[4][4];

	double m_zVector[3];

	double m_orientCenter[3];

	/*
	 * 处理参数
	 */
	AUTO_PROPERTY(ParonamaParameter, std::shared_ptr<CParonamaParameter>);

	AUTO_PROPERTY_REF(TransverseResult, std::shared_ptr<CParonamaResult>);
	AUTO_PROPERTY_REF(SaggitalResult, std::shared_ptr<CParonamaResult>);
	AUTO_PROPERTY_REF(CoronalResult, std::shared_ptr<CParonamaResult>);
	AUTO_PROPERTY_REF(ParonamaResult, std::shared_ptr<CParonamaResult>);
};

#endif

