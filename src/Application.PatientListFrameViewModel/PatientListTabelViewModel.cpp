#include "pch.h"
#include <Application/PatientListFrameViewModel/PatientListTabelViewModel.h>

#include "Foundations/Common/TimeHelper.h"


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
	if (!index.isValid())        //安全判断
	{
		return QVariant();
	}

	auto studyIt = m_Studies.begin() + index.row();

	auto study = const_cast<CStudyDto*>(&(*studyIt));
	
	switch(index.column())
	{
	case CStudyDto::PatientIdIndex:
		return study->GetPatientId().c_str();

	case CStudyDto::PatientNameIndex:
		return study->GetPatientName().c_str();

	case CStudyDto::SexIndex:
		return SexText(study->GetSex());

	case CStudyDto::AgeIndex:
		{
			CString strText;
			strText.Format("%d%s", study->GetAge(), AgeUnitText(study->GetAgeUnit()));

			return strText.GetString();
		}
	case CStudyDto::ModalitysIndex:
		return study->GetModalitys().c_str();

	case CStudyDto::AccessionNumberIndex:
		return study->GetAccessionNumber().c_str();

	case CStudyDto::StudyDescriptionIndex:
		return study->GetStudyDescription().c_str();

	case CStudyDto::BodyPartsIndex:
		return study->GetBodyParts().c_str();

	case CStudyDto::StudyDateTimeIndex:
		return CTimeHelper::ToDateTimeText(study->GetStudyDateTime()).c_str();

	case CStudyDto::BirthDateIndex:
		return CTimeHelper::ToDateTimeText(study->GetBirthDate()).c_str();

	case CStudyDto::StudyIdIndex:
		return study->GetStudyId().c_str();

	default:
		return QVariant();
	}

}

QVariant CPatientListTabelViewModel::headerData(
	int section, Qt::Orientation orientation, int role)
{
	if (Qt::DisplayRole != role)
		return QVariant();
	
	if (orientation == Qt::Horizontal) {    //水平表头
		if (role == Qt::DisplayRole) {  //角色为显示
			switch (section)
			{
			case CStudyDto::PatientIdIndex:
				return CStudyDto::GetPatientIdDesc().c_str();

			case CStudyDto::PatientNameIndex:
				return CStudyDto::GetPatientNameDesc().c_str();

			case CStudyDto::SexIndex:
				return CStudyDto::GetSexDesc().c_str();

			case CStudyDto::AgeIndex:
				return CStudyDto::GetAgeDesc().c_str();
				
			case CStudyDto::ModalitysIndex:
				return CStudyDto::GetModalitysDesc().c_str();

			case CStudyDto::AccessionNumberIndex:
				return CStudyDto::GetAccessionNumberDesc().c_str();

			case CStudyDto::StudyDescriptionIndex:
				return CStudyDto::GetStudyDescriptionDesc().c_str();

			case CStudyDto::BodyPartsIndex:
				return CStudyDto::GetBodyPartsDesc().c_str();

			case CStudyDto::StudyDateTimeIndex:
				return CStudyDto::GetStudyDateTimeDesc().c_str();

			case CStudyDto::BirthDateIndex:
				return CStudyDto::GetBirthDateDesc().c_str();

			case CStudyDto::StudyIdIndex:
				return CStudyDto::GetStudyIdDesc().c_str();
				
			default:
				return QVariant();
			}
		}
	}
	return QAbstractTableModel::headerData(section, orientation, role);
	
}

void CPatientListTabelViewModel::UpdatePatients(std::vector<CStudyDto>& studyDtos)
{
	GetStudies().clear();
	GetStudies() = studyDtos;
}
