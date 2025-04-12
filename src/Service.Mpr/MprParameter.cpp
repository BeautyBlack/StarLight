#include "pch.h"

#include <algorithm>
#include <Services/Mpr/MprParameter.h>

#include "ToBeimport/MPRPlane.h"
#include "ToBeimport/BoxIntersectionProcess.h"

#include "../Foundation.Common/MathUtility.h"


struct sctMprRotateParam
{
	double dbAngle;
	ImagePlanDirection direction;
	double dbVector[3];

	sctMprRotateParam()
	{
		dbAngle = 0;
		direction = ImagePlanDirection::UnknownPlan;
		dbVector[0] = dbVector[1] = dbVector[2] = 0;
	}
};

CMprParameterItem::CMprParameterItem()
{
	SetMPRPlanePtr(std::make_shared<MPRPlane>());

	SetPlanDirection(ImagePlanDirection::UnknownPlan);
	//SetCenter(sctPoint3D(0, 0, 0));
	SetNormalX(sctNormal3D(1, 0, 0));
	SetNormalY(sctNormal3D(1, 0, 0));
	SetNormalZ(sctNormal3D(1, 0, 0));
	//SetSlicePosition(0);
	SetSlicePositionEnd(1);
	SetSlicePositionStart(0);
	SetImageSlabMode(ImageSlabMode::MIP);
	SetSlabSliceCount(1);
	SetSourcePixelSpacingX(1);
	SetSourcePixelSpacingY(1);
	SetSourceSpacingBetweenSlice(1);
	SetBoundX(0);
	SetBoundX2(0);
	SetBoundY(0);
	SetBoundY2(0);
	SetBoundZ(0);
	SetBoundZ2(0);
}

CMprParameterItem::CMprParameterItem(const CMprParameterItem& copySrc)
{
	CopyFrom(copySrc);
}

CMprParameterItem& CMprParameterItem::operator=(const CMprParameterItem& copySrc)
{
	CopyFrom(copySrc);

	return *this;
}

void CMprParameterItem::MoveCenter(sctPoint3D center)
{
	/*CMessenger::GetInstance().SendMessage(
		EMessengerKey::MprTransformImagePositionToWorld,
		this,
		&center);*/
	
	CMessenger::GetInstance().SendMessage(
		EMessengerKey::MprSynchronizeCenter,
		this,
		&center);
}

int CMprParameterItem::GetCurrentScrollSlicePosition()
{
	switch(GetPlanDirection()) {
		case ImagePlanDirection::Transverse:
			return GetSlicePositionEnd()
					- 1
					- round(((double)GetCenter().z - GetBoundZ())
						/ (GetBoundZ2() - GetBoundZ())
						* GetSlicePositionEnd());

		case ImagePlanDirection::Coronal:
			return GetSlicePositionEnd()
					- 1
					- round(((double)GetCenter().y - GetBoundY())
						/ (GetBoundY2() - GetBoundY())
						* GetSlicePositionEnd());

		case ImagePlanDirection::Sagittal:
			return round(((double)GetCenter().x - GetBoundX())
				/ (GetBoundX2() - GetBoundX())
				* GetSlicePositionEnd());
	}

	return 1;
}

void CMprParameterItem::SetCurrentScrollSlicePosition(int position)
{
	/*const bound = Messager.Instance.call(MessagerKeys.MessagerKeys.MprGetVolumeBounds, this.messageGroupId);

	if (!bound)
		return;

	let newCenter = [...this.center];
*/
	sctPoint3D newCenter = GetCenter();
	
	switch(GetPlanDirection()) {
		case ImagePlanDirection::Transverse:
			newCenter.z = (GetSlicePositionEnd()
				- 1
				- std::min<double>(
					std::max(0, position),
					GetSlicePositionEnd() - 1))
		/ GetSlicePositionEnd()
		* (GetBoundZ2() - GetBoundZ())
		+ GetBoundZ();
			break;

		case ImagePlanDirection::Coronal:
			newCenter.y = (GetSlicePositionEnd()
				- 1
				- std::min<double>(
					std::max(0, position),
					GetSlicePositionEnd() - 1))
		/ GetSlicePositionEnd()
		* (GetBoundY2() - GetBoundY())
		+ GetBoundY();
			break;

		case ImagePlanDirection::Sagittal:
			newCenter.x = std::min<double>(
				std::max(0, position),
				GetSlicePositionEnd() - 1)
		/ GetSlicePositionEnd()
		* (GetBoundX2() - GetBoundX())
		+ GetBoundX();
			break;
	}

	CMessenger::GetInstance().SendMessage(
		EMessengerKey::MprSynchronizeCenter,
		this,
		&newCenter);
}

