#include "stdafx.h"
#include <ViewModels/ImageFrameViewModel/Annotation/CurveLineAnnotation.h>

#include <ViewModels/ImageFrameViewModel/DisplayImage.h>
#include "../Foundation.Common/MathUtility.h"

CCurveLineAnnotation::CCurveLineAnnotation()
{
	SetAnnotationType(AnnotationType::CurveAnnotation);
	
	GetCurveLine().setZValue(1);

	GetCurveLine().setPen(GetDefaultLinePen());

	GetCurveLine().setFlags(
		QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemSendsGeometryChanges |
		QGraphicsItem::ItemIsFocusable);

	GetControlPointRectItem().push_back(CreateControlPointRectItem());
}

void CCurveLineAnnotation::RemoveFromScene(QGraphicsScene * pScene)
{
	CAnnotationBase::RemoveFromScene(pScene);

	if (pScene != NULL_SYMBAL)
		pScene->removeItem(&GetCurveLine());
}

void CCurveLineAnnotation::UpdatePositions(double dbScale,
	double dbXOffset, double dbYOffset,
	QPointF &suggestResultTextScenePos)
{
	CAnnotationBase::UpdatePositions(dbScale, dbXOffset, dbYOffset, suggestResultTextScenePos);

	GetCurveLine().resetTransform();
	GetCurveLine().setPos(dbXOffset, dbYOffset);
	GetCurveLine().setScale(dbScale);
}

void CCurveLineAnnotation::AddToScene(QGraphicsScene * pScene)
{
	CAnnotationBase::AddToScene(pScene);

	if (pScene != NULL_SYMBAL)
		pScene->addItem(&GetCurveLine());
}

int CCurveLineAnnotation::MaxControlPointCount()
{
	return INT_MAX;
}

void CCurveLineAnnotation::ComputeResult()
{
	const auto count = GetOrientPointCount();

	if (count < 3)
		return;

	auto length = 0.0;

	auto imagePlan = GetImagePlanModule().lock();

	for (auto i = 0; i < count - 1; ++i)
	{
		const auto it = GetOrientPoints().begin() + i;
		const auto it2 = GetOrientPoints().begin() + (i + 1);

		const auto x = abs(it2->x - it->x) * imagePlan->GetPixelSpacingX();
		const auto y = abs(it2->y - it->y) * imagePlan->GetPixelSpacingY();

		length += sqrt(x * x + y * y) / 10;
	}

	CString strResult;

	strResult.Format("%0.2lf cm", length);

	SetResult(strResult.GetString());
}

bool CCurveLineAnnotation::OnMouseLeftButtonDownInternal(int x, int y)
{
	auto result = false;

	if (GetIsCompleted())
	{
		if (IsHitAnnotation(x, y))
		{
			SetIsTranslateAnnotation(true);

			result = true;
		}
		
		if (result)
			DisplayControlPoints();
	}
	else
	{
		// Creating
		AddPoint(x, y);

		GetCurveLinePath().moveTo(x, y);
		
		GetCurveLine().setPath(GetCurveLinePath());

		UpdateControlPointPosition();
		
		result = true;
	}

	GetLastMousePosition().x = x;
	GetLastMousePosition().y = y;

	return result;
}

bool CCurveLineAnnotation::OnMouseMovingInternal(int x, int y)
{
	auto result = false;

	if(GetIsCompleted())
	{
		if (GetSelectedControlPointIndex() <= INVALID_CONTROL_POINT_INDEX)
		{
			if (GetIsTranslateAnnotation())
			{
				auto offsetX = x - GetLastMousePosition().x;
				auto offsetY = y - GetLastMousePosition().y;

				GetCurveLinePath().translate(offsetX, offsetY);

				GetCurveLine().setPath(GetCurveLinePath());

				for (auto &it : GetOrientPoints())
				{
					it.x = it.x + offsetX;
					it.y = it.y + offsetY;
				}
				
				UpdateControlPointPosition();

				result = true;
			}
			else
			{
				if (IsHitAnnotation(x, y))
					DisplayControlPoints();
				else
					HideControlPoints();
			}
		}
	}
	else
	{
		AddPoint(x, y);
		
		GetCurveLinePath().lineTo(x, y);

		GetCurveLine().setPath(GetCurveLinePath());

		UpdateControlPointPosition();
		
		result = true;
	}

	GetLastMousePosition().x = x;
	GetLastMousePosition().y = y;

	return result;
}

bool CCurveLineAnnotation::OnMouseLeftButtonUpInternal(int x, int y)
{
	auto result = CAnnotationBase::OnMouseLeftButtonUpInternal(x, y);

	if (result)
	{
		if (GetSelectedControlPointIndex() <= INVALID_CONTROL_POINT_INDEX)
		{
			if (GetIsTranslateAnnotation())
			{
				auto offsetX = x - GetLastMousePosition().x;
				auto offsetY = y - GetLastMousePosition().y;

				for (auto &it : GetOrientPoints())
				{
					it.x = it.x + offsetX;
					it.y = it.y + offsetY;
				}

				GetCurveLinePath().translate(offsetX, offsetY);

				UpdateControlPointPosition();

				GetLastMousePosition().x = x;
				GetLastMousePosition().y = y;

				return true;
			}

			return false;
		}
		
		AddPoint(x, y);

		GetCurveLinePath().lineTo(x, y);

		GetCurveLine().setPath(GetCurveLinePath());

		UpdateControlPointPosition();
	}

	return result;
}

void CCurveLineAnnotation::UpdateControlPointPosition()
{
	const auto firstControlPointItem = GetControlPointRectItem().begin();
	
	(*firstControlPointItem)->setRect(
		GetOrientPoints()[0].x - 5,
		GetOrientPoints()[0].y - 5,
		10,
		10);
}

bool CCurveLineAnnotation::IsHitAnnotation(int x, int y)
{
	for(auto &it : GetOrientPoints())
	{
		if (abs(it.x - x) <= ANNOTATION_HIT_TEST_OFFSET
			&& abs(it.y - y) <= ANNOTATION_HIT_TEST_OFFSET)
			return true;
	}

	return false;
}

