#include "stdafx.h"
#include <ViewModels/ImageFrameViewModel/Annotation/LineAnnotation.h>
#include "../Foundation.Common/MathUtility.h"
#include <ViewModels/ImageFrameViewModel/DisplayImage.h>


CLineAnnotation::CLineAnnotation()
{
	SetAnnotationType(AnnotationType::LineAnnotation);

	GetLineItem().setZValue(1);

	GetLineItem().setPen(GetDefaultLinePen());

	GetLineItem().setFlags(
		QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemSendsGeometryChanges |
		QGraphicsItem::ItemIsFocusable);

	for(int i = 0; i < 2; i++)
	{
		auto rectItem =
			CreateControlPointRectItem();
		
		GetControlPointRectItem().push_back(rectItem);
	}
}

int CLineAnnotation::MaxControlPointCount()
{
	return 2;
}

void CLineAnnotation::ComputeResult()
{
	if (GetOrientPointCount() != MaxControlPointCount()
		|| GetImagePlanModule().expired())
		return;

	const auto ptStart = GetOrientPoints()[0];
	const auto ptEnd = GetOrientPoints()[1];

	auto imagePlan = GetImagePlanModule().lock();
	
	const auto x = abs(ptEnd.x - ptStart.x) * imagePlan->GetPixelSpacingX();
	const auto y = abs(ptEnd.y - ptStart.y) * imagePlan->GetPixelSpacingY();

	const auto length = sqrt(x * x + y * y) / 10;

	CString strResult;

	strResult.Format("%0.2lf cm", length);

	SetResult(strResult.GetString());
}

bool CLineAnnotation::IsHitAnnotation(int x, int y)
{
	if (GetOrientPointCount() < MaxControlPointCount())
		return false;

	const auto itBegin = GetOrientPoints().begin();
	const auto itEnd = itBegin + 1;

	return CMathUtility::IsHitLine(itBegin->x, itBegin->y,
		itEnd->x, itEnd->y,
		x, y,
		ANNOTATION_HIT_TEST_OFFSET);
}

void CLineAnnotation::RemoveFromScene(QGraphicsScene* pScene)
{
	CAnnotationBase::RemoveFromScene(pScene);

	if (pScene != NULL_SYMBAL)
		pScene->removeItem(&GetLineItem());
}

void CLineAnnotation::UpdatePositions(double dbScale,
	double dbXOffset, double dbYOffset,
	QPointF &suggestResultTextScenePos)
{
	CAnnotationBase::UpdatePositions(dbScale, dbXOffset, dbYOffset, suggestResultTextScenePos);

	GetLineItem().resetTransform();
	GetLineItem().setPos(dbXOffset, dbYOffset);
	GetLineItem().setScale(dbScale);
}

void CLineAnnotation::AddToScene(QGraphicsScene* pScene)
{
	CAnnotationBase::AddToScene(pScene);

	if (pScene != NULL_SYMBAL)
		pScene->addItem( &GetLineItem());
}

void CLineAnnotation::UpdateControlPointPosition()
{
	auto firstControlPointItem = GetControlPointRectItem().begin();
	auto endControlPointItem = firstControlPointItem + 1;

	(*firstControlPointItem)->setRect(GetOrientPoints()[0].x - 5, GetOrientPoints()[0].y - 5, 10, 10);
	(*endControlPointItem)->setRect(GetOrientPoints()[1].x - 5, GetOrientPoints()[1].y - 5, 10, 10);
}

bool CLineAnnotation::OnMouseLeftButtonDownInternal(int x, int y)
{
	auto result = false;
	
	if(GetIsCompleted())
	{
		const auto selectIndex = IsHitControlPoint(x, y);

		if(selectIndex >= 0)
		{
			SetSelectedControlPointIndex(selectIndex);

			result = true;
		}
		else if(IsHitAnnotation(x, y))
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
		AddPoint(x, y);

		SetSelectedControlPointIndex(1);

		GetLineItem().setLine(x, y, x, y);

		UpdateControlPointPosition();

		result = true;
	}

	GetLastMousePosition().x = x;
	GetLastMousePosition().y = y;
	
	return result;
}

bool CLineAnnotation::OnMouseMovingInternal(int x, int y)
{
	if (GetSelectedControlPointIndex() <= INVALID_CONTROL_POINT_INDEX)
	{
		if(GetIsTranslateAnnotation())
		{
			auto offsetX = x - GetLastMousePosition().x;
			auto offsetY = y - GetLastMousePosition().y;
			
			TranslateAnnotation(offsetX, offsetY);

			GetLastMousePosition().x = x;
			GetLastMousePosition().y = y;

			return true;
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
		}
		
		return false;
	}

	(GetOrientPoints().begin() + GetSelectedControlPointIndex())->updateFrom(x, y);

	const auto line = GetLineItem().line();
	
	GetLineItem().setLine(line.p1().x(), line.p1().y(),
		x, y);

	UpdateControlPointPosition();
	
	return true;
}

bool CLineAnnotation::OnMouseLeftButtonUpInternal(int x, int y)
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

				TranslateAnnotation(offsetX, offsetY);

				GetLastMousePosition().x = x;
				GetLastMousePosition().y = y;

				return true;
			}

			return false;
		}

		auto pointCount = GetOrientPointCount();

		(GetOrientPoints().begin() + (pointCount - 1))
			->updateFrom(x, y);

		const auto line = GetLineItem().line();

		GetLineItem().setLine(line.p1().x(), line.p1().y(),
			x, y);

		UpdateControlPointPosition();
	}

	return result;
}

void CLineAnnotation::TranslateAnnotation(double offsetX, double offsetY)
{
	for (auto &it : GetOrientPoints())
	{
		it.x = it.x + offsetX;
		it.y = it.y + offsetY;
	}

	const auto line = GetLineItem().line();

	GetLineItem().setLine(
		GetOrientPoints()[0].x, GetOrientPoints()[0].y,
		GetOrientPoints()[1].x, GetOrientPoints()[1].y);
	
	UpdateControlPointPosition();
}
