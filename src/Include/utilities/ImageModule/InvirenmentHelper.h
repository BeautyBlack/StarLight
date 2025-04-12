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

#ifndef INVIRENMENT_HELPER_DAFB4742C9D146D0BD2D5A9D9FDE8951
#define INVIRENMENT_HELPER_DAFB4742C9D146D0BD2D5A9D9FDE8951

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <Utilities/Messenger/Messenger.h>


#include <utilities/ImageModule/ExportFlagDefine.h>

class UTILITIESIMAGEMODULE_API CInvirenmentHelper : public CMessengerOwner
{
public:
	CInvirenmentHelper();
	virtual ~CInvirenmentHelper();

	/*
	 * Description
	 * 初始化环境
	 */
	void Initialize();

private:
	void RegistMessage();

private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CInvirenmentHelper);
};

#endif

