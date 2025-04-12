#include "pch.h"
#include "MPRFrameWidget.h"

#include "ViewModels/MprFrameViewModel/MprFrameViewModel.h"

MPRFrameWidget::MPRFrameWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	CMprFrameViewModel::GetInstance().SetGroupId(GetGroupId());
	
	CMprFrameViewModel::GetInstance().Initialize();

	GetImageDiplayers().push_back(ui.saggitalDisplayer);
	GetImageDiplayers().push_back(ui.coronalDisplayer);
	GetImageDiplayers().push_back(ui.transverseDisplayer);

	ui.saggitalDisplayer->SetGroupId(GetGroupId());
	ui.coronalDisplayer->SetGroupId(GetGroupId());
	ui.transverseDisplayer->SetGroupId(GetGroupId());
	
	InitializeToolButtons();
	InitializeMessenger();
}

MPRFrameWidget::~MPRFrameWidget()
{
	CMessenger::GetInstance().Unregist(EMessengerKey::MprUpdateDisplayImages, this);
}

void MPRFrameWidget::InitializeMessenger()
{
	CMessenger::GetInstance().Regist(
	EMessengerKey::MprUpdateDisplayImages,
		this,
		[&](void* pParam)
	{
		ui.saggitalDisplayer->SetImageViewModel(
			CMprFrameViewModel::GetInstance().GetSaggitalDisplayImage());

		ui.transverseDisplayer->SetImageViewModel(
			CMprFrameViewModel::GetInstance().GetTransverseDisplayImage());

		ui.coronalDisplayer->SetImageViewModel(
			CMprFrameViewModel::GetInstance().GetCoronalDisplayImage());

		ui.saggitalDisplayer->Reset();
		ui.transverseDisplayer->Reset();
		ui.coronalDisplayer->Reset();
		
		return NULL_SYMBAL;
	});
}

void MPRFrameWidget::InitializeToolButtons()
{
	ui.selectImageButton->SetImageMouseAction(ImageMouseAction::SelectImage);
	ui.resetImageButton->SetImageMouseAction(ImageMouseAction::ResetImage);
	ui.windowLevelButton->SetImageMouseAction(ImageMouseAction::WindowLevel);
	ui.resiezeImageButton->SetImageMouseAction(ImageMouseAction::Scale);
	ui.moveImageButton->SetImageMouseAction(ImageMouseAction::Translate);
	ui.lineAnnotationButton->SetImageMouseAction(ImageMouseAction::AnnotationLine);
	ui.curveAnnotationButton->SetImageMouseAction(ImageMouseAction::AnnotationCurve);
	ui.pologonAnnotationButton->SetImageMouseAction(ImageMouseAction::AnnotationPolygon);
	ui.angleAnnotationButton->SetImageMouseAction(ImageMouseAction::AnnotationAngle);
	ui.rectAnnotationButton->SetImageMouseAction(ImageMouseAction::AnnotationRect);
	ui.deleteAnnotationButton->SetImageMouseAction(ImageMouseAction::AnnotationDelete);
	
	GetToolButtons().push_back(ui.selectImageButton);
	GetToolButtons().push_back(ui.resetImageButton);
	GetToolButtons().push_back(ui.windowLevelButton);
	GetToolButtons().push_back(ui.resiezeImageButton);
	GetToolButtons().push_back(ui.moveImageButton);
	GetToolButtons().push_back(ui.lineAnnotationButton);
	GetToolButtons().push_back(ui.curveAnnotationButton);
	GetToolButtons().push_back(ui.pologonAnnotationButton);
	GetToolButtons().push_back(ui.angleAnnotationButton);
	GetToolButtons().push_back(ui.rectAnnotationButton);
	GetToolButtons().push_back(ui.deleteAnnotationButton);
}

void MPRFrameWidget::UpdateToolButtonState(CIconButton* button)
{
	for (auto &t : GetToolButtons())
		t->OnMouseActionChanged(
			button->GetImageMouseAction());

	OnMouseActionChanged(button->GetImageMouseAction());
}

void MPRFrameWidget::on_selectImageButton_clicked()
{
	UpdateToolButtonState(ui.selectImageButton);
}

void MPRFrameWidget::on_resetImageButton_clicked()
{
	UpdateToolButtonState(ui.resetImageButton);
}

void MPRFrameWidget::on_windowLevelButton_clicked()
{
	UpdateToolButtonState(ui.windowLevelButton);
}

void MPRFrameWidget::on_resiezeImageButton_clicked()
{
	UpdateToolButtonState(ui.resiezeImageButton);
}

void MPRFrameWidget::on_moveImageButton_clicked()
{
	UpdateToolButtonState(ui.moveImageButton);
}

void MPRFrameWidget::on_lineAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.lineAnnotationButton);
}

void MPRFrameWidget::on_curveAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.curveAnnotationButton);
}

void MPRFrameWidget::on_pologonAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.pologonAnnotationButton);
}

void MPRFrameWidget::on_angleAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.angleAnnotationButton);
}

void MPRFrameWidget::on_rectAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.rectAnnotationButton);
}

void MPRFrameWidget::on_deleteAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.deleteAnnotationButton);
}

void MPRFrameWidget::resizeEvent(QResizeEvent* event)
{
	auto clientSize = ui.widget->rect();
	auto width = clientSize.width() - 12*2;
	auto height = clientSize.height() - 12*2;
	
	const auto nHeight = height / 2;
	auto nWidth = width / 3;

	ui.transverseDisplayer->setMinimumSize(max(0, 2*nWidth - 2), max(0, height - 2));
	ui.transverseDisplayer->setMaximumSize(max(0, 2*nWidth - 2), max(0, height - 2));

	ui.saggitalDisplayer->setMinimumSize(max(0, nWidth - 2), max(0, nHeight - 2));
	ui.saggitalDisplayer->setMaximumSize(max(0, nWidth - 2), max(0, nHeight - 2));

	ui.coronalDisplayer->setMinimumSize(max(0, nWidth - 2), max(0, nHeight - 2));
	ui.coronalDisplayer->setMaximumSize(max(0, nWidth - 2), max(0, nHeight - 2));
}

void MPRFrameWidget::OnMouseActionChanged(ImageMouseAction toolType)
{
	if (toolType == ImageMouseAction::ResetImage)
	{
		/*for (auto &it : GetImageDiplayers())
		{
			if (it->GetIsSelected())
				it->Reset();
		}*/
		
		CMprFrameViewModel::GetInstance().Reset();

		toolType = ImageMouseAction::UnkownAction;
	}
	else if (toolType == ImageMouseAction::AnnotationDelete)
	{
		for (auto &it : GetImageDiplayers())
		{
			if (it->GetIsSelected())
				it->DeleteLastAnnotation();
		}

		return;
	}

	for (auto it : GetImageDiplayers())
		it->SetMouseAction(toolType);
}

