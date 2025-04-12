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

#ifndef THEME_MESSAGE_BOX_DFA0A8DCC7C44B29BA4B856F67566C43
#define THEME_MESSAGE_BOX_DFA0A8DCC7C44B29BA4B856F67566C43

#pragma once

#include <QWidget>
#include "ui_ThemeMessageBox.h"
#include <qdialog.h>

class ThemeMessageBox : public QDialog
{
	Q_OBJECT

public:
	ThemeMessageBox(QWidget *parent = Q_NULLPTR);
	~ThemeMessageBox();

	void SetTitleText(const char* strText, int nTextLen = -1);

	void SetMessageText(const char* strText, int nTextLen = -1);

	void Display();

private:
	Ui::ThemeMessageBox ui;
};

#endif
