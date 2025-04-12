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

#define ParonamaVRImageDisplayerWidgetBase ImageDisplayerWidget


class CParonamaVRImageDisplayerWidget : public ParonamaVRImageDisplayerWidgetBase
{
	Q_OBJECT

public:
	explicit CParonamaVRImageDisplayerWidget(QWidget *parent);
	explicit CParonamaVRImageDisplayerWidget(QWidget *parent, MESSAGE_GROUP_ID_TYPE messengerOwnerId);
	~CParonamaVRImageDisplayerWidget() = default;

	void Reset() override;
};

