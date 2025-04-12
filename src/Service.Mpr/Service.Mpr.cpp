// Service.Mpr.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "Service.Mpr.h"


// This is an example of an exported variable
SERVICEMPR_API int nServiceMpr=0;

// This is an example of an exported function.
SERVICEMPR_API int fnServiceMpr(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CServiceMpr::CServiceMpr()
{
    return;
}
