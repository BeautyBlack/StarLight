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

#ifndef MPR_IMAGE_DISPLAYER_WIDGET_853D82DF29604DABBF6729295809466C
#define MPR_IMAGE_DISPLAYER_WIDGET_853D82DF29604DABBF6729295809466C

#pragma once

#include "MprScoutLine.h"

#include <QGraphicsView>

#include "ImageDisplayerWidget.h"

#define MPRImageDisplayerWidgetBase ImageDisplayerWidget

class MprImageDisplayerWidget : public MPRImageDisplayerWidgetBase
{
	Q_OBJECT

public:
	explicit MprImageDisplayerWidget(QWidget *parent);
	explicit MprImageDisplayerWidget(QWidget *parent, MESSAGE_GROUP_ID_TYPE messengerOwnerId);
	~MprImageDisplayerWidget() = default;

	void Reset() override;
	
protected:

	void mouseMoveEvent(QMouseEvent *ev) override;
	void mousePressEvent(QMouseEvent *ev) override;
	void mouseReleaseEvent(QMouseEvent *ev) override;
	void resizeEvent(QResizeEvent *event) override;

	void wheelEvent(QWheelEvent *event) override;

	void UpdateScoutLines();

	void UpdateFromViewModel() override;

	void SwitchToNextSlice(bool isForward = true);
	
private:
	AUTO_PROPERTY_REF_NO_SET(ScoutLine, CMprScoutLine);
};

#endif
