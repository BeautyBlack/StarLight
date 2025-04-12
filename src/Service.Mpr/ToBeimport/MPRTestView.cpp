#include "pch.h"


#include <functional>
#include <vtk-9.0/vtkMath.h>
#include <vtk-9.0/vtkMatrix4x4.h>
#include <vtk-9.0/vtkImageReslice.h>
#include <vtk-9.0/vtkImageData.h>

#include "BoxIntersectionProcess.h"
#include "MPRPlane.h"

void ReconstructMpr(vtkImageData *pData,
	MPRPlanePtr para,
	bool isSigned,
	bool isWord,
	std::function<void(vtkImageData*, double*, double*)> callback)
{
	para->UpdateMPR();

	//通过reslice构建新的图像数据
	vtkSmartPointer<vtkImageReslice> tImageReslice = vtkSmartPointer<vtkImageReslice>::New();
	//tImageReslice->SetInput(pData);
	tImageReslice->SetInputData(pData);
	tImageReslice->SetOutputScalarType(
		isWord ?
		(isSigned ? VTK_SHORT : VTK_UNSIGNED_SHORT)
		: (isSigned ? VTK_SIGNED_CHAR : VTK_UNSIGNED_CHAR));

	tImageReslice->SetOutputSpacing(pData->GetSpacing());

	tImageReslice->SetSlabModeToMax();
	tImageReslice->SetSlabNumberOfSlices(para->GetSlapSliceNum());


	//背景
	tImageReslice->SetBackgroundLevel(-1024);

	tImageReslice->SetOutputDimensionality(2);

	tImageReslice->SetInterpolationModeToCubic();

	vtkSmartPointer<vtkMatrix4x4> matrix = para->GetMPRMatrix();
	tImageReslice->SetResliceAxes(matrix);
	tImageReslice->Update();

	double direction[6];
	double original[3];
	para->GetXDirection(&direction[0]);
	para->GetYDirection(&direction[3]);
	para->GetOriginal(original);
	tImageReslice->GetOutput();

	callback(tImageReslice->GetOutput(),
		original,
		direction);
}


