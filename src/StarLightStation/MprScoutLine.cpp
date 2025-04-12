#include "pch.h"
#include "MprScoutLine.h"

#include <ViewModels/MprFrameViewModel/MprDisplayImage.h>
#include <Foundations/Common/Math/Point2D.h>
#include "../Foundation.Common/MathUtility.h"
#include "utilities/Messenger/EventArgs/UI/MprFrame/MprRotateScoutLineEventArgs.h"
#include "utilities/Messenger/EventArgs/UI/MprFrame/MprUpdateScoutLineCenterEventArgs.h"

//QColor CMprScoutLine::m_DefaultCoronalLineQColor(255, 0, 0);
//
//QColor CMprScoutLine::m_DefaultSaggitalLineQColor(0, 255, 0);
//
//QColor CMprScoutLine::m_DefaultTransverseLineQColor(0, 125, 255);

QPen CMprScoutLine::m_DefaultCoronalLinePen(
	QBrush(QColor(255, 0, 0)),
	1,
	Qt::SolidLine);

QPen CMprScoutLine::m_DefaultSaggitalLinePen(
	QBrush(QColor(0, 255, 0)),
	1,
	Qt::SolidLine);

QPen CMprScoutLine::m_DefaultTransverseLinePen(
	QBrush(QColor(0, 125, 255)),
	1,
	Qt::SolidLine);

CMprScoutLine::CMprScoutLine()
{
	SetReferenceImage(NULL_SYMBAL);

	SetIsHitControlPoint(false);
	SetIsMouseDown(false);
	SetHitScoutLineType(HIT_SCOUT_LINE_TYPE::Unkown);
	SetLastHitPoint(QPointF(0, 0));
	
	GetScoutLineX().setZValue(1);

	GetScoutLineX().setFlags(
		QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemSendsGeometryChanges |
		QGraphicsItem::ItemIsFocusable);
	
	GetScoutLineZ().setZValue(1);

	GetScoutLineZ().setFlags(
		QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemSendsGeometryChanges |
		QGraphicsItem::ItemIsFocusable);

	GetScoutLineXControl1().setZValue(1);
	GetScoutLineXControl1().setFlags(
		QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemSendsGeometryChanges |
		QGraphicsItem::ItemIsFocusable);

	GetScoutLineXControl2().setZValue(1);
	GetScoutLineXControl2().setFlags(
		QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemSendsGeometryChanges |
		QGraphicsItem::ItemIsFocusable);

	GetScoutLineZControl1().setZValue(1);
	GetScoutLineZControl1().setFlags(
		QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemSendsGeometryChanges |
		QGraphicsItem::ItemIsFocusable);

	GetScoutLineZControl2().setZValue(1);
	GetScoutLineZControl2().setFlags(
		QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemSendsGeometryChanges |
		QGraphicsItem::ItemIsFocusable);
}

void CMprScoutLine::AddToScene(QGraphicsScene* pScene)
{
	if (pScene != NULL_SYMBAL)
	{
		pScene->addItem(&GetScoutLineX());
		pScene->addItem(&GetScoutLineZ());

		pScene->addItem(&GetScoutLineXControl1());
		pScene->addItem(&GetScoutLineXControl2());

		pScene->addItem(&GetScoutLineZControl1());
		pScene->addItem(&GetScoutLineZControl2());
	}
}

