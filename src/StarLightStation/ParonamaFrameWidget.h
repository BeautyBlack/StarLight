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

#ifndef PARONAMA_FRAME_WIDGET_BBFCEA6EE8BC40C8BB97FD938156FB39
#define PARONAMA_FRAME_WIDGET_BBFCEA6EE8BC40C8BB97FD938156FB39


#pragma once

#include <QWidget>
#include "ui_ParonamaFrameWidget.h"

class CIconButton;

class ParonamaFrameWidget : public QWidget, public CMessengerOwner
{
	Q_OBJECT

public:
	ParonamaFrameWidget(QWidget *parent = Q_NULLPTR);
	~ParonamaFrameWidget();


	void UpdateToolButtonState(CIconButton *button);

	void OnMouseActionChanged(ImageMouseAction toolType);
	
private:
	void InitializeMessenger();

	void InitializeToolButtons();

private slots:
	void on_selectImageButton_clicked();
	void on_resetImageButton_clicked();
	void on_windowLevelButton_clicked();
	void on_resiezeImageButton_clicked();
	void on_moveImageButton_clicked();
	void on_lineAnnotationButton_clicked();
	void on_curveAnnotationButton_clicked();
	void on_pologonAnnotationButton_clicked();
	void on_angleAnnotationButton_clicked();
	void on_rectAnnotationButton_clicked();
	void on_deleteAnnotationButton_clicked();

	void resizeEvent(QResizeEvent *event) override;
	
private:
	Ui::ParonamaFrameWidget ui;

	AUTO_PROPERTY_REF_NO_SET(ToolButtons, std::vector<CIconButton*>);

	using ImageDisplayWidgetType = std::vector<ImageDisplayerWidget*>;

	AUTO_PROPERTY_REF(ImageDiplayers, ImageDisplayWidgetType);
};

#endif
