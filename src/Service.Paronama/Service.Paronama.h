// Service.Paronama.h : main header file for the Service.Paronama DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CServiceParonamaApp
// See Service.Paronama.cpp for the implementation of this class
//

class CServiceParonamaApp : public CWinApp
{
public:
	CServiceParonamaApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
