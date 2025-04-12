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

		eventArgs.SetTitleText("����Ӱ��");
		eventArgs.SetMessageText("�ļ���Ϊ�հ��ļ���, ������ѡ���ļ��е���Ӱ��");

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

	strTotalCount.Format("�� %d Ӱ��", GetTotalCount());

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

	strText.Format("�Ѵ��� %d Ӱ��, ʣ�� %d Ӱ��, ʧ�� %d Ӱ��. ����Ϊʧ��Ӱ���б�: ",
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

	eventArgs.SetTitleText("����Ӱ��");
	eventArgs.SetMessageText("Ӱ�������");

	CMessenger::GetInstance().SendMessage(EMessengerKey::DisplayMessageBox,
		&eventArgs);

	if ((int)GetFailureFiles().size() > 0)
		ui.closeButton->SetIsEnable(true);
	else
		close();
}

