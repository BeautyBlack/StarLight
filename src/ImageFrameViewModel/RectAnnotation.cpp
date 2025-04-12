#include "stdafx.h"
#include "ViewModels/ImageFrameViewModel/Annotation/RectAnnotation.h"
#include <ViewModels/ImageFrameViewModel/DisplayImage.h>

#include "../Foundation.Common/MathUtility.h"

CRectAnnotation::CRectAnnotation()
{
	SetAnnotationType(AnnotationType::RectAnnotation);

	GetRectItem().setZValue(1);

	GetRectItem().setPen(GetDefaultLinePen());

	GetRectItem().setFlags(
		QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemSendsGeometryChanges |
		QGraphicsItem::ItemIsFocusable);

	for (int i = 0; i < 4; i++)
	{
		auto rectItem =
			CreateControlPointRectItem();

		GetControlPointRectItem().push_back(rectItem);
	}
}

void CRectAnnotation::RemoveFromScene(QGraphicsScene * pScene)
{
	CAnnotationBase::RemoveFromScene(pScene);

	if (pScene != NULL_SYMBAL)
		pScene->removeItem(&GetRectItem());
}

void CRectAnnotation::UpdatePositions(double dbScale,
	double dbXOffset, double dbYOffset,
	QPointF &suggestResultTextScenePos)
{
	CAnnotationBase::UpdatePositions(dbScale, dbXOffset, dbYOffset, suggestResultTextScenePos);

	GetRectItem().resetTransform();
	GetRectItem().setPos(dbXOffset, dbYOffset);
	GetRectItem().setScale(dbScale);
}

void CRectAnnotation::AddToScene(QGraphicsScene * pScene)
{
	CAnnotationBase::AddToScene(pScene);

	if (pScene != NULL_SYMBAL)
		pScene->addItem(&GetRectItem());
}

int CRectAnnotation::MaxControlPointCount()
{
	return 4;
}

void CRectAnnotation::ComputeResult()
{
	const int nCount = GetOrientPointCount();

	if (nCount < 3)
		return;

	if (GetImagePlanModule().expired())
		return;

	auto imagePlan = GetImagePlanModule().lock();

	auto points = new POINT[nCount];

	auto nIndex = 0;
	for (auto &it : GetOrientPoints())
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
	if (imagePlan->GetIsSampleWord())
	{
		if (imagePlan->GetIsSigned())
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

bool CRectAnnotation::OnMouseLeftButtonDownInternal(int x, int y)
{
	auto result = false;

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
		AddPoint(x, y);// top-left
		AddPoint(x, y);// top-right
		AddPoint(x, y);// bottom-left
		AddPoint(x, y);// bottom-right

		GetRectItem().setRect(x, y, 0, 0);
		
		result = true;
	}

	UpdateControlPointPosition();
	
	GetLastMousePosition().x = x;
	GetLastMousePosition().y = y;

	return result;
}

#define TOP_LEFT_RECT_ANNOTATION_INDEX 0
#define TOP_RIGHT_RECT_ANNOTATION_INDEX 1
#define BOTTOM_LEFT_RECT_ANNOTATION_INDEX 2
#define BOTTOM_RIGHT_RECT_ANNOTATION_INDEX 3

bool CRectAnnotation::OnMouseMovingInternal(int x, int y)
{
	auto result = false;
	auto pointCount = GetOrientPointCount();

	if (GetIsCompleted())
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
			auto itTopLeft = GetOrientPoints().begin();
			auto itTopRight = itTopLeft + TOP_RIGHT_RECT_ANNOTATION_INDEX;
			auto itBottomLeft = itTopLeft + BOTTOM_LEFT_RECT_ANNOTATION_INDEX;
			auto itBottomRight = itTopLeft + BOTTOM_RIGHT_RECT_ANNOTATION_INDEX;

			switch (GetSelectedControlPointIndex())
			{
			case TOP_LEFT_RECT_ANNOTATION_INDEX:
				itTopLeft->updateFrom(x, y);
				itTopRight->updateFrom(itBottomRight->x, y);
				itBottomLeft->updateFrom(x, itBottomLeft->y);
				break;

			case TOP_RIGHT_RECT_ANNOTATION_INDEX:
				itTopRight->updateFrom(x, y);
				itTopLeft->updateFrom(itTopLeft->x, y);
				itBottomRight->updateFrom(x, itBottomRight->y);
				break;

			case BOTTOM_LEFT_RECT_ANNOTATION_INDEX:
				itBottomLeft->updateFrom(x, y);
				itTopLeft->updateFrom(x, itTopLeft->y);
				itBottomRight->updateFrom(itBottomRight->x, y);
				break;

			case BOTTOM_RIGHT_RECT_ANNOTATION_INDEX:
				itBottomRight->updateFrom(x, y);
				itTopRight->updateFrom(x, itTopLeft->y);
				itBottomLeft->updateFrom(itTopLeft->x, y);
				break;
			default:
				//Empty
				;
			}
			result = true;
		}
	}
	else
	{
		auto itTopLeft = GetOrientPoints().begin();
		auto itTopRight = itTopLeft + TOP_RIGHT_RECT_ANNOTATION_INDEX;
		auto itBottomLeft = itTopLeft + BOTTOM_LEFT_RECT_ANNOTATION_INDEX;
		auto itBottomRight = itTopLeft + BOTTOM_RIGHT_RECT_ANNOTATION_INDEX;

		itBottomRight->updateFrom(x, y);
		itTopRight->updateFrom(x, itTopLeft->y);
		itBottomLeft->updateFrom(itTopLeft->x, y);

		//GetRectItem().setRect(
		//	min(itTopLeft->x, itBottomRight->x),
		//	min(itTopLeft->y, itBottomRight->y),
		//	abs(itBottomRight->x - itTopLeft->x),
		//	abs(itBottomRight->y - itTopLeft->y));
		
		result = true;
	}

	UpdateControlPointPosition();
	
	GetLastMousePosition().x = x;
	GetLastMousePosition().y = y;

	return result;
}

