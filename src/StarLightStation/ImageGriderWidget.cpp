#include "pch.h"
#include "ImageGriderWidget.h"

#include "utilities/Configuration/Configuration.h"
#include <QLabel>
#include "utilities/Messenger/Messenger.h"
#include "Foundations/Interfaces/ImageMouseAction.h"
#include "utilities/Messenger/EventArgs/UI/ImageFrame/SelectedImageChangedEventArgs.h"

ImageGriderWidget::ImageGriderWidget(QWidget* parent)
	: QWidget(parent)
{
	setLayout(&GetLayoutGrid());

	GetLayoutGrid().setSpacing(1);
	GetLayoutGrid().setMargin(1);
	GetLayoutGrid().setContentsMargins(1, 1, 1, 1);

	GetLayoutGrid().setRowStretch(0, 1);//设置行列比例系数
	GetLayoutGrid().setRowStretch(1, 1);
	GetLayoutGrid().setColumnStretch(0, 1);
	GetLayoutGrid().setColumnStretch(1, 1);

	//GetLayoutGrid().setAlignment(Qt::AlignCenter);

	SetRows(2);
	SetColumns(2);

	InitializeMessage();

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	static bool firstTime = true;

	if(firstTime)
	{
		firstTime = false;

		QFont font;
		font.setPointSize(12);
		CAnnotationBase::SetDefaultResultTextFont(font);
	}
}

ImageGriderWidget::~ImageGriderWidget()
{
}

void ImageGriderWidget::ResizeLayout(int nRows, int nColumns)
{
	SetRows(nRows);
	SetColumns(nColumns);

	CreateOrUpdateImageDiplayerWidgets(nRows, nColumns);

	Relayout();
}

void ImageGriderWidget::SetStudyTabItemViewModel(std::shared_ptr<CStudyTabItemViewModel> viewModel)
{
	m_StudyTabItemViewModel = viewModel;

	ResizeLayout(GetStudyTabItemViewModel()->GetLayoutRows(),
		GetStudyTabItemViewModel()->GetLayoutColumn());
}

void ImageGriderWidget::OnMouseActionChanged(ImageMouseAction toolType)
{
	if (toolType == ImageMouseAction::ResetImage)
	{
		for (auto &it : GetImageDiplayers())
		{
			if (it->GetIsSelected())
				it->Reset();
		}

		toolType = ImageMouseAction::SelectImage;
	}
	else if(toolType == ImageMouseAction::MPR)
	{
		for (auto &it : GetImageDiplayers())
		{
			if (it->GetIsSelected() && it->GetImageViewModel())
			{
				GetStudyTabItemViewModel()->SendToMpr(it->GetImageViewModel());

				break;
			}
		}

		toolType = ImageMouseAction::SelectImage;
	}
	else if (toolType == ImageMouseAction::VR)
	{
		for (auto &it : GetImageDiplayers())
		{
			if (it->GetIsSelected() && it->GetImageViewModel())
			{
				GetStudyTabItemViewModel()->SendToVR(it->GetImageViewModel());

				break;
			}
		}

		toolType = ImageMouseAction::SelectImage;
	}
	else if (toolType == ImageMouseAction::Paronama)
	{
		for (auto &it : GetImageDiplayers())
		{
			if (it->GetIsSelected() && it->GetImageViewModel())
			{
				GetStudyTabItemViewModel()->SendToParonama(it->GetImageViewModel());

				break;
			}
		}

		toolType = ImageMouseAction::SelectImage;
	}
	else if(toolType == ImageMouseAction::AnnotationDelete)
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

void ImageGriderWidget::resizeEvent(QResizeEvent* event)
{
	Relayout();
}

void ImageGriderWidget::InitializeMessage()
{
	CMessenger::GetInstance().Regist(
		EMessengerKey::SelectImageChanged,
		this,
		[&](void* pParam) -> void*
	{
		auto pEventArgs = (CSelectedImageChangedEventArgs*)pParam;

		for(auto &it : GetImageDiplayers())
		{
			if(it.get() == pEventArgs->GetActivadeTarget())
			{
				it->SetIsSelected(true);
				it->SetIsActive(true);
			}
			else
			{
				it->SetIsActive(false);

				if (it->GetIsSelected())
					it->SetIsSelected(pEventArgs->GetIsMultiSelectMode());
			}

			it->update();
		}

		return NULL_SYMBAL;
	});
}

void ImageGriderWidget::CreateOrUpdateImageDiplayerWidgets(int nRows, int nColumns)
{
	auto widgetCount = max(
		(int)GetStudyTabItemViewModel()->GetDisplayImages().size(),
		nRows * nColumns);

	if (widgetCount & 0x01 == 0x01)
		++widgetCount;
	
	if(widgetCount > static_cast<int>(GetImageDiplayers().size()))
	{
		auto itBegin = GetImageDiplayers().begin();
		const auto itEnd = GetImageDiplayers().end();

		while (itBegin != itEnd)
		{
			GetLayoutGrid().removeWidget(itBegin->get());

			++itBegin;
		}

		GetImageDiplayers().clear();

		for (auto i = 0; i < widgetCount; ++i)
		{
			GetImageDiplayers().push_back(
				std::make_shared<ImageDisplayerWidget>(NULL_SYMBAL, GetGroupId()));
		}
	}

	const int nAllRows = max(nRows, GetImageDiplayers().size() / GetColumns() + 1);

	for (auto i = 0; i < nAllRows; ++i)
		GetLayoutGrid().setRowStretch(i, 1);

	for (auto i = 0; i < GetColumns(); ++i)
		GetLayoutGrid().setColumnStretch(i, 1);

	auto itBegin = GetImageDiplayers().begin();
	auto itEnd = GetImageDiplayers().end();

	if(GetStudyTabItemViewModel() == NULL_SYMBAL)
		return;
	
	const auto imageSize = static_cast<int>(
		GetStudyTabItemViewModel()->GetDisplayImages().size());

	for(auto i = 0; i < widgetCount; i++)
	{
		const auto row = i / nColumns;
		const auto column = i % nColumns;

		auto widget = GetImageDiplayers()[i].get();

		if (i < imageSize)
			widget->SetImageViewModel(
				GetStudyTabItemViewModel()->GetDisplayImages()[i]);

		GetLayoutGrid().addWidget(widget, row, column);

	}
}

void ImageGriderWidget::Relayout()
{
	const auto nSize = static_cast<int>(GetImageDiplayers().size());

	int nRows = nSize / GetColumns();

	if (nSize & 0x01 == 0x01)
		++nRows;

	for (auto i = 0; i < nRows; ++i)
		GetLayoutGrid().setRowStretch(i, 1);

	for (auto i = 0; i < GetColumns(); ++i)
		GetLayoutGrid().setColumnStretch(i, 1);

	auto itBegin = GetImageDiplayers().begin();
	const auto itEnd = GetImageDiplayers().end();

	auto clientSize = dynamic_cast<QWidget*>(parent())->rect();

	const auto nHeight = clientSize.height() / GetRows();
	auto nWidth = clientSize.width() / GetColumns();

	nWidth = nRows > 2 ? nWidth - 5 : nWidth;
	
	while (itBegin != itEnd)
	{
		(*itBegin)->setMinimumSize(max(0,nWidth-2), max(0,nHeight-1));
		(*itBegin)->setMaximumSize(max(0, nWidth-2), max(0, nHeight-1));

		++itBegin;
	}
}
