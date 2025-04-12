#include "stdafx.h"
#include <Utilities/ImageModule/Modules/ImagePlanModule.h>

#include "../Foundation.Common/MathUtility.h"

int CImagePlanModule::DefaultFrameIndex = 0;

CImagePlanModule::CImagePlanModule()
{
	SetFrameCount(1);
	SetFrameIndex(0);
	SetWidth(0);
	SetHeight(0);
	SetPixelSpacingX(1);
	SetPixelSpacingY(1);
	SetSliceThickness(1);
	SetSpacingBetweenSlice(0);
	SetOneFramePixelSize(0);
	SetOneFramePixelSizeInBytes(0);
	SetPhotometricInterpretation(PhotometricInterpretation::MONOCHROME2);
	SetIsSampleWord(false);
	SetIsSigned(false);
	SetIntercept(0);
	SetSlope(1);
	SetWindowCenter(128);
	SetWindowWidth(256);
}

CImagePlanModule::~CImagePlanModule()
{
}

void CImagePlanModule::SetFrameIndex(int nIndex)
{
	if (!GetAllFramePixels()
		|| nIndex < 0
		|| nIndex >= GetFrameCount())
		return;

	m_FrameIndex = nIndex;

	//SetPixels(GetAllFramePixels().get() + nIndex * GetOneFramePixelSizeInBytes());
}

const BYTE* CImagePlanModule::GetCurrentFramePixelData()
{
	return GetAllFramePixels().get()
		+ GetOneFramePixelSizeInBytes() * GetFrameIndex();
}

sctPoint2D CImagePlanModule::PatientPointToImagePlane(sctPoint3D &point)
{
	return PatientPointToImagePlane(point.x, point.y, point.z);
}

sctPoint2D CImagePlanModule::PatientPointToImagePlane(double x, double y, double z)
{
	double v3[3] = { x, y, z };
	double rowCosines[3] =
	{ GetOrientationX().x, GetOrientationX().y, GetOrientationX().z };
	
	double columnCosines[3] =
	{ GetOrientationY().x, GetOrientationY().y, GetOrientationY().z };
	
	double imagePositionPatient[3] =
	{ GetPosition().x, GetPosition().y, GetPosition().z };
	
	double point[3] = { 0 };
	CMathUtility::VectorSubVector(
		point,
		v3,
		imagePositionPatient,
		_countof(point));
	
	return sctPoint2D(CMathUtility::VectorDotVector(
		rowCosines,
		point,
		_countof(point))
		/ GetPixelSpacingX(),
		CMathUtility::VectorDotVector(
		columnCosines,
			point,
			_countof(point))
		/ GetPixelSpacingY()
		);
}

sctPoint3D CImagePlanModule::ImagePointToPatientPoint(sctPoint2D &point)
{
	return ImagePointToPatientPoint(point.x, point.y);
}

sctPoint3D CImagePlanModule::ImagePointToPatientPoint(double x, double y)
{
	double rowCosines[3] =
	{ GetOrientationX().x, GetOrientationX().y, GetOrientationX().z };
	
	double columnCosines[3] =
	{ GetOrientationY().x, GetOrientationY().y, GetOrientationY().z };
	
	double imagePositionPatient[3] =
	{ GetPosition().x, GetPosition().y, GetPosition().z };
	
	double vectorX[3] = { 0 };
	CMathUtility::VectorMultiply(
		vectorX,
		rowCosines,
		_countof(vectorX),
		x * GetPixelSpacingX());


	double vectorY[3] = { 0 };
	CMathUtility::VectorMultiply(
	vectorY,
		columnCosines,
		_countof(vectorY),
		y * GetPixelSpacingY());

	double patientPoint[3] = { 0 };

	CMathUtility::VectorAddVector(patientPoint,
		vectorX, vectorY, _countof(patientPoint));

	CMathUtility::VectorAddVector(patientPoint,
		patientPoint,
		imagePositionPatient,
		_countof(patientPoint));

	return sctPoint3D(patientPoint[0],
		patientPoint[1],
		patientPoint[2]);
}


