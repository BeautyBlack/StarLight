// Service.VR.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "Service.VR.h"


// This is an example of an exported variable
SERVICEVR_API int nServiceVR=0;

// This is an example of an exported function.
SERVICEVR_API int fnServiceVR(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CServiceVR::CServiceVR()
{
    return;
}
