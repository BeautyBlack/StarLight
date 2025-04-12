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

#define ParonamaSagittalImageDisplayerWidgetBase ImageDisplayerWidget

class CParonamaSagittalImageDisplayerWidget : public ParonamaSagittalImageDisplayerWidgetBase
{
	Q_OBJECT

public:
	explicit CParonamaSagittalImageDisplayerWidget(QWidget *parent);
	explicit CParonamaSagittalImageDisplayerWidget(QWidget *parent, MESSAGE_GROUP_ID_TYPE messengerOwnerId);
	~CParonamaSagittalImageDisplayerWidget() = default;

	void Reset() override;

protected:
	//void resizeEvent(QResizeEvent *event) override;

	void wheelEvent(QWheelEvent *event) override;

	void UpdateFromViewModel() override;

	void SwitchToNextSlice(bool isForward = true);
};

