#include "pch.h"

#include <Services/Mpr/MprService.h>
#include <Services/Mpr/MprParameter.h>
#include <Services/Mpr/MprResult.h>

#include <Foundations/Common/StringUtility.h>

#include "../Foundation.Common/MathUtility.h"
#include "Foundations/Log/LogHelper.h"

#include <vtk-9.0/vtkSmartPointer.h>
//#include <vtk-9.0/vtkImageReslice.h>
#include <vtk-9.0/vtkPointData.h>
#include <vtk-9.0/vtkDataArray.h>

#include "vtkImageResliceEx.h"
#include <vtk-9.0/vtkMath.h>
#include <vtk-9.0/vtkImageReslice.h>

CMprService::CMprService()
{
	SetServiceType(ServiceType::MprService);
	
	SetMyGuid(CStringUtility::GenerateGuid().GetString());

	SetMprParameter(std::make_shared<CMprParameter>());
	SetParameter(GetMprParameter());

	GetParameter()->SetMyGuid(GetMyGuid());
	GetParameter()->SetGroupId(GetGroupId());

	ZeroMemory(m_zVector, sizeof(m_zVector));
	ZeroMemory(m_firstImageMatrix, sizeof(m_firstImageMatrix));
	ZeroMemory(m_orientCenter, sizeof(m_orientCenter));

	SetTranseverseImageData(std::make_shared<CMprResult>());
	SetSagitalImageData(std::make_shared<CMprResult>());
	SetCoronalImageData(std::make_shared<CMprResult>());

	GetTranseverseImageData()->SetImagePlan(ImagePlanDirection::Transverse);
	GetSagitalImageData()->SetImagePlan(ImagePlanDirection::Sagittal);
	GetCoronalImageData()->SetImagePlan(ImagePlanDirection::Coronal);
	
	GetResults()[ImagePlanDirection::Sagittal] = GetSagitalImageData();
	GetResults()[ImagePlanDirection::Coronal] = GetCoronalImageData();
	GetResults()[ImagePlanDirection::Transverse] = GetTranseverseImageData();
}

CMprService::~CMprService()
{
	GetResults().clear();
}


bool CMprService::Initialize(std::shared_ptr<CServiceRequest> request)
{
	GetParameter()->Initialize();
	
	SetRequestParam(request);

	GetTranseverseImageData()->SetReferenceStudy(request->GetReferenceStudy());
	GetSagitalImageData()->SetReferenceStudy(request->GetReferenceStudy());
	GetCoronalImageData()->SetReferenceStudy(request->GetReferenceStudy());

	LoadImages();
	
	return true;
}

bool CMprService::Start()
{
	return true;
}

bool CMprService::Release()
{
	return true;
}

bool CMprService::Process()
{
	if(!GetVtkImageData()
		|| !GetRequestParam()
		|| !GetStudy())
	{
		CLogHelper::InfoLog("影像体数据未创建");
		return false;
	}

	Reconstruct(ImagePlanDirection::Sagittal);
	Reconstruct(ImagePlanDirection::Transverse);
	Reconstruct(ImagePlanDirection::Coronal);

	ComputeScoutLine();

	return true;
}

bool CMprService::Reset()
{
	ResetActionParameters();
	
	return true;
}