void CMprScoutLine::UpdatePositions(double dbScale,
	double dbXOffset, double dbYOffset)
{
	GetScoutLineX().resetTransform();
	GetScoutLineX().setPos(dbXOffset, dbYOffset);
	GetScoutLineX().setScale(dbScale);

	GetScoutLineZ().resetTransform();
	GetScoutLineZ().setPos(dbXOffset, dbYOffset);
	GetScoutLineZ().setScale(dbScale);

	GetScoutLineXControl1().resetTransform();
	GetScoutLineXControl1().setPos(dbXOffset, dbYOffset);
	GetScoutLineXControl1().setScale(dbScale);

	GetScoutLineXControl2().resetTransform();
	GetScoutLineXControl2().setPos(dbXOffset, dbYOffset);
	GetScoutLineXControl2().setScale(dbScale);

	GetScoutLineZControl1().resetTransform();
	GetScoutLineZControl1().setPos(dbXOffset, dbYOffset);
	GetScoutLineZControl1().setScale(dbScale);

	GetScoutLineZControl2().resetTransform();
	GetScoutLineZControl2().setPos(dbXOffset, dbYOffset);
	GetScoutLineZControl2().setScale(dbScale);

	if (GetReferenceImage() == NULL_SYMBAL)
		return;

	switch(GetReferenceImage()->GetPlanDirection())
	{
	case ImagePlanDirection::Sagittal:
		GetScoutLineX().setPen(GetDefaultTransverseLinePen());
		GetScoutLineZ().setPen(GetDefaultCoronalLinePen());

		GetScoutLineXControl1().setPen(GetDefaultTransverseLinePen());
		GetScoutLineXControl2().setPen(GetDefaultTransverseLinePen());

		GetScoutLineZControl1().setPen(GetDefaultCoronalLinePen());
		GetScoutLineZControl2().setPen(GetDefaultCoronalLinePen());
		break;

	case ImagePlanDirection::Coronal:
		GetScoutLineX().setPen(GetDefaultTransverseLinePen());
		GetScoutLineZ().setPen(GetDefaultSaggitalLinePen());

		GetScoutLineXControl1().setPen(GetDefaultTransverseLinePen());
		GetScoutLineXControl2().setPen(GetDefaultTransverseLinePen());

		GetScoutLineZControl1().setPen(GetDefaultSaggitalLinePen());
		GetScoutLineZControl2().setPen(GetDefaultSaggitalLinePen());
		break;
		
	default:
		GetScoutLineX().setPen(GetDefaultCoronalLinePen());
		GetScoutLineZ().setPen(GetDefaultSaggitalLinePen());

		GetScoutLineXControl1().setPen(GetDefaultCoronalLinePen());
		GetScoutLineXControl2().setPen(GetDefaultCoronalLinePen());

		GetScoutLineZControl1().setPen(GetDefaultSaggitalLinePen());
		GetScoutLineZControl2().setPen(GetDefaultSaggitalLinePen());
		break;
	}
	
	GetScoutLineX().setLine(
		GetReferenceImage()->GetScoutLineBeginX2D().x,
		GetReferenceImage()->GetScoutLineBeginX2D().y,
		GetReferenceImage()->GetScoutLineEndX2D().x,
		GetReferenceImage()->GetScoutLineEndX2D().y);

	GetScoutLineZ().setLine(
		GetReferenceImage()->GetScoutLineBeginZ2D().x,
		GetReferenceImage()->GetScoutLineBeginZ2D().y,
		GetReferenceImage()->GetScoutLineEndZ2D().x,
		GetReferenceImage()->GetScoutLineEndZ2D().y);

	GetScoutLineXControl1().setRect(
		GetReferenceImage()->GetScoutLineBeginX2D().x - 5,
		GetReferenceImage()->GetScoutLineBeginX2D().y - 5,
		10, 10);
	GetScoutLineXControl2().setRect(
		GetReferenceImage()->GetScoutLineEndX2D().x - 5,
		GetReferenceImage()->GetScoutLineEndX2D().y - 5,
		10, 10);

	GetScoutLineZControl1().setRect(
		GetReferenceImage()->GetScoutLineBeginZ2D().x - 5,
		GetReferenceImage()->GetScoutLineBeginZ2D().y - 5,
		10, 10);
	GetScoutLineZControl2().setRect(
		GetReferenceImage()->GetScoutLineEndZ2D().x - 5,
		GetReferenceImage()->GetScoutLineEndZ2D().y - 5,
		10, 10);
}

