#include "pch.h"
#include <Foundations/Interfaces/ServiceRequest.h>

CServiceRequest::CServiceRequest()
{
	SetImageWidth(0);
	SetImageHeight(0);
	SetSliceCount(0);

	SetWindowWidth(1);
	SetWindowCenter(0);
	SetIsPixelSigned(false);
	SetIsPixelWord(true);
	SetPixelSpacingX(1);
	SetPixelSpacingY(1);
	SetSpacingBetweenSlice(1);
	SetSlop(1);
	SetIntercept(0);
}

bool CServiceRequest::IsValidImageData()
{
	return GetImageData()
		&& GetImageWidth() > 0
		&& GetImageHeight() > 0
		&& GetSliceCount() > 3;
}
