#include "stdafx.h"
#include <ViewModels/ImageFrameViewModel/Annotation/PolygonAnnotation.h>

#include <ViewModels/ImageFrameViewModel/DisplayImage.h>
#include "../Foundation.Common/MathUtility.h"

CPolygonAnnotation::CPolygonAnnotation()
{
	SetAnnotationType(AnnotationType::PolygonAnnotation);

	GetCurveLine().setZValue(1);

	GetCurveLine().setPen(GetDefaultLinePen());

	GetCurveLine().setFlags(
		QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemSendsGeometryChanges |
		QGraphicsItem::ItemIsFocusable);

	GetControlPointRectItem().push_back(CreateControlPointRectItem());
}

void CPolygonAnnotation::RemoveFromScene(QGraphicsScene * pScene)
{
	CAnnotationBase::RemoveFromScene(pScene);

	if (pScene != NULL_SYMBAL)
		pScene->removeItem(&GetCurveLine());
}

void CPolygonAnnotation::UpdatePositions(double dbScale,
	double dbXOffset, double dbYOffset,
	QPointF &suggestResultTextScenePos)
{
	CAnnotationBase::UpdatePositions(dbScale, dbXOffset, dbYOffset, suggestResultTextScenePos);

	GetCurveLine().resetTransform();
	GetCurveLine().setPos(dbXOffset, dbYOffset);
	GetCurveLine().setScale(dbScale);
}

void CPolygonAnnotation::AddToScene(QGraphicsScene * pScene)
{
	CAnnotationBase::AddToScene(pScene);

	if (pScene != NULL_SYMBAL)
		pScene->addItem(&GetCurveLine());
}

QPointF CPolygonAnnotation::GetSuggestResultTextPosition()
{
	int nTop = 0, nRight = 0, nLeft = 0, nBottom = 0;

	GetOrientPointBound(nLeft, nTop, nRight, nBottom);

	return QPointF(nRight + 5, nBottom + 5);
}

int CPolygonAnnotation::MaxControlPointCount()
{
	return INT_MAX;
}

void CPolygonAnnotation::ComputeResult()
{
	const int nCount = GetOrientPointCount();
	
	if (nCount < 3)
		return;

	if (GetImagePlanModule().expired())
		return;

	auto imagePlan = GetImagePlanModule().lock();

	auto points = new POINT[nCount];

	auto nIndex = 0;
	for(auto &it : GetOrientPoints())
	{
		points[nIndex].x = it.x;
		points[nIndex++].y = it.y;
	}

	HRGN hPoly = CreatePolygonRgn(points, nCount, ALTERNATE);

	int nLeft = 0, nTop = 0, nRight = 0, nBottom = 0;

	GetOrientPointBound(nLeft, nTop, nRight, nBottom);

	double dbSum = 0;
	int nSumCount = 0;

	int nMin = INT_MAX, nMax = INT_MIN;

	auto pixels = imagePlan->GetCurrentFramePixelData();

	// 会引入HRGN, 不做成template方式
	if(imagePlan->GetIsSampleWord())
	{
		if(imagePlan->GetIsSigned())
		{
			auto pixelT = (short*)pixels;
			
			for (auto col = nTop; col < nBottom; col++)
			{
				auto begin = pixelT + (col * imagePlan->GetWidth() + nLeft);

				for (auto row = nLeft;
					row < nRight;
					row++, begin++)
				{
					if (PtInRegion(hPoly, row, col))
					{
						const auto p = *begin;
						
						dbSum += p;
						nSumCount++;

						nMin = min(nMin, p);
						nMax = max(nMax, p);
					}
				}
			}
		}
		else
		{
			auto pixelT = (ushort*)pixels;

			for (auto col = nTop; col < nBottom; col++)
			{
				auto begin = pixelT + (col * imagePlan->GetWidth() + nLeft);

				for (auto row = nLeft;
					row < nRight;
					row++, begin++)
				{
					if (PtInRegion(hPoly, row, col))
					{
						const auto p = *begin;

						dbSum += p;
						nSumCount++;

						nMin = min(nMin, p);
						nMax = max(nMax, p);
					}
				}
			}
		}
	}
	else
	{
		if (imagePlan->GetIsSigned())
		{
			auto pixelT = (INT8*)pixels;

			for (auto col = nTop; col < nBottom; col++)
			{
				auto begin = pixelT + (col * imagePlan->GetWidth() + nLeft);

				for (auto row = nLeft;
					row < nRight;
					row++, begin++)
				{
					if (PtInRegion(hPoly, row, col))
					{
						const auto p = *begin;

						dbSum += p;
						nSumCount++;

						nMin = min(nMin, p);
						nMax = max(nMax, p);
					}
				}
			}
		}
		else
		{
			auto pixelT = (UINT8*)pixels;

			for (auto col = nTop; col < nBottom; col++)
			{
				auto begin = pixelT + (col * imagePlan->GetWidth() + nLeft);

				for (auto row = nLeft;
					row < nRight;
					row++, begin++)
				{
					if (PtInRegion(hPoly, row, col))
					{
						const auto p = *begin;

						dbSum += p;
						nSumCount++;

						nMin = min(nMin, p);
						nMax = max(nMax, p);
					}
				}
			}
		}
	}

	DeleteObject(hPoly);

	delete[] points;

	double dbArea = nSumCount
		* imagePlan->GetPixelSpacingX()
		* imagePlan->GetPixelSpacingY();

	double dbAverage = dbSum / nSumCount;

	char strMax[32] = { 0 };
	char strMin[32] = { 0 };

	if (nMin != INT_MAX)
		_snprintf_s(strMin, sizeof(strMin),
			_TRUNCATE, "%d HU",
			static_cast<int>(nMin * imagePlan->GetSlope()
				+ imagePlan->GetIntercept()));

	if (nMax != INT_MIN)
		_snprintf_s(strMax, sizeof(strMax),
			_TRUNCATE, "%d HU",
			static_cast<int>(nMax * imagePlan->GetSlope()
				+ imagePlan->GetIntercept()));

	char strResult[1024] = { 0 };

	_snprintf_s(strResult, sizeof(strResult), _TRUNCATE,
		"Area: %0.2lf mm*mm\nAve: %0.2lf HU\nMax: %s\nMin: %s",
		dbArea, dbAverage, strMax, strMin);

	SetResult(strResult);
}


