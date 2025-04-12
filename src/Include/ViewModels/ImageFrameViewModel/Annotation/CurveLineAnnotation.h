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

#ifndef CURVE_LINE_ANNOTATION_869FDA8E4FC84E08B9FD71A78015DDAE
#define CURVE_LINE_ANNOTATION_869FDA8E4FC84E08B9FD71A78015DDAE

#pragma once

#include <viewmodels/ImageFrameViewModel/Annotation/AnnotationBase.h>

#include <QPainterPath>
#include <QGraphicsPathItem>

/**
 * 非闭合曲线
 * -- 无实际控制点, 仅提供移动
 * -- 显示的控制点仅用于提示当前鼠标位置可以激活本标注
 */
class IMAGEFRAMEVIEWMODEL_API CCurveLineAnnotation : public CAnnotationBase
{
public:
	CCurveLineAnnotation();
	virtual ~CCurveLineAnnotation() = default;


	/**
	 * 从scene中移除标注
	 */
	void RemoveFromScene(QGraphicsScene *pScene) override;

	void UpdatePositions(double dbScale,
		double dbXOffset, double dbYOffset,
		QPointF &suggestResultTextScenePos) override;

	/**
	 * 添加标注控件到scene
	 */
	void AddToScene(QGraphicsScene *pScene) override;

protected:
	int MaxControlPointCount() override;

	void ComputeResult() override;

	/**
	 * 选择控制点
	 */
	bool OnMouseLeftButtonDownInternal(int x, int y) override;

	/**
	 * 调整控制点
	 */
	bool OnMouseMovingInternal(int x, int y) override;

	/**
	 * 应用控制点
	 */
	bool OnMouseLeftButtonUpInternal(int x, int y) override;

	void UpdateControlPointPosition() override;

	/**
	 * 是否位于标注附近
	 */
	bool IsHitAnnotation(int x, int y) override;
	
private:
	AUTO_PROPERTY_REF_NO_SET(CurveLine, QGraphicsPathItem);

	AUTO_PROPERTY_REF_NO_SET(CurveLinePath, QPainterPath);
};

#endif

