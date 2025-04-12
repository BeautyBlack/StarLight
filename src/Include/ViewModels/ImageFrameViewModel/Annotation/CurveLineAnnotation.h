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
 * �Ǳպ�����
 * -- ��ʵ�ʿ��Ƶ�, ���ṩ�ƶ�
 * -- ��ʾ�Ŀ��Ƶ��������ʾ��ǰ���λ�ÿ��Լ����ע
 */
class IMAGEFRAMEVIEWMODEL_API CCurveLineAnnotation : public CAnnotationBase
{
public:
	CCurveLineAnnotation();
	virtual ~CCurveLineAnnotation() = default;


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

	/**
	 * �Ƿ�λ�ڱ�ע����
	 */
	bool IsHitAnnotation(int x, int y) override;
	
private:
	AUTO_PROPERTY_REF_NO_SET(CurveLine, QGraphicsPathItem);

	AUTO_PROPERTY_REF_NO_SET(CurveLinePath, QPainterPath);
};

#endif

