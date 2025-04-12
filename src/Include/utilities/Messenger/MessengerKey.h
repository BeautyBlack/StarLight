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
	 * 创建Study Module
	 */
	CreateStudyModule = 1,

	/*
	 * 查找文件夹
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
	 * 同步MPR定位线中线点
	 * 参数： sctPoint3D*
	 */
	MprSynchronizeCenter,

	MprPlanVectorChanged,

	/**
	 * 转换图像坐标到VTK 世界坐标
	 */
	MprTransformImagePositionToWorld,
	
	/**
	 * MPR旋转坐标轴
	 */
	MprRotate,

	/**
	 * 加载MPR
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
