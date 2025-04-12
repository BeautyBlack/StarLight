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

#ifndef SERVICE_PARAMETER_BASE_CDCEB9AE505945A1886294F73C52F342
#define SERVICE_PARAMETER_BASE_CDCEB9AE505945A1886294F73C52F342

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <Foundations/Interfaces/ServiceBase.h>

#include "utilities/Messenger/Messenger.h"

class CServiceBase;

/*
 * 服务参数基类
 */
_declspec(novtable) class CServiceParameterBase : public CMessengerOwner
{
public:
	CServiceParameterBase()
	{
		SetReferenceServcie(NULL_SYMBAL);
	}

	virtual ~CServiceParameterBase() {/* Empty */}

	virtual void Initialize() {/* Empty */ }

private:
	AUTO_PROPERTY(ReferenceServcie, CServiceBase*);

	AUTO_PROPERTY_REF(MyGuid, STRING_TYPE);
};

#endif
