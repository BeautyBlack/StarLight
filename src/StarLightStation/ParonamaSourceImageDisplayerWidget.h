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

#pragma once

#include "ImageDisplayerWidget.h"

#define ParonamaSourceImageDisplayerWidgetBase ImageDisplayerWidget

class CParonamaSourceImageDisplayerWidget : public ParonamaSourceImageDisplayerWidgetBase
{
	Q_OBJECT

public:
	explicit CParonamaSourceImageDisplayerWidget(QWidget *parent);
	explicit CParonamaSourceImageDisplayerWidget(QWidget *parent, MESSAGE_GROUP_ID_TYPE messengerOwnerId);
	~CParonamaSourceImageDisplayerWidget() = default;

	void Reset() override;

protected:
	void mouseMoveEvent(QMouseEvent *ev) override;
	void mousePressEvent(QMouseEvent *ev) override;
	void mouseReleaseEvent(QMouseEvent *ev) override;
	//void resizeEvent(QResizeEvent *event) override;

	void wheelEvent(QWheelEvent *event) override;

	void UpdateFromViewModel() override;
};

