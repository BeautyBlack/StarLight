#include "ParonamaFrameWidget.h"


#include "iconbutton.h"
#include "ViewModels/MprFrameViewModel/MprFrameViewModel.h"
#include "ViewModels/ParonamaFrameViewModel/ParonamaFrameViewModel.h"

ParonamaFrameWidget::ParonamaFrameWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	CParonamaFrameViewModel::GetInstance().SetGroupId(GetGroupId());

	CParonamaFrameViewModel::GetInstance().Initialize();

	GetImageDiplayers().push_back(ui.sourceImageWidget);
	GetImageDiplayers().push_back(ui.sagittalImageWidget);
	GetImageDiplayers().push_back(ui.cprImageWidget);
	GetImageDiplayers().push_back(ui.vrImageWidget);

	ui.sourceImageWidget->SetGroupId(GetGroupId());
	ui.sagittalImageWidget->SetGroupId(GetGroupId());
	ui.cprImageWidget->SetGroupId(GetGroupId());
	ui.vrImageWidget->SetGroupId(GetGroupId());

	InitializeToolButtons();
	InitializeMessenger();
}

ParonamaFrameWidget::~ParonamaFrameWidget()
{
	CMessenger::GetInstance().Unregist(EMessengerKey::ParonamaUpdateDisplayImages, this);
}

void ParonamaFrameWidget::InitializeMessenger()
{
	CMessenger::GetInstance().Regist(
		EMessengerKey::ParonamaUpdateDisplayImages,
		this,
		[&](void* pParam)
	{
		ui.sourceImageWidget->SetImageViewModel(
			CParonamaFrameViewModel::GetInstance().GetTransverseImage());

		ui.sagittalImageWidget->SetImageViewModel(
			CParonamaFrameViewModel::GetInstance().GetSaggitalImage());

		ui.cprImageWidget->SetImageViewModel(
			CParonamaFrameViewModel::GetInstance().GetParonamaImage());

		ui.sourceImageWidget->Reset();
		ui.sagittalImageWidget->Reset();
		ui.cprImageWidget->Reset();

		return NULL_SYMBAL;
	});
}

void ParonamaFrameWidget::InitializeToolButtons()
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

void ParonamaFrameWidget::UpdateToolButtonState(CIconButton* button)
{
	for (auto &t : GetToolButtons())
		t->OnMouseActionChanged(
			button->GetImageMouseAction());

	OnMouseActionChanged(button->GetImageMouseAction());
}

void ParonamaFrameWidget::OnMouseActionChanged(ImageMouseAction toolType)
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

void ParonamaFrameWidget::resizeEvent(QResizeEvent* event)
{
	auto clientSize = ui.imageAreaWidget->rect();
	auto width = clientSize.width() - 10 * 2;
	auto height = clientSize.height() - 10 * 2;

	const auto nHeight = height / 2;
	auto nWidth = width / 2;

	ui.sourceImageWidget->setMinimumSize(max(0, nWidth - 2), max(0, nHeight - 2));
	ui.sourceImageWidget->setMaximumSize(max(0, nWidth - 2), max(0, nHeight - 2));

	ui.sagittalImageWidget->setMinimumSize(max(0, nWidth - 2), max(0, nHeight - 2));
	ui.sagittalImageWidget->setMaximumSize(max(0, nWidth - 2), max(0, nHeight - 2));

	ui.cprImageWidget->setMinimumSize(max(0, nWidth - 2), max(0, nHeight - 2));
	ui.cprImageWidget->setMaximumSize(max(0, nWidth - 2), max(0, nHeight - 2));

	ui.vrImageWidget->setMinimumSize(max(0, nWidth - 2), max(0, nHeight - 2));
	ui.vrImageWidget->setMaximumSize(max(0, nWidth - 2), max(0, nHeight - 2));
}


void ParonamaFrameWidget::on_selectImageButton_clicked()
{
	UpdateToolButtonState(ui.selectImageButton);
}

void ParonamaFrameWidget::on_resetImageButton_clicked()
{
	UpdateToolButtonState(ui.resetImageButton);
}

void ParonamaFrameWidget::on_windowLevelButton_clicked()
{
	UpdateToolButtonState(ui.windowLevelButton);
}

void ParonamaFrameWidget::on_resiezeImageButton_clicked()
{
	UpdateToolButtonState(ui.resiezeImageButton);
}

void ParonamaFrameWidget::on_moveImageButton_clicked()
{
	UpdateToolButtonState(ui.moveImageButton);
}

void ParonamaFrameWidget::on_lineAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.lineAnnotationButton);
}

void ParonamaFrameWidget::on_curveAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.curveAnnotationButton);
}

void ParonamaFrameWidget::on_pologonAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.pologonAnnotationButton);
}

void ParonamaFrameWidget::on_angleAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.angleAnnotationButton);
}

void ParonamaFrameWidget::on_rectAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.rectAnnotationButton);
}

void ParonamaFrameWidget::on_deleteAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.deleteAnnotationButton);
}


