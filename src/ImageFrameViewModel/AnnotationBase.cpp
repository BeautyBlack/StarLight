#include "stdafx.h"
#include <ViewModels/ImageFrameViewModel/Annotation/AnnotationBase.h>
#include "../Foundation.Common/MathUtility.h"
#include <ViewModels/ImageFrameViewModel/DisplayImage.h>

#include <QTextDocument>

#include "../StarLightStation/ThemeDefine.h"

QFont CAnnotationBase::m_DefaultResultTextFont;
QColor CAnnotationBase::m_DefaultResultTextForground;
QPen CAnnotationBase::m_DefaultLinePen;

CAnnotationBase::CAnnotationBase()
{
	static long staticId = 1;

	auto nextId = InterlockedIncrement(&staticId);

	SetInternalId(nextId);
	
	SetAnnotationType(AnnotationType::UnkownAnnotation);

	SetIsCompleted(false);

	SetSelectedControlPointIndex(INVALID_CONTROL_POINT_INDEX);

	SetIsTranslateAnnotation(false);

	GetAnnotationResultTextItem().setFont(GetDefaultResultTextFont());
	GetAnnotationResultTextItem().setDefaultTextColor(GetDefaultResultTextForground());
	GetAnnotationResultTextItem().setZValue(1);
	/*GetAnnotationResultTextItem().setFlags(
		QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemSendsGeometryChanges |
		QGraphicsItem::ItemIsFocusable);*/
	GetAnnotationResultTextItem().setTextWidth(300);
	GetAnnotationResultTextItem().setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

}

bool CAnnotationBase::IsMe(CAnnotationBase* pCheck)
{
	return pCheck == NULL_SYMBAL ? false :
		pCheck->GetInternalId() == GetInternalId();
}

int CAnnotationBase::GetOrientPointCount()
{
	return static_cast<int>(GetOrientPoints().size());
}

void CAnnotationBase::GetOrientPointBound(int& nLeft, int& nTop, int& nRight, int& nBottom)
{
	nLeft = nTop = INT_MAX;
	nRight = nBottom = 0;

	for(auto &it : GetOrientPoints())
	{
		nLeft = min(it.x, nLeft);
		nTop = min(it.y, nTop);

		nRight = max(it.x, nRight);
		nBottom = max(it.y, nBottom);
	}
}

void CAnnotationBase::HideControlPoints()
{
	for (auto &it : GetControlPointRectItem())
		it->setVisible(false);
}

void CAnnotationBase::DisplayControlPoints()
{
	for (auto &it : GetControlPointRectItem())
		it->setVisible(true);
}

std::shared_ptr<QGraphicsRectItem> CAnnotationBase::CreateControlPointRectItem()
{
	auto rectItem = std::make_shared<QGraphicsRectItem>();

	rectItem->setVisible(false);
	rectItem->setRect(0, 0, 10, 10);
	rectItem->setPen(GetDefaultLinePen());
	rectItem->setZValue(1);

	rectItem->setFlags(
		QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemSendsGeometryChanges |
		QGraphicsItem::ItemIsFocusable);

	return rectItem;
}

QPointF CAnnotationBase::GetSuggestResultTextPosition()
{
	static QPointF ZERO_POINT(0, 0);
	
	auto count = GetOrientPointCount();

	if (count == 0)
		return ZERO_POINT;

	const auto lastPoint = GetOrientPoints()[count - 1];

	return QPoint(lastPoint.x, lastPoint.y);
}

void CAnnotationBase::AddPoint(int x, int y)
{
	if (GetOrientPointCount() >= MaxControlPointCount())
		return;

	GetOrientPoints().push_back(sctPoint2D(x, y));
}

bool CAnnotationBase::IsHitAnnotation(int x, int y)
{
	auto count = GetOrientPointCount();

	/*if (count < MaxControlPointCount())
		return false;*/

	for (auto i = 0; i < count - 1; ++i)
	{
		auto it = GetOrientPoints().begin() + i;
		auto it2 = GetOrientPoints().begin() + (i + 1);

		if (CMathUtility::IsHitLine(it->x, it->y,
			it2->x, it2->y,
			x, y,
			ANNOTATION_HIT_TEST_OFFSET))
			return true;
	}

	return false;
}