void CMprService::LoadImages()
{
	if ( !GetRequestParam()->IsValidImageData()) {
		CLogHelper::ErrorLog("MPR LoadImage 失败, request无效");
		return;
	}

	CLogHelper::InfoLog("MPR 图像后处理 - Images Loading");

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
				- GetRequestParam()->GetImagePosition().z};
		
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
		GetMprParameter()->SetOrientImageCenter(t);
		
		auto dim = GetVtkImageData()->GetDimensions();

		GetTranseverseImageData()->SetScrollSlicePositionStart(1);
		GetTranseverseImageData()->SetScrollSlicePositionEnd(dim[2]); // 张数

		GetSagitalImageData()->SetScrollSlicePositionStart(1);
		GetSagitalImageData()->SetScrollSlicePositionEnd(dim[0]); // 宽度
		
		GetCoronalImageData()->SetScrollSlicePositionStart(1);
		GetCoronalImageData()->SetScrollSlicePositionEnd(dim[1]); // 高度

		GetMprParameter()->GetSagittalParameter().SetSlicePositionEnd(
			GetSagitalImageData()->GetScrollSlicePositionEnd());
		
		GetMprParameter()->GetTransverseParameter().SetSlicePositionEnd(
			GetTranseverseImageData()->GetScrollSlicePositionEnd());
		
		GetMprParameter()->GetCoronalParameter().SetSlicePositionEnd(
			GetCoronalImageData()->GetScrollSlicePositionEnd());
		
		ResetActionParameters();
	}
	catch (...) {
		// empty
	}
}

