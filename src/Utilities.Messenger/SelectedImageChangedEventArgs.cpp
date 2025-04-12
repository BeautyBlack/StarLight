#include "pch.h"
#include "utilities/Messenger/EventArgs/UI/ImageFrame/SelectedImageChangedEventArgs.h"

CSelectedImageChangedEventArgs::CSelectedImageChangedEventArgs()
{
	SetIsMultiSelectMode(false);

	SetActivadeTarget(NULL_SYMBAL);
}
