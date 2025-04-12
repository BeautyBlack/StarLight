
#pragma once


#ifdef VR_FRAME_SERVICEMPR_EXPORTS
#define VR_FRAME_SERVICEMPR_API __declspec(dllexport)
#else
#define VR_FRAME_SERVICEMPR_API __declspec(dllimport)
#pragma comment(lib, "VRFrameViewModel.lib")
#endif

