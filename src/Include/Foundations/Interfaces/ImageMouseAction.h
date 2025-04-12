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
 * ��궯������
 */
enum class ImageMouseAction
{
	UnkownAction = 0,

	/**
	 * ����
	 */
	Scale,

	/**
	 * ƽ��
	 */
	Translate,

	/**
	 * ��ת
	 */
	Rotate,

	/**
	 * ����λ
	 */
	WindowLevel,

	/**
	 * ѡ��ͼ��
	 */
	SelectImage,

	/**
	 * ����
	 */
	ResetImage,

	/**
	 * ֱ�߱�ע
	 */
	AnnotationLine,

	/**
	 * ���߱�ע
	 */
	AnnotationCurve,

	/**
	 * ����α�׼
	 */
	AnnotationPolygon,

	/**
	 * �ı���ע
	 */
	AnnotationText,

	/**
	 * �Ƕȱ�ע
	 */
	AnnotationAngle,

	/**
	 * ���α�ע
	 */
	AnnotationRect,

	/**
	 * ɾ����ע
	 */
	AnnotationDelete,

	/**
	 * MPR ����
	 */
	MPR,

	VR,

	/**
	 * ������
	 */
	Paronama,

	/**
	 * ��Ч����
	 */
	InvalidAction
};

#endif

