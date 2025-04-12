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

#ifndef SERVICE_STOP_FLAG_0E64AD68D21E4BE097C437A2416316F7
#define SERVICE_STOP_FLAG_0E64AD68D21E4BE097C437A2416316F7

#pragma once


class CServiceStopFlag
{
public:
	static inline bool IsRunning() { return m_bRunning; }
	static inline void StopRunning() { m_bRunning = false; }
	static inline void ResumeRunning() { m_bRunning = true; }
	static inline const bool* StopFlag() { return &m_bRunning; }

private:
	static bool m_bRunning;
};

#endif