bool CPolygonAnnotation::OnMouseLeftButtonDownInternal(int x, int y)
{
	auto result = false;

	if (GetIsCompleted())
	{
		if (IsHitAnnotation(x, y))
		{
			SetIsTranslateAnnotation(true);

			result = true;
			
			DisplayControlPoints();
		}

		/*const auto selectIndex = IsHitControlPoint(x, y);

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
			DisplayControlPoints();*/
	}
	else
	{
		// Creating
		AddPoint(x, y);

		GetCurveLinePolygon().append(QPoint(x, y));

		result = true;
	}

	UpdateControlPointPosition();
	
	GetLastMousePosition().x = x;
	GetLastMousePosition().y = y;

	return result;
}

bool CPolygonAnnotation::OnMouseMovingInternal(int x, int y)
{
	auto result = false;

	if (GetIsCompleted())
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

				GetCurveLinePolygon().translate(offsetX, offsetY);

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
		/*else
		{
			(GetOrientPoints().begin() + GetSelectedControlPointIndex())
				->updateFrom(x, y);

			result = true;
		}*/
	}
	else
	{
		AddPoint(x, y);

		GetCurveLinePolygon().append(QPoint(x, y));

		GetCurveLine().setPolygon(GetCurveLinePolygon());

		result = true;
	}

	UpdateControlPointPosition();
	
	GetLastMousePosition().x = x;
	GetLastMousePosition().y = y;

	return result;
}

bool CPolygonAnnotation::OnMouseLeftButtonUpInternal(int x, int y)
{
	auto result = CAnnotationBase::OnMouseLeftButtonUpInternal(x, y);

	if (!result)
		return result;
	
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

			GetCurveLinePolygon().translate(offsetX, offsetY);

			UpdateControlPointPosition();

			GetLastMousePosition().x = x;
			GetLastMousePosition().y = y;

			return true;
		}

		return false;
	}
	
	AddPoint(x, y);

	GetCurveLinePolygon().append(QPoint(x, y));

	GetCurveLine().setPolygon(GetCurveLinePolygon());

	return result;
}

void CPolygonAnnotation::UpdateControlPointPosition()
{
	GetCurveLine().setPolygon(GetCurveLinePolygon());
	
	const auto firstControlPointItem = GetControlPointRectItem().begin();

	(*firstControlPointItem)->setRect(
		GetOrientPoints()[0].x - 5,
		GetOrientPoints()[0].y - 5,
		10,
		10);
}

bool CPolygonAnnotation::IsHitAnnotation(int x, int y)
{
	auto result = CAnnotationBase::IsHitAnnotation(x, y);

	if (result)
		return result;

	auto count = GetOrientPointCount();
	if (count < 2)
		return false;
	
	auto it = GetOrientPoints().begin();
	auto it2 = GetOrientPoints().begin() + (count-1);

	return CMathUtility::IsHitLine(it->x, it->y,
		it2->x, it2->y,
		x, y,
		ANNOTATION_HIT_TEST_OFFSET);
}
