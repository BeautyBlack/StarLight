#pragma once

#include "FrameMenu.h"

#include <QLabel>

#include <Foundations/Common/ToDefine.h>

#include "utilities/Messenger/Messenger.h"

class CMenuLabel : public QLabel, public CMessengerOwner
{
	Q_OBJECT

public:
	CMenuLabel(QWidget *parent);
	~CMenuLabel();

	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

	void updateLabelColor(bool isEnterLabel = false);
	
private:
	AUTO_PROPERTY(FrameMenu, FrameMenu);

	AUTO_PROPERTY(Selected, bool);
};