int CAnnotationBase::IsHitControlPoint(int x, int y)
{
	double tmp[2], tmp2[2];

	tmp[0] = x;
	tmp[1] = y;

	int nIndex = 0;

	for (auto &it : GetOrientPoints())
	{
		tmp2[0] = it.x;
		tmp2[1] = it.y;

		if (CMathUtility::ComputeDistance(tmp, tmp2, _countof(tmp)) <= ANNOTATION_HIT_TEST_OFFSET)
			return nIndex;

		nIndex++;
	}

	return -1;
}

bool CAnnotationBase::OnMouseLeftButtonDown(int x, int y)
{
	const auto result = OnMouseLeftButtonDownInternal(x, y);

	if (result || GetAnnotationType() == AnnotationType::TextAnnotation)
	{
		ComputeResult();

		if (!GetReferenceImage().expired())
		{
			auto image = GetReferenceImage().lock();

			UpdatePositions(image->GetImageScale(),
				image->GetTranslateX(),
				image->GetTranslateY(),
				GetResultPositionInScene());
		}
	}

	return result;
}

bool CAnnotationBase::OnMouseMoving(int x, int y)
{
	const auto result = OnMouseMovingInternal(x, y);

	if (result)
	{
		ComputeResult();

		if( !GetReferenceImage().expired())
		{
			auto image = GetReferenceImage().lock();
			
			UpdatePositions(image->GetImageScale(),
				image->GetTranslateX(),
				image->GetTranslateY(),
				GetResultPositionInScene());
		}
	}

	return result;
}

bool CAnnotationBase::OnMouseLeftButtonUp(int x, int y)
{
	const auto result = OnMouseLeftButtonUpInternal(x, y);

	if (result)
	{
		ComputeResult();
		
		if (!GetReferenceImage().expired())
		{
			auto image = GetReferenceImage().lock();

			UpdatePositions(image->GetImageScale(),
				image->GetTranslateX(),
				image->GetTranslateY(),
				GetResultPositionInScene());
		}
	}

	if (GetIsCompleted())
		HideControlPoints();

	return result;
}

bool CAnnotationBase::OnMouseRightButtonDown(int x, int y)
{
	return false;
}

void CAnnotationBase::AddToScene(QGraphicsScene* pScene)
{
	if (pScene != NULL_SYMBAL)
	{
		pScene->addItem(&GetAnnotationResultTextItem());

		for (auto &it : GetControlPointRectItem())
			pScene->addItem(it.get());
	}
}

void CAnnotationBase::RemoveFromScene(QGraphicsScene* pScene)
{
	if (pScene == NULL_SYMBAL)
		return;

	pScene->removeItem(&GetAnnotationResultTextItem());

	for (auto &it : GetControlPointRectItem())
		pScene->removeItem(it.get());
}

void CAnnotationBase::UpdatePositions(double dbScale,
	double dbXOffset, double dbYOffset,
	QPointF &suggestResultTextScenePos)
{
	GetAnnotationResultTextItem().resetTransform();
	GetAnnotationResultTextItem().setPos(suggestResultTextScenePos);

	SetResultPositionInScene(suggestResultTextScenePos);
	
	GetAnnotationResultTextItem().setPlainText(
		QString::fromLocal8Bit(
			GetResult().c_str(), GetResult().length()));

	GetAnnotationResultTextItem().setTextWidth(
		GetAnnotationResultTextItem().document()->idealWidth() * 2);

	for(auto &it : GetControlPointRectItem())
	{
		it->resetTransform();
		it->setPos(dbXOffset, dbYOffset);
		it->setScale(dbScale);
	}
}

bool CAnnotationBase::OnMouseLeftButtonUpInternal(int x, int y)
{
	if (GetIsCompleted())
	{
		if (GetSelectedControlPointIndex() <= INVALID_CONTROL_POINT_INDEX
			&& !GetIsTranslateAnnotation())
			return false;
	}

	SetIsCompleted(true);
	SetSelectedControlPointIndex(INVALID_CONTROL_POINT_INDEX);
	SetIsTranslateAnnotation(false);

	GetLastMousePosition().x = INVALID_CONTROL_POINT_POSITION;
	GetLastMousePosition().y = INVALID_CONTROL_POINT_POSITION;

	return true;
}