void CMprParameterItem::RotateNormalX(double dbRadius)
{
	GetMPRPlanePtr()->RotalLine1Plane(dbRadius);
	
	//Rotate(dbRadius, true);
}

void CMprParameterItem::RotateNormalZ(double dbRadius)
{
	GetMPRPlanePtr()->RotalLine2Plane(dbRadius);
	//Rotate(dbRadius, false);
}

vtkSmartPointer<vtkMatrix4x4> CMprParameterItem::GenerateVtkResliceMatrix()
{
	auto result = vtkSmartPointer<vtkMatrix4x4>::New();

	result->SetElement(0, 0, GetNormalX().x);
	result->SetElement(1, 0, GetNormalX().y);
	result->SetElement(2, 0, GetNormalX().z);
	result->SetElement(3, 0, 0);
	result->SetElement(0, 1, GetNormalZ().x);
	result->SetElement(1, 1, GetNormalZ().y);
	result->SetElement(2, 1, GetNormalZ().z);
	result->SetElement(3, 1, 0);
	result->SetElement(0, 2, GetNormalY().x);
	result->SetElement(1, 2, GetNormalY().y);
	result->SetElement(2, 2, GetNormalY().z);
	result->SetElement(3, 2, 0);
	result->SetElement(0, 3, GetCenter().x);
	result->SetElement(1, 3, GetCenter().y);
	result->SetElement(2, 3, GetCenter().z);
	result->SetElement(3, 3, 1);
	
	/*result->Element[0][0] = GetNormalX().x;
	result->Element[0][1] = GetNormalX().y;
	result->Element[0][2] = GetNormalX().z;
	result->Element[0][3] = 0;
	result->Element[1][0] = GetNormalZ().x;
	result->Element[1][1] = GetNormalZ().y;
	result->Element[1][2] = GetNormalZ().z;
	result->Element[1][3] = 0;
	result->Element[2][0] = GetNormalY().x;
	result->Element[2][1] = GetNormalY().y;
	result->Element[2][2] = GetNormalY().z;
	result->Element[2][3] = 0;
	result->Element[3][0] = GetCenter().x;
	result->Element[3][1] = GetCenter().y;
	result->Element[3][2] = GetCenter().z;
	result->Element[3][3] = 1;*/

	return result;
}

void CMprParameterItem::SetSourcePixelSpacingX(double spacing)
{
	m_SourcePixelSpacingX = spacing;

	double dbSpacing[2] = { m_SourcePixelSpacingX, m_SourcePixelSpacingY };

	GetMPRPlanePtr()->SetSpacing(dbSpacing, false);
}

void CMprParameterItem::SetSourcePixelSpacingY(double spacing)
{
	m_SourcePixelSpacingY = spacing;

	double dbSpacing[2] = { m_SourcePixelSpacingX, m_SourcePixelSpacingY };

	GetMPRPlanePtr()->SetSpacing(dbSpacing, false);
}

void CMprParameterItem::InitVolume(double dOriginal[3],
	double xDirection[3], double yDirection[3], double zDirection[3],
	double space[3],
	int cx, int cy, int cz)
{
	if( !GetMPRPlanePtr()->IsVolumeBoxValid())
	{
		auto volumeBox = std::make_shared<BoxIntersectionProcess>();

		volumeBox->InitVolume(dOriginal,
			xDirection, yDirection, zDirection,
			space,
			cx, cy, cz);

		GetMPRPlanePtr()->SetBoxIntersectionProcess(volumeBox);

	}

	GetMPRPlanePtr()->UpdateMPR();
	
}