void CMprService::CreateVolume()
{
	CServiceRequest &request = *GetRequestParam().get();
	
	SetVtkImageData(
		std::shared_ptr<vtkImageData>(vtkImageData::New(),
			[](vtkImageData* deleteVtkImageData){
					deleteVtkImageData->Delete();}));

	int nDimensions[3];
	nDimensions[0] = request.GetImageWidth();
	nDimensions[1] = request.GetImageHeight();
	nDimensions[2] = request.GetSliceCount();
	
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

void CMprService::ResetActionParameters()
{
	auto nDim = GetVtkImageData()->GetDimensions();
	auto nBounds = GetVtkImageData()->GetBounds();
	
	GetMprParameter()->SetImageBound(nDim[0], nDim[1], nDim[2]);

	GetMprParameter()->SetVolumeBound(nBounds[0], nBounds[1],
		nBounds[2], nBounds[3],
		nBounds[4], nBounds[5]);
	
	GetMprParameter()->Reset();

	double space[3] = {
		GetRequestParam()->GetPixelSpacingX(),
		GetRequestParam()->GetPixelSpacingY(),
		GetRequestParam()->GetSpacingBetweenSlice()
		};

	for(auto &it : GetMprParameter()->GetParameters())
	{
		it.second.SetSourcePixelSpacingX(
			GetRequestParam()->GetPixelSpacingX());

		it.second.SetSourcePixelSpacingY(
			GetRequestParam()->GetPixelSpacingY());

		it.second.SetSourceSpacingBetweenSlice(
			GetRequestParam()->GetSpacingBetweenSlice());

		double org[3] = { 0 };
		
		GetVtkImageData()->GetOrigin(org);
		double /*org[3] = {
		//	m_firstImageMatrix[0][3],
		//	m_firstImageMatrix[1][3],
		//	m_firstImageMatrix[2][3]
		//	/*it.second.GetCenter().x,
		//	it.second.GetCenter().y,
		//	it.second.GetCenter().z*/
		//},*/
			xDirection[3] = {
				1,0,0
				/*GetRequestParam()->GetOrientationX().x,
				GetRequestParam()->GetOrientationX().y,
				GetRequestParam()->GetOrientationX().z*/
		},
			yDirection[3] = {
				0,1,0
				/*GetRequestParam()->GetOrientationY().x,
				GetRequestParam()->GetOrientationY().y,
				GetRequestParam()->GetOrientationY().z*/
		},
			space[3] = {
				GetRequestParam()->GetPixelSpacingX(),
				GetRequestParam()->GetPixelSpacingY(),
				GetRequestParam()->GetSpacingBetweenSlice()
		};

		double zDirection[3] = {0,0,1};

		/*vtkMath::Cross(xDirection, yDirection, zDirection);

		zDirection[0] = -zDirection[0];
		zDirection[1] = -zDirection[1];
		zDirection[2] = -zDirection[2];*/

		auto nDim = GetVtkImageData()->GetDimensions();

		it.second.InitVolume(org, xDirection, yDirection, zDirection, space, nDim[0], nDim[1], nDim[2]);
	}
}

void CMprService::Reconstruct(ImagePlanDirection direction)
{
	auto t = GetMprResult(direction);

	if (!t)
		return;

	auto &imageData = *t.get();
	auto &mprParameterItem = GetMprParameter()->GetParameter(direction);

	mprParameterItem.GetMPRPlanePtr()->UpdateMPR();

	imageData.SetPlanDirection(mprParameterItem.GetPlanDirection());
	
	//通过reslice构建新的图像数据
	vtkSmartPointer<vtkImageReslice> tImageReslice
		= vtkSmartPointer<vtkImageReslice>::New();

	tImageReslice->SetInputData(GetVtkImageData().get());
	tImageReslice->SetOutputScalarType(
		GetRequestParam()->GetIsPixelWord() ?
		(GetRequestParam()->GetIsPixelSigned() ? VTK_SHORT : VTK_UNSIGNED_SHORT)
		: (GetRequestParam()->GetIsPixelSigned() ? VTK_SIGNED_CHAR : VTK_UNSIGNED_CHAR));

	tImageReslice->SetOutputSpacing(GetVtkImageData()->GetSpacing());

	tImageReslice->SetSlabModeToMax();
	tImageReslice->SetSlabNumberOfSlices(mprParameterItem.GetMPRPlanePtr()->GetSlapSliceNum());

	//背景
	tImageReslice->SetBackgroundLevel(GetRequestParam()->GetIntercept() /*-1024*/);

	tImageReslice->SetOutputDimensionality(2);

	tImageReslice->SetInterpolationModeToCubic();

	auto axes = mprParameterItem.GetMPRPlanePtr()->GetMPRMatrix();

	tImageReslice->SetResliceAxes(axes);
	//tImageReslice->SetAutoCropOutput(true);
	tImageReslice->Update();

	double directionNormal[6];
	double original[3];
	mprParameterItem.GetMPRPlanePtr()->GetXDirection(&directionNormal[0]);
	mprParameterItem.GetMPRPlanePtr()->GetYDirection(&directionNormal[3]);
	mprParameterItem.GetMPRPlanePtr()->GetOriginal(original);
	//tImageReslice->GetOutput();

	auto resliceData = tImageReslice->GetOutput();

	imageData.GetImagePosition().LoadFrom(
		original[0],
		original[1],
		original[2]);

	imageData.GetOrientationX().Update(
		directionNormal[0],
		directionNormal[1],
		directionNormal[2]);
	
	imageData.GetOrientationY().Update(
		directionNormal[3],
		directionNormal[4],
		directionNormal[5]);
	
	auto resliceDimension = resliceData->GetDimensions();
	auto outputSpacing = resliceData->GetSpacing();

	imageData.SetImageWidth(resliceDimension[0]);
	imageData.SetImageHeight(resliceDimension[1]);

	TRACE("\n%s[%d] Spacing [%.3lf, %.3lf], Dim [%d, %d]\n",
		__FUNCTION__,
		mprParameterItem.GetPlanDirection(),
		outputSpacing[0], outputSpacing[1],
		resliceDimension[0], resliceDimension[1]);
	
	imageData.SetIsPixelWord(GetRequestParam()->GetIsPixelWord());
	imageData.SetIsPixelSigned(GetRequestParam()->GetIsPixelSigned());

	const auto imageSizeInByte = resliceDimension[0]
		* resliceDimension[1]
		* (GetRequestParam()->GetIsPixelWord() ? 2 : 1);

	imageData.SetImageData(
		std::shared_ptr<BYTE_TYPE>(
			new unsigned char[imageSizeInByte],
			std::default_delete<BYTE_TYPE[]>()));

	memcpy_s(imageData.GetImageData().get(),
		imageSizeInByte,
		resliceData->GetScalarPointer(),
		imageSizeInByte);

	imageData.SetPixelSpacingX(outputSpacing[0]);
	imageData.SetPixelSpacingY(outputSpacing[1]);
	imageData.SetSpacingBetweenSlice(outputSpacing[2]);

	imageData.SetWindowCenter(GetRequestParam()->GetWindowCenter());
	imageData.SetWindowWidth(GetRequestParam()->GetWindowWidth());

	imageData.SetSlop(GetRequestParam()->GetSlop());
	imageData.SetIntercept(GetRequestParam()->GetIntercept());

}

void CMprService::Reconstruct2(ImagePlanDirection direction)
{
	auto t = GetMprResult(direction);

	if (!t)
		return;
	
	auto &imageData = *t.get();

	//let imageReslice = null;

	// 注意，不要使用foreach语法来遍历indexes， 因为offset=0的reslice是特殊reslice, 要用这个reslice来计算后续的方位等参数
	// foreach会为indexes排序，不能保证最后留下来的是offset=0的reslice, 当然也可以使用临时变量来存储0 reslice
	//for (let indexT = 0; indexT < indexes.length; indexT++) {
		// 因为浅拷贝的原因导致会缓存interpolationInfo在vtkAbstractImageInterpolator default value里面, 抽离出interpolator
	auto imageReslice = vtkSmartPointer<vtkImageResliceEx>::New();

	imageReslice->SetInputData(GetVtkImageData().get());
	imageReslice->SetOutputSpacing(GetVtkImageData()->GetSpacing());

	auto &mprParameterItem = GetMprParameter()->GetParameter(direction);
	
	//多层模式
	switch (mprParameterItem.GetImageSlabMode())
	{
	case ImageSlabMode::MIP:
		imageReslice->SetSlabModeToMax();
		break;
	case ImageSlabMode::MinIP:
		imageReslice->SetSlabModeToMin();
		break;
	case ImageSlabMode::AIP:
		imageReslice->SetSlabModeToMean();
		break;
	default:
		imageReslice->SetSlabModeToMean();
		break;
	}
	
	imageReslice->SetSlabNumberOfSlices(mprParameterItem.GetSlabSliceCount());

	//背景
	imageReslice->SetBackgroundLevel(GetRequestParam()->GetIntercept());

	imageReslice->SetOutputDimensionality(2);

	imageReslice->SetInterpolationModeToCubic();

	imageReslice->SetResliceAxes(mprParameterItem.GenerateVtkResliceMatrix());

	imageReslice->SetOutputScalarType(
		GetRequestParam()->GetIsPixelWord() ?
		(GetRequestParam()->GetIsPixelSigned() ? VTK_SHORT : VTK_UNSIGNED_SHORT)
		: (GetRequestParam()->GetIsPixelSigned() ? VTK_SIGNED_CHAR : VTK_UNSIGNED_CHAR));

	imageReslice->Update();

	auto resliceData = imageReslice->GetOutput();

	/*let conerstoneImageData = [];
	vtkImageDataToCornerstoneImageEx.newInstance().requestData([resliceData], conerstoneImageData);
*/
	auto resliceDimension = resliceData->GetDimensions();
	auto outputSpacing = resliceData->GetSpacing();

	imageData.SetImageWidth(resliceDimension[0]);
	imageData.SetImageHeight(resliceDimension[1]);

	imageData.SetIsPixelWord(GetRequestParam()->GetIsPixelWord());
	imageData.SetIsPixelSigned(GetRequestParam()->GetIsPixelSigned());

	const auto imageSizeInByte = resliceDimension[0]
		* resliceDimension[1]
		* (GetRequestParam()->GetIsPixelWord() ? 2 : 1);
	
	imageData.SetImageData(
		std::shared_ptr<BYTE_TYPE>(
				new unsigned char[imageSizeInByte],
			std::default_delete<BYTE_TYPE[]>()));

	memcpy_s(imageData.GetImageData().get(),
		imageSizeInByte,
		resliceData->GetScalarPointer(), 
		imageSizeInByte);

	imageData.SetPixelSpacingX(outputSpacing[0]);
	imageData.SetPixelSpacingY(outputSpacing[1]);
	imageData.SetSpacingBetweenSlice(outputSpacing[2]);

	/*const vtkPosition = imageReslice.getSlicePosition();
	imageData.imagePosition = this.projectToImagePosition(vtkPosition);

	let _vout = [];
	this.imageData.indexToWorldVec3(vtkPosition, _vout);
	param.imagePosition = _vout

		let topRight = this.projectToImagePosition(imageReslice.getTopRightPosition());
	let bottomLeft = this.projectToImagePosition(imageReslice.getBottomLeftPosition());

	let orientX = normalizeVector([topRight[0] - imageData.imagePosition[0],
		topRight[1] - imageData.imagePosition[1],
		topRight[2] - imageData.imagePosition[2]]);
	let orientY = normalizeVector([bottomLeft[0] - imageData.imagePosition[0],
		bottomLeft[1] - imageData.imagePosition[1],
		bottomLeft[2] - imageData.imagePosition[2]]);

	imageData.imageOrientation = orientX.concat(orientY);//param.imageOrientation;
	console.timeEnd("test imagePlanDirection" + param.imagePlanDirection)
*/

	//var minPixelCTValue = this.windowMinPixel;
	//var maxPixelCTValue = this.windowMaxPixel;
	imageData.SetWindowCenter(GetRequestParam()->GetWindowCenter());
	//.windowCenter = Math.round((minPixelCTValue + maxPixelCTValue) / 2);//this.defaultWindowLevel.center;
	imageData.SetWindowWidth(GetRequestParam()->GetWindowWidth());
	//= maxPixelCTValue - minPixelCTValue;//this.defaultWindowLevel.level;

	//imageData.CornerStoneImage.windowCenter = imageData.windowCenter;
	//imageData.CornerStoneImage.windowWidth = imageData.windowLevel;

	imageData.SetSlop(GetRequestParam()->GetSlop());
	imageData.SetIntercept(GetRequestParam()->GetIntercept());

	//imageData.slicePosition = param.sliceScrollPosition;

	//let vout = [0, 0, 0];

	//// vtk Volume 坐标转图像坐标
	//this.imageData.worldToIndexVec3(param.center, vout);

	//imageData.center3d = this.projectToImagePosition(vout);
}

void CMprService::ComputeScoutLine(
	std::shared_ptr<CMprResult> mprResult,
	CMprParameterItem &mprParam)
{
	mprResult->SetScrollSlicePosition(mprParam.GetCurrentScrollSlicePosition());
	
	double dbCenter[3] = { 0 };
	mprParam.GetMPRPlanePtr()->GetCenterPoint(dbCenter);

	double dbSagLine1Start[3] = { 0 }, dbSagLine1End[3] = { 0 };
	double dbSagLine2Start[3] = { 0 }, dbSagLine2End[3] = { 0 };

	mprParam.GetMPRPlanePtr()->GetNewCrossline1Pts(
		dbCenter, dbSagLine1Start, dbSagLine1End);

	mprParam.GetMPRPlanePtr()->GetNewCrossline2Pts(
		dbCenter, dbSagLine2Start, dbSagLine2End);

	mprResult->GetCenter().LoadFrom(dbCenter[0], dbCenter[1], dbCenter[2]);

	mprResult->GetScoutLineX().begin.LoadFrom(
		dbSagLine1Start[0], dbSagLine1Start[1], dbSagLine1Start[2]);

	mprResult->GetScoutLineX().end.LoadFrom(
		dbSagLine1End[0], dbSagLine1End[1], dbSagLine1End[2]);

	mprResult->GetScoutLineZ().begin.LoadFrom(
		dbSagLine2Start[0], dbSagLine2Start[1], dbSagLine2Start[2]);

	mprResult->GetScoutLineZ().end.LoadFrom(
		dbSagLine2End[0], dbSagLine2End[1], dbSagLine2End[2]);
}

void CMprService::ComputeScoutLine()
{
	ComputeScoutLine(
		GetMprResult(ImagePlanDirection::Transverse),
GetMprParameter()->GetParameter(ImagePlanDirection::Transverse));

	ComputeScoutLine(
		GetMprResult(ImagePlanDirection::Coronal),
		GetMprParameter()->GetParameter(ImagePlanDirection::Coronal));

	ComputeScoutLine(
		GetMprResult(ImagePlanDirection::Sagittal),
		GetMprParameter()->GetParameter(ImagePlanDirection::Sagittal));

}

std::shared_ptr<CMprResult> CMprService::GetMprResult(ImagePlanDirection direction)
{
	if (direction == ImagePlanDirection::Sagittal)
		return GetSagitalImageData();

	if (direction == ImagePlanDirection::Transverse)
		return GetTranseverseImageData();

	if (direction == ImagePlanDirection::Coronal)
		return GetCoronalImageData();

	return NULL_SYMBAL;
}

/**
	 * VTK Volume坐标系转换到Image Volume 图像坐标系
	 * @param volumePosition
	 * @returns {*[]}
	 */
sctPoint3D CMprService::ProjectToImagePosition(const sctPoint3D& volumePosition)
{
	double t[3] = { volumePosition.x, volumePosition.y, volumePosition.z };
	
	return ProjectToImagePosition(t);
}
sctPoint3D CMprService::ProjectToImagePosition(double volumePosition[3]) {
	double p[3] = { m_firstImageMatrix[0][3], m_firstImageMatrix[1][3], m_firstImageMatrix[2][3] };
	double x[3] = { m_firstImageMatrix[0][0], m_firstImageMatrix[1][0], m_firstImageMatrix[2][0] };
	double y[3] = { m_firstImageMatrix[0][1], m_firstImageMatrix[1][1], m_firstImageMatrix[2][1] };

	return CMathUtility::ComputePlanPosition(
		volumePosition[0], volumePosition[1], volumePosition[2],
		p, x, y, m_zVector,
		GetRequestParam()->GetPixelSpacingX(),
		GetRequestParam()->GetPixelSpacingY(),
		GetRequestParam()->GetSpacingBetweenSlice());

}

/**
 * Image Volume图像坐标系转换到VTK Volume 图像坐标系
 * @param imagePosition
 * @returns {number[]}
 */
sctPoint3D CMprService::ProjectToVtkVolumePosition(sctPoint3D &imagePosition)
{
	double t[3] = { imagePosition.x, imagePosition.y, imagePosition.z };
	
	return ProjectToVtkVolumePosition(t);
}

sctPoint3D CMprService::ProjectToVtkVolumePosition(double imagePosition[3])
{
	const double pos[3] = {
		imagePosition[0] - m_firstImageMatrix[0][3],
		imagePosition[1] - m_firstImageMatrix[1][3],
		imagePosition[2] - m_firstImageMatrix[2][3] };
	
	double result[] = { 0, 0, 0 };

	for (auto i = 0; i < 3; i++)
	{
		double vectorA[3] = { 0 };
		
		if(i == 2 )
		{
			vectorA[0] = m_zVector[0];
			vectorA[1] = m_zVector[1];
			vectorA[2] = m_zVector[2];
		}
		else
		{
			vectorA[0] = m_firstImageMatrix[0][i];
			vectorA[1] = m_firstImageMatrix[1][i];
			vectorA[2] = m_firstImageMatrix[2][i];
		}
		
		const auto a = pos[0] * vectorA[0] + pos[1] * vectorA[1] + pos[2] * vectorA[2];
		const auto b = sqrt(vectorA[0] * vectorA[0] + vectorA[1] * vectorA[1] + vectorA[2] * vectorA[2]);

		result[i] = a / (abs(b) < 0.0001 ? 1 : b);
	}

	return sctPoint3D(
	result[0] / GetRequestParam()->GetPixelSpacingX(),
	result[1] / GetRequestParam()->GetPixelSpacingY(),
	result[2] / GetRequestParam()->GetSpacingBetweenSlice());
}

