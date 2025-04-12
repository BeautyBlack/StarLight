#include "pch.h"

#include <utilities/Messenger/EventArgs/UI/MprFrame/MprRotateScoutLineEventArgs.h>

CMprRotateScoutLineEventArgs::CMprRotateScoutLineEventArgs()
{
	SetImagePlan(ImagePlanDirection::UnknownPlan);

	SetAngle(0);
	SetIsVectorX(true);
}

