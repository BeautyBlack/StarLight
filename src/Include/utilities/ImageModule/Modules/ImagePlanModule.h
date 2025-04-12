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

#ifndef IMAGE_PLAN_MODULE_BB93A4238F294757902C6CEDF5A94040
#define IMAGE_PLAN_MODULE_BB93A4238F294757902C6CEDF5A94040

#pragma once

#include <Foundations/Common/ToDefine.h>
#include <Foundations/Interfaces/PhotometricInterpretation.h>
#include "Foundations/Common/Math/Point3D.h"
#include "Foundations/Common/Math/Normal3D.h"

#include <memory>

#include "Foundations/Common/Math/Point2D.h"

class CImageInstanceModule;


#include <utilities/ImageModule/ExportFlagDefine.h>

class UTILITIESIMAGEMODULE_API CImagePlanModule
{
public:
	/*
	 * Ĭ�ϲ�
	 */
	static int DefaultFrameIndex;

	CImagePlanModule();
	~CImagePlanModule();

	/*
	 * Description
	 * �л�������
	 */
	void SetFrameIndex(int nIndex);

	const unsigned char* GetCurrentFramePixelData();

	sctPoint2D PatientPointToImagePlane(sctPoint3D &point);
	sctPoint2D PatientPointToImagePlane(double x, double y, double z);
	
	sctPoint3D ImagePointToPatientPoint(sctPoint2D &point);
	sctPoint3D ImagePointToPatientPoint(double x, double y);


private:

	/*
	 * Reference Image Instance ����Image Instance
	 */
	AUTO_PROPERTY_REF(ReferenceImageInstance, std::weak_ptr<CImageInstanceModule>);

	/*
	 * Width ���
	 */
	AUTO_PROPERTY_REF(Width, int);

	/*
	 * Height �߶�
	 */
	AUTO_PROPERTY_REF(Height, int);

	/*
	 * Pixel Spacing X X����ֱ���
	 */
	AUTO_PROPERTY_REF(PixelSpacingX, double);

	/*
	 * Pixel Spacing Y Y����ֱ���
	 */
	AUTO_PROPERTY_REF(PixelSpacingY, double);

	/*
	 * Spacing Between Slice ����
	 */
	AUTO_PROPERTY_REF(SpacingBetweenSlice, double);

	/*
	 * ���
	 */
	AUTO_PROPERTY_REF(SliceThickness, double);

	/*
	 * Frame Count Frame����
	 */
	AUTO_PROPERTY_REF(FrameCount, int);

	/*
	 * Frame ������
	 */
	AUTO_PROPERTY_REF_NO_SET(FrameIndex, int);

	/*
	 * Pixel ����
	 */
	//AUTO_PROPERTY_REF(Pixels, BYTE_TYPE*);

	/*
	 * OneFramePixelSize ���ؾ����С
	 */
	AUTO_PROPERTY_REF(OneFramePixelSize, int);

	/*
	 * OneFramePixelSizeInBytes ���ؾ����ֽڴ�С
	 */
	AUTO_PROPERTY_REF(OneFramePixelSizeInBytes, int);

	/*
	 * AllFramePixels ������
	 */
	AUTO_PROPERTY_REF(AllFramePixels, std::shared_ptr<unsigned char>);

	/*
	 * HighBit �����Чλ
	 */
	//AUTO_PROPERTY(HighBit, int);

	/*
	 * BitsStored ���ش洢λ
	 */
	//AUTO_PROPERTY(BitsStored, int);

	/*
	 * BitsAllocated �������ֽ�λ��С
	 */
	//AUTO_PROPERTY(BitsAllocated, int);

	/*
	 * PhotometricInterpretation ������ʾ���뷽ʽ
	 */
	AUTO_PROPERTY_REF(PhotometricInterpretation, PhotometricInterpretation);

	/*
	 * IsSampleWord ˫�ֽ�����
	 */
	AUTO_PROPERTY_REF(IsSampleWord, bool);

	/*
	 * IsSigned �з�������
	 */
	AUTO_PROPERTY_REF(IsSigned, bool);

	/*
	 * Intercept �ؾ�
	 */
	AUTO_PROPERTY_REF(Intercept, double);

	/*
	 * Slop б��
	 */
	AUTO_PROPERTY_REF(Slope, double);

	/*
	 * Position �׶���
	 */
	AUTO_PROPERTY_REF(Position, sctPoint3D);

	/*
	 * OrientationX X����������
	 */
	AUTO_PROPERTY_REF(OrientationX, sctNormal3D);

	/*
	 * OrientationY Y����������
	 */
	AUTO_PROPERTY_REF(OrientationY, sctNormal3D);

	/*
	 * WindowCenter ��λ
	 */
	AUTO_PROPERTY_REF(WindowCenter, double);

	/*
	 * WindowWidth ����
	 */
	AUTO_PROPERTY_REF(WindowWidth, double);
};

#endif
