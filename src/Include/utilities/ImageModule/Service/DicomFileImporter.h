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

#ifndef DICOM_FILE_IMPORTER_DADDF9417ACF4144BEDA065C634EAD8B
#define DICOM_FILE_IMPORTER_DADDF9417ACF4144BEDA065C634EAD8B

#pragma once

#include <utilities/ImageModule/ExportFlagDefine.h>
#include <Foundations/DataModule/StudyDto.h>

class DcmFileFormat;
class DcmDataset;

class UTILITIESIMAGEMODULE_API CDicomFileImporter
{
public:
	static bool ImportFile(const char* strFilePath);

	static bool ImportFile(DcmFileFormat *fileFormat);

	static STRING_TYPE GenerateNewStudyInstanceUID();
	
private:

	static CStudyDto DatasetToStudyDto(DcmDataset* pDataset);
};

#endif

