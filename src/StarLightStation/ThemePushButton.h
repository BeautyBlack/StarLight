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

#ifndef THEME_PUSH_BUTTON_775E8CA6CA7D4DE0A85522E92311456C
#define THEME_PUSH_BUTTON_775E8CA6CA7D4DE0A85522E92311456C

#pragma once

#include <QPushButton>

#include <Foundations/Common/ToDefine.h>

class ThemePushButton : public QPushButton
{
	Q_OBJECT

public:
	ThemePushButton(QWidget *parent = Q_NULLPTR);
	~ThemePushButton();

	void SetIsEnable(bool isEnable = true);
	
private:
	AUTO_PROPERTY_REF_NO_SET(IsEnable, bool);
};

#endif
