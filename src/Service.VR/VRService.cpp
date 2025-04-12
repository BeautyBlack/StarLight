#include "pch.h"

#include <Services/VR/VRService.h>
#include <Services/VR/VRParameter.h>

#include <Foundations/Common/StringUtility.h>

#include <utilities/ImageModule/Modules/StudyModule.h>

#include "../Foundation.Common/MathUtility.h"
#include "Foundations/Log/LogHelper.h"

#include <vtk-9.0/vtkSmartPointer.h>
#include <vtk-9.0/vtkPointData.h>
#include <vtk-9.0/vtkDataArray.h>
#include <vtk-9.0/vtkProperty.h>

#include <vtk-9.0/vtkMath.h>
#include <vtk-9.0/vtkImageReslice.h>
#include <vtk-9.0/vtkInteractorStyleTrackballCamera.h>
#include <vtk-9.0/vtkOpenGLGPUVolumeRayCastMapper.h>
#include <vtk-9.0/vtkOutputWindow.h>
#include <vtk-9.0/vtkCamera.h>
#include <vtk-9.0/vtkTextActor.h>
#include <vtk-9.0/vtkTextProperty.h>

#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtk-9.0/vtkTextMapper.h>
#include <vtk-9.0/vtkNamedColors.h>

CVRService::CVRService()
{
	SetServiceType(ServiceType::VRService);

	SetMyGuid(CStringUtility::GenerateGuid().GetString());

	SetVRParameter(std::make_shared<CVRParameter>());
	SetParameter(GetVRParameter());
	
	GetParameter()->SetMyGuid(GetMyGuid());
	GetParameter()->SetGroupId(GetGroupId());

	ZeroMemory(m_zVector, sizeof(m_zVector));
	ZeroMemory(m_firstImageMatrix, sizeof(m_firstImageMatrix));
	ZeroMemory(m_orientCenter, sizeof(m_orientCenter));

	SetRenderWindow(NULL_SYMBAL);
	SetRenderer(NULL_SYMBAL);
	SetVolume(NULL_SYMBAL);
	SetVolumeProperty(NULL_SYMBAL);
	SetVolumeMapper(NULL_SYMBAL);
	SetPiecewiseFunction(NULL_SYMBAL);
	SetColorTransferFunction(NULL_SYMBAL);
	SetInteractorStyle(NULL_SYMBAL);
	SetIterator(NULL_SYMBAL);
	SetAnnotatedCubeActor(NULL_SYMBAL);
	SetOrientationMarkerWidget(NULL_SYMBAL);

	vtkOutputWindow::SetGlobalWarningDisplay(0);
}

CVRService::~CVRService()
{
	Release();
}

bool CVRService::Initialize(std::shared_ptr<CServiceRequest> request)
{
	GetParameter()->Initialize();

	SetRequestParam(
		std::dynamic_pointer_cast<CVRServiceRequest>(
			request));

	LoadImages();

	CreateVtkComponent();

	Reset();
	
	return true;
}

bool CVRService::Start()
{
	return true;
}

bool CVRService::Release()
{
	if(GetRenderWindow() != NULL_SYMBAL)
		GetRenderWindow()->Delete();

	if(GetRenderer() != NULL_SYMBAL)
		GetRenderer()->Delete();

	if(GetVolume() != NULL_SYMBAL)
		GetVolume()->Delete();

	if(GetVolumeProperty() != NULL_SYMBAL)
		GetVolumeProperty()->Delete();

	if(GetVolumeMapper() != NULL_SYMBAL)
		GetVolumeMapper()->Delete();

	if(GetPiecewiseFunction() != NULL_SYMBAL)
		GetPiecewiseFunction()->Delete();

	if(GetColorTransferFunction() != NULL_SYMBAL)
		GetColorTransferFunction()->Delete();

	if(GetInteractorStyle() != NULL_SYMBAL)
		GetInteractorStyle()->Delete();

	if(GetIterator() != NULL_SYMBAL)
		GetIterator()->Delete();

	if (GetAnnotatedCubeActor() != NULL_SYMBAL)
		GetAnnotatedCubeActor()->Delete();

	if (GetOrientationMarkerWidget() != NULL_SYMBAL)
		GetOrientationMarkerWidget()->Delete();

	SetRenderWindow(NULL_SYMBAL);
	SetRenderer(NULL_SYMBAL);
	SetVolume(NULL_SYMBAL);
	SetVolumeProperty(NULL_SYMBAL);
	SetVolumeMapper(NULL_SYMBAL);
	SetPiecewiseFunction(NULL_SYMBAL);
	SetColorTransferFunction(NULL_SYMBAL);
	SetInteractorStyle(NULL_SYMBAL);
	SetIterator(NULL_SYMBAL);
	SetAnnotatedCubeActor(NULL_SYMBAL);
	SetOrientationMarkerWidget(NULL_SYMBAL);
	
	return true;
}

