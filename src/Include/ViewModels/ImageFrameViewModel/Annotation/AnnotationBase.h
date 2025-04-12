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

#ifndef ANNOTATION_BASE_D3BD454806A741C2A645165826314509
#define ANNOTATION_BASE_D3BD454806A741C2A645165826314509

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <ViewModels/ImageFrameViewModel/imageframeviewmodel_global.h>

#include <utilities/ImageModule/Modules/ImagePlanModule.h>

#include <Foundations/Common/Math/Point2D.h>

#include <vector>
#include "Foundations/Interfaces/Enums/AnnotationType.h"

#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QFont>
#include <QColor>
#include <QPen>
#include <QPointF>
#include <QGraphicsRectItem>

#define ANNOTATION_HIT_TEST_OFFSET 10
#define INVALID_CONTROL_POINT_INDEX -1
#define INVALID_CONTROL_POINT_POSITION -9999

class CDisplayImage;

/**
 * 标注基类
 */
class IMAGEFRAMEVIEWMODEL_API CAnnotationBase
{
public:
	CAnnotationBase();
	virtual ~CAnnotationBase() = default;

	bool IsMe(CAnnotationBase *pCheck);
	
	/**
	 * 添加标注点
	 */
	virtual void AddPoint(int x, int y);

	/**
	 * 是否位于标注控制点附近
	 */
	virtual int IsHitControlPoint(int x, int y);

	/**
	 * 是否位于标注附近
	 */
	virtual bool IsHitAnnotation(int x, int y);

	/**
	 * 选择控制点
	 */
	virtual bool OnMouseLeftButtonDown(int x, int y);

	/**
	 * 调整控制点
	 */
	virtual bool OnMouseMoving(int x, int y);

	/**
	 * 应用控制点
	 */
	virtual bool OnMouseLeftButtonUp(int x, int y);

	virtual bool OnMouseRightButtonDown(int x, int y);

	/**
	 * 添加标注控件到scene
	 */
	virtual void AddToScene(QGraphicsScene *pScene);

	/**
	 * 从scene中移除标注
	 */
	virtual void RemoveFromScene(QGraphicsScene *pScene);

	/**
	 * 更新点位置
	 * @suggestResultTextScenePos 建议的标注结果文本位置(基于scene坐标)
	 */
	virtual void UpdatePositions(double dbScale,
		double dbXOffset, double dbYOffset,
		QPointF &suggestResultTextScenePos);

	virtual QPointF GetSuggestResultTextPosition();

	virtual void UpdateControlPointPosition() {}
	
protected:
	virtual int GetOrientPointCount();

	virtual int MaxControlPointCount() = 0;

	virtual void ComputeResult() = 0;
	/**
	 * 选择控制点
	 */
	virtual bool OnMouseLeftButtonDownInternal(int x, int y) = 0;

	/**
	 * 调整控制点
	 */
	virtual bool OnMouseMovingInternal(int x, int y) = 0;

	/**
	 * 应用控制点
	 */
	virtual bool OnMouseLeftButtonUpInternal(int x, int y);


	virtual void GetOrientPointBound(int &nLeft, int &nTop, int &nRight, int &nBottom);

	virtual void HideControlPoints();

	virtual void DisplayControlPoints();

	static std::shared_ptr<QGraphicsRectItem> CreateControlPointRectItem();
	
protected:
	using OrientPointArrayType = std::vector<sctPoint2D>;
	using OrientPointIndexType = OrientPointArrayType::iterator;
	using ControlPointRectsItemType = std::vector<std::shared_ptr<QGraphicsRectItem>>;
	
private:
	AUTO_STATIC_PROPERTY_REF(DefaultResultTextFont, QFont);
	AUTO_STATIC_PROPERTY_REF(DefaultResultTextForground, QColor);
	AUTO_STATIC_PROPERTY_REF(DefaultLinePen, QPen);
	
	AUTO_PROPERTY(InternalId, long);
	
	AUTO_PROPERTY(AnnotationType, AnnotationType);

	AUTO_PROPERTY(IsCompleted, bool);

	/**
	 * 图像参数信息
	 */
	AUTO_PROPERTY_REF(ImagePlanModule, std::weak_ptr<CImagePlanModule>);

	/**
	 * 归属的DisplayImage
	 */
	AUTO_PROPERTY_REF(ReferenceImage, std::weak_ptr<CDisplayImage>);

	/**
	 * 基于图像坐标的标注点
	 */
	AUTO_PROPERTY_REF(OrientPoints, OrientPointArrayType);

	/**
	 * 基于图像坐标的标注点
	 */
	AUTO_PROPERTY_REF(SelectedControlPointIndex, int);

	/**
	 * 计算结果
	 */
	AUTO_PROPERTY_REF(Result, STRING_TYPE);

	AUTO_PROPERTY_REF_NO_SET(AnnotationResultTextItem, QGraphicsTextItem);

	/**
	 * Mouse Move动作是否为移动标注
	 */
	AUTO_PROPERTY_REF(IsTranslateAnnotation, bool);

	AUTO_PROPERTY_REF(LastMousePosition, sctPoint2D);

	AUTO_PROPERTY_REF(ResultPositionInScene, QPointF);

	AUTO_PROPERTY_REF_NO_SET(ControlPointRectItem, ControlPointRectsItemType);
};

#endif

