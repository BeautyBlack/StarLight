#include "pch.h"
#include "Services/Paronama/ParonamaService.h"


#include "Foundations/Common/StringUtility.h"
#include "Foundations/Log/LogHelper.h"

#include <paronama/Paronama.h>
#include <paronama/common.h>

#include "../Foundation.Common/MathUtility.h"
#include "omp.h"

CParonamaService::CParonamaService()
{
	SetServiceType(ServiceType::Paronama);

	SetMyGuid(CStringUtility::GenerateGuid().GetString());

	SetParonamaParameter(std::make_shared<CParonamaParameter>());
	SetParameter(GetParonamaParameter());

	GetParameter()->SetMyGuid(GetMyGuid());
	GetParameter()->SetGroupId(GetGroupId());

	ZeroMemory(m_zVector, sizeof(m_zVector));
	ZeroMemory(m_firstImageMatrix, sizeof(m_firstImageMatrix));
	ZeroMemory(m_orientCenter, sizeof(m_orientCenter));

	SetTransverseResult(std::make_shared<CParonamaResult>());
	SetSaggitalResult(std::make_shared<CParonamaResult>());
	SetCoronalResult(std::make_shared<CParonamaResult>());
	SetParonamaResult(std::make_shared<CParonamaResult>());

	GetTransverseResult()->SetImagePlan(ImagePlanDirection::Transverse);
	GetSaggitalResult()->SetImagePlan(ImagePlanDirection::Sagittal);
	GetCoronalResult()->SetImagePlan(ImagePlanDirection::Coronal);

	GetResults()[ImagePlanDirection::Sagittal] = GetSaggitalResult();
	GetResults()[ImagePlanDirection::Coronal] = GetCoronalResult();
	GetResults()[ImagePlanDirection::Transverse] = GetTransverseResult();
	GetResults()[ImagePlanDirection::Paronama] = GetParonamaResult();

	m_reconstructFuns[ImagePlanDirection::Sagittal] =
		[&]() -> void {
		ReconstructSaggital(); };
	
	m_reconstructFuns[ImagePlanDirection::Coronal] =
		[&]() -> void {
		ReconstructCoronal(); };
	
	m_reconstructFuns[ImagePlanDirection::Transverse] =
		[&]() -> void {
		ReconstructTransverse(); };

	m_reconstructFuns[ImagePlanDirection::Paronama] =
		[&]() -> void {
		ReconstructParonama(); };
}

CParonamaService::~CParonamaService()
{
	GetResults().clear();
}

bool CParonamaService::Initialize(std::shared_ptr<CServiceRequest> request)
{
	GetParameter()->Initialize();

	SetRequestParam(request);

	GetTransverseResult()->SetReferenceStudy(request->GetReferenceStudy());
	GetCoronalResult()->SetReferenceStudy(request->GetReferenceStudy());
	GetSaggitalResult()->SetReferenceStudy(request->GetReferenceStudy());
	GetParonamaResult()->SetReferenceStudy(request->GetReferenceStudy());
	
	LoadImages();

	return true;
}

bool CParonamaService::Start()
{
	return true;
}

bool CParonamaService::Release()
{
	return true;
}

bool CParonamaService::Process()
{
	if (!GetVtkImageData()
		|| !GetRequestParam()
		|| !GetStudy())
	{
		CLogHelper::InfoLog("影像体数据未创建");
		return false;
	}

	Reconstruct(ImagePlanDirection::Sagittal);
	Reconstruct(ImagePlanDirection::Transverse);
	Reconstruct(ImagePlanDirection::Coronal);
	Reconstruct(ImagePlanDirection::Paronama);

	ComputeScoutLine();
	
	return true;
}

bool CParonamaService::Reset()
{
	ResetActionParameters();
	return true;
}

