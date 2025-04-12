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

#ifndef POLYGON_ANNOTATION_F0865017601F430E914FE6E0B1CA60A2
#define POLYGON_ANNOTATION_F0865017601F430E914FE6E0B1CA60A2

#pragma once

#include <viewmodels/ImageFrameViewModel/Annotation/AnnotationBase.h>

#include <QPolygon>
#include <QGraphicsPolygonItem>

/**
 * 闭合曲线标注
 * -- 无实际控制点, 仅提供移动
 * -- 显示的控制点仅用于提示当前鼠标位置可以激活本标注
 * 
 * 由于画图时,以画线方式呈现,为了能画出封闭区域,OrientPoint集合的首尾点是同一位置的
 * 鼠标按下开始
 */
class IMAGEFRAMEVIEWMODEL_API CPolygonAnnotation : public CAnnotationBase
{
public:
	CPolygonAnnotation();
	virtual ~CPolygonAnnotation() = default;


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

	QPointF GetSuggestResultTextPosition() override;

	void UpdateControlPointPosition() override;

	/**
	 * 是否位于标注附近
	 */
	bool IsHitAnnotation(int x, int y) override;
	
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


	//bool OnMouseRightButtonDown(int x, int y) override;

private:
	AUTO_PROPERTY_REF_NO_SET(CurveLine, QGraphicsPolygonItem);

	AUTO_PROPERTY_REF_NO_SET(CurveLinePolygon, QPolygon);
};

#endif

