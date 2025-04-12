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

#ifndef THEME_DEFINE_5E52ADEFA6544A17B431D75A6F343B88
#define THEME_DEFINE_5E52ADEFA6544A17B431D75A6F343B88

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <QWidget>
#include <QColor>
#include <QFont>
#include <QPen>

#define CLOSE_BUTTON_BACKGROUND Qt::white
#define CLOSE_BUTTON_FOREGROUND QColor(20,20,20)

class CThemeDefine
{
public:
	CThemeDefine();
	
private:
	AUTO_STATIC_PROPERTY_REF_NO_SET(Instance, CThemeDefine);
	
	AUTO_PROPERTY_REF(ImageInfoFont, QFont);
	AUTO_PROPERTY_REF(AnnotationFont, QFont);

	AUTO_PROPERTY_REF(ImageInfoForground, QColor);
	AUTO_PROPERTY_REF(AnnotationForground, QColor);
	
	AUTO_PROPERTY_REF(ImageInfoPen, QPen);

	AUTO_PROPERTY_REF(SelectedForground, QColor);
	AUTO_PROPERTY_REF(ImageBackground, QColor);

	AUTO_PROPERTY_REF(ActivatedForground, QColor);
};

#endif
