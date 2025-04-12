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

#include <Foundations/Common/ToDefine.h>

#include <utilities/ImageModule/ExportFlagDefine.h>

class UTILITIESIMAGEMODULE_API CDcmtkJpegRegister
{
public:
	CDcmtkJpegRegister();
	~CDcmtkJpegRegister();

	void Initialize();
	void Release();

private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CDcmtkJpegRegister);

	AUTO_PROPERTY(HasInit, bool);
};

