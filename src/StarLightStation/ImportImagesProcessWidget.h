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

#ifndef IMPORT_IMAGES_PROCESS_WIDGET_C3B0898C80E1487594C45E527C58B337
#define IMPORT_IMAGES_PROCESS_WIDGET_C3B0898C80E1487594C45E527C58B337

#pragma once

#include <QDialog>
#include "ui_ImportImagesProcessWidget.h"

#include <Foundations/Common/ToDefine.h>

#include <vector>

class ImportImagesProcessWidget : public QDialog
{
	Q_OBJECT

public:
	ImportImagesProcessWidget(QWidget *parent = Q_NULLPTR);
	~ImportImagesProcessWidget();

	void StartImport();

private slots:
	void ProcessOneFile(int nLeftCount, int nProcessedCount, int nFailureCount, STRING_TYPE strFailureFile);

	void FinishedImport();

private:
	Ui::ImportImagesProcessWidget ui;

	AUTO_PROPERTY_REF(ImportingPath, STRING_TYPE);

	AUTO_PROPERTY_REF_NO_SET(ImportingFilePaths, std::vector<STRING_TYPE>);

	AUTO_PROPERTY_REF(TotalCount, int);

	AUTO_PROPERTY_REF(ProcessedCount, int);

	AUTO_PROPERTY_REF(LeftCount, int);

	AUTO_PROPERTY_REF(FailureCount, int);

	AUTO_PROPERTY_REF_NO_SET(FailureFiles, std::vector<STRING_TYPE>);
};

#endif

