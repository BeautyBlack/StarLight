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

#pragma once

#ifdef UTILITIESDICOMCLIENT_EXPORTS
#define UTILITIESDICOMCLIENT_API __declspec(dllexport)
#else
#define UTILITIESDICOMCLIENT_API __declspec(dllimport)
#pragma comment(lib,"Utilities.DicomClient.lib")
#endif
