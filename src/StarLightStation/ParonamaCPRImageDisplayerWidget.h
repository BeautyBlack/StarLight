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

#define ParonamaCPRImageDisplayerWidgetBase ImageDisplayerWidget

class CParonamaCPRImageDisplayerWidget : public ParonamaCPRImageDisplayerWidgetBase
{
	Q_OBJECT

public:
	explicit CParonamaCPRImageDisplayerWidget(QWidget *parent);
	explicit CParonamaCPRImageDisplayerWidget(QWidget *parent, MESSAGE_GROUP_ID_TYPE messengerOwnerId);
	~CParonamaCPRImageDisplayerWidget() = default;

	void Reset() override;

protected:
	//void resizeEvent(QResizeEvent *event) override;

	//void wheelEvent(QWheelEvent *event) override;

	void UpdateFromViewModel() override;

	void SwitchToNextSlice(bool isForward = true);
};

