#include "stdafx.h"
#include <ViewModels/ImageFrameViewModel/Annotation/AngleAnnotation.h>

#include <ViewModels/ImageFrameViewModel/DisplayImage.h>
#include "../Foundation.Common/MathUtility.h"

CAngleAnnotation::CAngleAnnotation()
{
	SetAnnotationType(AnnotationType::AngleAnnotation);

	GetCurveLine().setZValue(1);

	GetCurveLine().setPen(GetDefaultLinePen());

	GetCurveLine().setFlags(
		QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemSendsGeometryChanges |
		QGraphicsItem::ItemIsFocusable);

	for (int i = 0; i < 3; i++)
	{
		auto rectItem =
			CreateControlPointRectItem();

		GetControlPointRectItem().push_back(rectItem);
	}
}

void CAngleAnnotation::RemoveFromScene(QGraphicsScene * pScene)
{
	CAnnotationBase::RemoveFromScene(pScene);

	if (pScene != NULL_SYMBAL)
		pScene->removeItem(&GetCurveLine());
}

void CAngleAnnotation::UpdatePositions(double dbScale,
	double dbXOffset, double dbYOffset,
	QPointF &suggestResultTextScenePos)
{
	CAnnotationBase::UpdatePositions(dbScale, dbXOffset, dbYOffset,suggestResultTextScenePos);

	GetCurveLine().resetTransform();
	GetCurveLine().setPos(dbXOffset, dbYOffset);
	GetCurveLine().setScale(dbScale);
}

void CAngleAnnotation::AddToScene(QGraphicsScene * pScene)
{
	CAnnotationBase::AddToScene(pScene);

	if (pScene != NULL_SYMBAL)
		pScene->addItem(&GetCurveLine());
}

int CAngleAnnotation::MaxControlPointCount()
{
	return 3;
}

void CAngleAnnotation::ComputeResult()
{
	static const auto transform = 45.0 / atan(1.0);
	
	if (GetOrientPointCount() != MaxControlPointCount())
		return;

	auto it1 = GetOrientPoints().begin();
	auto it2 = GetOrientPoints().begin() + 1;
	auto it3 = GetOrientPoints().begin() + 2;

	auto angle = CMathUtility::ComputeAngle2D(
		it1->x - it2->x,
		it1->y - it2->y,
		it3->x - it2->x,
		it3->y - it2->y)
		* transform;
	
	char strResult[64] = { 0 };
	_snprintf_s(strResult, sizeof(strResult), _TRUNCATE,
		"%0.1lf °", angle);

	SetResult(strResult);
}

bool CAngleAnnotation::OnMouseLeftButtonDownInternal(int x, int y)
{
	auto result = false;
	auto pointCount = GetOrientPointCount();

	if (GetIsCompleted())
	{
		const auto selectIndex = IsHitControlPoint(x, y);

		if (selectIndex >= 0)
		{
			SetSelectedControlPointIndex(selectIndex);

			result = true;
		}
		else if (IsHitAnnotation(x, y))
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
		AddPoint(x + 100, y);
		AddPoint(x + 100, y + 100);

		GetCurveLinePath().moveTo(x, y);
		GetCurveLinePath().lineTo(x + 100, y);
		GetCurveLinePath().lineTo(x + 100, y + 100);

		GetCurveLine().setPath(GetCurveLinePath());

		SetIsCompleted(true);

		UpdateControlPointPosition();

		result = true;
	}

	GetLastMousePosition().x = x;
	GetLastMousePosition().y = y;

	return result;
}

bool CAngleAnnotation::OnMouseMovingInternal(int x, int y)
{
	auto result = false;

	if(GetIsCompleted())
	{
		if (GetSelectedControlPointIndex() <= INVALID_CONTROL_POINT_INDEX)
		{
			// 未选中控制点
			if (GetIsTranslateAnnotation())
			{
				// 移动标注
				auto offsetX = x - GetLastMousePosition().x;
				auto offsetY = y - GetLastMousePosition().y;

				for (auto &it : GetOrientPoints())
				{
					it.x = it.x + offsetX;
					it.y = it.y + offsetY;
				}

				GetCurveLinePath().translate(offsetX, offsetY);

				result = true;
			}
			else
			{
				auto shouldDisplayControlPoint = false;

				const auto selectIndex = IsHitControlPoint(x, y);

				if (selectIndex >= 0 || IsHitAnnotation(x, y))
					shouldDisplayControlPoint = true;

				if (shouldDisplayControlPoint)
					DisplayControlPoints();
				else
					HideControlPoints();

				return false;
			}
		}
		else
		{
			(GetOrientPoints().begin()
				+ GetSelectedControlPointIndex())
				->updateFrom(x, y);
			
			GetCurveLinePath().setElementPositionAt(GetSelectedControlPointIndex(),
				x, y);

			result = true;
		}
	}
	else
	{
		result = true;
	}

	UpdateControlPointPosition();
	
	GetLastMousePosition().x = x;
	GetLastMousePosition().y = y;

	return result;
}


bool CAngleAnnotation::OnMouseLeftButtonUpInternal(int x, int y)
{
	auto result = CAnnotationBase::OnMouseLeftButtonUpInternal(x, y);

	if (result)
		return result;

	if (GetSelectedControlPointIndex() <= INVALID_CONTROL_POINT_INDEX)
	{
		if (GetIsTranslateAnnotation())
		{
			// 移动标注
			auto offsetX = x - GetLastMousePosition().x;
			auto offsetY = y - GetLastMousePosition().y;

			for (auto &it : GetOrientPoints())
			{
				it.x = it.x + offsetX;
				it.y = it.y + offsetY;
			}

			GetCurveLinePath().translate(offsetX, offsetY);

			result = true;
		}
		else
			return false;
	}
	else
	{
		(GetOrientPoints().begin()
			+ GetSelectedControlPointIndex())
			->updateFrom(x, y);

		GetCurveLinePath().setElementPositionAt(GetSelectedControlPointIndex(),
			x, y);
		/*auto nIndex = 0;

		for (auto &it : GetOrientPoints())
		{
			if (nIndex++ == 0)
				GetCurveLinePath().moveTo(it.x, it.y);
			else
				GetCurveLinePath().lineTo(it.x, it.y);
		}*/

		result = true;
	}

	UpdateControlPointPosition();

	GetLastMousePosition().x = x;
	GetLastMousePosition().y = y;
	
	return result;
}


void CAngleAnnotation::UpdateControlPointPosition()
{
	GetCurveLine().setPath(GetCurveLinePath());
	
	for(auto i = 0; i < 3; i++)
	{
		auto firstControlPointItem = GetControlPointRectItem().begin() + i;
		
		(*firstControlPointItem)->setRect(
			GetOrientPoints()[i].x - 5,
			GetOrientPoints()[i].y - 5,
			10,
			10);
	}
}

