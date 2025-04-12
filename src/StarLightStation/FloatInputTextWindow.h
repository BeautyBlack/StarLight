#pragma once

#include <Foundations/Common/ToDefine.h>
#include <QDialog>
#include <QString>
#include <QPoint>
#include "ui_FloatInputTextWindow.h"

class FloatInputTextWindow : public QDialog
{
	Q_OBJECT

public:
	FloatInputTextWindow(QWidget *parent = Q_NULLPTR);
	~FloatInputTextWindow();

	void SetDiplayLocation(QPoint position);
	
protected slots:
	void onInputEditTextChanged();
	
private:
	Ui::FloatInputTextWindow ui;

	AUTO_PROPERTY_REF(InputTextValue, QString);
};
