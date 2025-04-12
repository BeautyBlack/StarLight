#include "pch.h"
#include "ImportImagesProcessWidget.h"

#include "Foundations/Common/FileUtility.h"
#include "utilities/Messenger/EventArgs/UI/MessageBox/RequestMessageBoxEventArgs.h"

#include "utilities/Messenger/Messenger.h"

#include <utilities/ImageModule/Service/DicomFileImporter.h>

#include <thread>

ImportImagesProcessWidget::ImportImagesProcessWidget(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
}

ImportImagesProcessWidget::~ImportImagesProcessWidget()
{
}

void ImportImagesProcessWidget::StartImport()
{
	CFileUtility::SearchPaths([&](const char* t) -> void {
		GetImportingFilePaths().push_back(t); },
		GetImportingPath().c_str(),
			NULL_SYMBAL,
			true);

	if (GetImportingFilePaths().size() == 0)
	{
		CRequestMessageBoxEventArgs eventArgs;

		eventArgs.SetTitleText("导入影像");
		eventArgs.SetMessageText("文件夹为空白文件夹, 请重新选择文件夹导入影像");

		CMessenger::GetInstance().SendMessage(
			EMessengerKey::DisplayMessageBox,
			&eventArgs);

		return;
	}

	SetTotalCount(GetImportingFilePaths().size());
	SetLeftCount(GetTotalCount());
	SetProcessedCount(0);
	SetFailureCount(0);

	CString strTotalCount;

	strTotalCount.Format("共 %d 影像", GetTotalCount());

	ui.totalCountLable->setText(CSTRING_TO_QSTRING(strTotalCount));
	ui.progressBar->setValue(0);
	ui.progressBar->setRange(0, GetTotalCount());

	STRING_TYPE emptyText = "";

	std::thread processThread([&]()->void {
		Sleep(500);
		for(auto &it : GetImportingFilePaths())
		{
			auto success = CDicomFileImporter::ImportFile(it.c_str());

			if (!success)
			{
				SetFailureCount(GetFailureCount() + 1);
				GetFailureFiles().push_back(it);
			}

			SetLeftCount(GetLeftCount() - 1);
			SetProcessedCount(GetProcessedCount() + 1);

			QMetaObject::invokeMethod(this, "ProcessOneFile",
				Qt::BlockingQueuedConnection,
				Q_ARG(int, GetLeftCount()),
				Q_ARG(int, GetProcessedCount()),
				Q_ARG(int, GetFailureCount()),
				Q_ARG(STRING_TYPE, success ? emptyText : it));
		}

		QMetaObject::invokeMethod(this, "FinishedImport");
	});

	processThread.detach();

	exec();
}

void ImportImagesProcessWidget::ProcessOneFile(
	int nLeftCount, int nProcessedCount, int nFailureCount, STRING_TYPE strFailureFile)
{
	CString strText;

	strText.Format("已处理 %d 影像, 剩余 %d 影像, 失败 %d 影像. 下面为失败影像列表: ",
		nProcessedCount, nLeftCount, nFailureCount);

	ui.processStatusLable->setText(CSTRING_TO_QSTRING(strText));

	if (!strFailureFile.empty())
		ui.failureFileList->addItem(
			TEXT_TO_QSTRING(strFailureFile.c_str(), strFailureFile.length()));

	ui.progressBar->setValue(nProcessedCount);
}

void ImportImagesProcessWidget::FinishedImport()
{
	CRequestMessageBoxEventArgs eventArgs;

	eventArgs.SetTitleText("导入影像");
	eventArgs.SetMessageText("影像导入完成");

	CMessenger::GetInstance().SendMessage(EMessengerKey::DisplayMessageBox,
		&eventArgs);

	if ((int)GetFailureFiles().size() > 0)
		ui.closeButton->SetIsEnable(true);
	else
		close();
}