bool CRectAnnotation::OnMouseLeftButtonUpInternal(int x, int y)
{
	/*auto result = CAnnotationBase::OnMouseLeftButtonUpInternal(x, y);

	if (!result)
		return result;
	*/
	auto itTopLeft = GetOrientPoints().begin();
	auto itTopRight = itTopLeft + TOP_RIGHT_RECT_ANNOTATION_INDEX;
	auto itBottomLeft = itTopLeft + BOTTOM_LEFT_RECT_ANNOTATION_INDEX;
	auto itBottomRight = itTopLeft + BOTTOM_RIGHT_RECT_ANNOTATION_INDEX;

	if (GetIsCompleted())
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
			}
			else
			{
				return false;
			}
		}
		else
		{
			/*auto itTopLeft = GetOrientPoints().begin();
			auto itTopRight = itTopLeft + TOP_RIGHT_RECT_ANNOTATION_INDEX;
			auto itBottomLeft = itTopLeft + BOTTOM_LEFT_RECT_ANNOTATION_INDEX;
			auto itBottomRight = itTopLeft + BOTTOM_RIGHT_RECT_ANNOTATION_INDEX;
*/
			switch (GetSelectedControlPointIndex())
			{
			case TOP_LEFT_RECT_ANNOTATION_INDEX:
				itTopLeft->updateFrom(x, y);
				itTopRight->updateFrom(itBottomRight->x, y);
				itBottomLeft->updateFrom(x, itBottomLeft->y);
				break;

			case TOP_RIGHT_RECT_ANNOTATION_INDEX:
				itTopRight->updateFrom(x, y);
				itTopLeft->updateFrom(itTopLeft->x, y);
				itBottomRight->updateFrom(x, itBottomRight->y);
				break;

			case BOTTOM_LEFT_RECT_ANNOTATION_INDEX:
				itBottomLeft->updateFrom(x, y);
				itTopLeft->updateFrom(x, itTopLeft->y);
				itBottomRight->updateFrom(itBottomRight->x, y);
				break;

			case BOTTOM_RIGHT_RECT_ANNOTATION_INDEX:
				itBottomRight->updateFrom(x, y);
				itTopRight->updateFrom(x, itTopLeft->y);
				itBottomLeft->updateFrom(itTopLeft->x, y);
				break;
			default:
				//Empty
				;
			}
		}
		//if (GetSelectedControlPointIndex() <= INVALID_CONTROL_POINT_INDEX
		//	&& !GetIsTranslateAnnotation())
		//	return false;

		//switch(GetSelectedControlPointIndex())
		//{
		//case TOP_LEFT_RECT_ANNOTATION_INDEX:
		//	itTopLeft->updateFrom(x, y);
		//	itTopRight->updateFrom(itBottomRight->x, y);
		//	itBottomLeft->updateFrom(x, itBottomLeft->y);
		//	break;

		//case TOP_RIGHT_RECT_ANNOTATION_INDEX:
		//	itTopRight->updateFrom(x, y);
		//	itTopLeft->updateFrom(itTopLeft->x, y);
		//	itBottomRight->updateFrom(x, itBottomRight->y);
		//	break;

		//case BOTTOM_LEFT_RECT_ANNOTATION_INDEX:
		//	itBottomLeft->updateFrom(x, y);
		//	itTopLeft->updateFrom(x, itTopLeft->y);
		//	itBottomRight->updateFrom(itBottomRight->x, y);
		//	break;

		//case BOTTOM_RIGHT_RECT_ANNOTATION_INDEX:
		//	itBottomRight->updateFrom(x, y);
		//	itTopRight->updateFrom(x, itTopLeft->y);
		//	itBottomLeft->updateFrom(itTopLeft->x, y);
		//	break;
		//default: 
		//	//Empty
		//	;
		//}
	}
	else
	{
		itBottomRight->updateFrom(x, y);
		itTopRight->updateFrom(x, itTopLeft->y);
		itBottomLeft->updateFrom(itTopLeft->x, y);

		GetRectItem().setRect(
			min(itTopLeft->x, itBottomRight->x),
			min(itTopLeft->y, itBottomRight->y),
			abs(itBottomRight->x - itTopLeft->x),
			abs(itBottomRight->y - itTopLeft->y));
	}

	SetIsCompleted(true);
	SetSelectedControlPointIndex(INVALID_CONTROL_POINT_INDEX);
	SetIsTranslateAnnotation(false);

	UpdateControlPointPosition();
	
	GetLastMousePosition().x = INVALID_CONTROL_POINT_POSITION;
	GetLastMousePosition().y = INVALID_CONTROL_POINT_POSITION;

	return true;
}

