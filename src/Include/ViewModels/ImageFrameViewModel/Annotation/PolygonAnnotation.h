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
 * �պ����߱�ע
 * -- ��ʵ�ʿ��Ƶ�, ���ṩ�ƶ�
 * -- ��ʾ�Ŀ��Ƶ��������ʾ��ǰ���λ�ÿ��Լ����ע
 * 
 * ���ڻ�ͼʱ,�Ի��߷�ʽ����,Ϊ���ܻ����������,OrientPoint���ϵ���β����ͬһλ�õ�
 * ��갴�¿�ʼ
 */
class IMAGEFRAMEVIEWMODEL_API CPolygonAnnotation : public CAnnotationBase
{
public:
	CPolygonAnnotation();
	virtual ~CPolygonAnnotation() = default;


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

	QPointF GetSuggestResultTextPosition() override;

	void UpdateControlPointPosition() override;

	/**
	 * �Ƿ�λ�ڱ�ע����
	 */
	bool IsHitAnnotation(int x, int y) override;
	
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


	//bool OnMouseRightButtonDown(int x, int y) override;

private:
	AUTO_PROPERTY_REF_NO_SET(CurveLine, QGraphicsPolygonItem);

	AUTO_PROPERTY_REF_NO_SET(CurveLinePolygon, QPolygon);
};

#endif