void CParonamaService::LoadImages()
{
	if (!GetRequestParam()->IsValidImageData()) {
		CLogHelper::ErrorLog("牙弓线加载影像失败, request无效");
		return;
	}

	CLogHelper::InfoLog("牙弓线图像后处理 - Images Loading");

	try {
		CreateVolume();

		double dbOrientX[3], dbOrientY[3], dbRowVector[3], dbColVector[3];

		GetRequestParam()->GetOrientationX().ToArray(dbOrientX);
		GetRequestParam()->GetOrientationY().ToArray(dbOrientY);

		CorrectVector(dbOrientX, _countof(dbOrientX), dbRowVector);
		CorrectVector(dbOrientY, _countof(dbOrientY), dbColVector);

		FormatImagePlanMatrix(dbRowVector, dbColVector, m_firstImageMatrix);

		m_firstImageMatrix[0][3] = GetRequestParam()->GetImagePosition().x;
		m_firstImageMatrix[1][3] = GetRequestParam()->GetImagePosition().y;
		m_firstImageMatrix[2][3] = GetRequestParam()->GetImagePosition().z;

		double normalizePosition[3] = {
			GetRequestParam()->GetLastSliceImagePosition().x
				- GetRequestParam()->GetImagePosition().x,
			GetRequestParam()->GetLastSliceImagePosition().y
				- GetRequestParam()->GetImagePosition().y,
			GetRequestParam()->GetLastSliceImagePosition().z
				- GetRequestParam()->GetImagePosition().z };

		CMathUtility::NormalizeVector(normalizePosition,
			_countof(normalizePosition),
			m_zVector);

		double imageMatrixNormal[3] = { m_firstImageMatrix[0][2],
			m_firstImageMatrix[1][2], m_firstImageMatrix[2][2] };

		auto normalAngle = CMathUtility::ComputeAngle3D(
			m_zVector, imageMatrixNormal);

		m_zVector[0] = m_firstImageMatrix[0][2];
		m_zVector[1] = m_firstImageMatrix[1][2];
		m_zVector[2] = m_firstImageMatrix[2][2];

		if (normalAngle > CMathUtility::PI() / 2) {
			m_zVector[0] *= -1;
			m_zVector[1] *= -1;
			m_zVector[2] *= -1;
		}

		double dbTmp[3] = { 0 };
		CorrectVector(m_zVector, _countof(m_zVector), dbTmp);
		memcpy_s(m_zVector, sizeof(m_zVector), dbTmp, sizeof(dbTmp));

		/*let spacing = this.imageData.getSpacing();

		this.pixelSpacingX = spacing[0];
		this.pixelSpacingY = spacing[1];
		this.spaceBetweenSlice = spacing[2];*/

		memcpy_s(m_orientCenter, sizeof(m_orientCenter),
			GetVtkImageData()->GetCenter(), sizeof(m_orientCenter));

		sctPoint3D t(m_orientCenter[0], m_orientCenter[1], m_orientCenter[2]);
		//GetParonamaParameter()->SetCenter(t);

		auto dim = GetVtkImageData()->GetDimensions();


		//GetTranseverseImageData()->SetScrollSlicePositionStart(1);
		//GetTranseverseImageData()->SetScrollSlicePositionEnd(dim[2]); // 张数

		//GetSagitalImageData()->SetScrollSlicePositionStart(1);
		//GetSagitalImageData()->SetScrollSlicePositionEnd(dim[0]); // 宽度

		//GetCoronalImageData()->SetScrollSlicePositionStart(1);
		//GetCoronalImageData()->SetScrollSlicePositionEnd(dim[1]); // 高度

		//GetMprParameter()->GetSagittalParameter().SetSlicePositionEnd(
		//	GetSagitalImageData()->GetScrollSlicePositionEnd());

		//GetMprParameter()->GetTransverseParameter().SetSlicePositionEnd(
		//	GetTranseverseImageData()->GetScrollSlicePositionEnd());

		//GetMprParameter()->GetCoronalParameter().SetSlicePositionEnd(
		//	GetCoronalImageData()->GetScrollSlicePositionEnd());

		ResetActionParameters();
	}
	catch (...) {
		// empty
	}
}

std::shared_ptr<CParonamaResult> CParonamaService::GetResult(ImagePlanDirection direction)
{
	switch (direction)
	{
	case ImagePlanDirection::Sagittal:
		return GetSaggitalResult();

	case ImagePlanDirection::Transverse:
		return GetTransverseResult();

	case ImagePlanDirection::Coronal:
		return GetCoronalResult();

	case ImagePlanDirection::Paronama:
		return GetParonamaResult();

	default:
		return NULL_SYMBAL;
	}
}

void CParonamaService::ComputeScoutLine()
{
	ComputeScoutLine(GetResult(ImagePlanDirection::Coronal), *GetParonamaParameter().get());
	ComputeScoutLine(GetResult(ImagePlanDirection::Transverse), *GetParonamaParameter().get());
	ComputeScoutLine(GetResult(ImagePlanDirection::Sagittal), *GetParonamaParameter().get());
	ComputeScoutLine(GetResult(ImagePlanDirection::Paronama), *GetParonamaParameter().get());
}

void CParonamaService::ComputeScoutLine(std::shared_ptr<CParonamaResult> mprResult, CParonamaParameter& paramItem)
{
	// todo
}

