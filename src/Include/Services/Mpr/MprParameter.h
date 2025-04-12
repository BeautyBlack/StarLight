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

#ifndef MPR_PARAMETER_5287DF42BCB04021B69067F019C60DE7
#define MPR_PARAMETER_5287DF42BCB04021B69067F019C60DE7

#pragma once

#include <Foundations/Interfaces/ServiceParameterBase.h>
#include <Foundations/Interfaces/ImageSlabMode.h>

#include <Foundations/Common/ToDefine.h>
#include <list>
#include <Services/Mpr/ExportFlagDefine.h>
#include <vtk-9.0/vtkMatrix4x4.h>
#include <vtk-9.0/vtkSmartPointer.h>


#include "utilities/Messenger/Messenger.h"

class CMprParameter;
class MPRPlane;
typedef std::shared_ptr<MPRPlane> MPRPlanePtr;

/*
 * Description
 * ��״�桢ʸ״�桢�������ĳһ�����MPR��λ�߲���
 */
class SERVICEMPR_API CMprParameterItem : public CMessengerOwner
{
public:
	CMprParameterItem();
	CMprParameterItem(const CMprParameterItem& copySrc);

	CMprParameterItem& operator=(const CMprParameterItem& copySrc);
	
	/*
	 * Description
	 * ���ö�λ�����ĵ�
	 */
	void MoveCenter(sctPoint3D center);

	/*
	 * Description
	 * ���ĵ����ű�ƽ�淨�߷��������ݼ�offset����λ
	 * @offset : int, ��offsetΪ������ʱ,���ĵ����offset����λ����֮���ĵ�ݼ�offset����λ
	 */
	void SetCurrentScrollSlicePosition(int offset);

	/**
	 * ��ȡ��ǰ���ĵ�����ڱ��ӷ�����������λ��
	 */
	int GetCurrentScrollSlicePosition();
	
	/*
	 * Description
	 * ѡ��λ��X�ض��ĽǶ�
	 * @dbRadius double, ��ת�Ƕȣ���λ������
	 */
	void RotateNormalX(double dbRadius);

	/*
	 * Description
	 * ѡ��λ��Z�ض��ĽǶ�
	 * @dbRadius double, ��ת�Ƕȣ���λ������
	 */
	void RotateNormalZ(double dbRadius);

	/*
	 * Description
	 * CPR ·����
	 */
	void CPRPath(std::list<sctPoint3D> path);

	/*
	 * Description
	 * �����ؽ����,����MPR�ݻ�ģʽ
	 * ����ʹ��Use��ͷ��Ϊ���ԱAUTO_PROPERTY���Զ�������SetSliceThickness����,����Use��ͷ����ͬ��
	 * @dbThickness double, �ؽ����
	 */
	void SetSliceThickness(double dbThickness);

	/**
	 * �����ؽ���Ƭ�������
	 */
	vtkSmartPointer<vtkMatrix4x4> GenerateVtkResliceMatrix();

	void SetSourcePixelSpacingX(double spacing);

	void SetSourcePixelSpacingY(double spacing);

	void InitVolume(double dOriginal[3], double dx[3], double dy[3], double dz[3],
		double dspace[3],
		int cx, int cy, int cz);

	void SetCenter(const sctPoint3D &point);
	
private:
	void CopyFrom(const CMprParameterItem& copySrc);

	void Rotate(double dbAngle, bool isNormalX);

	int GetSlicePositionStart() const;
	
	int GetSlicePositionEnd() const;
	
private:
	/*
	 * Parameter��ƽ�淽��
	 */
	AUTO_PROPERTY(PlanDirection, ImagePlanDirection);

	/*
	 * ������λ��X/Z�����ĵ�
	 */
	AUTO_PROPERTY_REF_NO_SET(Center, sctPoint3D);

	/*
	 * ��λ��X
	 */
	AUTO_PROPERTY_REF(NormalX, sctNormal3D);

	/*
	 * ��λ��X �� ��λ��Z�ķ��ߣ�Ҳ���ع������淨��
	 * ��λ��X �� ��λ��Z��һ����������ϵ
	 */
	AUTO_PROPERTY_REF(NormalY, sctNormal3D);

	/*
	 * ��λ��Z
	 */
	AUTO_PROPERTY_REF(NormalZ, sctNormal3D);

