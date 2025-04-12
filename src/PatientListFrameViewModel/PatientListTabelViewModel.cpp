#include "stdafx.h"
#include <ViewModels/PatientListFrameViewModel/PatientListTabelViewModel.h>

#include "Foundations/Common/TimeHelper.h"

#include <QTextCodec>


CPatientListTabelViewModel::CPatientListTabelViewModel(QObject *parent) :
	QAbstractTableModel(parent)
{
}

int CPatientListTabelViewModel::rowCount(const QModelIndex &parent) const
{
	return m_Studies.size();
}

int CPatientListTabelViewModel::columnCount(const QModelIndex &parent) const
{
	return CStudyDto::DisplayItemCounts();
}

QVariant CPatientListTabelViewModel::data(
	const QModelIndex &index, int role) const
{
	if (!index.isValid() || role != Qt::DisplayRole)        //安全判断
	{
		return QVariant();
	}

	auto studyIt = m_Studies.begin() + index.row();

	auto study = const_cast<CStudyDto*>(&(*studyIt));
	
	switch(index.column())
	{
	case CStudyDto::PatientIdIndex:
		return QString::fromLocal8Bit(
			study->GetPatientId().c_str(),
			study->GetPatientId().length());

	case CStudyDto::PatientNameIndex:
		return QString::fromLocal8Bit(
			study->GetPatientName().c_str(),
			study->GetPatientName().length());

	case CStudyDto::SexIndex:
		return SexText(study->GetSex());

	case CStudyDto::AgeIndex:
		{
			CString strText;
			strText.Format("%d%s", study->GetAge(), AgeUnitText(study->GetAgeUnit()));

			return QString::fromLocal8Bit(
				strText.GetString(), strText.GetLength());
		}
	case CStudyDto::ModalitysIndex:
		return QString::fromLocal8Bit(
			study->GetModalitys().c_str(),
			study->GetModalitys().length());

	case CStudyDto::AccessionNumberIndex:
		return QString::fromLocal8Bit(
			study->GetAccessionNumber().c_str(),
			study->GetAccessionNumber().length());

	case CStudyDto::StudyDescriptionIndex:
		return QString::fromLocal8Bit(
			study->GetStudyDescription().c_str(),
			study->GetStudyDescription().length());

	case CStudyDto::BodyPartsIndex:
		return QString::fromLocal8Bit(
			study->GetBodyParts().c_str(),
			study->GetBodyParts().length());

	case CStudyDto::StudyDateTimeIndex:
		return CTimeHelper::ToDateTimeText(study->GetStudyDateTime()).c_str();

	case CStudyDto::BirthDateIndex:
		return CTimeHelper::ToDateText(study->GetBirthDate()).c_str();

	case CStudyDto::StudyIdIndex:
		return study->GetStudyId().c_str();

	default:
		return QVariant();
	}

}

QVariant CPatientListTabelViewModel::headerData(
	int section, Qt::Orientation orientation, int role) const
{
	if (Qt::DisplayRole != role)
		return QVariant();
	
	if (orientation == Qt::Horizontal) {    //水平表头
		if (role == Qt::DisplayRole) {  //角色为显示
			switch (section)
			{
			case CStudyDto::PatientIdIndex:
				return QString::fromLocal8Bit(CStudyDto::GetPatientIdDesc().c_str());

			case CStudyDto::PatientNameIndex:
				return QString::fromLocal8Bit(CStudyDto::GetPatientNameDesc().c_str());

			case CStudyDto::SexIndex:
				return QString::fromLocal8Bit(CStudyDto::GetSexDesc().c_str());

			case CStudyDto::AgeIndex:
				return QString::fromLocal8Bit(CStudyDto::GetAgeDesc().c_str());
				
			case CStudyDto::ModalitysIndex:
				return QString::fromLocal8Bit(CStudyDto::GetModalitysDesc().c_str());

			case CStudyDto::AccessionNumberIndex:
				return QString::fromLocal8Bit(CStudyDto::GetAccessionNumberDesc().c_str());

			case CStudyDto::StudyDescriptionIndex:
				return QString::fromLocal8Bit(CStudyDto::GetStudyDescriptionDesc().c_str());

			case CStudyDto::BodyPartsIndex:
				return QString::fromLocal8Bit(CStudyDto::GetBodyPartsDesc().c_str());

			case CStudyDto::StudyDateTimeIndex:
				return QString::fromLocal8Bit(CStudyDto::GetStudyDateTimeDesc().c_str());

			case CStudyDto::BirthDateIndex:
				return QString::fromLocal8Bit(CStudyDto::GetBirthDateDesc().c_str());

			case CStudyDto::StudyIdIndex:
				return QString::fromLocal8Bit(CStudyDto::GetStudyIdDesc().c_str());
				
			default:
				return QVariant();
			}
		}
	}
	return QAbstractTableModel::headerData(section, orientation, role);
	
}

const std::map<int, int>& CPatientListTabelViewModel::GetSuggestColumnWidths()
{
	static bool hasInit = false;
	static std::map<int, int> result;

	if( !hasInit)
	{
		hasInit = true;

		result[CStudyDto::PatientIdIndex] = 180;
		result[CStudyDto::PatientNameIndex] = 180;
		result[CStudyDto::SexIndex] = 0;
		result[CStudyDto::AgeIndex] = 0;
		result[CStudyDto::ModalitysIndex] = 0;
		result[CStudyDto::AccessionNumberIndex] = 180;
		result[CStudyDto::StudyDescriptionIndex] = 180;
		result[CStudyDto::BodyPartsIndex] = 180;
		result[CStudyDto::StudyDateTimeIndex] = 220;
		result[CStudyDto::BirthDateIndex] = 150;
		result[CStudyDto::StudyIdIndex] = 180;
	}

	return result;
}

void CPatientListTabelViewModel::UpdatePatients(std::vector<CStudyDto>& studyDtos)
{
	beginResetModel();
	
	GetStudies().clear();
	GetStudies() = studyDtos;

	endResetModel();
}

CStudyDto& CPatientListTabelViewModel::GetStudy(int nIndex)
{
	static CStudyDto empty;

	if (nIndex < 0 || nIndex > (int)GetStudies().size())
		return empty;

	return GetStudies()[nIndex];
}
