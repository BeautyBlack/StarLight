#include "pch.h"
#include "VRFrameWidget.h"

#include <ViewModels/VRFrameViewModel/VRFrameViewModel.h>


#include "utilities/Configuration/Configuration.h"
#include "VRColorSettingWidget.h"

VRFrameWidget::VRFrameWidget(QWidget *parent)
	: QWidget(parent), m_ColorSettingWidget(this)
{
	ui.setupUi(this);

	CVRFrameViewModel::GetInstance().SetGroupId(GetGroupId());

	GetColorSettingWidget().SetGroupId(GetGroupId());

	CVRFrameViewModel::GetInstance().Initialize();

	InitializeMessenger();
}

VRFrameWidget::~VRFrameWidget()
{
}

void VRFrameWidget::on_colorSettingButton_clicked()
{
	GetColorSettingWidget().SetSetting(
		CConfiguration::GetInstance().GetVRInfo());

	GetColorSettingWidget().LoadSetting();
	
	GetColorSettingWidget().show();
}

void VRFrameWidget::InitializeMessenger()
{
	CMessenger::GetInstance().Regist(
		EMessengerKey::GetVRRenderWindowParentHandle,
		this,
		[&](void *pParam)
	{
		return (void *)ui.vrRenderWidget->winId();
	});
	
	CMessenger::GetInstance().Regist(
		EMessengerKey::SaveColorSetting,
		this,
		[&](void *pParam)
	{
		CVRColorSetting *pSetting = (CVRColorSetting*)pParam;

		if (pSetting == NULL_SYMBAL)
			return NULL_SYMBAL;

		CConfiguration::GetInstance().GetVRInfo() = *pSetting;
		CConfiguration::GetInstance().Save();
	});
}