	/**
	 * �ӷ���������
	 */
	AUTO_PROPERTY_REF(SlicePositionStart, int);

	/**
	 * �ӷ������
	 */
	AUTO_PROPERTY_REF(SlicePositionEnd, int);

	/*
	 * �ؽ���ʽ
	 * ���ؽ������1����λʱ,���ؽ���ʽ���һ����
	 * ��ʹ���ݻ����ʽʱ,��ͬ�ؽ���ʽ����᲻һ��
	 */
	AUTO_PROPERTY_REF(ImageSlabMode, ImageSlabMode);

	AUTO_PROPERTY_REF(SlabSliceCount, int);

	/*
	 * ԭʼ���е�Pixel Spacing X
	 */
	AUTO_PROPERTY_REF_NO_SET(SourcePixelSpacingX, double);

	/*
	 * ԭʼ���е�Pixel Spacing Y
	 */
	AUTO_PROPERTY_REF_NO_SET(SourcePixelSpacingY, double);

	/*
	 * ԭʼ���еĲ���
	 */
	AUTO_PROPERTY(SourceSpacingBetweenSlice, double);

	/**
	 * ���ؿ�
	 */
	AUTO_PROPERTY_REF(VolumeWidth, int);

	/**
	 * ���س�
	 */
	AUTO_PROPERTY_REF(VolumeHeight, int);

	/**
	 * ���ظ�
	 */
	AUTO_PROPERTY_REF(VolumeSliceCount, int);

	/**
	 * ������������X������Сֵ
	 */
	AUTO_PROPERTY_REF(BoundX, int);

	/**
	 * ������������X�������ֵ
	 */
	AUTO_PROPERTY_REF(BoundX2, int);

	/**
	 * ������������Y������Сֵ
	 */
	AUTO_PROPERTY_REF(BoundY, int);

	/**
	 * ������������Y�������ֵ
	 */
	AUTO_PROPERTY_REF(BoundY2, int);

	/**
	 * ������������Z������Сֵ
	 */
	AUTO_PROPERTY_REF(BoundZ, int);

	/**
	 * ������������Z�������ֵ
	 */
	AUTO_PROPERTY_REF(BoundZ2, int);

	AUTO_PROPERTY(MPRPlanePtr, MPRPlanePtr);
};

/*
 * Description
 * MPR ����
 */
class SERVICEMPR_API CMprParameter : public CServiceParameterBase /*, public CMessengerOwner*/
{
public:
	CMprParameter();
	~CMprParameter();

	inline CMprParameterItem& GetSagittalParameter();

	inline CMprParameterItem& GetTransverseParameter();

	inline CMprParameterItem& GetCoronalParameter();

	CMprParameterItem& GetParameter(ImagePlanDirection direction);

	/**
	 * �������ĵ�
	 */
	void SetCenter(sctPoint3D &newCenter);

	/**
	 * �����������ĵ�
	 */
	void SetOrientImageCenter(sctPoint3D &newCenter);

	/**
	 * ����
	 */
	void Reset();

	/**
	 * ��ת
	 */
	void Rotate(ImagePlanDirection direction, double dbAngle, double normal[3]);

	/**
	 * �������س����
	 */
	void SetImageBound(int nWidth, int nHeight, int nSliceCount);

	/**
	 * ����������������߽�ֵ
	 */
	void SetVolumeBound(int x, int x2, int y, int y2, int z, int z2);

	void Initialize() override;
	
protected:
	void InitializeMessenger();
	
private:
	using ParameterItemMap = std::map<ImagePlanDirection, CMprParameterItem>;

	/*
	 * ʸ״��/��״��/����� MPR ����
	 * �������治��һֱ�����������ϵ
	 * ����ĳһ����ת�˸���Ķ�λ�ߺ�,��������Ͳ�һ���Ǹ�������Ϊ������ϵ
	 */
	AUTO_PROPERTY_REF_NO_SET(Parameters, ParameterItemMap);

	/**
	 * ������ת��Ĺ�ͬ���ĵ�����
	 * ��ǰ����ת��λ������
	 */
	AUTO_PROPERTY_REF_NO_SET(Center, sctPoint3D);

	/**
	 * ��ʼ��λ�����ĵ�
	 */
	AUTO_PROPERTY_REF_NO_SET(OrientImageCenter, sctPoint3D);
};


#endif

