#include "stdafx.h"
#include <ViewModels/ImageFrameViewModel/Annotation/TextAnnotation.h>
#include "utilities/Messenger/Messenger.h"
#include "utilities/Messenger/EventArgs/UI/ImageFrame/DisplayTextAnnotationEditControlEventArgs.h"

#include <ViewModels/ImageFrameViewModel/DisplayImage.h>

#include <QTextDocument>

CTextAnnotation::CTextAnnotation()
{
	SetAnnotationType(AnnotationType::TextAnnotation);
}

int CTextAnnotation::MaxControlPointCount()
{
	return 1;
}

void CTextAnnotation::ComputeResult()
{
	// Empty
}


bool CTextAnnotation::IsHitAnnotation(int x, int y)
{
	return false;
}

int CTextAnnotation::IsHitControlPoint(int x, int y)
{
	return INVALID_CONTROL_POINT_INDEX;
}

void CTextAnnotation::SetInputText(STRING_TYPE text,
	const QPointF &pos,
	const QPointF &posInScene)
{
	SetResult(text);
	SetIsCompleted(true);

	AddPoint(pos.x(), pos.y());

	SetResultPositionInScene(posInScene);
}

bool CTextAnnotation::OnMouseLeftButtonDownInternal(int x, int y)
{
	if(GetIsCompleted())
	{
		auto count = GetOrientPointCount();

		if (count == 0)
			return false;
		
		auto point = GetOrientPoints()[count - 1];
		auto width = GetAnnotationResultTextItem().document()->idealWidth();

		if (x >= point.x && x <= (point.x + width)
			&& y >= point.y && y <= (point.y + 35))
		{
			SetIsTranslateAnnotation(true);

			GetLastMousePosition().x = x;
			GetLastMousePosition().y = y;

			return true;
		}

		return false;
	}
	/*else
	{
		CDisplayTextAnnotationEditControlEventArgs eventArgs;

		eventArgs.SetAnnotationText(GetResult());

		CMessenger::GetInstance().SendMessage(EMessengerKey::DisplayAnnotationEditControl,
			this, &eventArgs);

		SetResult(eventArgs.GetAnnotationText());

		SetTextBoundLeft(eventArgs.GetTextBoundLeft());
		SetTextBoundTop(eventArgs.GetTextBoundTop());
		SetTextBoundRight(eventArgs.GetTextBoundRight());
		SetTextBoundBottom(eventArgs.GetTextBoundBottom());
	}*/
	
	return true;
}

bool CTextAnnotation::OnMouseMovingInternal(int x, int y)
{
	if(GetIsTranslateAnnotation())
	{
		auto offsetX = x - GetLastMousePosition().x;
		auto offsetY = y - GetLastMousePosition().y;

		auto count = GetOrientPointCount();

		if (count == 0)
			return false;

		auto point = GetOrientPoints().begin() + (count - 1);

		point->x = point->x + offsetX;
		point->y = point->y + offsetY;
		/*SetTextBoundLeft(GetTextBoundLeft() + offsetX);
		SetTextBoundTop(GetTextBoundTop() + offsetY);
		SetTextBoundRight(GetTextBoundRight() + offsetX);
		SetTextBoundBottom(GetTextBoundBottom() + offsetY);*/

		GetLastMousePosition().x = x;
		GetLastMousePosition().y = y;

		return true;
	}

	return false;
}

