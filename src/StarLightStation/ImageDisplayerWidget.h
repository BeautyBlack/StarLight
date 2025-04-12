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

#ifndef IMAGE_DISPLAYER_WIDGET_AFF166F4A39345579201B82F596E907E
#define IMAGE_DISPLAYER_WIDGET_AFF166F4A39345579201B82F596E907E

#pragma once
#include "pch.h"

#include <ViewModels/ImageFrameViewModel/DisplayImage.h>

#include <Foundations/Interfaces/ImageMouseAction.h>
#include <Foundations/Interfaces/ImageToolType.h>
#include "utilities/Messenger/Messenger.h"
#include <Foundations/Common/ToDefine.h>

#include <QWidget>
#include <QImage>
#include <QStaticText>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsView>


#define ImageDisplayerWidgetBase QGraphicsView

/**
 * 单个图像显示窗口控件
 */
class ImageDisplayerWidget : public ImageDisplayerWidgetBase
	, public CMessengerOwner
{
	Q_OBJECT

public:
	explicit ImageDisplayerWidget(QWidget *parent, MESSAGE_GROUP_ID_TYPE messengerOwnerId);
	~ImageDisplayerWidget() = default;

	/**
	 * 设置Image ViewModel
	 */
	void SetImageViewModel(std::shared_ptr<CDisplayImage> imageViewModel);

	virtual void Reset();

	//bool IsSelected();

	void SetIsSelected(bool bValue);
	void SetIsActive(bool bValue);

	void DeleteLastAnnotation();
	
protected:

	void mouseMoveEvent(QMouseEvent *ev) override;
	void mousePressEvent(QMouseEvent *ev) override;
	void mouseReleaseEvent(QMouseEvent *ev) override;
	void resizeEvent(QResizeEvent *event) override;

	void wheelEvent(QWheelEvent *event) override;

	virtual void UpdateFromViewModel();
	
protected:

	void RelayoutImageInfoText();

	QPointF ConvertToImageCoordinate(QMouseEvent *ev);
	
private:
	void CreateQImage();

	void UpdateQImageByWindowLevel();

	void InitializeImageInfo(QRect &clientRect);

	QString CreateTemplateEditControl(QPoint position);
	
private:
	AUTO_PROPERTY_REF_NO_SET(ImageViewModel, std::shared_ptr<CDisplayImage>);

	/**
	 * 显示的影像
	 */
	AUTO_PROPERTY_REF(QPixmap, QPixmap);

	/**
	 * 背景画刷
	 */
	AUTO_PROPERTY_REF(BackgroundBrush, QBrush);

	AUTO_PROPERTY_REF(LastClientRectWidth, int);

	AUTO_PROPERTY_REF(LastClientRectHeight, int);

	AUTO_PROPERTY_REF_NO_SET(GraphicsScene, QGraphicsScene);
	AUTO_PROPERTY_REF_NO_SET(GraphicsPixmapItem, QGraphicsPixmapItem);
	
	AUTO_PROPERTY_REF_NO_SET(TopLeftTextItem, QGraphicsTextItem);
	AUTO_PROPERTY_REF_NO_SET(TopRightTextItem, QGraphicsTextItem);
	AUTO_PROPERTY_REF_NO_SET(BottomLeftTextItem, QGraphicsTextItem);
	AUTO_PROPERTY_REF_NO_SET(BottomRightTextItem, QGraphicsTextItem);

	AUTO_PROPERTY(MouseAction, ImageMouseAction);
	
	AUTO_PROPERTY(IsMousePress, bool);

	AUTO_PROPERTY_REF(LastMousePosition, QPointF);

	AUTO_PROPERTY_REF_NO_SET(SelectedBound, QGraphicsRectItem);
	AUTO_PROPERTY_REF_NO_SET(ActivatedBound, QGraphicsRectItem);

	AUTO_PROPERTY_REF_NO_SET(IsSelected, bool);

	AUTO_PROPERTY_REF_NO_SET(IsActive, bool);

	///////////////  Annotations  //////////////////////

	AUTO_PROPERTY_REF(EditingAnnotation, std::shared_ptr<CAnnotationBase>);

	void ClearAnnotations();
	void UpdateAnnotations();
	
	// Line Annotation
	void BeginLineAnnotation();
	void EndLineAnnnotation();
	void EditingLineAnnotation();

	QPointF ComputeAnnotationResultTextPosition(CAnnotationBase *pAnn);
	///////////////  Annotations  //////////////////////
};


#endif
