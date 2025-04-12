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

#ifndef SERVICE_TYPE_CDCEB9AE505945A1886294F73C52F342
#define SERVICE_TYPE_CDCEB9AE505945A1886294F73C52F342

#pragma once

/*
 * 服务类型
 */
enum class ServiceType
{
	UnknownService = 0,

	/**
	 * MPR
	 */
	MprService = 1,

	/**
	 * VR
	 */
	VRService = 2,

	Paronama = 3,

	/**
	 * Invalid Value
	 */
	InvalidService = 0xEFFFFF
};

#endif
