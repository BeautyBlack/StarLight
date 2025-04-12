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

#ifndef LINE_ANNOTATION_8264C60CBDE94FA3A858FE5ECA25A10C
#define LINE_ANNOTATION_8264C60CBDE94FA3A858FE5ECA25A10C

#pragma once

#include <QGraphicsLineItem>

#include <viewmodels/ImageFrameViewModel/Annotation/AnnotationBase.h>

class IMAGEFRAMEVIEWMODEL_API CLineAnnotation : public CAnnotationBase
{
public:
	CLineAnnotation();
	virtual ~CLineAnnotation() = default;

	/**
	 * 是否位于标注附近
	 */
	bool IsHitAnnotation(int x, int y) override;

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

	void UpdateControlPointPosition() override;
	
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

	void TranslateAnnotation(double dbOffsetX, double dbOffsetY);

private:
	AUTO_PROPERTY_REF_NO_SET(LineItem, QGraphicsLineItem);
};

#endif

