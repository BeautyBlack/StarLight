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

#ifndef CLOSE_BUTTON_92FF317D32E94077A37A7572DC930EDA
#define CLOSE_BUTTON_92FF317D32E94077A37A7572DC930EDA

#pragma once

#include <QWidget>

class CCloseButton : public QWidget
{
	Q_OBJECT

public:
	explicit CCloseButton(QWidget *parent);
	~CCloseButton() = default;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QBrush m_backgroundBrush;
};

#endif

