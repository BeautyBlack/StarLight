#include "pch.h"
#include "VRColorSettingWidget.h"

#include <QTableWidgetItem>

VRColorSettingWidget::VRColorSettingWidget(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	QStringList headerColor;
	headerColor << "" << "CT Value" << "R" << "G" << "B";

	ui.colorTable->setColumnCount(5);
	ui.colorTable->setHorizontalHeaderLabels(headerColor);

	QStringList headerOpacity;
	headerOpacity << "" << "CT Value" << "Opacity";

	ui.opacityTable->setColumnCount(3);

	ui.opacityTable->setHorizontalHeaderLabels(headerOpacity);

	m_colorButton.setText("Del");
	connect(&m_colorButton, SIGNAL(clicked()), this, SLOT(OnBtnClicked()));
	connect(&m_opcaityButton, SIGNAL(clicked()), this, SLOT(OnOpacityBtnClicked()));

	//ui.opacityTable->setItem(0, 0, new QTableWidgetItem("Jan"));
	//ui.opacityTable->setItem(1, 0, new QTableWidgetItem("Feb"));
	//ui.opacityTable->setItem(2, 0, new QTableWidgetItem("Mar"));
}

VRColorSettingWidget::~VRColorSettingWidget()
{
}

void VRColorSettingWidget::LoadSetting()
{
	UpdateDisplay();
}

void VRColorSettingWidget::on_addColorButton_clicked()
{
	sctColor color;

	color.ctValue = atof(ui.colorCTEdit->text().toStdString().c_str());
	color.r = atoi(ui.colorREdit->text().toStdString().c_str());
	color.g = atoi(ui.colorGEdit->text().toStdString().c_str());
	color.b = atoi(ui.colorBEdit->text().toStdString().c_str());

	GetSetting().GetColors().push_back(color);
	
	UpdateDisplay();
}

void VRColorSettingWidget::on_addOpacityButton_clicked()
{
	sctOpacity opacity;

	opacity.ctValue = atof(ui.opacityCTEdit->text().toStdString().c_str());
	opacity.opacity = atoi(ui.opacityValueEdit->text().toStdString().c_str());

	GetSetting().GetOpacities().push_back(opacity);
	
	UpdateDisplay();
}

void VRColorSettingWidget::on_applyButton_clicked()
{
	GetSetting().SetAmbient(atof(ui.ambientEdit->text().toStdString().c_str()));
	GetSetting().SetSpecularPower(atof(ui.specularPowerEdit->text().toStdString().c_str()));
	GetSetting().SetDiffuse(atof(ui.diffuseEdit->text().toStdString().c_str()));
	GetSetting().SetSpecular(atof(ui.specularEdit->text().toStdString().c_str()));
	
	hide();
	
	CMessenger::GetInstance().SendMessage(
		EMessengerKey::UpdateColorSetting,
		&GetSetting());
}

void VRColorSettingWidget::on_saveButton_clicked()
{
	on_applyButton_clicked();

	CMessenger::GetInstance().SendMessage(
		EMessengerKey::SaveColorSetting,
		&GetSetting());
}

void VRColorSettingWidget::on_clearOpacityButton_clicked()
{
	GetSetting().GetOpacities().clear();
	UpdateDisplay();
}

void VRColorSettingWidget::on_clearColorButton_clicked()
{
	GetSetting().GetColors().clear();
	UpdateDisplay();
}

void VRColorSettingWidget::UpdateDisplay()
{
	CString strText;

	strText.Format("%0.2lf", GetSetting().GetAmbient());
	ui.ambientEdit->setText(strText.GetString());

	strText.Format("%0.2lf", GetSetting().GetDiffuse());
	ui.diffuseEdit->setText(strText.GetString());

	strText.Format("%0.2lf", GetSetting().GetSpecularPower());
	ui.specularPowerEdit->setText(strText.GetString());

	strText.Format("%0.2lf", GetSetting().GetSpecular());
	ui.specularEdit->setText(strText.GetString());
	
	ui.colorTable->clearContents();
	ui.opacityTable->clearContents();

	ui.colorTable->setRowCount(GetSetting().GetColors().size());
	ui.opacityTable->setRowCount(GetSetting().GetOpacities().size());

	auto nIndex = 0;
	for(auto &it : GetSetting().GetColors())
	{
		auto button = new QPushButton;
		button->setText("Del");
		connect(button, SIGNAL(clicked()), this, SLOT(OnBtnClicked()));
		
		ui.colorTable->setCellWidget(nIndex, 0, button);
		
		strText.Format("%.1lf", it.ctValue);
		//auto ctValue = new QTableWidgetItem(strText.GetString());
		ui.colorTable->setItem(nIndex, 1, new QTableWidgetItem(strText.GetString()));

		strText.Format("%d", it.r);
		//auto r = QTableWidgetItem(strText.GetString());
		ui.colorTable->setItem(nIndex, 2, new QTableWidgetItem(strText.GetString()));

		strText.Format("%d", it.g);
		//auto g = QTableWidgetItem(strText.GetString());
		ui.colorTable->setItem(nIndex, 3, new QTableWidgetItem(strText.GetString()));

		strText.Format("%d", it.b);
		//auto b = QTableWidgetItem(strText.GetString());
		ui.colorTable->setItem(nIndex, 4, new QTableWidgetItem(strText.GetString()));

		nIndex++;
	}
	
	nIndex = 0;
	
	for (auto &it : GetSetting().GetOpacities())
	{
		CString strText;

		auto button = new QPushButton;
		button->setText("Del");
		connect(button, SIGNAL(clicked()), this, SLOT(OnOpacityBtnClicked()));

		ui.opacityTable->setCellWidget(nIndex, 0, button);

		strText.Format("%.1lf", it.ctValue);
		//auto ctValue = QTableWidgetItem(strText.GetString());
		ui.opacityTable->setItem(nIndex, 1, new QTableWidgetItem(strText.GetString()));

		strText.Format("%d", it.opacity);
		//auto r = QTableWidgetItem(strText.GetString());
		ui.opacityTable->setItem(nIndex, 2, new QTableWidgetItem(strText.GetString()));

		nIndex++;
	}
	
}

void VRColorSettingWidget::OnBtnClicked()
{
	QPushButton *senderObj = qobject_cast<QPushButton*>(sender());
	if (senderObj == nullptr)
	{
		return;
	}
	
	QModelIndex idx = ui.colorTable->indexAt(QPoint(senderObj->frameGeometry().x(), senderObj->frameGeometry().y()));
	int row = idx.row();

	if(row >= 0 && row < GetSetting().GetColors().size())
	{
		GetSetting().GetColors().erase(GetSetting().GetColors().begin() + row);

		UpdateDisplay();
	}
}

void VRColorSettingWidget::OnOpacityBtnClicked()
{
	QPushButton *senderObj = qobject_cast<QPushButton*>(sender());
	if (senderObj == nullptr)
	{
		return;
	}

	QModelIndex idx = ui.opacityTable->indexAt(QPoint(senderObj->frameGeometry().x(), senderObj->frameGeometry().y()));
	int row = idx.row();

	if (row >= 0 && row < GetSetting().GetOpacities().size())
	{
		GetSetting().GetOpacities().erase(GetSetting().GetOpacities().begin() + row);

		UpdateDisplay();
	}
}


