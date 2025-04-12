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

#ifndef IMAGE_PLAN_DIRECTION_04AAF5F3926B48BB943A0E18CDD3EDD1
#define IMAGE_PLAN_DIRECTION_04AAF5F3926B48BB943A0E18CDD3EDD1

#pragma once

/*
 * ͼ��ƽ�淽��
 */
enum class ImagePlanDirection
{
	UnknownPlan = 0,

	/*
	 * ʸ״�� 
	 */
	Sagittal = 1,

	/*
	 * �����
	 */
	Transverse = 2,

	/*
	 * ��״��
	 */
	Coronal = 3,

	/*
	 * �ؽ���
	 */
	Reconstruct = 4,

	Paronama = 5,

	/**
	 * Invalid Value
	 */
	InvalidPlan = 0xEFFFFF
};

#endif
