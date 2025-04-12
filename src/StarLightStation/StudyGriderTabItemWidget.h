#pragma once

#include <QWidget>
#include "ui_StudyGriderTabItemWidget.h"

#include <Foundations/Common/ToDefine.h>

#include <Viewmodels/ImageFrameViewModel/StudyTabItemViewModel.h>

#include <vector>
#include "utilities/Messenger/Messenger.h"

class StudyGriderTabItemWidget : public QWidget,
	public std::enable_shared_from_this<StudyGriderTabItemWidget>,
	public CMessengerOwner
{
	Q_OBJECT

public:
	StudyGriderTabItemWidget(QWidget *parent = Q_NULLPTR);
	~StudyGriderTabItemWidget();
	
	/**
	 * °ó¶¨¼ì²éÓ°Ïñ
	 */
	void SetStudyTabItemViewModel(std::shared_ptr<CStudyTabItemViewModel> viewModel);

	bool IsMeDisplayStudyViewModel(std::shared_ptr<CStudyTabItemViewModel> &viewModel);

	void OnStudyViewModelChanged();

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	void InitializeMessengers();

	void InitializeToolButtons();

	void UpdateToolButtonState(CIconButton *button);

private slots:
	void on_selectImageButton_clicked();
	void on_resetImageButton_clicked();
	void on_windowLevelButton_clicked();
	void on_resiezeImageButton_clicked();
	void on_moveImageButton_clicked();
	void on_lineAnnotationButton_clicked();
	void on_curveAnnotationButton_clicked();
	void on_pologonAnnotationButton_clicked();
	void on_editAnnotationButton_clicked();
	void on_angleAnnotationButton_clicked();
	void on_rectAnnotationButton_clicked();
	void on_deleteAnnotationButton_clicked();
	void on_mprButton_clicked();
	void on_vrButton_clicked();
	void on_paronamaButton_clicked();

private:
	Ui::StudyGriderTabItemWidget ui;

	AUTO_PROPERTY_REF_NO_SET(StudyTabItemViewModel, std::shared_ptr<CStudyTabItemViewModel>);

	AUTO_PROPERTY_REF(TabItemName, QString);

	AUTO_PROPERTY_REF_NO_SET(ToolButtons, std::vector<CIconButton*>);
};
