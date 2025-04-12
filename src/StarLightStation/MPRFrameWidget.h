#pragma once

#include <QWidget>

#include <Foundations/Common/ToDefine.h>

#include <Utilities/Messenger/Messenger.h>

#include "ui_MPRFrameWidget.h"

#include <vector>

class MPRFrameWidget : public QWidget, public CMessengerOwner
{
	Q_OBJECT

public:
	MPRFrameWidget(QWidget *parent = Q_NULLPTR);
	~MPRFrameWidget();

private:
	void InitializeMessenger();

	void InitializeToolButtons();

	void UpdateToolButtonState(CIconButton *button);

	void OnMouseActionChanged(ImageMouseAction toolType);
	
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
	Ui::MPRFrameWidget ui;
	
	AUTO_PROPERTY_REF_NO_SET(ToolButtons, std::vector<CIconButton*>);
	
	using ImageDisplayWidgetType = std::vector<ImageDisplayerWidget*>;

	AUTO_PROPERTY_REF(ImageDiplayers, ImageDisplayWidgetType);

};
