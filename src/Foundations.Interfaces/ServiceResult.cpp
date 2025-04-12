#include "pch.h"
#include <Foundations/Interfaces/ServiceResult.h>


CServiceResult::CServiceResult()
{
	SetImagePlan(ImagePlanDirection::UnknownPlan);

	SetImageWidth(0);

	SetImageHeight(0);

	SetImageData(NULL_SYMBAL);

	SetWindowWidth(1);

	SetWindowCenter(0);

	SetIsPixelSigned(false);

	SetIsPixelWord(true);

	SetPixelSpacingX(1);

	SetPixelSpacingY(1);

	SetSpacingBetweenSlice(1);

	SetSlop(1);

	SetIntercept(0);

	SetPhotometricInterpretation(PhotometricInterpretation::MONOCHROME2);
}


CServiceResult::~CServiceResult()
{
	//Empty
}
