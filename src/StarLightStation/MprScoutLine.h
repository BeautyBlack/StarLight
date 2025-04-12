/*
 *****************************************************************
 *                 C O P Y R I G H T  (c) 2020
 *                    All Rights Reserved
 *****************************************************************
 *
 *       THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF
 *                      Zhang Jiaqi.
 *      The copyright notice above does not evidence any
 *     actual or intended publication of such source code.
 *
 *****************************************************************
 *
 *  Author: Zhang Jiaqi
 *
 */

#ifndef MPR_SCOUT_LINE_AFEEC0F8258F48ECADAC719581E69C5E
#define MPR_SCOUT_LINE_AFEEC0F8258F48ECADAC719581E69C5E

#pragma once


#include <Foundations/Common/ToDefine.h>

//#include <utilities/ImageModule/Modules/ImagePlanModule.h>

#include <vector>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QFont>
#include <QColor>
#include <QPen>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include "utilities/Messenger/Messenger.h"

class CMprDisplayImage;

class CMprScoutLine : public CMessengerOwner
{
private:
	enum class HIT_SCOUT_LINE_TYPE
	{
		Unkown = 0,

		XControl1,

		XControl2,

		ZControl1,

		ZControl2
	};

public:
	CMprScoutLine();
	~CMprScoutLine() = default;

	/**
	 * 添加标注控件到scene
	 */
	virtual void AddToScene(QGraphicsScene *pScene);

	virtual void UpdatePositions(double dbScale,
		double dbXOffset, double dbYOffset);
	
	/**
	 * 选择控制点
	 */
	virtual bool OnMouseLeftButtonDown(const QPointF &point);

	/**
	 * 调整控制点
	 */
	virtual bool OnMouseMoving(const QPointF &point);

	/**
	 * 应用控制点
	 */
	virtual bool OnMouseLeftButtonUp(const QPointF &point);

private:
	void UpdateScoutLineCenter(const QPointF &point);

private:
	AUTO_STATIC_PROPERTY_REF(DefaultTransverseLinePen, QPen);
	AUTO_STATIC_PROPERTY_REF(DefaultSaggitalLinePen, QPen);
	AUTO_STATIC_PROPERTY_REF(DefaultCoronalLinePen, QPen);
	AUTO_STATIC_PROPERTY_REF(DefaultTransverseLineBrush, QBrush);
	AUTO_STATIC_PROPERTY_REF(DefaultSaggitalLineBrush, QBrush);
	AUTO_STATIC_PROPERTY_REF(DefaultCoronalLineBrush, QBrush);

	/**
	 * 图像参数信息
	 */
	//AUTO_PROPERTY_REF(ImagePlanModule, std::weak_ptr<CImagePlanModule>);

	/**
	 * 归属的DisplayImage
	 */
	AUTO_PROPERTY_REF(ReferenceImage, CMprDisplayImage*);

	AUTO_PROPERTY_REF_NO_SET(ScoutLineX, QGraphicsLineItem);
	AUTO_PROPERTY_REF_NO_SET(ScoutLineZ, QGraphicsLineItem);

	AUTO_PROPERTY_REF_NO_SET(ScoutLineXControl1, QGraphicsRectItem);
	AUTO_PROPERTY_REF_NO_SET(ScoutLineXControl2, QGraphicsRectItem);

	AUTO_PROPERTY_REF_NO_SET(ScoutLineZControl1, QGraphicsRectItem);
	AUTO_PROPERTY_REF_NO_SET(ScoutLineZControl2, QGraphicsRectItem);

	AUTO_PROPERTY(IsHitControlPoint, bool);

	AUTO_PROPERTY_REF(LastHitPoint, QPointF);

	AUTO_PROPERTY_REF(HitScoutLineType, HIT_SCOUT_LINE_TYPE);

	AUTO_PROPERTY(IsMouseDown, bool);
};

#endif