void CMprParameterItem::SetCenter(const sctPoint3D& point)
{
	m_Center = point;

	double center[3] = { point.x, point.y, point.z };

	GetMPRPlanePtr()->SetCenterPoint(center, false);

}

void CMprParameterItem::CopyFrom(const CMprParameterItem& copySrc)
{
	SetGroupId(copySrc.GetGroupId());
	
	SetPlanDirection(copySrc.m_PlanDirection);
	SetCenter(copySrc.m_Center);
	SetNormalX(copySrc.m_NormalX);
	SetNormalY(copySrc.m_NormalY);
	SetNormalZ(copySrc.m_NormalZ);

	SetImageSlabMode(copySrc.m_ImageSlabMode);

	SetVolumeHeight(copySrc.m_VolumeHeight);
	SetVolumeWidth(copySrc.m_VolumeWidth);
	SetVolumeSliceCount(copySrc.m_VolumeSliceCount);
	
	SetSourcePixelSpacingX(copySrc.m_SourcePixelSpacingX);
	SetSourcePixelSpacingY(copySrc.m_SourcePixelSpacingY);
	SetSourceSpacingBetweenSlice(copySrc.m_SourceSpacingBetweenSlice);

	SetSlicePositionEnd(copySrc.GetSlicePositionEnd());
	SetSlicePositionStart(copySrc.GetSlicePositionStart());
	
	SetBoundX(copySrc.m_BoundX);
	SetBoundX2(copySrc.m_BoundX2);

	SetBoundY(copySrc.m_BoundY);
	SetBoundY2(copySrc.m_BoundY2);

	SetBoundZ(copySrc.m_BoundZ);
	SetBoundZ2(copySrc.m_BoundZ2);

	SetMPRPlanePtr(copySrc.m_MPRPlanePtr);
}


int CMprParameterItem::GetSlicePositionStart() const
{
	return m_SlicePositionStart;
}

int CMprParameterItem::GetSlicePositionEnd() const
{
	return m_SlicePositionEnd;
}

void CMprParameterItem::Rotate(double dbAngle, bool isNormalX)
{
	sctMprRotateParam param;
	param.dbAngle = dbAngle;
	
	switch (GetPlanDirection()) {
		case ImagePlanDirection::Coronal:
			param.direction = isNormalX ? ImagePlanDirection::Transverse : ImagePlanDirection::Sagittal;
			break;

		case ImagePlanDirection::Transverse:
			param.direction = isNormalX ? ImagePlanDirection::Coronal : ImagePlanDirection::Sagittal;
			break;

		case ImagePlanDirection::Sagittal:
			param.direction = isNormalX ? ImagePlanDirection::Transverse : ImagePlanDirection::Coronal;
			break;
	}

	if (param.direction == ImagePlanDirection::UnknownPlan)
		return;

	param.dbVector[0] = GetNormalY().x;
	param.dbVector[1] = GetNormalY().y;
	param.dbVector[2] = GetNormalY().z;
	
	CMessenger::GetInstance().SendMessage(EMessengerKey::MprRotate,
		this,
		&param);
}


CMprParameter::CMprParameter()
{
	CMprParameterItem sagittalItem, transverseItem, coronalItem;

	sagittalItem.SetPlanDirection(ImagePlanDirection::Sagittal);
	transverseItem.SetPlanDirection(ImagePlanDirection::Transverse);
	coronalItem.SetPlanDirection(ImagePlanDirection::Coronal);

	GetParameters()[ImagePlanDirection::Sagittal] = sagittalItem;
	GetParameters()[ImagePlanDirection::Transverse] = transverseItem;
	GetParameters()[ImagePlanDirection::Coronal] = coronalItem;

	Reset();
	
	InitializeMessenger();
}

