#include "pch.h"
#include "ThemeMessageBox.h"

ThemeMessageBox::ThemeMessageBox(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	setWindowModality(Qt::WindowModal);

}

ThemeMessageBox::~ThemeMessageBox()
{
}

void ThemeMessageBox::SetTitleText(const char* strText, int nTextLen)
{
	ui.titleTextLable->setText(QString::fromLocal8Bit(strText, nTextLen));
}

void ThemeMessageBox::SetMessageText(const char* strText, int nTextLen)
{
	ui.messageTextLable->setText(QString::fromLocal8Bit(strText, nTextLen));
}

void ThemeMessageBox::Display()
{
	exec();
}