void ReconstructMpr2(vtkImageData *pData,
	double space[3],
	bool isSigned,
	bool isWord,
	//double pos[3],
	double org[3],
	double xDirection[3],
	double yDirection[3],
	//double zDirection[3],
	int   nDim[3],
	double rx[3],
	double ry[3],
	std::function<void(vtkImageData*, double*, double*)> callback)
{
		double zDirection[3];

		vtkMath::Cross(xDirection, yDirection, zDirection);

		zDirection[0] = -zDirection[0];
		zDirection[1] = -zDirection[1];
		zDirection[2] = -zDirection[2];

		//space[2] = space[0];
		double pos[3];
		pos[0] = org[0]+nDim[0]*space[0]*xDirection[0]/2+nDim[1]*space[1]*xDirection[1]/2+nDim[2]*space[2]*xDirection[2]/2;
		pos[1] = org[1]+nDim[0]*space[0]*yDirection[0]/2+nDim[1]*space[1]*yDirection[1]/2+nDim[2]*space[2]*yDirection[2]/2;
		pos[2] = org[2]+nDim[0]*space[0]*zDirection[0]/2+nDim[1]*space[1]*zDirection[1]/2+nDim[2]*space[2]*zDirection[2]/2;

		BoxIntersectionProcessPtr VolumeBox(new BoxIntersectionProcess);
		VolumeBox->InitVolume(org, xDirection, yDirection, zDirection, space, nDim[0], nDim[1], nDim[2]);

		MPRPlanePtr para(new MPRPlane);
		para->SetBoxIntersectionProcess(VolumeBox);
		/*double rx[3], ry[3];
		switch (i)
		{
		case 0:
			rx[0]=1; rx[1]=0; rx[2] = 0;
			ry[0]=0; ry[1]=1; ry[2] = 0;
			break;

		case 1:
			rx[0]=0; rx[1]=1; rx[2] = 0;
			ry[0]=0; ry[1]=0; ry[2] = -1;             
			break;

		case 2:
			rx[0]=1; rx[1]=0; rx[2] = 0;
			ry[0]=0; ry[1]=0; ry[2] = -1;           
			break;
		default:
			break;
		}*/

		para->SetXDirection(rx, false);
		para->SetYDirection(ry, false);
		para->SetCrossLine1(rx, false);
		para->SetCrossLine2(ry, false);
		para->SetCenterPoint(org/*pos*/, false);
		para->SetSpacing(space, false);
		para->UpdateMPR();
	
/*
		COLORREF colorx = RGB(0xff,0x0,0x0);
		COLORREF colory = RGB(0x00,0xff,0x0);
		COLORREF colorz = RGB(0x00,0x0,0xff);

		m_MPRSliceWnd[0]->SetLine1Color(colorx);
		m_MPRSliceWnd[0]->SetLine2Color(colory);
		m_MPRSliceWnd[0]->GetMPRPlanePara()->SetLine1CrossPlane(m_MPRSliceWnd[2]->GetMPRPlanePara());
		m_MPRSliceWnd[0]->GetMPRPlanePara()->SetLine2CrossPlane(m_MPRSliceWnd[1]->GetMPRPlanePara());

		m_MPRSliceWnd[1]->SetLine1Color(colory);
		m_MPRSliceWnd[1]->SetLine2Color(colorz);
		m_MPRSliceWnd[1]->GetMPRPlanePara()->SetLine1CrossPlane(m_MPRSliceWnd[0]->GetMPRPlanePara());
		m_MPRSliceWnd[1]->GetMPRPlanePara()->SetLine2CrossPlane(m_MPRSliceWnd[2]->GetMPRPlanePara());

		m_MPRSliceWnd[2]->SetLine1Color(colorx);
		m_MPRSliceWnd[2]->SetLine2Color(colorz);
		m_MPRSliceWnd[2]->GetMPRPlanePara()->SetLine1CrossPlane(m_MPRSliceWnd[0]->GetMPRPlanePara());
		m_MPRSliceWnd[2]->GetMPRPlanePara()->SetLine2CrossPlane(m_MPRSliceWnd[1]->GetMPRPlanePara());
*/
		
		//MPRPlanePtr para(new MPRPlane);
		/*double space[3] = { 0 };
		para->GetSpacing(space);
		space[2] = space[1];*/

		//通过reslice构建新的图像数据
		vtkSmartPointer<vtkImageReslice> tImageReslice = vtkSmartPointer<vtkImageReslice>::New();
		//tImageReslice->SetInput(pData);
		tImageReslice->SetInputData(pData);
		tImageReslice->SetOutputScalarType(
			isWord ?
			(isSigned ? VTK_SHORT : VTK_UNSIGNED_SHORT)
			: (isSigned ? VTK_SIGNED_CHAR : VTK_UNSIGNED_CHAR));

		tImageReslice->SetOutputSpacing(space);

		//多层模式
		/*switch (para->GetSlapMode())
		{
		case SLAP_MAX:
			
		case SLAP_MIN:
			tImageReslice->SetSlabModeToMin();
		case SLAP_MEAN:
			tImageReslice->SetSlabModeToMean();
		default:
			tImageReslice->SetSlabModeToMean();
			break;
		}*/
		tImageReslice->SetSlabModeToMax();
		tImageReslice->SetSlabNumberOfSlices(para->GetSlapSliceNum());


		//背景
		tImageReslice->SetBackgroundLevel(-1024);

		tImageReslice->SetOutputDimensionality(2);

		tImageReslice->SetInterpolationModeToCubic();

		vtkSmartPointer<vtkMatrix4x4> matrix = para->GetMPRMatrix();
		tImageReslice->SetResliceAxes(matrix);
		//tImageReslice->SetOutputOrigin(0,0,0);
		//tImageReslice->SetOutputExtent(0,para->GetPlaneCX()-1,0,para->GetPlaneCY()-1,0,0);
		tImageReslice->Update();

		double direction[6];
		double original[3];
		para->GetXDirection(&direction[0]);
		para->GetYDirection(&direction[3]);
		para->GetOriginal(original);
		tImageReslice->GetOutput();

		callback(tImageReslice->GetOutput(),
			original,
			direction);
	
		/*SliceImageDataPtr imgData(new SliceImageData);
		imgData->SetImageData();
		imgData->SetImageInformation(pDoc->m_SliceImageData->GetImageInfomation());
		imgData->SetImageDirection(direction);
		imgData->SetOriginal(original);*/
}

