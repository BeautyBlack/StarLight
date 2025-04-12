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

#ifndef IMAGE_GRIDER_WIDGET_E1E456272910417EBA1921169C0FEE67
#define IMAGE_GRIDER_WIDGET_E1E456272910417EBA1921169C0FEE67

#pragma once

#include <QStackedLayout>
#include <QGridLayout>
#include <QLayout>

#include "ImageDisplayerWidget.h"

#include <Foundations/Common/ToDefine.h>

#include <Viewmodels/ImageFrameViewModel/StudyTabItemViewModel.h>

#include <vector>
#include "utilities/Messenger/Messenger.h"

/**
 * 图像显示控件容器, 用于布局1~N个图像显示框
 */
class ImageGriderWidget : public QWidget,
	public std::enable_shared_from_this<ImageGriderWidget>,
	public CMessengerOwner
{
	Q_OBJECT

public:
	explicit ImageGriderWidget(QWidget *parent = NULL_SYMBAL);
	~ImageGriderWidget();

	std::shared_ptr<ImageGriderWidget> getptr() {
		return shared_from_this();
	}

	/**
	 * 图像布局【如 2X2】
	 */
	void ResizeLayout(int nRows, int nColumns);

	/**
	 * 切换显示的检查影像
	 */
	void SetStudyTabItemViewModel(std::shared_ptr<CStudyTabItemViewModel> viewModel);

	void OnMouseActionChanged(ImageMouseAction toolType);
	
protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	/**
	 * 初始化消息响应
	 */
	void InitializeMessage();

	void CreateOrUpdateImageDiplayerWidgets(int nRows, int nColumns);

	void Relayout();

private:
	using ImageDisplayWidgetType = std::vector<std::shared_ptr<ImageDisplayerWidget>>;
	
	AUTO_PROPERTY_REF(ImageDiplayers, ImageDisplayWidgetType);

	AUTO_PROPERTY_REF_NO_SET(LayoutGrid, QGridLayout);

	AUTO_PROPERTY_REF_NO_SET(StudyTabItemViewModel, std::shared_ptr<CStudyTabItemViewModel>);

	AUTO_PROPERTY(Rows, int);

	AUTO_PROPERTY(Columns, int);
};

#endif

