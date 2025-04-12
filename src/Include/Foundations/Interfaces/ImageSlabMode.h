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

#ifndef RECONSTRUCT_METHOD_D728BC52C9CD4205A51FE325083ED92F
#define RECONSTRUCT_METHOD_D728BC52C9CD4205A51FE325083ED92F

#pragma once

enum class ImageSlabMode
{
	Unknown = 0,

	/*
	 * ����ܶ�
	 */
	MIP = 1,

	/*
	 * ��С�ܶ�
	 */
	MinIP = 2,

	/*
	 * ��ֵ
	 */
	AIP = 3,

	/*
	 * �ݻ��ۼ�
	 */
	SUM = 4,

	/*
	 * ���
	 */
	Composite = 5,

	Invalid = 0xEFFFFF
};

#endif