bool CVRService::Process()
{
	if (!GetVtkImageData()
		|| !GetRequestParam()
		|| !GetStudy())
	{
		CLogHelper::InfoLog("影像体数据未创建");
		return false;
	}

	Reconstruct();

	return true;
}

bool CVRService::Reset()
{
	ResetActionParameters();

	UpdateNon3DVolumeModeImgPosInfo();
	
	return true;
}

void CVRService::LoadImages()
{
	if (!GetRequestParam()->IsValidImageData()) {
		CLogHelper::ErrorLog("VR LoadImage 失败, request无效");
		return;
	}

	CLogHelper::InfoLog("VR 图像后处理 - Images Loading");

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

		/*m_zVector[0] = m_firstImageMatrix[0][2];
		m_zVector[1] = m_firstImageMatrix[1][2];
		m_zVector[2] = m_firstImageMatrix[2][2];*/

		if (normalAngle > CMathUtility::PI() / 2) {
			m_zVector[0] *= -1;
			m_zVector[1] *= -1;
			m_zVector[2] *= -1;
		}

		double dbTmp[3] = { 0 };
		CorrectVector(m_zVector, _countof(m_zVector), dbTmp);
		memcpy_s(m_zVector, sizeof(m_zVector), dbTmp, sizeof(dbTmp));

		memcpy_s(m_orientCenter, sizeof(m_orientCenter),
			GetVtkImageData()->GetCenter(), sizeof(m_orientCenter));

	}
	catch (...) {
		// empty
	}
}

void CVRService::CreateVolume()
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

	/*auto pixelCount = nDimensions[0] * nDimensions[1] * nDimensions[2];

	short *pScalarShort = (short*)pScalar;
	
	for (auto i = 0; i < pixelCount; i++, pScalarShort++)
		*pScalarShort = (short)ImagePixelToCTValue(*pScalarShort,
			GetRequestParam()->GetSlop(),
			GetRequestParam()->GetIntercept());*/
}

void CVRService::ResetActionParameters()
{
	GetVRParameter()->Reset();
}

void CVRService::UpdateColor()
{
	GetVolumeProperty()->SetAmbient(GetVRParameter()->GetAmbient());
	GetVolumeProperty()->SetDiffuse(GetVRParameter()->GetDiffuse());
	GetVolumeProperty()->SetSpecular(GetVRParameter()->GetSpecular());
	GetVolumeProperty()->SetSpecularPower(GetVRParameter()->GetSpecularPower());

	GetPiecewiseFunction()->RemoveAllPoints();
	GetColorTransferFunction()->RemoveAllPoints();

	GetPiecewiseFunction()->AddPoint(-65536, 0.0);
	GetPiecewiseFunction()->AddPoint(65536, 0.0);
	GetColorTransferFunction()->AddRGBPoint(-65536, 0, 0, 0);
	GetColorTransferFunction()->AddRGBPoint(65536, 0, 0, 1);

	for (auto &it : GetVRParameter()->GetColorSegments())
	{
		const int value = it.GetValue();

		const auto r = value >> 16;
		const auto g = (value >> 8) & 0x00FF;
		const auto b = value & 0x00FF;

		GetColorTransferFunction()->AddRGBPoint(
			CTValueToImagePixel(it.GetPixel(),
				GetRequestParam()->GetSlop(),
				GetRequestParam()->GetIntercept()),
			r / 255.0,
			g / 255.0,
			b / 255.0);
	}

	for (auto &it : GetVRParameter()->GetOpacitySegments())
	{
		GetPiecewiseFunction()->AddPoint(
			CTValueToImagePixel(it.GetPixel(),
				GetRequestParam()->GetSlop(),
				GetRequestParam()->GetIntercept()),
			it.GetValue());
	}

	GetPiecewiseFunction()->AddPoint(65536, 1.0);
	GetColorTransferFunction()->AddRGBPoint(65536, 1, 1, 1);

}