void CParonamaService::CreateVolume()
{
	CServiceRequest &request = *GetRequestParam().get();

	SetVtkImageData(
		std::shared_ptr<vtkImageData>(vtkImageData::New(),
			[](vtkImageData* deleteVtkImageData) {
		deleteVtkImageData->Delete(); }));

	int nDimensions[3];
	nDimensions[0] = request.GetImageWidth();
	nDimensions[1] = request.GetImageHeight();
	nDimensions[2] = request.GetSliceCount();

	GetParonamaParameter()->SetVolumeHeight(request.GetImageHeight());
	GetParonamaParameter()->SetVolumeWidth(request.GetImageWidth());
	GetParonamaParameter()->SetVolumeSliceCount(request.GetSliceCount());

	GetParonamaParameter()->SetSourcePixelSpacingX(request.GetPixelSpacingX());
	GetParonamaParameter()->SetSourcePixelSpacingY(request.GetPixelSpacingY());
	GetParonamaParameter()->SetSourceSpacingBetweenSlice(request.GetSpacingBetweenSlice());

	GetVtkImageData()->SetDimensions(nDimensions);

	GetVtkImageData()->SetExtent(0, request.GetImageWidth() - 1,
		0, request.GetImageHeight() - 1,
		0, request.GetSliceCount() - 1);

	GetVtkImageData()->SetSpacing(request.GetPixelSpacingX(), request.GetPixelSpacingY(), request.GetSpacingBetweenSlice());

	auto pixelSizeInByte = request.GetImageWidth() * request.GetImageHeight() * request.GetSliceCount();

	if (request.GetIsPixelWord())
	{
		if (request.GetIsPixelSigned())
			GetVtkImageData()->SetScalarType(VTK_SHORT, GetVtkImageData()->GetInformation());
		else
			GetVtkImageData()->SetScalarType(VTK_UNSIGNED_SHORT, GetVtkImageData()->GetInformation());

		pixelSizeInByte *= 2;
	}
	else
	{
		if (request.GetIsPixelSigned())
			GetVtkImageData()->SetScalarType(VTK_CHAR, GetVtkImageData()->GetInformation());
		else
			GetVtkImageData()->SetScalarType(VTK_UNSIGNED_CHAR, GetVtkImageData()->GetInformation());
	}

	GetVtkImageData()->AllocateScalars(GetVtkImageData()->GetInformation());

	byte *pScalar = (byte*)GetVtkImageData()->GetScalarPointer();

	memcpy_s(pScalar, pixelSizeInByte, request.GetImageData().get(), pixelSizeInByte);
	
}

void CParonamaService::ResetActionParameters()
{
}

void CParonamaService::Reconstruct(ImagePlanDirection direction)
{
	auto it = m_reconstructFuns.find(direction);

	if (it != m_reconstructFuns.end() && it->second != NULL_SYMBAL)
		it->second();
}

void CParonamaService::ReconstructTransverse()
{
	auto t = GetTransverseResult();

	if (!t)
		return;

	if (t->GetResultImages() && t->GetResultImages()->size() > 1)
		return;
	
	auto count = GetParonamaParameter()->GetVolumeSliceCount();

	t->SetResultImages(std::make_shared<CParonamaResult::RESULT_VECTOR>());
	t->GetResultImages()->clear();
	t->SetScrollSlicePosition(0);
	t->SetScrollSlicePositionStart(0);
	t->SetScrollSlicePositionEnd(count);

	for(auto i = 0; i < count; i++)
	{
		auto sourceSlice = GetRequestParam()->GetReferenceSeries()->GetInstances()[i];
		
		auto imageData = std::make_shared<CParonamaResult>();

		t->GetResultImages()->push_back(imageData);

		imageData->SetImagePosition(
			sourceSlice->GetImagePlan()->GetPosition());

		imageData->SetOrientationX(
			sourceSlice->GetImagePlan()->GetOrientationX());

		imageData->SetOrientationY(
			sourceSlice->GetImagePlan()->GetOrientationY());
		
		imageData->SetImageWidth(GetParonamaParameter()->GetVolumeWidth());
		imageData->SetImageHeight(GetParonamaParameter()->GetVolumeHeight());

		imageData->SetIsPixelWord(GetRequestParam()->GetIsPixelWord());
		imageData->SetIsPixelSigned(GetRequestParam()->GetIsPixelSigned());

		const auto imageSizeInByte = GetParonamaParameter()->GetVolumeWidth()
			* GetParonamaParameter()->GetVolumeHeight()
			* (GetRequestParam()->GetIsPixelWord() ? 2 : 1);

		imageData->SetImageData(
			std::shared_ptr<BYTE_TYPE>(
				new unsigned char[imageSizeInByte],
				std::default_delete<BYTE_TYPE[]>()));

		memcpy_s(imageData->GetImageData().get(),
			imageSizeInByte,
			GetRequestParam()->GetImageData().get()
			+ i * imageSizeInByte,
			imageSizeInByte);

		imageData->SetPixelSpacingX(GetParonamaParameter()->GetSourcePixelSpacingX());
		imageData->SetPixelSpacingY(GetParonamaParameter()->GetSourcePixelSpacingY());
		imageData->SetSpacingBetweenSlice(GetParonamaParameter()->GetSourceSpacingBetweenSlice());

		imageData->SetWindowCenter(GetRequestParam()->GetWindowCenter());
		imageData->SetWindowWidth(GetRequestParam()->GetWindowWidth());

		imageData->SetSlop(GetRequestParam()->GetSlop());
		imageData->SetIntercept(GetRequestParam()->GetIntercept());
	}
}

