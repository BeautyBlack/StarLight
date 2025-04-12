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

#ifndef MPR_FRAME_VIEW_MODEL_GLOBAL_BF68FEA91D76443C9DE9CB787F221A72
#define MPR_FRAME_VIEW_MODEL_GLOBAL_BF68FEA91D76443C9DE9CB787F221A72

#pragma once


//#ifndef BUILD_STATIC
//	#if defined(IMAGEFRAMEVIEWMODEL_LIB)
//		#define IMAGEFRAMEVIEWMODEL_EXPORT Q_DECL_EXPORT
//	#else
//		#define IMAGEFRAMEVIEWMODEL_EXPORT Q_DECL_IMPORT
//	#endif
//#else
//	#define IMAGEFRAMEVIEWMODEL_EXPORT
//#endif

#ifdef MPRFRAMEVIEWMODEL_LIB
#define MPRFRAMEVIEWMODEL_API __declspec(dllexport)
#else
#define MPRFRAMEVIEWMODEL_API __declspec(dllimport)
#include <ViewModels/MprFrameViewModel/MprFrameViewModel_global.h>
#pragma comment(lib, "MprFrameViewModel.lib");
#endif

#endif
