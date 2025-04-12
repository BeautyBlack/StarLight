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

#ifndef IMAGE_MOUSE_ACTION_5E2BE98BF43F4DE487F78723E4BC2A7F
#define IMAGE_MOUSE_ACTION_5E2BE98BF43F4DE487F78723E4BC2A7F

#pragma once

/**
 * 鼠标动作类型
 */
enum class ImageMouseAction
{
	UnkownAction = 0,

	/**
	 * 缩放
	 */
	Scale,

	/**
	 * 平移
	 */
	Translate,

	/**
	 * 旋转
	 */
	Rotate,

	/**
	 * 窗宽窗位
	 */
	WindowLevel,

	/**
	 * 选择图像
	 */
	SelectImage,

	/**
	 * 重置
	 */
	ResetImage,

	/**
	 * 直线标注
	 */
	AnnotationLine,

	/**
	 * 曲线标注
	 */
	AnnotationCurve,

	/**
	 * 多边形标准
	 */
	AnnotationPolygon,

	/**
	 * 文本标注
	 */
	AnnotationText,

	/**
	 * 角度标注
	 */
	AnnotationAngle,

	/**
	 * 矩形标注
	 */
	AnnotationRect,

	/**
	 * 删除标注
	 */
	AnnotationDelete,

	/**
	 * MPR 后处理
	 */
	MPR,

	VR,

	/**
	 * 牙弓线
	 */
	Paronama,

	/**
	 * 无效操作
	 */
	InvalidAction
};

#endif

