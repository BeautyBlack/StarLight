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

#ifndef THEME_DATETIME_DISPLAYER_8C583A42364545FDAE52905ADF21B39C
#define THEME_DATETIME_DISPLAYER_8C583A42364545FDAE52905ADF21B39C

#pragma once

#include <QLabel>
#include <QTimer>

#include <Foundations/Common/ToDefine.h>

class ThemeDatetimeDisplayer : public QLabel
{
	Q_OBJECT
	
public:
	explicit ThemeDatetimeDisplayer(QWidget *parent);
	~ThemeDatetimeDisplayer() = default;

private slots:
	void UpdateDatetime();
	
private:
	AUTO_PROPERTY_REF_NO_SET(Timer, QTimer);
};

#endif

