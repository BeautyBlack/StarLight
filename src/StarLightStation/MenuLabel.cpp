#include "pch.h"
#include "MenuLabel.h"

#include <QEvent>

#include "Foundations/Common/StringUtility.h"

CMenuLabel::CMenuLabel(QWidget *parent)
	: QLabel(parent)
{
	SetFrameMenu(FrameMenu::Unknown);

	SetSelected(false);

	CMessenger::GetInstance().Regist(
		EMessengerKey::MainFrameMenuChanged,
		this,
		[&](void* param)
	{
		auto pCheck = (CMenuLabel*)param;

		SetSelected(pCheck->GetGroupId() == GetGroupId());

		updateLabelColor();
		
		return NULL_SYMBAL;
	});
}

CMenuLabel::~CMenuLabel()
{
}

void CMenuLabel::enterEvent(QEvent* event)
{
	updateLabelColor(true);
}

void CMenuLabel::leaveEvent(QEvent* event)
{
	updateLabelColor();
}

void CMenuLabel::mousePressEvent(QMouseEvent* event)
{
	if (GetSelected())
		return;

	/*SetSelected(true);
	
	updateLabelColor();*/

	CMessenger::GetInstance().SendMessage(EMessengerKey::MainFrameMenuChanged, this);
}

void CMenuLabel::updateLabelColor(bool isEnterLabel)
{
	CString strStyleSheet;

	strStyleSheet.Format("color:%s;background-color:%s;",
		GetSelected() ? "rgb(0, 170, 255)" : "rgb(211, 211, 211)",
		isEnterLabel ? "rgb(72,61,139)" : "");

	setStyleSheet(strStyleSheet.GetString());
	//setStyleSheet(GetSelected()
	//	? "background-color:;color:rgb(211, 211, 211)"
	//	: "background-color:;color:rgb(0, 170, 255)");  //…Ë÷√±≥æ∞—’…´
}
