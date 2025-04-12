#pragma once

#include <QWidget>
#include "ui_ImageFrameWidget.h"
#include "StudyGriderTabItemWidget.h"
#include "utilities/Messenger/Messenger.h"
#include "ViewModels/ImageFrameViewModel/StudyTabItemViewModel.h"

#include <vector>


class ImageFrameWidget : public QWidget, public CMessengerOwner
{
	Q_OBJECT

public:
	ImageFrameWidget(QWidget *parent = Q_NULLPTR);
	~ImageFrameWidget();

	void SwitchToStudyTab(std::shared_ptr<CStudyTabItemViewModel> studyTab);

private:
	void InitializeMessengers();

private slots:
	void CloseTabItem(int nTabIndex);

private:
	Ui::ImageFrameWidget ui;

	using STUDY_TAB_ITEMS_TYPE = std::vector<std::shared_ptr<StudyGriderTabItemWidget>>;
	
	AUTO_PROPERTY_REF_NO_SET(StudyTabItems, STUDY_TAB_ITEMS_TYPE);
};