bool CMprScoutLine::OnMouseLeftButtonDown(const QPointF &point)
{
	SetIsHitControlPoint(false);

	SetIsMouseDown(true);
	SetLastHitPoint(point);

	if(GetScoutLineXControl1().contains(point))
	{
		SetIsHitControlPoint(true);

		SetLastHitPoint(point);

		SetHitScoutLineType(HIT_SCOUT_LINE_TYPE::XControl1);

		return true;
	}

	if (GetScoutLineXControl2().contains(point))
	{
		SetIsHitControlPoint(true);

		SetHitScoutLineType(HIT_SCOUT_LINE_TYPE::XControl2);

		return true;
	}

	if (GetScoutLineZControl1().contains(point))
	{
		SetIsHitControlPoint(true);

		SetHitScoutLineType(HIT_SCOUT_LINE_TYPE::ZControl1);

		return true;
	}

	if (GetScoutLineZControl2().contains(point))
	{
		SetIsHitControlPoint(true);

		SetHitScoutLineType(HIT_SCOUT_LINE_TYPE::ZControl2);

		return true;
	}

	UpdateScoutLineCenter(point);

	return true;
}

bool CMprScoutLine::OnMouseMoving(const QPointF &point)
{
	if( !GetIsMouseDown() || !GetReferenceImage())
		return false;

	if(GetIsHitControlPoint())
	{
		auto &center = GetReferenceImage()->GetCenter2D();

		sctPoint2D begin(GetLastHitPoint().x(), GetLastHitPoint().y());
		sctPoint2D end(point.x(), point.y());
		
		auto angle =
			CMathUtility::ComputeAngle2D(
				center,
				begin,
				end,
				GetReferenceImage()->GetPixelSpacingX(),
				GetReferenceImage()->GetPixelSpacingY());
			
		if (abs(angle) <= 0.001)
			return true;

		CMprRotateScoutLineEventArgs eventArgs;

		eventArgs.SetAngle(angle);
		eventArgs.SetImagePlan(GetReferenceImage()->GetPlanDirection());
		
		switch (GetHitScoutLineType())
		{
		case HIT_SCOUT_LINE_TYPE::Unkown:
			return false;
		
		case HIT_SCOUT_LINE_TYPE::XControl1:
		case HIT_SCOUT_LINE_TYPE::XControl2:
			eventArgs.SetIsVectorX(true);
			break;
		
		case HIT_SCOUT_LINE_TYPE::ZControl1:
		case HIT_SCOUT_LINE_TYPE::ZControl2:
			eventArgs.SetIsVectorX(false);
			break;
		
		default:
			return false;
		}

		SetLastHitPoint(point);

		CMessenger::GetInstance().SendMessage(
			EMessengerKey::MprScoutLineAngleChanged,
			&eventArgs);
	}
	else
	{
		UpdateScoutLineCenter(point);
	}

	return true;
}

bool CMprScoutLine::OnMouseLeftButtonUp(const QPointF &point)
{
	if (!GetIsMouseDown())
		return false;

	if (GetIsHitControlPoint())
	{

	}
	else
	{
		UpdateScoutLineCenter(point);
	}

	SetIsHitControlPoint(false);
	SetIsMouseDown(false);
	SetHitScoutLineType(HIT_SCOUT_LINE_TYPE::Unkown);
	SetLastHitPoint(QPointF(0, 0));

	return true;
}

void CMprScoutLine::UpdateScoutLineCenter(const QPointF &point)
{
	// 更新坐标中心点
	CMprUpdateScoutLineCenterEventArgs eventArgs;

	eventArgs.SetImagePlan(GetReferenceImage()->GetPlanDirection());
	eventArgs.SetNewCenter(sctPoint2D(point.x(), point.y()));

	CMessenger::GetInstance().SendMessage(
		EMessengerKey::MprUpdateScoutLineCenter,
		//this,
		&eventArgs);
}
