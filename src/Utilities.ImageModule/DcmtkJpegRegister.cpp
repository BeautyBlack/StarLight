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

	// ע���ѹ����ģ��
	DJDecoderRegistration::registerCodecs();

	SetHasInit(true);
}

void CDcmtkJpegRegister::Release()
{
	if (!GetHasInit())
		return;

	// �ͷŽ�ѹ����ģ��
	DJDecoderRegistration::cleanup();

	SetHasInit(false);
}