void CVRService::ManualRender()
{
	GetRenderWindow()->Render();
}


void CVRService::CreateVtkComponent()
{
	if (GetRenderWindow() != NULL_SYMBAL)
		return;
	
	SetRenderWindow(vtkRenderWindow::New());

	GetRenderWindow()->SetParentId(
		GetRequestParam()->GetParentRenderWindowHandle());

	HWND hwnd = (HWND)GetRequestParam()->GetParentRenderWindowHandle();

	CRect rcClient;

	GetClientRect(hwnd, &rcClient);
	
	GetRenderWindow()->SetSize(rcClient.Width(), rcClient.Height());
	//GetRenderWindow()->FullScreenOn();
	
	SetRenderer(vtkRenderer::New());
	
	SetVolume(vtkVolume::New());
	SetVolumeProperty(vtkVolumeProperty::New());
	SetVolumeMapper(vtkOpenGLGPUVolumeRayCastMapper::New());
	SetInteractorStyle(vtkInteractorStyleTrackballCamera::New());
	SetPiecewiseFunction(vtkPiecewiseFunction::New());
	SetColorTransferFunction(vtkColorTransferFunction::New());
	SetIterator(vtkRenderWindowInteractor::New());
	
	GetVolumeMapper()->SetBlendModeToComposite();
	GetVolumeMapper()->SetSampleDistance(0.5);
	GetVolumeMapper()->SetInputData(GetVtkImageData().get());

	GetRenderWindow()->SetInteractor(GetIterator());
	GetIterator()->SetInteractorStyle(GetInteractorStyle());

	/*GetPiecewiseFunction()->RemoveAllPoints();
	GetColorTransferFunction()->RemoveAllPoints();
	
	GetPiecewiseFunction()->AddPoint(-65536, 1.0);
	GetPiecewiseFunction()->AddPoint(65536, 1.0);
	GetColorTransferFunction()->AddRGBPoint(-65536, 0, 0, 0, 0, 0);
	GetColorTransferFunction()->AddRGBPoint(65536, 1, 1, 1, 1, 1);

	for(auto &it : GetVRParameter()->GetColorSegments())
	{
		const int value = it.GetValue();
		
		const auto r = value >> 16;
		const auto g = (value >> 8) & 0x00FF;
		const auto b = value & 0x00FF;
		
		GetColorTransferFunction()->AddRGBPoint(
			CTValueToImagePixel(it.GetPixel(),
				GetRequestParam()->GetSlop(),
				GetRequestParam()->GetIntercept()),
			r / 255.0,
			g / 255.0,
			b / 255.0);
	}

	for (auto &it : GetVRParameter()->GetOpacitySegments())
	{
		GetPiecewiseFunction()->AddPoint(
			CTValueToImagePixel(it.GetPixel(),
				GetRequestParam()->GetSlop(),
				GetRequestParam()->GetIntercept()),
			it.GetValue());
	}*/

	UpdateColor();
	
	GetVolume()->SetMapper(GetVolumeMapper());
	GetVolume()->SetProperty(GetVolumeProperty());

	GetVolumeProperty()->SetColor(GetColorTransferFunction());
	GetVolumeProperty()->SetScalarOpacity(GetPiecewiseFunction());
	GetVolumeProperty()->SetInterpolationTypeToLinear();

	GetVolumeProperty()->ShadeOn();
	//GetVolumeProperty()->SetAmbient(0.5/*0.5*/);
	//GetVolumeProperty()->SetDiffuse(0.9/*0.2*/);
	//GetVolumeProperty()->SetSpecular(0.2/*0.3*/);
	//GetVolumeProperty()->SetSpecularPower(10/*5*/);
	
	GetRenderer()->AddVolume(GetVolume());
	
	InitDisplayPatientInfo();
	
	GetRenderer()->ResetCamera();
	GetRenderer()->GetActiveCamera()->SetParallelProjection(1);
	//GetRenderer()->GetActiveCamera()->Zoom(1.2);
	
	GetRenderWindow()->AddRenderer(GetRenderer());

	int nTickBegin = GetTickCount();
	
	GetRenderWindow()->Render();
	InitAnnotatedCubeActor();
	UpdateNon3DVolumeModeImgPosInfo();

	TRACE("\n%s : Render Takes Tick -- %d\n",
		__FUNCTION__,
		GetTickCount() - nTickBegin);
	
}

