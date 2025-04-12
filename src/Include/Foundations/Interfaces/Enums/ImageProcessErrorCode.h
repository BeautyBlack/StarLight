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
	 * �ɹ�
	 */
	Success = 0,
	
	/**
	 * ͼ����������
	 */
	ImagesCountNotEnought = 1,

	/**
	 * ������ͼ�����겻һ��
	 */
	ImagesCoordinateConflict = 2,

	/**
	 * ������ͼ���С��һ��
	 */
	ImagesWidthOrHeightConflict = 3,

	/**
	 * ͼ��ؾ�б�ʲ�һ��
	 */
	ImagesSlopOrInterceptConflict = 4,

	ImagesSpacingConflict = 5
};

#endif
