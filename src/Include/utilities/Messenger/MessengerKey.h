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

#ifndef UTILITIES_MESSENGER_KEY_C2770EE2E23F44BAB9CB61783FC4DB1B
#define UTILITIES_MESSENGER_KEY_C2770EE2E23F44BAB9CB61783FC4DB1B

#pragma once

#include <utilities/Messenger/ExportFlagDefine.h>

enum class UTILITIESMESSENGER_API EMessengerKey
{
	/*
	 * ����Study Module
	 */
	CreateStudyModule = 1,

	/*
	 * �����ļ���
	 */
	SearchFiles = 2,

	/**********  Frame Menu  **************/
	MainFrameMenuChanged,
	/**********  Frame Menu  **************/

	OpenStudyTab,

	OpenPatientListTab,

	StudyImageButtonActiveStateChanged,

	SelectImageChanged,

	DisplayMessageBox,

	DisplayOkCancelMessageBox,

	DisplayAnnotationEditControl,

	StationNameChanged,

	/**********  MRP  **************/

	/**
	 * ͬ��MPR��λ�����ߵ�
	 * ������ sctPoint3D*
	 */
	MprSynchronizeCenter,

	MprPlanVectorChanged,

	/**
	 * ת��ͼ�����굽VTK ��������
	 */
	MprTransformImagePositionToWorld,
	
	/**
	 * MPR��ת������
	 */
	MprRotate,

	/**
	 * ����MPR
	 */
	MprLoadImages,

	MprUpdateDisplayImages,

	MprUpdateScoutLineCenter,

	MprScrollPositionChanged,

	MprScoutLineAngleChanged,
	/**********  MRP  **************/


	/**********  VR  **************/
	VRLoadImages,

	GetVRRenderWindowParentHandle,

	UpdateColorSetting,

	SaveColorSetting,
	/**********  VR  **************/

	/********** Paronama  **************/
	ParonamaUpdateDisplayImages,
	ParonamaLoadImages,
	/********** Paronama  **************/
};

#endif
