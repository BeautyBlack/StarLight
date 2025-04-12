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

#ifndef MPR_SERVICE_C4EE51D6438442EDB5385BC0593D352C
#define MPR_SERVICE_C4EE51D6438442EDB5385BC0593D352C

#pragma once

#include <Services/Mpr/ExportFlagDefine.h>

#include <Foundations/Interfaces/ServiceBase.h>

#include <Foundations/Common/ToDefine.h>

#include <Services/Mpr/ExportFlagDefine.h>
#include <Services/Mpr/MprParameter.h>
#include <Services/Mpr/MprResult.h>

#include <vtk-9.0/vtkImageData.h>
#include <vtk-9.0/vtkMatrix4x4.h>

/*
 * MPR Service
 */
class SERVICEMPR_API CMprService : public CServiceBase, public CMessengerOwner
{
public:
	CMprService();
	~CMprService();

	/*
	 * Description
	 * ��ʼ��
	 * @request [CServiceRequest&] Ӱ�����
	 * @return [bool] true : �ɹ�; false : ʧ��
	 */
	virtual bool Initialize(std::shared_ptr<CServiceRequest> request);

	/*
	 * Description
	 * ����Service
	 */
	virtual bool Start();

	/*
	 * Description
	 * ֹͣService, �ͷ���Դ
	 */
	virtual bool Release();

	///////////  Actions   /////////////////

	/*
	 * Description
	 * ����ͼ��
	 */
	virtual bool Process();

	/*
	 * Description
	 * ����
	 */
	virtual bool Reset();

private:
	void LoadImages();

	/**
	 * ����������
	 */
	virtual void CreateVolume();

	void ResetActionParameters();
	
	void Reconstruct(ImagePlanDirection direction);
	void Reconstruct2(ImagePlanDirection direction);

	void ComputeScoutLine();

	void ComputeScoutLine(
		std::shared_ptr<CMprResult> mprResult,
		CMprParameterItem &paramItem);

	std::shared_ptr<CMprResult> GetMprResult(ImagePlanDirection direction);

	sctPoint3D ProjectToImagePosition(double volumePosition[3]);
	sctPoint3D ProjectToImagePosition(const sctPoint3D &volumePosition);

	sctPoint3D ProjectToVtkVolumePosition(double imagePosition[3]);
	sctPoint3D ProjectToVtkVolumePosition(sctPoint3D &imagePosition);

private:

	AUTO_PROPERTY_REF(Study, std::shared_ptr<CStudyModule>);

	AUTO_PROPERTY_REF(VtkImageData, std::shared_ptr<vtkImageData>);

	AUTO_PROPERTY_REF(RequestParam, std::shared_ptr<CServiceRequest>);

	double m_firstImageMatrix[4][4];
	
	double m_zVector[3];

	double m_orientCenter[3];

	AUTO_PROPERTY_REF(TranseverseImageData, std::shared_ptr<CMprResult>);
	AUTO_PROPERTY_REF(SagitalImageData, std::shared_ptr<CMprResult>);
	AUTO_PROPERTY_REF(CoronalImageData, std::shared_ptr<CMprResult>);

	/*
	 * �������
	 */
	AUTO_PROPERTY(MprParameter, std::shared_ptr<CMprParameter>);
};

#endif
