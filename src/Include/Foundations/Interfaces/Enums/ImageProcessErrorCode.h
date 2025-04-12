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

#ifndef IMAGE_PROCESS_ERROR_CODE_BAFAAD1976AD43638CE0FC77C5DDAC47
#define IMAGE_PROCESS_ERROR_CODE_BAFAAD1976AD43638CE0FC77C5DDAC47

#pragma once

enum class ImageProcessErrorCode
{
	/**
	 * 成功
	 */
	Success = 0,
	
	/**
	 * 图像数量过少
	 */
	ImagesCountNotEnought = 1,

	/**
	 * 序列内图像坐标不一致
	 */
	ImagesCoordinateConflict = 2,

	/**
	 * 序列内图像大小不一致
	 */
	ImagesWidthOrHeightConflict = 3,

	/**
	 * 图像截距斜率不一致
	 */
	ImagesSlopOrInterceptConflict = 4,

	ImagesSpacingConflict = 5
};

#endif
