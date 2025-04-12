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
	 * �Ƿ�λ�ڱ�ע����
	 */
	bool IsHitAnnotation(int x, int y) override;

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

	void UpdateControlPointPosition() override;
	
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

	void TranslateAnnotation(double dbOffsetX, double dbOffsetY);

private:
	AUTO_PROPERTY_REF_NO_SET(LineItem, QGraphicsLineItem);
};

#endif

