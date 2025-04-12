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

#ifndef IMAGE_FRAME_VIEW_MODEL_GLOBAL_57F2F264301C4B488F30B683B6FE862F
#define IMAGE_FRAME_VIEW_MODEL_GLOBAL_57F2F264301C4B488F30B683B6FE862F

#pragma once


#ifndef BUILD_STATIC
	#if defined(IMAGEFRAMEVIEWMODEL_LIB)
		#define IMAGEFRAMEVIEWMODEL_EXPORT Q_DECL_EXPORT
	#else
		#define IMAGEFRAMEVIEWMODEL_EXPORT Q_DECL_IMPORT
	#endif
#else
	#define IMAGEFRAMEVIEWMODEL_EXPORT
#endif

#ifdef IMAGEFRAMEVIEWMODEL_LIB
#define IMAGEFRAMEVIEWMODEL_API __declspec(dllexport)
#else
#define IMAGEFRAMEVIEWMODEL_API __declspec(dllimport)
#include <ViewModels/ImageFrameViewModel/imageframeviewmodel_global.h>
#pragma comment(lib, "ImageFrameViewModel.lib");
#endif

#endif
