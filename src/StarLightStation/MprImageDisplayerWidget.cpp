#include "pch.h"
#include "MprImageDisplayerWidget.h"

#include <QMouseEvent>

#include "utilities/Messenger/EventArgs/UI/MprFrame/MprScrollSlicePositionChangedEventArgs.h"
#include "ViewModels/MprFrameViewModel/MprDisplayImage.h"

MprImageDisplayerWidget::MprImageDisplayerWidget(QWidget *parent, MESSAGE_GROUP_ID_TYPE messengerOwnerId)
	: MPRImageDisplayerWidgetBase(parent, messengerOwnerId)
{
	GetScoutLine().SetGroupId(GetGroupId());
}

void MprImageDisplayerWidget::Reset()
{
	ImageDisplayerWidget::Reset();

	UpdateScoutLines();
}

void MprImageDisplayerWidget::mouseMoveEvent(QMouseEvent* ev)
{
	ev->accept();

	const auto posInImage = ConvertToImageCoordinate(ev);

	if (!GetScoutLine().OnMouseMoving(posInImage))
		ImageDisplayerWidget::mouseMoveEvent(ev);

	UpdateScoutLines();
}

void MprImageDisplayerWidget::mousePressEvent(QMouseEvent* ev)
{
	if(GetMouseAction() == ImageMouseAction::Scale
		|| GetMouseAction() == ImageMouseAction::Translate
		|| GetMouseAction() == ImageMouseAction::WindowLevel)
	{
		ImageDisplayerWidget::mousePressEvent(ev);

		UpdateScoutLines();

		return;
	}

	ev->accept();

	const auto posInImage = ConvertToImageCoordinate(ev);

	if( !GetScoutLine().OnMouseLeftButtonDown(posInImage))
		ImageDisplayerWidget::mousePressEvent(ev);

	UpdateScoutLines();
}

void MprImageDisplayerWidget::mouseReleaseEvent(QMouseEvent* ev)
{
	ev->accept();

	const auto posInImage = ConvertToImageCoordinate(ev);

	if (!GetScoutLine().OnMouseLeftButtonUp(posInImage))
		ImageDisplayerWidget::mouseReleaseEvent(ev);

	UpdateScoutLines();
}

void MprImageDisplayerWidget::resizeEvent(QResizeEvent* event)
{
	ImageDisplayerWidget::resizeEvent(event);

	UpdateScoutLines();
}

void MprImageDisplayerWidget::wheelEvent(QWheelEvent* event)
{
	SwitchToNextSlice(event->delta() > 0);

	//ImageDisplayerWidget::wheelEvent(event);

	UpdateScoutLines();
}

MprImageDisplayerWidget::MprImageDisplayerWidget(QWidget *parent)
	: MPRImageDisplayerWidgetBase(parent, 0)
{
}

void MprImageDisplayerWidget::UpdateScoutLines()
{
	if ( !GetImageViewModel())
		return;

	GetScoutLine().SetReferenceImage(
		(CMprDisplayImage*)GetImageViewModel().get());

	GetScoutLine().AddToScene(&GetGraphicsScene());
	
	GetScoutLine().UpdatePositions(
		GetImageViewModel()->GetImageScale(),
		GetImageViewModel()->GetTranslateX(),
		GetImageViewModel()->GetTranslateY());
}

void MprImageDisplayerWidget::UpdateFromViewModel()
{
	ImageDisplayerWidget::UpdateFromViewModel();

	UpdateScoutLines();
}

void MprImageDisplayerWidget::SwitchToNextSlice(bool isForward)
{
	if (!GetImageViewModel())
		return;

	auto eventArgs = CMprScrollSlicePositionChangedEventArgs();
	auto image = (CMprDisplayImage*)GetImageViewModel().get();
	auto newPosition = (isForward ? 1 : -1) +
		image->GetScrollSlicePosition();
	
	eventArgs.SetImagePlan(image->GetPlanDirection());
	eventArgs.SetNewPosition(
		min(image->GetScrollSlicePositionEnd(),
			max(newPosition,
				image->GetScrollSlicePositionStart())));
	
	CMessenger::GetInstance().SendMessage(
		EMessengerKey::MprScrollPositionChanged,
		&eventArgs);
}


