#pragma once


#include <QStyleOptionTab>
#include <QStylePainter>
#include <QTabBar>
#include <QTabWidget>

//#include "ui_ThemeTabWidget.h"


class ThemeTabBar : public QTabBar {
public:
	QSize tabSizeHint(int index) const {
		QSize s = QTabBar::tabSizeHint(index);
		s.transpose();
		return s;
	}
	
protected:
	void paintEvent(QPaintEvent * /*event*/) {
		QStylePainter painter(this);
		QStyleOptionTab opt;

		for (int i = 0; i < count(); i++)
		{
			initStyleOption(&opt, i);
			painter.drawControl(QStyle::CE_TabBarTabShape, opt);
			painter.save();

			QSize s = opt.rect.size();
			s.transpose();
			QRect r(QPoint(), s);
			r.moveCenter(opt.rect.center());
			opt.rect = r;

			QPoint c = tabRect(i).center();
			painter.translate(c);
			painter.rotate(90);
			painter.translate(-c);
			painter.drawControl(QStyle::CE_TabBarTabLabel, opt);
			painter.restore();
		}
	}
};


class ThemeTabWidget : public QTabWidget
{
	//Q_OBJECT

public:
	ThemeTabWidget(QWidget *parent = Q_NULLPTR);
	~ThemeTabWidget();

//private:
//	Ui::QThemeTabWidget ui;
};
