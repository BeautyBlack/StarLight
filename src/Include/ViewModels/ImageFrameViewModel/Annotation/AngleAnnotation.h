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

#ifndef ANGLE_ANNOTATION_205AD5C5CE97442DBF525383895CDA15
#define ANGLE_ANNOTATION_205AD5C5CE97442DBF525383895CDA15

#pragma once

#include <viewmodels/ImageFrameViewModel/Annotation/AnnotationBase.h>
#include <ViewModels/ImageFrameViewModel/imageframeviewmodel_global.h>


#include <QPainterPath>
#include <QGraphicsPathItem>

class IMAGEFRAMEVIEWMODEL_API CAngleAnnotation : public CAnnotationBase
{
public:
	CAngleAnnotation();
	virtual ~CAngleAnnotation() = default;


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
	
	bool OnMouseLeftButtonUpInternal(int x, int y) override;

	void UpdateAngle(int x, int y);
	
private:
	AUTO_PROPERTY_REF_NO_SET(CurveLine, QGraphicsPathItem);

	AUTO_PROPERTY_REF_NO_SET(CurveLinePath, QPainterPath);
};


#endif

