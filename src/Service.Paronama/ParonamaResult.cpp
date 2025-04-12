#include "pch.h"
#include "Services/Paronama/ParonamaResult.h"

CParonamaResult::CParonamaResult()
{
	SetImagePlan(ImagePlanDirection::UnknownPlan);
	
	SetScrollSlicePositionStart(0);
	SetScrollSlicePositionEnd(0);

	SetScrollSlicePosition(0);
}

CParonamaResult::~CParonamaResult()
{
}
