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

#ifndef UTILITIES_IMAGE_MODULE_EXPORT_FLAG_DEFINE_B0544ADFBE984A96A189B6BDDDC141ED
#define UTILITIES_IMAGE_MODULE_EXPORT_FLAG_DEFINE_B0544ADFBE984A96A189B6BDDDC141ED

#pragma once


#ifdef UTILITIESIMAGEMODULE_EXPORTS
#define UTILITIESIMAGEMODULE_API __declspec(dllexport)
#else
#define UTILITIESIMAGEMODULE_API __declspec(dllimport)
#pragma comment(lib, "Utilities.ImageModule.lib")
#endif

#endif