void CParonamaService::ReconstructCoronal()
{
	
}

void CParonamaService::ReconstructSaggital()
{
	
}

void CParonamaService::ReconstructParonama()
{
	auto count = GetRequestParam()->GetSliceCount();
	
	int ncpus = omp_get_max_threads();
	omp_set_num_threads(ncpus);

	CProduceParonamaImage paromama_;
	paromama_.initializePara();
	paromama_.set_image_value(GetRequestParam()->GetImageWidth(),
		count,
		(short*)GetRequestParam()->GetImageData().get());
	paromama_.set_paronama_value(7, 5.0, 2);
	paromama_.set_paronama_pana(0.0, 0.0, 10.0);

	float xi[] = { 0.0,100,200,240,350 };
	float yi[] = { 0.0,100,300,150,70 };
	int ny = GetRequestParam()->GetImageWidth() / 2; // 5
	int nx = paromama_.generation_arch_wire(xi, yi, ny);

	ny += 2;
	short* img_out = new short[nx * /*7*/ny * count];
	paromama_.get_image_panoramic(img_out, nx);

	/////////////////////////
	auto t = GetParonamaResult();

	if (!t)
		return;

	if (t->GetResultImages() && t->GetResultImages()->size() > 1)
		return;

	t->SetImagePlan(ImagePlanDirection::Paronama);
	t->SetResultImages(std::make_shared<CParonamaResult::RESULT_VECTOR>());
	t->GetResultImages()->clear();
	t->SetScrollSlicePosition(0);
	t->SetScrollSlicePositionStart(0);
	t->SetScrollSlicePositionEnd(count);
	
	for (auto i = 0; i < count; i++)
	{
		auto imageData = std::make_shared<CParonamaResult>();

		t->GetResultImages()->push_back(imageData);

		/*imageData->SetImagePosition(
			sourceSlice->GetImagePlan()->GetPosition());

		imageData->SetOrientationX(
			sourceSlice->GetImagePlan()->GetOrientationX());

		imageData->SetOrientationY(
			sourceSlice->GetImagePlan()->GetOrientationY());*/

		imageData->SetImageWidth(nx);
		imageData->SetImageHeight(ny);

		imageData->SetIsPixelWord(GetRequestParam()->GetIsPixelWord());
		imageData->SetIsPixelSigned(GetRequestParam()->GetIsPixelSigned());

		const auto imageSizeInByte =
			imageData->GetImageWidth()
			* imageData->GetImageHeight()
			* (GetRequestParam()->GetIsPixelWord() ? 2 : 1);

		imageData->SetImageData(
			std::shared_ptr<BYTE_TYPE>(
				new unsigned char[imageSizeInByte],
				std::default_delete<BYTE_TYPE[]>()));

		memcpy_s(imageData->GetImageData().get(),
			imageSizeInByte,
			(byte*)img_out
			+ i * imageSizeInByte,
			imageSizeInByte);

		imageData->SetPixelSpacingX(GetParonamaParameter()->GetSourcePixelSpacingX());
		imageData->SetPixelSpacingY(GetParonamaParameter()->GetSourcePixelSpacingY());
		imageData->SetSpacingBetweenSlice(GetParonamaParameter()->GetSourceSpacingBetweenSlice());

		imageData->SetWindowCenter(GetRequestParam()->GetWindowCenter());
		imageData->SetWindowWidth(GetRequestParam()->GetWindowWidth());

		imageData->SetSlop(GetRequestParam()->GetSlop());
		imageData->SetIntercept(GetRequestParam()->GetIntercept());
	}
}