void CVRService::Reconstruct()
{
	if (GetIterator() && GetRenderWindow())
	{
		int nTickInit = GetTickCount();
		
		GetIterator()->Initialize();

		int nTickStart = GetTickCount();


		TRACE("\n%s : Reconstruct Initialize Takes Tick -- [%d]\n",
			__FUNCTION__,
			nTickStart - nTickInit);
		
		GetIterator()->Start();
		
		TRACE("\n%s : Reconstruct Takes Tick -- [%d]\n",
			__FUNCTION__,GetTickCount() - nTickStart);

	}
}

/**
	 * VTK Volume坐标系转换到Image Volume 图像坐标系
	 * @param volumePosition
	 * @returns {*[]}
	 */
sctPoint3D CVRService::ProjectToImagePosition(const sctPoint3D& volumePosition)
{
	double t[3] = { volumePosition.x, volumePosition.y, volumePosition.z };

	return ProjectToImagePosition(t);
}
sctPoint3D CVRService::ProjectToImagePosition(double volumePosition[3]) {
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
sctPoint3D CVRService::ProjectToVtkVolumePosition(sctPoint3D &imagePosition)
{
	double t[3] = { imagePosition.x, imagePosition.y, imagePosition.z };

	return ProjectToVtkVolumePosition(t);
}

inline double CVRService::CTValueToImagePixel(int value, double slop, double intercept)
{
	return (value / slop) - intercept;
}

double CVRService::ImagePixelToCTValue(int pixel, double slop, double intercept)
{
	return pixel * slop + intercept;
}

sctPoint3D CVRService::ProjectToVtkVolumePosition(double imagePosition[3])
{
	const double pos[3] = {
		imagePosition[0] - m_firstImageMatrix[0][3],
		imagePosition[1] - m_firstImageMatrix[1][3],
		imagePosition[2] - m_firstImageMatrix[2][3] };

	double result[] = { 0, 0, 0 };

	for (auto i = 0; i < 3; i++)
	{
		double vectorA[3] = { 0 };

		if (i == 2)
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

void CVRService::InitAnnotatedCubeActor()
{
	if (GetAnnotatedCubeActor() != NULL_SYMBAL)
		GetAnnotatedCubeActor()->Delete();
	
	SetAnnotatedCubeActor(vtkAnnotatedCubeActor::New());
	
	/*GetAnnotatedCubeActor()->SetXPlusFaceText("L");
	GetAnnotatedCubeActor()->SetXMinusFaceText("R");
	GetAnnotatedCubeActor()->SetYPlusFaceText("P");
	GetAnnotatedCubeActor()->SetYMinusFaceText("A");
	GetAnnotatedCubeActor()->SetZPlusFaceText("H");
	GetAnnotatedCubeActor()->SetZMinusFaceText("F");*/
	GetAnnotatedCubeActor()->SetZFaceTextRotation(90);
	GetAnnotatedCubeActor()->SetFaceTextScale(0.65);
	GetAnnotatedCubeActor()->GetCubeProperty()->SetColor(0.5, 1, 1);
	GetAnnotatedCubeActor()->GetTextEdgesProperty()->SetLineWidth(1);
	GetAnnotatedCubeActor()->GetTextEdgesProperty()->SetDiffuse(0);
	GetAnnotatedCubeActor()->GetTextEdgesProperty()->SetAmbient(1);
	GetAnnotatedCubeActor()->GetTextEdgesProperty()->SetColor(0.98, 0.68, 0.23);
	GetAnnotatedCubeActor()->SetTextEdgesVisibility(1);
	GetAnnotatedCubeActor()->SetCubeVisibility(1);
	GetAnnotatedCubeActor()->SetFaceTextVisibility(1);

	GetAnnotatedCubeActor()->GetXPlusFaceProperty()->SetColor(1, 0, 0);
	GetAnnotatedCubeActor()->GetXPlusFaceProperty()->SetInterpolationToFlat();
	GetAnnotatedCubeActor()->GetXMinusFaceProperty()->SetColor(1, 0, 0);
	GetAnnotatedCubeActor()->GetXMinusFaceProperty()->SetInterpolationToFlat();
	GetAnnotatedCubeActor()->GetYPlusFaceProperty()->SetColor(0, 1, 0);
	GetAnnotatedCubeActor()->GetYPlusFaceProperty()->SetInterpolationToFlat();
	GetAnnotatedCubeActor()->GetYMinusFaceProperty()->SetColor(0, 1, 0);
	GetAnnotatedCubeActor()->GetYMinusFaceProperty()->SetInterpolationToFlat();
	GetAnnotatedCubeActor()->GetZPlusFaceProperty()->SetColor(0, 0, 1);
	GetAnnotatedCubeActor()->GetZPlusFaceProperty()->SetInterpolationToFlat();
	GetAnnotatedCubeActor()->GetZMinusFaceProperty()->SetColor(0, 0, 1);
	GetAnnotatedCubeActor()->GetZMinusFaceProperty()->SetInterpolationToFlat();

	if (GetOrientationMarkerWidget() != NULL_SYMBAL)
		GetOrientationMarkerWidget()->Delete();

	SetOrientationMarkerWidget(vtkOrientationMarkerWidget::New());

	GetOrientationMarkerWidget()->SetOutlineColor(0.93, 0.57, 0.13);
	GetOrientationMarkerWidget()->SetOrientationMarker(GetAnnotatedCubeActor());
	GetOrientationMarkerWidget()->SetInteractor(GetIterator());
	GetOrientationMarkerWidget()->SetViewport(0.88, 0.02, 0.96, 0.1);
	
	GetOrientationMarkerWidget()->SetEnabled(1);
	GetOrientationMarkerWidget()->InteractiveOff();
}

void CVRService::InitDisplayPatientInfo()
{
	vtkNew<vtkNamedColors> colors;

	SetPatientNameText(vtkTextActor::New());

	vtkTextProperty* txtprop = GetPatientNameText()->GetTextProperty();

	txtprop->SetFontFamilyToArial();
	txtprop->BoldOn();
	txtprop->SetFontSize(36);
	txtprop->ShadowOn();
	txtprop->SetShadowOffset(4, 4);
	txtprop->SetColor(colors->GetColor3d("Cornsilk").GetData());
	
	GetPatientNameText()->SetDisplayPosition(15, GetRenderWindow()->GetSize()[1]-40);

	GetPatientNameText()->GetTextProperty()->SetFontFamily(VTK_FONT_FILE);
	
	GetPatientNameText()->GetTextProperty()->SetFontFile(
		"C:\\Windows\\Fonts\\simhei.ttf");
	
	GetPatientNameText()->SetInput(
	CStringUtility::ConvertGBToUTF8(
		GetRequestParam()->GetReferenceStudy()->GetPatientName().c_str(),
		GetRequestParam()->GetReferenceStudy()->GetPatientName().length())
		.GetString()
	); // 文字内容
	
	GetRenderer()->AddActor2D(GetPatientNameText());
}

void CVRService::UpdateNon3DVolumeModeImgPosInfo()
{
	static bool first = true;

	if (!first)
		return;
	
	if(first)
		first = false;
	if (GetAnnotatedCubeActor() == NULL_SYMBAL
		|| GetOrientationMarkerWidget() == NULL_SYMBAL)
		return;
	
	switch (CMathUtility::GetImageViewOrientation(m_firstImageMatrix))
	{
	case 11:
		GetAnnotatedCubeActor()->SetXMinusFaceText("P");
		GetAnnotatedCubeActor()->SetXPlusFaceText("A");
		GetAnnotatedCubeActor()->SetYMinusFaceText("H");
		GetAnnotatedCubeActor()->SetYPlusFaceText("F");
		GetAnnotatedCubeActor()->SetZMinusFaceText("R");
		GetAnnotatedCubeActor()->SetZPlusFaceText("L");
		break;

	case 12:
		GetAnnotatedCubeActor()->SetXMinusFaceText("P");
		GetAnnotatedCubeActor()->SetXPlusFaceText("A");
		GetAnnotatedCubeActor()->SetYMinusFaceText("F");
		GetAnnotatedCubeActor()->SetYPlusFaceText("H");
		GetAnnotatedCubeActor()->SetZMinusFaceText("L");
		GetAnnotatedCubeActor()->SetZPlusFaceText("R");
		break;

	case 13:
		GetAnnotatedCubeActor()->SetXMinusFaceText("A");
		GetAnnotatedCubeActor()->SetXPlusFaceText("P");
		GetAnnotatedCubeActor()->SetYMinusFaceText("H");
		GetAnnotatedCubeActor()->SetYPlusFaceText("F");
		GetAnnotatedCubeActor()->SetZMinusFaceText("L");
		GetAnnotatedCubeActor()->SetZPlusFaceText("R");
		break;

	case 14:
		GetAnnotatedCubeActor()->SetXMinusFaceText("A");
		GetAnnotatedCubeActor()->SetXPlusFaceText("P");
		GetAnnotatedCubeActor()->SetYMinusFaceText("F");
		GetAnnotatedCubeActor()->SetYPlusFaceText("H");
		GetAnnotatedCubeActor()->SetZMinusFaceText("R");
		GetAnnotatedCubeActor()->SetZPlusFaceText("L");
		break;

	case 18:
		GetAnnotatedCubeActor()->SetXMinusFaceText("F");
		GetAnnotatedCubeActor()->SetXPlusFaceText("H");
		GetAnnotatedCubeActor()->SetYMinusFaceText("A");
		GetAnnotatedCubeActor()->SetYPlusFaceText("P");
		GetAnnotatedCubeActor()->SetZMinusFaceText("L");
		GetAnnotatedCubeActor()->SetZPlusFaceText("R");
		break;

	case 16:
		GetAnnotatedCubeActor()->SetXMinusFaceText("F");
		GetAnnotatedCubeActor()->SetXPlusFaceText("H");
		GetAnnotatedCubeActor()->SetYMinusFaceText("P");
		GetAnnotatedCubeActor()->SetYPlusFaceText("A");
		GetAnnotatedCubeActor()->SetZMinusFaceText("R");
		GetAnnotatedCubeActor()->SetZPlusFaceText("L");
		break;

	case 17:
		GetAnnotatedCubeActor()->SetXMinusFaceText("H");
		GetAnnotatedCubeActor()->SetXPlusFaceText("H");
		GetAnnotatedCubeActor()->SetYMinusFaceText("A");
		GetAnnotatedCubeActor()->SetYPlusFaceText("P");
		GetAnnotatedCubeActor()->SetZMinusFaceText("R");
		GetAnnotatedCubeActor()->SetZPlusFaceText("L");
		break;

	case 15:
		GetAnnotatedCubeActor()->SetXMinusFaceText("H");
		GetAnnotatedCubeActor()->SetXPlusFaceText("F");
		GetAnnotatedCubeActor()->SetYMinusFaceText("P");
		GetAnnotatedCubeActor()->SetYPlusFaceText("A");
		GetAnnotatedCubeActor()->SetZMinusFaceText("L");
		GetAnnotatedCubeActor()->SetZPlusFaceText("R");
		break;

	case 21:
		GetAnnotatedCubeActor()->SetXMinusFaceText("L");
		GetAnnotatedCubeActor()->SetXPlusFaceText("R");
		GetAnnotatedCubeActor()->SetYMinusFaceText("A");
		GetAnnotatedCubeActor()->SetYPlusFaceText("P");
		GetAnnotatedCubeActor()->SetZMinusFaceText("H");
		GetAnnotatedCubeActor()->SetZPlusFaceText("F");
		break;

	case 22:
		GetAnnotatedCubeActor()->SetXMinusFaceText("L");
		GetAnnotatedCubeActor()->SetXPlusFaceText("R");
		GetAnnotatedCubeActor()->SetYMinusFaceText("P");
		GetAnnotatedCubeActor()->SetYPlusFaceText("A");
		GetAnnotatedCubeActor()->SetZMinusFaceText("F");
		GetAnnotatedCubeActor()->SetZPlusFaceText("H");
		break;

	case 24://23:
		GetAnnotatedCubeActor()->SetXMinusFaceText("R");
		GetAnnotatedCubeActor()->SetXPlusFaceText("L");
		GetAnnotatedCubeActor()->SetYMinusFaceText("A");
		GetAnnotatedCubeActor()->SetYPlusFaceText("P");
		GetAnnotatedCubeActor()->SetZMinusFaceText("F");
		GetAnnotatedCubeActor()->SetZPlusFaceText("H");
		break;

	case 23://24:
		GetAnnotatedCubeActor()->SetXMinusFaceText("R");
		GetAnnotatedCubeActor()->SetXPlusFaceText("L");
		GetAnnotatedCubeActor()->SetYMinusFaceText("P");
		GetAnnotatedCubeActor()->SetYPlusFaceText("A");
		GetAnnotatedCubeActor()->SetZMinusFaceText("H");
		GetAnnotatedCubeActor()->SetZPlusFaceText("F");
		break;

	case 25:
		GetAnnotatedCubeActor()->SetXMinusFaceText("P");
		GetAnnotatedCubeActor()->SetXPlusFaceText("A");
		GetAnnotatedCubeActor()->SetYMinusFaceText("R");
		GetAnnotatedCubeActor()->SetYPlusFaceText("L");
		GetAnnotatedCubeActor()->SetZMinusFaceText("F");
		GetAnnotatedCubeActor()->SetZPlusFaceText("H");
		break;

	case 27:
		GetAnnotatedCubeActor()->SetXMinusFaceText("P");
		GetAnnotatedCubeActor()->SetXPlusFaceText("A");
		GetAnnotatedCubeActor()->SetYMinusFaceText("L");
		GetAnnotatedCubeActor()->SetYPlusFaceText("R");
		GetAnnotatedCubeActor()->SetZMinusFaceText("H");
		GetAnnotatedCubeActor()->SetZPlusFaceText("F");
		break;

	case 26:
		GetAnnotatedCubeActor()->SetXMinusFaceText("A");
		GetAnnotatedCubeActor()->SetXPlusFaceText("P");
		GetAnnotatedCubeActor()->SetYMinusFaceText("R");
		GetAnnotatedCubeActor()->SetYPlusFaceText("L");
		GetAnnotatedCubeActor()->SetZMinusFaceText("H");
		GetAnnotatedCubeActor()->SetZPlusFaceText("F");
		break;

	case 28:
		GetAnnotatedCubeActor()->SetXMinusFaceText("A");
		GetAnnotatedCubeActor()->SetXPlusFaceText("P");
		GetAnnotatedCubeActor()->SetYMinusFaceText("L");
		GetAnnotatedCubeActor()->SetYPlusFaceText("R");
		GetAnnotatedCubeActor()->SetZMinusFaceText("F");
		GetAnnotatedCubeActor()->SetZPlusFaceText("H");
		break;

	case 31:
		GetAnnotatedCubeActor()->SetXMinusFaceText("L");
		GetAnnotatedCubeActor()->SetXPlusFaceText("R");
		GetAnnotatedCubeActor()->SetYMinusFaceText("H");
		GetAnnotatedCubeActor()->SetYPlusFaceText("F");
		GetAnnotatedCubeActor()->SetZMinusFaceText("P");
		GetAnnotatedCubeActor()->SetZPlusFaceText("A");
		break;

	case 32:
		GetAnnotatedCubeActor()->SetXMinusFaceText("L");
		GetAnnotatedCubeActor()->SetXPlusFaceText("R");
		GetAnnotatedCubeActor()->SetYMinusFaceText("F");
		GetAnnotatedCubeActor()->SetYPlusFaceText("H");
		GetAnnotatedCubeActor()->SetZMinusFaceText("A");
		GetAnnotatedCubeActor()->SetZPlusFaceText("P");
		break;

	case 33:
		GetAnnotatedCubeActor()->SetXMinusFaceText("R");
		GetAnnotatedCubeActor()->SetXPlusFaceText("L");
		GetAnnotatedCubeActor()->SetYMinusFaceText("H");
		GetAnnotatedCubeActor()->SetYPlusFaceText("F");
		GetAnnotatedCubeActor()->SetZMinusFaceText("A");
		GetAnnotatedCubeActor()->SetZPlusFaceText("P");
		break;

	case 34:
		GetAnnotatedCubeActor()->SetXMinusFaceText("R");
		GetAnnotatedCubeActor()->SetXPlusFaceText("L");
		GetAnnotatedCubeActor()->SetYMinusFaceText("F");
		GetAnnotatedCubeActor()->SetYPlusFaceText("H");
		GetAnnotatedCubeActor()->SetZMinusFaceText("P");
		GetAnnotatedCubeActor()->SetZPlusFaceText("A");
		break;

	case 38:
		GetAnnotatedCubeActor()->SetXMinusFaceText("F");
		GetAnnotatedCubeActor()->SetXPlusFaceText("H");
		GetAnnotatedCubeActor()->SetYMinusFaceText("R");
		GetAnnotatedCubeActor()->SetYPlusFaceText("L");
		GetAnnotatedCubeActor()->SetZMinusFaceText("A");
		GetAnnotatedCubeActor()->SetZPlusFaceText("P");
		break;

	case 36:
		GetAnnotatedCubeActor()->SetXMinusFaceText("F");
		GetAnnotatedCubeActor()->SetXPlusFaceText("H");
		GetAnnotatedCubeActor()->SetYMinusFaceText("L");
		GetAnnotatedCubeActor()->SetYPlusFaceText("R");
		GetAnnotatedCubeActor()->SetZMinusFaceText("P");
		GetAnnotatedCubeActor()->SetZPlusFaceText("A");
		break;

	case 37:
		GetAnnotatedCubeActor()->SetXMinusFaceText("H");
		GetAnnotatedCubeActor()->SetXPlusFaceText("F");
		GetAnnotatedCubeActor()->SetYMinusFaceText("R");
		GetAnnotatedCubeActor()->SetYPlusFaceText("L");
		GetAnnotatedCubeActor()->SetZMinusFaceText("P");
		GetAnnotatedCubeActor()->SetZPlusFaceText("A");
		break;

	case 35:
		GetAnnotatedCubeActor()->SetXMinusFaceText("H");
		GetAnnotatedCubeActor()->SetXPlusFaceText("F");
		GetAnnotatedCubeActor()->SetYMinusFaceText("L");
		GetAnnotatedCubeActor()->SetYPlusFaceText("R");
		GetAnnotatedCubeActor()->SetZMinusFaceText("A");
		GetAnnotatedCubeActor()->SetZPlusFaceText("P");
		break;
	}
}


