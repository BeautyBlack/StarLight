#include "stdafx.h"

#include <utilities/ImageModule/DcmtkJpegRegister.h>

#include <dcmtk/dcmjpeg/djdecode.h>

CDcmtkJpegRegister CDcmtkJpegRegister::m_Instance;

CDcmtkJpegRegister::CDcmtkJpegRegister()
{
	SetHasInit(false);
}


CDcmtkJpegRegister::~CDcmtkJpegRegister()
{
	Release();
}

void CDcmtkJpegRegister::Initialize()
{
	if (GetHasInit())
		return;

	// 注册解压缩等模块
	DJDecoderRegistration::registerCodecs();

	SetHasInit(true);
}

void CDcmtkJpegRegister::Release()
{
	if (!GetHasInit())
		return;

	// 释放解压缩等模块
	DJDecoderRegistration::cleanup();

	SetHasInit(false);
}