CMprParameter::~CMprParameter()
{
	
}

inline CMprParameterItem& CMprParameter::GetSagittalParameter()
{
	return GetParameters()[ImagePlanDirection::Sagittal];
}

inline CMprParameterItem & CMprParameter::GetTransverseParameter()
{
	return GetParameters()[ImagePlanDirection::Transverse];
}

inline CMprParameterItem & CMprParameter::GetCoronalParameter()
{
	return GetParameters()[ImagePlanDirection::Coronal];
}

CMprParameterItem& CMprParameter::GetParameter(ImagePlanDirection direction)
{
	if (direction == ImagePlanDirection::Sagittal)
		return GetSagittalParameter();

	if (direction == ImagePlanDirection::Coronal)
		return GetCoronalParameter();

	return GetTransverseParameter();
}

void CMprParameter::SetCenter(sctPoint3D& newCenter)
{
	GetCenter() = newCenter;

	for(auto &it : GetParameters())
	{
		it.second.SetCenter(newCenter);
	}
}

void CMprParameter::SetOrientImageCenter(sctPoint3D& newCenter)
{
	GetOrientImageCenter() = newCenter;

	SetCenter(newCenter);
}

void CMprParameter::Reset()
{
	static double coordinateX[] = { 1, 0, 0 };
	static double coordinateY[] = { 0, 1, 0 };
	static double coordinateZ[] = { 0, 0, 1 };

	static double inverseCoordinateX[] = { -1, 0, 0 };
	static double inverseCoordinateY[] = { 0, -1, 0 };
	static double inverseCoordinateZ[] = { 0, 0, -1 };

	auto &sagittal = GetSagittalParameter();

	auto &coronal = GetCoronalParameter();

	auto &transverse = GetTransverseParameter();

	sagittal.GetNormalX().Update(coordinateY);
	sagittal.GetNormalY().Update(inverseCoordinateX);
	sagittal.GetNormalZ().Update(inverseCoordinateZ);

	sagittal.GetMPRPlanePtr()->SetXDirection(coordinateY, false);
	sagittal.GetMPRPlanePtr()->SetYDirection(inverseCoordinateZ, false);
	sagittal.GetMPRPlanePtr()->SetCrossLine1(coordinateY, false);
	sagittal.GetMPRPlanePtr()->SetCrossLine2(inverseCoordinateZ, false);
	
	sagittal.GetMPRPlanePtr()->SetLine1CrossPlane(
		transverse.GetMPRPlanePtr());
	sagittal.GetMPRPlanePtr()->SetLine2CrossPlane(
		coronal.GetMPRPlanePtr());
	
	double center[3] = { GetOrientImageCenter().x, GetOrientImageCenter().y, GetOrientImageCenter().z };
	
	sagittal.GetMPRPlanePtr()->SetCenterPoint(center, false);
	//sagittal.GetMPRPlanePtr()->SetSpacing(space, false);
	sagittal.GetMPRPlanePtr()->UpdateMPR();

	coronal.GetNormalX().Update(coordinateX);
	coronal.GetNormalY().Update(coordinateY);
	coronal.GetNormalZ().Update(inverseCoordinateZ);

	coronal.GetMPRPlanePtr()->SetXDirection(coordinateX, false);
	coronal.GetMPRPlanePtr()->SetYDirection(inverseCoordinateZ, false);
	coronal.GetMPRPlanePtr()->SetCrossLine1(coordinateX, false);
	coronal.GetMPRPlanePtr()->SetCrossLine2(inverseCoordinateZ, false);
	coronal.GetMPRPlanePtr()->SetCenterPoint(center, false);

	coronal.GetMPRPlanePtr()->SetLine1CrossPlane(
		transverse.GetMPRPlanePtr());
	coronal.GetMPRPlanePtr()->SetLine2CrossPlane(
		sagittal.GetMPRPlanePtr());
	
	coronal.GetMPRPlanePtr()->UpdateMPR();

	transverse.GetNormalX().Update(coordinateX);
	transverse.GetNormalY().Update(coordinateZ);
	transverse.GetNormalZ().Update(coordinateY);

	transverse.GetMPRPlanePtr()->SetXDirection(coordinateX, false);
	transverse.GetMPRPlanePtr()->SetYDirection(coordinateY, false);
	transverse.GetMPRPlanePtr()->SetCrossLine1(coordinateX, false);
	transverse.GetMPRPlanePtr()->SetCrossLine2(coordinateY, false);
	transverse.GetMPRPlanePtr()->SetCenterPoint(center, false);

	transverse.GetMPRPlanePtr()->SetLine1CrossPlane(
		coronal.GetMPRPlanePtr());
	transverse.GetMPRPlanePtr()->SetLine2CrossPlane(
		sagittal.GetMPRPlanePtr());

	transverse.GetMPRPlanePtr()->UpdateMPR();

	SetCenter(GetOrientImageCenter());
}

