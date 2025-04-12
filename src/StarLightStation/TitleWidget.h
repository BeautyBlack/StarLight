#pragma once

#include <QWidget>
#include "ui_TitleWidget.h"

#include <Foundations/Common/ToDefine.h>

class TitleWidget : public QWidget
{
	Q_OBJECT

public:
	TitleWidget(QWidget *parent = Q_NULLPTR);
	~TitleWidget();

protected:

private:
	Ui::TitleWidget ui;

	AUTO_PROPERTY_REF(TitleText, STRING_TYPE);
};
