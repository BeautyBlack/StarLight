/*
 *****************************************************************
 *                 C O P Y R I G H T  (c) 2020
 *                    All Rights Reserved
 *****************************************************************
 *
 *       THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF
 *                      Zhang Jiaqi.
 *      The copyright notice above does not evidence any
 *     actual or intended publication of such source code.
 *
 *****************************************************************
 *
 *  Author: Zhang Jiaqi
 *
 */

#ifndef ICON_BUTTON_D54B364DF0054E708DB1D7A0D42DA082
#define ICON_BUTTON_D54B364DF0054E708DB1D7A0D42DA082

#pragma once

#include <Foundations/Common/ToDefine.h>

#include <QPushButton>

#include "utilities/Messenger/Messenger.h"
#include "Foundations/Interfaces/ImageMouseAction.h"


class CIconButton : public QPushButton, public CMessengerOwner
{
	Q_OBJECT

public:
	explicit CIconButton(QWidget *parent = nullptr);
	explicit CIconButton(const QString &text, QWidget *parent = nullptr);
	CIconButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);

	~CIconButton() = default;

	void OnMouseActionChanged(ImageMouseAction action);
	
private:
	Q_DISABLE_COPY(CIconButton)

protected:
	//void keyPressEvent(QKeyEvent *) override;

private:
	void InitializeMessenger();
	
private:
	AUTO_PROPERTY_REF(IsButtonActive, bool);

	AUTO_PROPERTY(ImageMouseAction, ImageMouseAction);
};

#endif

