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
 * ��ע����
 */
class IMAGEFRAMEVIEWMODEL_API CAnnotationBase
{
public:
	CAnnotationBase();
	virtual ~CAnnotationBase() = default;

	bool IsMe(CAnnotationBase *pCheck);
	
	/**
	 * ��ӱ�ע��
	 */
	virtual void AddPoint(int x, int y);

	/**
	 * �Ƿ�λ�ڱ�ע���Ƶ㸽��
	 */
	virtual int IsHitControlPoint(int x, int y);

	/**
	 * �Ƿ�λ�ڱ�ע����
	 */
	virtual bool IsHitAnnotation(int x, int y);

	/**
	 * ѡ����Ƶ�
	 */
	virtual bool OnMouseLeftButtonDown(int x, int y);

	/**
	 * �������Ƶ�
	 */
	virtual bool OnMouseMoving(int x, int y);

	/**
	 * Ӧ�ÿ��Ƶ�
	 */
	virtual bool OnMouseLeftButtonUp(int x, int y);

	virtual bool OnMouseRightButtonDown(int x, int y);

	/**
	 * ��ӱ�ע�ؼ���scene
	 */
	virtual void AddToScene(QGraphicsScene *pScene);

	/**
	 * ��scene���Ƴ���ע
	 */
	virtual void RemoveFromScene(QGraphicsScene *pScene);

	/**
	 * ���µ�λ��
	 * @suggestResultTextScenePos ����ı�ע����ı�λ��(����scene����)
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
	 * ѡ����Ƶ�
	 */
	virtual bool OnMouseLeftButtonDownInternal(int x, int y) = 0;

	/**
	 * �������Ƶ�
	 */
	virtual bool OnMouseMovingInternal(int x, int y) = 0;

	/**
	 * Ӧ�ÿ��Ƶ�
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
	 * ͼ�������Ϣ
	 */
	AUTO_PROPERTY_REF(ImagePlanModule, std::weak_ptr<CImagePlanModule>);

	/**
	 * ������DisplayImage
	 */
	AUTO_PROPERTY_REF(ReferenceImage, std::weak_ptr<CDisplayImage>);

	/**
	 * ����ͼ������ı�ע��
	 */
	AUTO_PROPERTY_REF(OrientPoints, OrientPointArrayType);

	/**
	 * ����ͼ������ı�ע��
	 */
	AUTO_PROPERTY_REF(SelectedControlPointIndex, int);

	/**
	 * ������
	 */
	AUTO_PROPERTY_REF(Result, STRING_TYPE);

	AUTO_PROPERTY_REF_NO_SET(AnnotationResultTextItem, QGraphicsTextItem);

	/**
	 * Mouse Move�����Ƿ�Ϊ�ƶ���ע
	 */
	AUTO_PROPERTY_REF(IsTranslateAnnotation, bool);

	AUTO_PROPERTY_REF(LastMousePosition, sctPoint2D);

	AUTO_PROPERTY_REF(ResultPositionInScene, QPointF);

	AUTO_PROPERTY_REF_NO_SET(ControlPointRectItem, ControlPointRectsItemType);
};

#endif

