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

#ifndef MAXIMUM_BUTTON_3EBA9BDC215647F3B23C5EB1689D6188
#define MAXIMUM_BUTTON_3EBA9BDC215647F3B23C5EB1689D6188

#pragma once

#include <QWidget>


class CMaximumButton : public QWidget
{
	Q_OBJECT

public:
	CMaximumButton(QWidget *parent);
	~CMaximumButton() = default;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QBrush m_backgroundBrush;
};

#endif