void CRectAnnotation::UpdateControlPointPosition()
{
	auto itTopLeft = GetOrientPoints().begin();
	auto itTopRight = itTopLeft + TOP_RIGHT_RECT_ANNOTATION_INDEX;
	auto itBottomLeft = itTopLeft + BOTTOM_LEFT_RECT_ANNOTATION_INDEX;
	auto itBottomRight = itTopLeft + BOTTOM_RIGHT_RECT_ANNOTATION_INDEX;

	GetRectItem().setRect(
		min(itTopLeft->x, itBottomRight->x),
		min(itTopLeft->y, itBottomRight->y),
		abs(itBottomRight->x - itTopLeft->x),
		abs(itBottomRight->y - itTopLeft->y));
	
	auto itTopLeftItem = GetControlPointRectItem().begin();
	auto itTopRightItem = itTopLeftItem + TOP_RIGHT_RECT_ANNOTATION_INDEX;
	auto itBottomLeftItem = itTopLeftItem + BOTTOM_LEFT_RECT_ANNOTATION_INDEX;
	auto itBottomRightItem = itTopLeftItem + BOTTOM_RIGHT_RECT_ANNOTATION_INDEX;

	(*itTopLeftItem)->setRect(
		itTopLeft->x - 5, itTopLeft->y - 5, 10, 10);

	(*itTopRightItem)->setRect(
		itTopRight->x - 5, itTopRight->y - 5, 10, 10);

	(*itBottomLeftItem)->setRect(
		itBottomLeft->x - 5, itBottomLeft->y - 5, 10, 10);

	(*itBottomRightItem)->setRect(
		itBottomRight->x - 5, itBottomRight->y - 5, 10, 10);

}

bool CRectAnnotation::IsHitAnnotation(int x, int y)
{
	auto itTopLeft = GetOrientPoints().begin();
	auto itTopRight = itTopLeft + TOP_RIGHT_RECT_ANNOTATION_INDEX;
	auto itBottomLeft = itTopLeft + BOTTOM_LEFT_RECT_ANNOTATION_INDEX;
	auto itBottomRight = itTopLeft + BOTTOM_RIGHT_RECT_ANNOTATION_INDEX;

	if (CMathUtility::IsHitLine(itTopLeft->x, itTopLeft->y,
		itTopRight->x, itTopRight->y,
		x, y,
		ANNOTATION_HIT_TEST_OFFSET))
		return true;

	if (CMathUtility::IsHitLine(itTopLeft->x, itTopLeft->y,
		itBottomLeft->x, itBottomLeft->y,
		x, y,
		ANNOTATION_HIT_TEST_OFFSET))
		return true;

	if (CMathUtility::IsHitLine(itBottomRight->x, itBottomRight->y,
		itTopRight->x, itTopRight->y,
		x, y,
		ANNOTATION_HIT_TEST_OFFSET))
		return true;

	if (CMathUtility::IsHitLine(itBottomRight->x, itBottomRight->y,
		itBottomLeft->x, itBottomLeft->y,
		x, y,
		ANNOTATION_HIT_TEST_OFFSET))
		return true;
	
	return false;
}
