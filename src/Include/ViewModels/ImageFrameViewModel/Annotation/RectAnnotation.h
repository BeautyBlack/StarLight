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

#ifndef RECT_ANNOTATION_7E1D50A54DC749F4B156FC572A7D854B
#define RECT_ANNOTATION_7E1D50A54DC749F4B156FC572A7D854B

#pragma once

#include <viewmodels/ImageFrameViewModel/Annotation/AnnotationBase.h>

#include <QGraphicsRectItem>

class IMAGEFRAMEVIEWMODEL_API CRectAnnotation : public CAnnotationBase
{
public:
	CRectAnnotation();
	virtual ~CRectAnnotation() = default;


	/**
	 * ��scene���Ƴ���ע
	 */
	void RemoveFromScene(QGraphicsScene *pScene) override;

	void UpdatePositions(double dbScale,
		double dbXOffset, double dbYOffset,
		QPointF &suggestResultTextScenePos) override;

	/**
	 * ��ӱ�ע�ؼ���scene
	 */
	void AddToScene(QGraphicsScene *pScene) override;
	
protected:
	int MaxControlPointCount() override;

	void ComputeResult() override;

	/**
	 * ѡ����Ƶ�
	 */
	bool OnMouseLeftButtonDownInternal(int x, int y) override;

	/**
	 * �������Ƶ�
	 */
	bool OnMouseMovingInternal(int x, int y) override;

	/**
	 * Ӧ�ÿ��Ƶ�
	 */
	bool OnMouseLeftButtonUpInternal(int x, int y) override;

	void UpdateControlPointPosition() override;

	bool IsHitAnnotation(int x, int y) override;
	
private:
	AUTO_PROPERTY_REF_NO_SET(RectItem, QGraphicsRectItem);
};

#endif
