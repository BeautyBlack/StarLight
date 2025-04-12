#include "QThemeTabWidget.h"

ThemeTabWidget::ThemeTabWidget(QWidget *parent)
	: QTabWidget(parent)
{
	ui.setupUi(this);

	setTabBar(new ThemeTabBar);
	setTabPosition(QTabWidget::West);
}

ThemeTabWidget::~ThemeTabWidget()
{
}