void CMprParameter::Rotate(
	ImagePlanDirection direction, double angle, double planNormal[3])
{
	auto param = GetParameters().find(direction);

	if (param == GetParameters().end())
		return;

	double newPlanVectorX[3] = { 0 };

	double t[3] = { 0 };

	t[0] = param->second.GetNormalX().x;
	t[1] = param->second.GetNormalX().y;
	t[2] = param->second.GetNormalX().z;
	CMathUtility::RotateVector3(t, planNormal, angle, newPlanVectorX);
	
	double newPlanVectorZ[3] = { 0 };
	t[0] = param->second.GetNormalZ().x;
	t[1] = param->second.GetNormalZ().y;
	t[2] = param->second.GetNormalZ().z;
	CMathUtility::RotateVector3(t, planNormal, angle, newPlanVectorZ);
	
	double newPlanVectorY[3] = { 0 };
	CMathUtility::VectorMulVector3X3(newPlanVectorX, newPlanVectorZ, newPlanVectorY);

	param->second.GetNormalX().Update(newPlanVectorX);
	param->second.GetNormalY().Update(newPlanVectorY);
	param->second.GetNormalZ().Update(newPlanVectorZ);
}

void CMprParameter::Initialize()
{
	GetParameter(ImagePlanDirection::Sagittal).SetGroupId(GetGroupId());
	GetParameter(ImagePlanDirection::Transverse).SetGroupId(GetGroupId());
	GetParameter(ImagePlanDirection::Coronal).SetGroupId(GetGroupId());

	InitializeMessenger();
}


void CMprParameter::InitializeMessenger()
{
	CMessenger::GetInstance().Regist(EMessengerKey::MprSynchronizeCenter,
		this,
		[&](void *pParam)
	{
		auto pPoint = (sctPoint3D*)pParam;

		if(pParam != NULL_SYMBAL)
			SetCenter(*(sctPoint3D*)pParam);

		return NULL_SYMBAL;
	});

	CMessenger::GetInstance().Regist(EMessengerKey::MprRotate,
		this,
		[&](void* pParam)
	{
		auto pRotateParam = (sctMprRotateParam*)pParam;
		
		Rotate(pRotateParam->direction,
			pRotateParam->dbAngle,
			pRotateParam->dbVector);
		
		return NULL_SYMBAL;
	});
}

void CMprParameter::SetImageBound(int nWidth, int nHeight, int nSliceCount)
{
	for (auto &it : GetParameters())
	{
		it.second.SetVolumeWidth(nWidth);
		it.second.SetVolumeHeight(nHeight);
		it.second.SetVolumeSliceCount(nSliceCount);
	}
}

void CMprParameter::SetVolumeBound(int x, int x2, int y, int y2, int z, int z2)
{
	for (auto &it : GetParameters())
	{
		it.second.SetBoundX(x);
		it.second.SetBoundX2(x2);

		it.second.SetBoundY(y);
		it.second.SetBoundY2(y2);

		it.second.SetBoundZ(z);
		it.second.SetBoundZ2(z2);
	}
}
