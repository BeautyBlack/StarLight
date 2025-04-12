#include "pch.h"
#include "IconButton.h"

CIconButton::CIconButton(QWidget* parent)
	: QPushButton(parent)
{
	SetIsButtonActive(false);

	InitializeMessenger();

	SetImageMouseAction(ImageMouseAction::UnkownAction);

	/*connect(this, SIGNAL(clicked()), this, SLOT([&]()->void
	{
		CMessenger::GetInstance().SendMessage(
			EMessengerKey::StudyImageButtonActiveStateChanged,
			(void*)(int)GetImageMouseAction());
	}));*/
}

CIconButton::CIconButton(const QString& text, QWidget* parent)
	: QPushButton(text, parent)
{
	SetIsButtonActive(false);

	InitializeMessenger();

	SetImageMouseAction(ImageMouseAction::UnkownAction);
}

CIconButton::CIconButton(
	const QIcon& icon, const QString& text, QWidget* parent)
	: QPushButton(icon, text, parent)
{
	SetIsButtonActive(false);

	SetImageMouseAction(ImageMouseAction::UnkownAction);

	InitializeMessenger();
}

void CIconButton::OnMouseActionChanged(ImageMouseAction action)
{
	if (action == ImageMouseAction::ResetImage)
		return; // RESET 不属于状态性动作

	SetIsButtonActive(action == GetImageMouseAction());

	update();
}

//void CIconButton::keyPressEvent(QKeyEvent* env)
//{
//	TRACE("Push Button Pressed\n");
//
//	QPushButton::keyPressEvent(env);
//
//	CMessenger::GetInstance().SendMessage(
//		EMessengerKey::StudyImageButtonActiveStateChanged,
//		(void*)(int)GetImageMouseAction());
//}

void CIconButton::InitializeMessenger()
{
	//CMessenger::GetInstance().Regist(
	//	EMessengerKey::StudyImageButtonActiveStateChanged,
	//	this,
	//	[&](void* pParam)->void*
	//{
	//	auto toolType = (ImageMouseAction)(int)pParam;

	//	if (toolType == ImageMouseAction::ResetImage)
	//		return NULL_SYMBAL; // RESET 不属于状态性动作

	//	SetIsButtonActive(toolType == GetImageMouseAction());

	//	update();

	//	return NULL_SYMBAL;
	//});
}
