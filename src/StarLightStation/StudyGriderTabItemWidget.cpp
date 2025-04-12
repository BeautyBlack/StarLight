#include "pch.h"
#include "StudyGriderTabItemWidget.h"
#include "utilities/Messenger/Messenger.h"
#include "IconButton.h"
#include "ViewModels/ImageFrameViewModel/StudyTabItemViewModel.h"
#include "ViewModels/ImageFrameViewModel/ImageFrameViewModel.h"

StudyGriderTabItemWidget::StudyGriderTabItemWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	SetTabItemName("*");

	InitializeToolButtons();

	InitializeMessengers();
}

StudyGriderTabItemWidget::~StudyGriderTabItemWidget()
{
	CImageFrameViewModel::GetInstance().ReleaseStudyTabItem(
		GetStudyTabItemViewModel());
}

void StudyGriderTabItemWidget::SetStudyTabItemViewModel(std::shared_ptr<CStudyTabItemViewModel> viewModel)
{
	if ( !viewModel || viewModel.get() == NULL_SYMBAL)
		return;
	
	m_StudyTabItemViewModel = viewModel;

	CString name;

	auto &study = viewModel->GetStudy();
	
	name.Format("%s %s %d%s",
		study->GetPatientName().c_str(),
		SexText(study->GetSex()),
		study->GetAge(),
		AgeUnitText(study->GetAgeUnit()));
	
	SetTabItemName(CSTRING_TO_QSTRING(name));

	ui.imageGrider->SetStudyTabItemViewModel(viewModel);
}

bool StudyGriderTabItemWidget::IsMeDisplayStudyViewModel(std::shared_ptr<CStudyTabItemViewModel>& viewModel)
{
	if ( !GetStudyTabItemViewModel())
		return false;

	return GetStudyTabItemViewModel()->GetStudy()->GetStudyInstanceUID().compare(
	viewModel->GetStudy()->GetStudyInstanceUID()) == 0;
}

void StudyGriderTabItemWidget::resizeEvent(QResizeEvent* event)
{
	if(static_cast<int>(ui.imageGrider->GetImageDiplayers().size())
		 > ui.imageGrider->GetColumns() * ui.imageGrider->GetRows())
	{
		auto clientSize = rect();

		const auto nRows = static_cast<int>(ui.imageGrider->GetImageDiplayers().size())
			/ ui.imageGrider->GetColumns();

		const auto nHeight = clientSize.height() / ui.imageGrider->GetRows();

		auto griderSize = ui.imageGrider->rect();

		ui.imageGrider->setMinimumSize(griderSize.width()-5, nHeight * nRows);
	}
}

void StudyGriderTabItemWidget::OnStudyViewModelChanged()
{
	// TODO -- 
}

void StudyGriderTabItemWidget::InitializeMessengers()
{
}

void StudyGriderTabItemWidget::InitializeToolButtons()
{
	ui.selectImageButton->SetImageMouseAction(ImageMouseAction::SelectImage);
	ui.resetImageButton->SetImageMouseAction(ImageMouseAction::ResetImage);
	ui.windowLevelButton->SetImageMouseAction(ImageMouseAction::WindowLevel);
	ui.resiezeImageButton->SetImageMouseAction(ImageMouseAction::Scale);
	ui.moveImageButton->SetImageMouseAction(ImageMouseAction::Translate);
	ui.lineAnnotationButton->SetImageMouseAction(ImageMouseAction::AnnotationLine);
	ui.curveAnnotationButton->SetImageMouseAction(ImageMouseAction::AnnotationCurve);
	ui.pologonAnnotationButton->SetImageMouseAction(ImageMouseAction::AnnotationPolygon);
	ui.editAnnotationButton->SetImageMouseAction(ImageMouseAction::AnnotationText);
	ui.angleAnnotationButton->SetImageMouseAction(ImageMouseAction::AnnotationAngle);
	ui.rectAnnotationButton->SetImageMouseAction(ImageMouseAction::AnnotationRect);
	ui.deleteAnnotationButton->SetImageMouseAction(ImageMouseAction::AnnotationDelete);
	ui.mprButton->SetImageMouseAction(ImageMouseAction::MPR);
	ui.vrButton->SetImageMouseAction(ImageMouseAction::VR);
	ui.paronamaButton->SetImageMouseAction(ImageMouseAction::Paronama);
	
	GetToolButtons().push_back(ui.selectImageButton);
	GetToolButtons().push_back(ui.resetImageButton);
	GetToolButtons().push_back(ui.windowLevelButton);
	GetToolButtons().push_back(ui.resiezeImageButton);
	GetToolButtons().push_back(ui.moveImageButton);
	GetToolButtons().push_back(ui.lineAnnotationButton);
	GetToolButtons().push_back(ui.curveAnnotationButton);
	GetToolButtons().push_back(ui.pologonAnnotationButton);
	GetToolButtons().push_back(ui.editAnnotationButton);
	GetToolButtons().push_back(ui.angleAnnotationButton);
	GetToolButtons().push_back(ui.rectAnnotationButton);
	GetToolButtons().push_back(ui.deleteAnnotationButton);
	GetToolButtons().push_back(ui.mprButton);
	GetToolButtons().push_back(ui.vrButton);
	GetToolButtons().push_back(ui.paronamaButton);
}

void StudyGriderTabItemWidget::UpdateToolButtonState(CIconButton* button)
{
	for (auto &t : GetToolButtons())
		t->OnMouseActionChanged(
			button->GetImageMouseAction());

	ui.imageGrider->OnMouseActionChanged(
		button->GetImageMouseAction());
}

void StudyGriderTabItemWidget::on_selectImageButton_clicked()
{
	UpdateToolButtonState(ui.selectImageButton);
}

void StudyGriderTabItemWidget::on_resetImageButton_clicked()
{
	UpdateToolButtonState(ui.resetImageButton);
}

void StudyGriderTabItemWidget::on_windowLevelButton_clicked()
{
	UpdateToolButtonState(ui.windowLevelButton);
}

void StudyGriderTabItemWidget::on_resiezeImageButton_clicked()
{
	UpdateToolButtonState(ui.resiezeImageButton);
}

void StudyGriderTabItemWidget::on_moveImageButton_clicked()
{
	UpdateToolButtonState(ui.moveImageButton);
}

void StudyGriderTabItemWidget::on_lineAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.lineAnnotationButton);
}

void StudyGriderTabItemWidget::on_curveAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.curveAnnotationButton);
}

void StudyGriderTabItemWidget::on_pologonAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.pologonAnnotationButton);
}

void StudyGriderTabItemWidget::on_editAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.editAnnotationButton);
}

void StudyGriderTabItemWidget::on_angleAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.angleAnnotationButton);
}

void StudyGriderTabItemWidget::on_rectAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.rectAnnotationButton);
}

void StudyGriderTabItemWidget::on_deleteAnnotationButton_clicked()
{
	UpdateToolButtonState(ui.deleteAnnotationButton);
}

void StudyGriderTabItemWidget::on_mprButton_clicked()
{
	UpdateToolButtonState(ui.mprButton);
}

void StudyGriderTabItemWidget::on_vrButton_clicked()
{
	UpdateToolButtonState(ui.vrButton);
}

void StudyGriderTabItemWidget::on_paronamaButton_clicked()
{
	UpdateToolButtonState(ui.paronamaButton);
}