void OnUpdate(vtkImageData *pData,
	double space[3],
	double pos[3],
	double org[3],
	double xDirection[3],
	double yDirection[3],
	//double zDirection[3],
	int   nDim[3])
{
	//CMPRTestDoc *pDoc = GetDocument();
	//if(pDoc->m_SliceImageData != nullptr)
	{
		//m_pTranFunDlg->InitList(pDoc->m_TranFunctions);

		/*double space[3] = {0};
		double pos[3];
		double org[3];
		double xDirection[3];
		double yDirection[3];
		int   nDim[3];*/
		double zDirection[3];

		//pDoc->m_SliceImageData->GetOriginal(org);
		//pDoc->m_SliceImageData->GetImageXDirection(xDirection);
		//pDoc->m_SliceImageData->GetImageYDirection(yDirection);
		vtkMath::Cross(xDirection, yDirection, zDirection);

		zDirection[0] = -zDirection[0];
		zDirection[1] = -zDirection[1];
		zDirection[2] = -zDirection[2];

		//pDoc->m_SliceImageData->GetImageData()->GetSpacing(space);
		//pDoc->m_SliceImageData->GetImageData()->GetCenter(pos);
		//pDoc->m_SliceImageData->GetImageData()->GetDimensions(nDim);

		space[2] = space[0];
		pos[0] = org[0] + nDim[0] * space[0] * xDirection[0] / 2 + nDim[1] * space[1] * xDirection[1] / 2 + nDim[2] * space[2] * xDirection[2] / 2;
		pos[1] = org[1] + nDim[0] * space[0] * yDirection[0] / 2 + nDim[1] * space[1] * yDirection[1] / 2 + nDim[2] * space[2] * yDirection[2] / 2;
		pos[2] = org[2] + nDim[0] * space[0] * zDirection[0] / 2 + nDim[1] * space[1] * zDirection[1] / 2 + nDim[2] * space[2] * zDirection[2] / 2;

		BoxIntersectionProcessPtr VolumeBox(new BoxIntersectionProcess);
		VolumeBox->InitVolume(org, xDirection, yDirection, zDirection, space, nDim[0], nDim[1], nDim[2]);

		for (int i = 0; i < 3; i++)
		{
			MPRPlanePtr para(new MPRPlane);
			para->SetBoxIntersectionProcess(VolumeBox);
			double rx[3], ry[3];
			switch (i)
			{
			case 0:
				rx[0] = 1; rx[1] = 0; rx[2] = 0;
				ry[0] = 0; ry[1] = 1; ry[2] = 0;
				break;

			case 1:
				rx[0] = 0; rx[1] = 1; rx[2] = 0;
				ry[0] = 0; ry[1] = 0; ry[2] = -1;
				break;

			case 2:
				rx[0] = 1; rx[1] = 0; rx[2] = 0;
				ry[0] = 0; ry[1] = 0; ry[2] = -1;
				break;
			default:
				break;
			}

			para->SetXDirection(rx, false);
			para->SetYDirection(ry, false);
			para->SetCrossLine1(rx, false);
			para->SetCrossLine2(ry, false);
			para->SetCenterPoint(pos, false);
			para->SetSpacing(space, false);
			para->UpdateMPR();

			//para->SetPlaneWnd(m_MPRSliceWnd[i]);
			//m_MPRSliceWnd[i]->SetMPRPlanePara(para);
			///m_MPRSliceWnd[i]->BindUpdateMPRImageEvent(std::tr1::bind( &CMPRTestView::UpdateMPRImage, this, std::placeholders::_1, FALSE));
		}
		/*
				COLORREF colorx = RGB(0xff,0x0,0x0);
				COLORREF colory = RGB(0x00,0xff,0x0);
				COLORREF colorz = RGB(0x00,0x0,0xff);

				m_MPRSliceWnd[0]->SetLine1Color(colorx);
				m_MPRSliceWnd[0]->SetLine2Color(colory);
				m_MPRSliceWnd[0]->GetMPRPlanePara()->SetLine1CrossPlane(m_MPRSliceWnd[2]->GetMPRPlanePara());
				m_MPRSliceWnd[0]->GetMPRPlanePara()->SetLine2CrossPlane(m_MPRSliceWnd[1]->GetMPRPlanePara());

				m_MPRSliceWnd[1]->SetLine1Color(colory);
				m_MPRSliceWnd[1]->SetLine2Color(colorz);
				m_MPRSliceWnd[1]->GetMPRPlanePara()->SetLine1CrossPlane(m_MPRSliceWnd[0]->GetMPRPlanePara());
				m_MPRSliceWnd[1]->GetMPRPlanePara()->SetLine2CrossPlane(m_MPRSliceWnd[2]->GetMPRPlanePara());

				m_MPRSliceWnd[2]->SetLine1Color(colorx);
				m_MPRSliceWnd[2]->SetLine2Color(colorz);
				m_MPRSliceWnd[2]->GetMPRPlanePara()->SetLine1CrossPlane(m_MPRSliceWnd[0]->GetMPRPlanePara());
				m_MPRSliceWnd[2]->GetMPRPlanePara()->SetLine2CrossPlane(m_MPRSliceWnd[1]->GetMPRPlanePara());
		*/
		for (int i = 0; i < 3; i++)
		{
			MPRPlanePtr para(new MPRPlane);
			double space[3] = { 0 };
			para->GetSpacing(space);
			space[2] = space[1];

			//通过reslice构建新的图像数据
			vtkSmartPointer<vtkImageReslice> tImageReslice = vtkSmartPointer<vtkImageReslice>::New();
			//tImageReslice->SetInput(pDoc->m_SliceImageData->GetImageData());
			tImageReslice->SetOutputSpacing(space);

			//多层模式
			/*switch (para->GetSlapMode())
			{
			case SLAP_MAX:
				tImageReslice->SetSlabModeToMax();
			case SLAP_MIN:
				tImageReslice->SetSlabModeToMin();
			case SLAP_MEAN:
				tImageReslice->SetSlabModeToMean();
			default:
				tImageReslice->SetSlabModeToMean();
				break;
			}*/
			tImageReslice->SetSlabNumberOfSlices(para->GetSlapSliceNum());


			//背景
			tImageReslice->SetBackgroundLevel(-1024);

			tImageReslice->SetOutputDimensionality(2);

			tImageReslice->SetInterpolationModeToLinear();

			vtkSmartPointer<vtkMatrix4x4> matrix = para->GetMPRMatrix();
			tImageReslice->SetResliceAxes(matrix);
			tImageReslice->SetOutputOrigin(0, 0, 0);
			tImageReslice->SetOutputExtent(0, para->GetPlaneCX() - 1, 0, para->GetPlaneCY() - 1, 0, 0);
			tImageReslice->Update();

			double direction[6];
			double original[3];
			para->GetXDirection(&direction[0]);
			para->GetYDirection(&direction[3]);
			para->GetOriginal(original);
			tImageReslice->GetOutput();
			/*SliceImageDataPtr imgData(new SliceImageData);
			imgData->SetImageData();
			imgData->SetImageInformation(pDoc->m_SliceImageData->GetImageInfomation());
			imgData->SetImageDirection(direction);
			imgData->SetOriginal(original);*/
		}
	}




}

