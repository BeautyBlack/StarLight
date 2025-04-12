#include "pch.h"
#include "ImageFrameWidget.h"
#include "utilities/Messenger/Messenger.h"
#include "IconButton.h"
#include "ViewModels/ImageFrameViewModel/StudyTabItemViewModel.h"

#include <QTabBar>
#include "ViewModels/ImageFrameViewModel/ImageFrameViewModel.h"

ImageFrameWidget::ImageFrameWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.studiesTabWidget->clear();

	InitializeMessengers();

	connect(ui.studiesTabWidget,
		SIGNAL(tabCloseRequested(int)),
		this,
		SLOT(CloseTabItem(int)));
}

ImageFrameWidget::~ImageFrameWidget()
{
}

void ImageFrameWidget::SwitchToStudyTab(std::shared_ptr<CStudyTabItemViewModel> studyTab)
{
	auto nIndex = 0;
	
	for(auto &t : GetStudyTabItems())
	{
		if(t->IsMeDisplayStudyViewModel(studyTab))
		{
			ui.studiesTabWidget->setCurrentIndex(nIndex);

			t->OnStudyViewModelChanged();
			
			return;
		}

		++nIndex;
	}

	auto newTabItem = std::make_shared<StudyGriderTabItemWidget>();

	newTabItem->SetStudyTabItemViewModel(studyTab);

	GetStudyTabItems().push_back(newTabItem);

	ui.studiesTabWidget->addTab(newTabItem.get(), newTabItem->GetTabItemName());

	ui.studiesTabWidget->setCurrentIndex(GetStudyTabItems().size() - 1);
}

//
void ImageFrameWidget::InitializeMessengers()
{
	CMessenger::GetInstance().Regist(EMessengerKey::OpenStudyTab,
		this, [&](void* pParam) -> void*
	{
		auto studyTab = static_cast<std::shared_ptr<CStudyTabItemViewModel>*>(pParam);

		SwitchToStudyTab(*studyTab);

		return NULL_SYMBAL;
	});
}

void ImageFrameWidget::CloseTabItem(int nTabIndex)
{
	if (nTabIndex < 0 || nTabIndex >= (int)GetStudyTabItems().size())
		return;

	ui.studiesTabWidget->removeTab(nTabIndex);

	GetStudyTabItems().erase(GetStudyTabItems().begin() + nTabIndex);

	if(GetStudyTabItems().empty())
		CMessenger::GetInstance().SendMessage(EMessengerKey::OpenPatientListTab, NULL_SYMBAL);
}
