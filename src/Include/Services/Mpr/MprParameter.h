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
 * 冠状面、矢状面、横断面中某一个面的MPR定位线参数
 */
class SERVICEMPR_API CMprParameterItem : public CMessengerOwner
{
public:
	CMprParameterItem();
	CMprParameterItem(const CMprParameterItem& copySrc);

	CMprParameterItem& operator=(const CMprParameterItem& copySrc);
	
	/*
	 * Description
	 * 设置定位线中心点
	 */
	void MoveCenter(sctPoint3D center);

	/*
	 * Description
	 * 中心点沿着本平面法线方向递增或递减offset个单位
	 * @offset : int, 当offset为正整数时,中心点递增offset个单位；反之中心点递减offset个单位
	 */
	void SetCurrentScrollSlicePosition(int offset);

	/**
	 * 获取当前中心点相对于本视方向层数的相对位置
	 */
	int GetCurrentScrollSlicePosition();
	
	/*
	 * Description
	 * 选择定位线X特定的角度
	 * @dbRadius double, 旋转角度，单位：弧度
	 */
	void RotateNormalX(double dbRadius);

	/*
	 * Description
	 * 选择定位线Z特定的角度
	 * @dbRadius double, 旋转角度，单位：弧度
	 */
	void RotateNormalZ(double dbRadius);

	/*
	 * Description
	 * CPR 路径点
	 */
	void CPRPath(std::list<sctPoint3D> path);

	/*
	 * Description
	 * 设置重建层厚,比如MPR容积模式
	 * 函数使用Use开头因为类成员AUTO_PROPERTY宏自动创建了SetSliceThickness函数,后面Use开头函数同理
	 * @dbThickness double, 重建层厚
	 */
	void SetSliceThickness(double dbThickness);

	/**
	 * 生成重建切片方向参数
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
	 * Parameter的平面方向
	 */
	AUTO_PROPERTY(PlanDirection, ImagePlanDirection);

	/*
	 * 两条定位线X/Z的中心点
	 */
	AUTO_PROPERTY_REF_NO_SET(Center, sctPoint3D);

	/*
	 * 定位线X
	 */
	AUTO_PROPERTY_REF(NormalX, sctNormal3D);

	/*
	 * 定位线X 与 定位线Z的法线，也是重构成像面法线
	 * 定位线X 与 定位线Z不一定是正交关系
	 */
	AUTO_PROPERTY_REF(NormalY, sctNormal3D);

	/*
	 * 定位线Z
	 */
	AUTO_PROPERTY_REF(NormalZ, sctNormal3D);

	/**
	 * 视方向层数起点
	 */
	AUTO_PROPERTY_REF(SlicePositionStart, int);

	/**
	 * 视方向层数
	 */
	AUTO_PROPERTY_REF(SlicePositionEnd, int);

	/*
	 * 重建方式
	 * 当重建层厚是1个单位时,各重建方式结果一样的
	 * 当使用容积层厚方式时,不同重建方式结果会不一样
	 */
	AUTO_PROPERTY_REF(ImageSlabMode, ImageSlabMode);

	AUTO_PROPERTY_REF(SlabSliceCount, int);

	/*
	 * 原始序列的Pixel Spacing X
	 */
	AUTO_PROPERTY_REF_NO_SET(SourcePixelSpacingX, double);

	/*
	 * 原始序列的Pixel Spacing Y
	 */
	AUTO_PROPERTY_REF_NO_SET(SourcePixelSpacingY, double);

	/*
	 * 原始序列的层间距
	 */
	AUTO_PROPERTY(SourceSpacingBetweenSlice, double);

	/**
	 * 体素宽
	 */
	AUTO_PROPERTY_REF(VolumeWidth, int);

	/**
	 * 体素长
	 */
	AUTO_PROPERTY_REF(VolumeHeight, int);

	/**
	 * 体素高
	 */
	AUTO_PROPERTY_REF(VolumeSliceCount, int);

	/**
	 * 体素世界坐标X方向最小值
	 */
	AUTO_PROPERTY_REF(BoundX, int);

	/**
	 * 体素世界坐标X方向最大值
	 */
	AUTO_PROPERTY_REF(BoundX2, int);

	/**
	 * 体素世界坐标Y方向最小值
	 */
	AUTO_PROPERTY_REF(BoundY, int);

	/**
	 * 体素世界坐标Y方向最大值
	 */
	AUTO_PROPERTY_REF(BoundY2, int);

	/**
	 * 体素世界坐标Z方向最小值
	 */
	AUTO_PROPERTY_REF(BoundZ, int);

	/**
	 * 体素世界坐标Z方向最大值
	 */
	AUTO_PROPERTY_REF(BoundZ2, int);

	AUTO_PROPERTY(MPRPlanePtr, MPRPlanePtr);
};

/*
 * Description
 * MPR 参数
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
	 * 设置中心点
	 */
	void SetCenter(sctPoint3D &newCenter);

	/**
	 * 设置体素中心点
	 */
	void SetOrientImageCenter(sctPoint3D &newCenter);

	/**
	 * 重置
	 */
	void Reset();

	/**
	 * 旋转
	 */
	void Rotate(ImagePlanDirection direction, double dbAngle, double normal[3]);

	/**
	 * 设置体素长宽高
	 */
	void SetImageBound(int nWidth, int nHeight, int nSliceCount);

	/**
	 * 设置体素世界坐标边界值
	 */
	void SetVolumeBound(int x, int x2, int y, int y2, int z, int z2);

	void Initialize() override;
	
protected:
	void InitializeMessenger();
	
private:
	using ParameterItemMap = std::map<ImagePlanDirection, CMprParameterItem>;

	/*
	 * 矢状面/冠状面/横断面 MPR 参数
	 * 这三个面不会一直保持正交面关系
	 * 当在某一面旋转了该面的定位线后,这三个面就不一定是各自两两为正交关系
	 */
	AUTO_PROPERTY_REF_NO_SET(Parameters, ParameterItemMap);

	/**
	 * 三个旋转轴的共同中心点坐标
	 * 当前的旋转定位线中心
	 */
	AUTO_PROPERTY_REF_NO_SET(Center, sctPoint3D);

	/**
	 * 初始定位线中心点
	 */
	AUTO_PROPERTY_REF_NO_SET(OrientImageCenter, sctPoint3D);
};


#endif

