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

#ifndef VR_SERVICE_E29BCEE254FB45AC823F5873D9E3BB08
#define VR_SERVICE_E29BCEE254FB45AC823F5873D9E3BB08

#pragma once

#include <Services/VR/ExportFlagDefine.h>
#include <Services/VR/VRParameter.h>

#include <Foundations/Interfaces/ServiceBase.h>

#include <Foundations/Common/ToDefine.h>

#include <Services/VR/ExportFlagDefine.h>

#include <vtk-9.0/vtkImageData.h>
#include <vtk-9.0/vtkMatrix4x4.h>
#include <vtk-9.0/vtkRenderWindow.h>
#include <vtk-9.0/vtkVolume.h>
#include <vtk-9.0/vtkVolumeMapper.h>
#include <vtk-9.0/vtkRenderer.h>
#include <vtk-9.0/vtkPiecewiseFunction.h>
#include <vtk-9.0/vtkColorTransferFunction.h>
#include <vtk-9.0/vtkInteractorStyle.h>
#include <vtk-9.0/vtkGPUVolumeRayCastMapper.h>
#include <vtk-9.0/vtkRenderWindowInteractor.h>
#include <vtk-9.0/vtkVolumeProperty.h>
#include <vtk-9.0/vtkAnnotatedCubeActor.h>
#include <vtk-9.0/vtkOrientationMarkerWidget.h>
#include <vtk-9.0/vtkTextActor.h>

#include <vtk-9.0/vtkOpenGLGPUVolumeRayCastMapper.h>

#include "Foundations/Interfaces/Services/VRServiceRequest.h"

class VR_SERVICEMPR_API CVRService : public CServiceBase, public CMessengerOwner
{
public:
	CVRService();
	~CVRService();

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

	void UpdateColor();
	void ManualRender();

private:
	void LoadImages();

	void Reconstruct();

	void CreateVtkComponent();

	void InitAnnotatedCubeActor();

	void InitDisplayPatientInfo();

	void UpdateNon3DVolumeModeImgPosInfo();

	/**
	 * 创建体数据
	 */
	virtual void CreateVolume();

	void ResetActionParameters();

	sctPoint3D ProjectToImagePosition(double volumePosition[3]);
	sctPoint3D ProjectToImagePosition(const sctPoint3D &volumePosition);

	sctPoint3D ProjectToVtkVolumePosition(double imagePosition[3]);
	sctPoint3D ProjectToVtkVolumePosition(sctPoint3D &imagePosition);

	inline double CTValueToImagePixel(int value, double slop, double intercept);
	inline double ImagePixelToCTValue(int pixel, double slop, double intercept);
private:

	AUTO_PROPERTY_REF(Study, std::shared_ptr<CStudyModule>);

	AUTO_PROPERTY_REF(VtkImageData, std::shared_ptr<vtkImageData>);

	AUTO_PROPERTY_REF(RequestParam, std::shared_ptr<CVRServiceRequest>);

	double m_firstImageMatrix[4][4];

	double m_zVector[3];

	double m_orientCenter[3];

	/*
	 * 处理参数
	 */
	AUTO_PROPERTY(VRParameter, std::shared_ptr<CVRParameter>);

	AUTO_PROPERTY(RenderWindow, vtkRenderWindow*);

	AUTO_PROPERTY_REF(Renderer, vtkRenderer*);

	AUTO_PROPERTY_REF(Volume, vtkVolume*);

	AUTO_PROPERTY_REF(PatientNameText, vtkTextActor*);

	AUTO_PROPERTY_REF(VolumeProperty, vtkVolumeProperty*);
	
	AUTO_PROPERTY_REF(VolumeMapper, vtkOpenGLGPUVolumeRayCastMapper*);

	AUTO_PROPERTY_REF(PiecewiseFunction, vtkPiecewiseFunction*);

	AUTO_PROPERTY_REF(ColorTransferFunction, vtkColorTransferFunction*);

	AUTO_PROPERTY_REF(Iterator, vtkRenderWindowInteractor*);
	
	AUTO_PROPERTY_REF(InteractorStyle, vtkInteractorStyle*);

	AUTO_PROPERTY_REF(AnnotatedCubeActor, vtkAnnotatedCubeActor*);
	AUTO_PROPERTY_REF(OrientationMarkerWidget, vtkOrientationMarkerWidget*);
};


#endif
