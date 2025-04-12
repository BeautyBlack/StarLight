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

#ifndef TEXT_ANNOTATION_BA45A63329594B02A3D4CD090171D2D2
#define TEXT_ANNOTATION_BA45A63329594B02A3D4CD090171D2D2

#pragma once

#include <viewmodels/ImageFrameViewModel/Annotation/AnnotationBase.h>
#include "utilities/Messenger/Messenger.h"

class IMAGEFRAMEVIEWMODEL_API CTextAnnotation : public CAnnotationBase, public CMessengerOwner
{
public:
	CTextAnnotation();
	virtual ~CTextAnnotation() = default;

	/**
	 * 是否位于标注附近
	 */
	bool IsHitAnnotation(int x, int y) override;

	int IsHitControlPoint(int x, int y) override;

	void SetInputText(STRING_TYPE text,
		const QPointF &pos,
		const QPointF &posInScene);
	
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

//private:
//	AUTO_PROPERTY_REF(TextBoundLeft, int);
//	AUTO_PROPERTY_REF(TextBoundTop, int);
//	AUTO_PROPERTY_REF(TextBoundRight, int);
//	AUTO_PROPERTY_REF(TextBoundBottom, int);
};

#endif

