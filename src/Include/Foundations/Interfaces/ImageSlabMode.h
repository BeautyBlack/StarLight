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
	 * 最大密度
	 */
	MIP = 1,

	/*
	 * 最小密度
	 */
	MinIP = 2,

	/*
	 * 均值
	 */
	AIP = 3,

	/*
	 * 容积累加
	 */
	SUM = 4,

	/*
	 * 组合
	 */
	Composite = 5,

	Invalid = 0xEFFFFF
};

#endif
