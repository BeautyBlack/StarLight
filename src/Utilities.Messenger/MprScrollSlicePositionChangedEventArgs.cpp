#include "pch.h"
#include <utilities/Messenger/EventArgs/UI/MprFrame/MprScrollSlicePositionChangedEventArgs.h>

CMprScrollSlicePositionChangedEventArgs::CMprScrollSlicePositionChangedEventArgs()
{
	SetImagePlan(ImagePlanDirection::UnknownPlan);

	SetNewPosition(0);
}
