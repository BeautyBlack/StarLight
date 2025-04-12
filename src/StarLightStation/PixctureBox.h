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

#ifndef PIXCTURE_BOX_70F5354B03504AAB98BAD7E6F3996E69
#define PIXCTURE_BOX_70F5354B03504AAB98BAD7E6F3996E69

#pragma once

#include <qobject.h>
#include <qgraphicsview.h>


class CPixctureBox final : public QWidget
{
	Q_OBJECT

public:
	enum PB_MODE { FIXED_SIZE, FIX_SIZE_CENTRED, AUTO_ZOOM, AUTO_SIZE };

	explicit CPixctureBox(QWidget* parent = nullptr);

	void setMode(PB_MODE mode);
	
	~CPixctureBox() = default;
	
protected:
	void paintEvent(QPaintEvent * event);
	
signals:

public slots:
	bool setImage(QImage &image, double scale = 1.0);
	void setBackground(QBrush brush);

	
private:
	QPixmap m_pixmap;
	
	double m_scale;
	
	PB_MODE m_mode;
	
	QBrush m_brush;
};

#endif
