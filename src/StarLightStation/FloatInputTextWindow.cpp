#include "pch.h"

#include "FloatInputTextWindow.h"

FloatInputTextWindow::FloatInputTextWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	setWindowModality(Qt::WindowModal);

	connect(ui.inputEdit, SIGNAL(textChanged()), this, SLOT(onInputEditTextChanged()));
}

FloatInputTextWindow::~FloatInputTextWindow()
{
}

void FloatInputTextWindow::SetDiplayLocation(QPoint position)
{
	setGeometry(position.x(), position.y(), 300, 32);
}

void FloatInputTextWindow::onInputEditTextChanged()
{
	auto text = ui.inputEdit->toPlainText();
	if (text.contains('\n'))
	{
		SetInputTextValue(text);
		
		close();
	}
}

