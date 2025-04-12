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

#ifndef SERVICE_BASE_2F8E27001D17413BBD9FA1C3C5C64E51
#define SERVICE_BASE_2F8E27001D17413BBD9FA1C3C5C64E51

#pragma once


#include <string>

#include <Foundations/Interfaces/ServiceType.h>
#include <Foundations/Interfaces/ServiceParameterBase.h>
#include <Foundations/Interfaces/ImagePlanDirection.h>
#include <Foundations/Interfaces/ServiceResult.h>
#include <Foundations/Interfaces/ServiceRequest.h>

#include <Foundations/Common/ToDefine.h>

#include <map>

// ����ͷ�ļ���������
class CServiceParameterBase;

/*
 * �������
 */
__declspec(novtable) class CServiceBase
{
public:
	/*
	 * Ĭ�ϻ��๹�캯��
	 */
	CServiceBase()
	{
		SetServiceType(ServiceType::UnknownService);
		SetParameter(NULL_SYMBAL);
	}

	/*
	 * ��������
	 * ���������ȷʵ����������
	 */
	virtual ~CServiceBase() = default;
	
	/*
	 * Description
	 * ��ʼ��
	 * @pRequest [CServiceRequestBase*] ��ʼ������
	 * @return [bool] true : �ɹ�; false : ʧ��
	 */
	virtual bool Initialize(std::shared_ptr<CServiceRequest> request) = 0;

	/*
	 * Description
	 * ����Service
	 */
	virtual bool Start() = 0;

	/*
	 * Description
	 * ֹͣService, �ͷ���Դ
	 */
	virtual bool Release() = 0;

	///////////  Actions   /////////////////

	/*
	 * Description
	 * ����ͼ��
	 */
	virtual bool Process() = 0;

	/*
	 * Description
	 * ����
	 */
	virtual bool Reset() = 0;

protected:
	template<class T>
	static void CorrectVector(T* vector, int nDim, T* result) {
		auto foundMax = false;
		auto maxIndex = -1;
		auto maxValue = INT_MIN;
		auto sumValue = 0;

		for (auto i = 0; i < nDim; i++) {
			auto absValue = abs(vector[i]);

			if (maxValue < absValue) {
				maxIndex = i;
				maxValue = absValue;
			}

			sumValue += absValue * absValue;

			result[i] = vector[i];

			if (absValue >= 1) {
				foundMax = true;

				for (auto n = 0; n < nDim; n++) {
					if (n != i)
						result[n] = 0;
					else
						result[n] = vector[n] > 0 ? 1 : -1;
				}

				break;
			}
		}

		if (foundMax)
			return;

		// �����ͳ���1����Ҫ��������
		if (sumValue > 1) {
			sumValue = 0;

			for (auto k = 0; k < nDim; k++) {
				if (k != maxIndex)
					sumValue += vector[k] * vector[k];
			}

			result[maxIndex] = sqrt(1 - sumValue) * (result[maxIndex] > 0 ? 1 : -1);
		}
	}

	static void FormatImagePlanMatrix(double rowVector[3],
		double colVector[3],
		double imgMatrix[4][4])
	{

		imgMatrix[0][3] = 0;
		imgMatrix[1][3] = 0;
		imgMatrix[2][3] = 0;
		imgMatrix[3][3] = 1;

		imgMatrix[0][0] = rowVector[0]; // XX
		imgMatrix[1][0] = rowVector[1]; // XY
		imgMatrix[2][0] = rowVector[2]; // XZ
		imgMatrix[3][0] = 0;

		imgMatrix[0][1] = colVector[0]; // YX
		imgMatrix[1][1] = colVector[1]; // YY
		imgMatrix[2][1] = colVector[2]; // YZ
		imgMatrix[3][1] = 0;

		imgMatrix[0][2] = rowVector[1] * colVector[2] - rowVector[2] * colVector[1]; // ZX
		imgMatrix[1][2] = -(rowVector[0] * colVector[2] - rowVector[2] * colVector[0]); // ZY
		imgMatrix[2][2] = rowVector[0] * colVector[1] - rowVector[1] * colVector[0]; // ZZ
		imgMatrix[3][2] = 0;

		auto dbSqr = sqrt(imgMatrix[0][2] * imgMatrix[0][2] + imgMatrix[1][2] * imgMatrix[1][2] + imgMatrix[2][2] * imgMatrix[2][2]);
		if (abs(dbSqr) > 0.0001)
		{
			// ��һ��
			imgMatrix[0][2] /= dbSqr;
			imgMatrix[1][2] /= dbSqr;
			imgMatrix[2][2] /= dbSqr;
		}
	}
	
private:
	/*
	 * ��serviceʵ����Ψһʶ����
	 */
	AUTO_PROPERTY_REF(MyGuid, STRING_TYPE);
	
	/*
	 * Service ����
	 */
	AUTO_PROPERTY(ServiceType, ServiceType);

	/*
	 * �������
	 */
	AUTO_PROPERTY(Parameter, std::shared_ptr<CServiceParameterBase>);

	typedef std::map<ImagePlanDirection, std::shared_ptr<CServiceResult>> RESULT_MAP;
	/*
	 * ������
	 */
	AUTO_PROPERTY_REF_NO_SET(Results, RESULT_MAP);

	/*
	 * �Ƿ���Թ���ͬһ��ʵ��
	 */
	AUTO_PROPERTY(CanShare, bool);
};

#endif
