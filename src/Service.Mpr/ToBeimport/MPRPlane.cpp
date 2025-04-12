#include "pch.h"
#include "MPRPlane.h"

#include <vtk-9.0/vtkMath.h>

#include "BoxIntersectionProcess.h"

MPRPlane::MPRPlane(void)
{
	m_CX = 512;
	m_CY = 512;

	m_X[0] = 1;
	m_X[1] = 0;
	m_X[2] = 0;

	m_Y[0] = 0;
	m_Y[1] = 0;
	m_Y[2] = 1;

	m_CrossPoint[0] = 256;
	m_CrossPoint[1] = 256;
	m_CrossPoint[2] = 256;

	m_CrossLine1[0] = 1;
	m_CrossLine1[1] = 0;
	m_CrossLine1[2] = 0;

	m_CrossLine2[0] = 0;
	m_CrossLine2[1] = 1;
	m_CrossLine2[2] = 0;

	m_dSpace[0] = 1.0;
	m_dSpace[1] = 1.0;

	m_nSlapSliceNumber = 1;
	m_SlapMode = SLAP_MEAN;

	UpdateMPR();
}

MPRPlane::MPRPlane(double X[3], double Y[3], 
				   double CrossLine1[3],
				   double CrossLine2[3], 
				   double CrossPoint[3], 
				   double dSpace[2])
{
	m_CX = 512;
	m_CY = 512;

	m_X[0] = X[0];
	m_X[1] = X[1];
	m_X[2] = X[2];

	m_Y[0] = Y[0];
	m_Y[1] = Y[1];
	m_Y[2] = Y[2];

	m_CrossPoint[0] = CrossPoint[0];
	m_CrossPoint[1] = CrossPoint[1];
	m_CrossPoint[2] = CrossPoint[2];

	m_CrossLine1[0] = CrossLine1[0];
	m_CrossLine1[1] = CrossLine1[1];
	m_CrossLine1[2] = CrossLine1[2];

	m_CrossLine2[0] = CrossLine2[0];
	m_CrossLine2[1] = CrossLine2[1];
	m_CrossLine2[2] = CrossLine2[2];

	m_dSpace[0] = dSpace[0];
	m_dSpace[1] = dSpace[1];

	UpdateMPR();
}


MPRPlane::~MPRPlane(void)
{

}

void MPRPlane::SetBoxIntersectionProcess(BoxIntersectionProcessPtr boxProcess)
{
	m_VolumeBox = boxProcess;
}

int MPRPlane::GetPlaneCX()
{
	return m_CX;
}

int MPRPlane::GetPlaneCY()
{
	return m_CY;
}

void MPRPlane::GetLine1Pts(double pt1[3], double pt2[3])
{
	pt1[0] = m_CrossLine1Pt1[0];
	pt1[1] = m_CrossLine1Pt1[1];
	pt1[2] = m_CrossLine1Pt1[2];

	pt2[0] = m_CrossLine1Pt2[0];
	pt2[1] = m_CrossLine1Pt2[1];
	pt2[2] = m_CrossLine1Pt2[2];
}


void MPRPlane::GetLine2Pts(double pt1[3], double pt2[3])
{
	pt1[0] = m_CrossLine2Pt1[0];
	pt1[1] = m_CrossLine2Pt1[1];
	pt1[2] = m_CrossLine2Pt1[2];

	pt2[0] = m_CrossLine2Pt2[0];
	pt2[1] = m_CrossLine2Pt2[1];
	pt2[2] = m_CrossLine2Pt2[2];
}

void MPRPlane::GetPlaneCornerPts(double pt1[3], double pt2[3], double pt3[3], double pt4[3])
{
	pt1[0] = m_A[0];
	pt1[1] = m_A[1];
	pt1[2] = m_A[2];

	pt2[0] = m_B[0];
	pt2[1] = m_B[1];
	pt2[2] = m_B[2];

	pt3[0] = m_C[0];
	pt3[1] = m_C[1];
	pt3[2] = m_C[2];

	pt4[0] = m_D[0];
	pt4[1] = m_D[1];
	pt4[2] = m_D[2];
}

void MPRPlane::GetPlaneOriginal(double pt[3])
{
	pt[0] = m_A[0];
	pt[1] = m_A[1];
	pt[2] = m_A[2];
}

void MPRPlane::SetCenterPoint(double dCenter[3], bool bUpdate)
{
	if(m_VolumeBox!=nullptr)
	{
		if(m_VolumeBox->PtInVolume(dCenter))
		{
			m_CrossPoint[0] = dCenter[0];
			m_CrossPoint[1] = dCenter[1];
			m_CrossPoint[2] = dCenter[2];
			if(bUpdate)
			{
				UpdateMPR();
			}
		}
	}
	else
	{
		m_CrossPoint[0] = dCenter[0];
		m_CrossPoint[1] = dCenter[1];
		m_CrossPoint[2] = dCenter[2];
		if(bUpdate)
		{
			UpdateMPR();
		}
	}	
}

void MPRPlane::SetCrossLine1(double dCrossLine[3], bool bUpdate)
{
	m_CrossLine1[0] = dCrossLine[0];
	m_CrossLine1[1] = dCrossLine[1];
	m_CrossLine1[2] = dCrossLine[2];
	if(bUpdate)
	{
		UpdateMPR();
	}
}

void MPRPlane::SetCrossLine2(double dCrossLine[3], bool bUpdate)
{
	m_CrossLine2[0] = dCrossLine[0];
	m_CrossLine2[1] = dCrossLine[1];
	m_CrossLine2[2] = dCrossLine[2];
	if(bUpdate)
	{
		UpdateMPR();
	}
}

void MPRPlane::SetXDirection(double X[3], bool bUpdate)
{
	m_X[0] = X[0];
	m_X[1] = X[1];
	m_X[2] = X[2];

	if(bUpdate)
	{
		UpdateMPR();
	}
}

void MPRPlane::SetYDirection(double Y[3], bool bUpdate)
{
	m_Y[0] = Y[0];
	m_Y[1] = Y[1];
	m_Y[2] = Y[2];

	if(bUpdate)
	{
		UpdateMPR();
	}
}

void MPRPlane::SetSpacing(double dSpace[2], bool bUpdate)
{
	m_dSpace[0] = dSpace[0];
	m_dSpace[1] = dSpace[1];

	if(bUpdate)
	{
		UpdateMPR();
	}
}

void MPRPlane::GetCenterPoint(double dCenter[3])
{
	dCenter[0] = m_CrossPoint[0];
	dCenter[1] = m_CrossPoint[1];
	dCenter[2] = m_CrossPoint[2];
}

void MPRPlane::GetCrossLine1(double dCrossLine1[3])
{
	dCrossLine1[0] = m_CrossLine1[0];
	dCrossLine1[1] = m_CrossLine1[1];
	dCrossLine1[2] = m_CrossLine1[2];
}

void MPRPlane::GetCrossLine2(double dCrossLine2[3])
{
	dCrossLine2[0] = m_CrossLine2[0];
	dCrossLine2[1] = m_CrossLine2[1];
	dCrossLine2[2] = m_CrossLine2[2];
}

void MPRPlane::GetXDirection(double X[3])
{
	X[0] = m_X[0];
	X[1] = m_X[1];
	X[2] = m_X[2];
}

void MPRPlane::GetYDirection(double Y[3])
{
	Y[0] = m_Y[0];
	Y[1] = m_Y[1];
	Y[2] = m_Y[2];
}

void MPRPlane::GetNormal(double dNormal[3])
{
	dNormal[0] = m_Normal[0];
	dNormal[1] = m_Normal[1];
	dNormal[2] = m_Normal[2];
}


void MPRPlane::GetSpacing(double dSpace[2])
{
	dSpace[0] = m_dSpace[0];
	dSpace[1] = m_dSpace[1];
}


bool MPRPlane::IsVolumeBoxValid()
{
	return m_VolumeBox != nullptr;
}

void MPRPlane::UpdateMPR()
{
	//计算法向量
	vtkMath::Normalize(m_X);
	vtkMath::Normalize(m_Y);
	vtkMath::Cross(m_X, m_Y, m_Normal);

	UpdateCrossLine();
	vtkMath::Normalize(m_CrossLine1);
	vtkMath::Normalize(m_CrossLine2);

	//计算交点;
	if(m_VolumeBox != nullptr)
	{
		std::vector<Vector3d> InterPoints;
		Plane temp = m_VolumeBox->CalPlane(m_Normal, m_CrossPoint);
		m_VolumeBox->IntersectPlane(temp, InterPoints);
		if(InterPoints.size()<3)
		{
			return;
		}

		m_matrix = vtkSmartPointer<vtkMatrix4x4>::New();
		m_matrix->SetElement(0,0,m_X[0]) ;
		m_matrix->SetElement(1,0,m_X[1]) ;
		m_matrix->SetElement(2,0,m_X[2]) ;
		m_matrix->SetElement(3,0,0);
		m_matrix->SetElement(0,1,m_Y[0]) ;
		m_matrix->SetElement(1,1,m_Y[1]) ;
		m_matrix->SetElement(2,1,m_Y[2]) ;
		m_matrix->SetElement(3,1,0);
		m_matrix->SetElement(0,2,m_Normal[0]) ;
		m_matrix->SetElement(1,2,m_Normal[1]) ;
		m_matrix->SetElement(2,2,m_Normal[2]) ;
		m_matrix->SetElement(3,2,0);

		m_matrix->SetElement(0,3,m_CrossPoint[0]) ;   
		m_matrix->SetElement(1,3,m_CrossPoint[1]) ;
		m_matrix->SetElement(2,3,m_CrossPoint[2]) ;
		m_matrix->SetElement(3,3,1);

		TRACE("\nReslice Matrix - [%.03lf, %.03lf, %.03lf]\n",
			m_CrossPoint[0], m_CrossPoint[1], m_CrossPoint[2]);

		vtkSmartPointer<vtkMatrix4x4> invmatrix = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkMatrix4x4::Invert(m_matrix, invmatrix);
		double minPos[2];
		double maxPos[2];
		double tempIn[4];
		double tempOut[4];
		bool bFirst=true;
		for(auto point:InterPoints)
		{
			tempIn[0] = point.x;
			tempIn[1] = point.y;
			tempIn[2] = point.z;
			tempIn[3] = 1;
			invmatrix->MultiplyPoint(tempIn, tempOut);        //平面坐标系到空间坐标系
			if(bFirst)
			{
				minPos[0] = tempOut[0];
				minPos[1] = tempOut[1];
				maxPos[0] = tempOut[0];
				maxPos[1] = tempOut[1];
				bFirst = false;
			}
			else
			{
				minPos[0] = std::min(minPos[0], tempOut[0]);
				minPos[1] = std::min(minPos[1], tempOut[1]);
				maxPos[0] = std::max(maxPos[0], tempOut[0]);
				maxPos[1] = std::max(maxPos[1], tempOut[1]);
			}
		}

		m_CX = (maxPos[0]-minPos[0])/m_dSpace[0]+1;
		m_CY = (maxPos[1]-minPos[1])/m_dSpace[1]+1;

		std::vector<Vector3d> InsertPts;
		m_VolumeBox->IntersectLineWithBox(m_CrossPoint, m_CrossLine1, InsertPts);
		if(InsertPts.size() != 2)
		{
			//计算交线的顶点坐标, 目前采用交叉点两边各延生多少的方式
			m_CrossLine1Pt1[0] = m_CrossPoint[0]+m_CrossLine1[0]*m_CX*m_dSpace[0]/2;
			m_CrossLine1Pt1[1] = m_CrossPoint[1]+m_CrossLine1[1]*m_CX*m_dSpace[0]/2;
			m_CrossLine1Pt1[2] = m_CrossPoint[2]+m_CrossLine1[2]*m_CX*m_dSpace[0]/2;

			m_CrossLine1Pt2[0] = m_CrossPoint[0]-m_CrossLine1[0]*m_CX*m_dSpace[0]/2;
			m_CrossLine1Pt2[1] = m_CrossPoint[1]-m_CrossLine1[1]*m_CX*m_dSpace[0]/2;
			m_CrossLine1Pt2[2] = m_CrossPoint[2]-m_CrossLine1[2]*m_CX*m_dSpace[0]/2;
		}
		else
		{
			m_CrossLine1Pt1[0] = InsertPts[0].x;
			m_CrossLine1Pt1[1] = InsertPts[0].y;
			m_CrossLine1Pt1[2] = InsertPts[0].z;

			m_CrossLine1Pt2[0] = InsertPts[1].x;
			m_CrossLine1Pt2[1] = InsertPts[1].y;
			m_CrossLine1Pt2[2] = InsertPts[1].z;
		}

		InsertPts.clear();
		m_VolumeBox->IntersectLineWithBox(m_CrossPoint, m_CrossLine2, InsertPts);
		if(InsertPts.size() != 2)
		{
			m_CrossLine2Pt1[0] = m_CrossPoint[0]+m_CrossLine2[0]*m_CY*m_dSpace[1]/2;
			m_CrossLine2Pt1[1] = m_CrossPoint[1]+m_CrossLine2[1]*m_CY*m_dSpace[1]/2;
			m_CrossLine2Pt1[2] = m_CrossPoint[2]+m_CrossLine2[2]*m_CY*m_dSpace[1]/2;

			m_CrossLine2Pt2[0] = m_CrossPoint[0]-m_CrossLine2[0]*m_CY*m_dSpace[1]/2;
			m_CrossLine2Pt2[1] = m_CrossPoint[1]-m_CrossLine2[1]*m_CY*m_dSpace[1]/2;
			m_CrossLine2Pt2[2] = m_CrossPoint[2]-m_CrossLine2[2]*m_CY*m_dSpace[1]/2;
		}
		else
		{
			m_CrossLine2Pt1[0] = InsertPts[0].x;
			m_CrossLine2Pt1[1] = InsertPts[0].y;
			m_CrossLine2Pt1[2] = InsertPts[0].z;

			m_CrossLine2Pt2[0] = InsertPts[1].x;
			m_CrossLine2Pt2[1] = InsertPts[1].y;
			m_CrossLine2Pt2[2] = InsertPts[1].z;
		}

		double p1[4]={minPos[0], minPos[1],  0, 1};
		double p2[4]={maxPos[0], minPos[1],  0, 1};
		double p3[4]={maxPos[0], maxPos[1],  0, 1};
		double p4[4]={minPos[0], maxPos[1],  0, 1};
		double newp1[4];
		double newp2[4];
		double newp3[4];
		double newp4[4];

		m_matrix->MultiplyPoint(p1, newp1);        
		m_matrix->MultiplyPoint(p2, newp2);
		m_matrix->MultiplyPoint(p3, newp3);
		m_matrix->MultiplyPoint(p4, newp4);

		//m_matrix->SetElement(0,3,newp1[0]) ;       //重新设置原点
		//m_matrix->SetElement(1,3,newp1[1]) ;
		//m_matrix->SetElement(2,3,newp1[2]) ;
		//m_matrix->SetElement(3,3,1.0);


		m_A[0] = newp1[0];
		m_A[1] = newp1[1];
		m_A[2] = newp1[2];

		m_B[0] = newp2[0];
		m_B[1] = newp2[1];
		m_B[2] = newp2[2];

		m_C[0] = newp3[0];
		m_C[1] = newp3[1];
		m_C[2] = newp3[2];

		m_D[0] = newp4[0];
		m_D[1] = newp4[1];
		m_D[2] = newp4[2];
	}
	return;
}


vtkSmartPointer<vtkMatrix4x4> MPRPlane::GetMPRMatrix()
{
	return m_matrix;
}

//向量围绕轴旋转angel角度
void MPRPlane::Rotal(double tVector[3], double RotalAxial[3], double angel)
{
	double Normal[3];

	Normal[0] = RotalAxial[0];
	Normal[1] = RotalAxial[1];
	Normal[2] = RotalAxial[2];

	double c = cos(angel);
	double s = sin(angel);

	vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
	matrix->SetElement(0,0,c+(1-c)*Normal[0]*Normal[0]);
	matrix->SetElement(1,0,(1-c)*Normal[0]*Normal[1]+s*Normal[2]) ;
	matrix->SetElement(2,0,(1-c)*Normal[2]*Normal[0]-s*Normal[1]) ;
	matrix->SetElement(3,0,0);
	matrix->SetElement(0,1,(1-c)*Normal[1]*Normal[0]-s*Normal[2]) ;
	matrix->SetElement(1,1,c+(1-c)*Normal[1]*Normal[1]) ;
	matrix->SetElement(2,1,(1-c)*Normal[1]*Normal[2]+s*Normal[0]) ;
	matrix->SetElement(3,1,0);
	matrix->SetElement(0,2,(1-c)*Normal[2]*Normal[0]+s*Normal[1]) ;
	matrix->SetElement(1,2,(1-c)*Normal[2]*Normal[1]-s*Normal[0]) ;
	matrix->SetElement(2,2,c+(1-c)*Normal[2]*Normal[2]) ;
	matrix->SetElement(3,2,0) ;
	matrix->SetElement(0,3,0) ;
	matrix->SetElement(1,3,0) ;
	matrix->SetElement(2,3,0) ;
	matrix->SetElement(3,3,1);

	double oldTempVector1[4];
	double oldTempVector2[4];
	double newTempVector1[4];
	double newTempVector2[4];
	oldTempVector1[0]=m_CrossPoint[0];
	oldTempVector1[1]=m_CrossPoint[1];
	oldTempVector1[2]=m_CrossPoint[2];
	oldTempVector1[3]=0;

	oldTempVector2[0]=m_CrossPoint[0]+100*tVector[0];
	oldTempVector2[1]=m_CrossPoint[1]+100*tVector[1];
	oldTempVector2[2]=m_CrossPoint[2]+100*tVector[2];
	oldTempVector2[3]=0;

	matrix->MultiplyPoint(oldTempVector1, newTempVector1);
	matrix->MultiplyPoint(oldTempVector2, newTempVector2);

	tVector[0] = newTempVector2[0] - newTempVector1[0];
	tVector[1] = newTempVector2[1] - newTempVector1[1];
	tVector[2] = newTempVector2[2] - newTempVector1[2];

	vtkMath::Normalize(tVector);
}

void MPRPlane::GetOriginal(double dOriginal[3])
{
	dOriginal[0] = m_A[0];
	dOriginal[1] = m_A[1];
	dOriginal[2] = m_A[2];
}

void MPRPlane::SetLine1CrossPlane(MPRPlanePtr LineCrossPlane)
{
	m_Line1CrossPlane = LineCrossPlane;
}

void MPRPlane::SetLine2CrossPlane(MPRPlanePtr LineCrossPlane)
{
	m_Line2CrossPlane = LineCrossPlane;
}

MPRPlanePtr MPRPlane::GetLine1CrossPlane()
{
	return m_Line1CrossPlane;
}

MPRPlanePtr MPRPlane::GetLine2CrossPlane()
{
	return m_Line2CrossPlane;
}

void MPRPlane::UpdateCrossLine()
{
	if(m_Line1CrossPlane!=nullptr)
	{
		double v1[3];
		m_Line1CrossPlane->GetNormal(v1);
		vtkMath::Cross(m_Normal, v1, m_CrossLine1);
	}

	if(m_Line2CrossPlane!=nullptr)
	{
		double v2[3];
		m_Line2CrossPlane->GetNormal(v2);
		vtkMath::Cross(m_Normal, v2, m_CrossLine2);
	}
}


void MPRPlane::RotalLine1Plane(double degree)
{
	if(m_Line1CrossPlane!=nullptr)
	{
		m_Line1CrossPlane->RotalAllVector(m_Normal, degree);
	}
}

void MPRPlane::RotalLine2Plane(double degree)
{
	if(m_Line2CrossPlane!=nullptr)
	{
		m_Line2CrossPlane->RotalAllVector(m_Normal, degree);
	}
}

//平面上根据向量进行选择degree角度，保持交点不变
void MPRPlane::RotalAllVector(double RotalAxial[3], double degree)
{
	Rotal(m_X, RotalAxial, degree);
	Rotal(m_Y, RotalAxial, degree);
}


void MPRPlane::SetSlapThickness(double dThickness)
{
	m_nSlapSliceNumber = std::max(1.0,dThickness/m_dSpace[2]+1);
}

double MPRPlane::GetSlapThickness()
{
	return (m_nSlapSliceNumber-1)*m_dSpace[2];
}

void MPRPlane::SetSlapSliceNum(int nSliceNum)
{
	m_nSlapSliceNumber = nSliceNum;
}

int  MPRPlane::GetSlapSliceNum()
{
	return m_nSlapSliceNumber;
}


void MPRPlane::SetSlapMode(SlapSliceMergerMode nMode)
{
	m_SlapMode = nMode;
}

SlapSliceMergerMode MPRPlane::GetSlapMode()
{
	return m_SlapMode;
}


//根据新的中心点和crossline1方向，计算该直线和volume的交点
bool MPRPlane::GetNewCrossline1Pts(double newCrossPoint[3], double pt1[3], double pt2[3])
{
	std::vector<Vector3d> InsertPts;
	m_VolumeBox->IntersectLineWithBox(newCrossPoint, m_CrossLine1, InsertPts);
	if(InsertPts.size() == 2)
	{
		pt1[0] = InsertPts[0].x;
		pt1[1] = InsertPts[0].y;
		pt1[2] = InsertPts[0].z;

		pt2[0] = InsertPts[1].x;
		pt2[1] = InsertPts[1].y;
		pt2[2] = InsertPts[1].z;
		return true;
	}
	return false;
}

bool MPRPlane::GetNewCrossline2Pts(double newCrossPoint[3], double pt1[3], double pt2[3])
{
	std::vector<Vector3d> InsertPts;
	m_VolumeBox->IntersectLineWithBox(newCrossPoint, m_CrossLine2, InsertPts);
	if(InsertPts.size() == 2)
	{
		pt1[0] = InsertPts[0].x;
		pt1[1] = InsertPts[0].y;
		pt1[2] = InsertPts[0].z;

		pt2[0] = InsertPts[1].x;
		pt2[1] = InsertPts[1].y;
		pt2[2] = InsertPts[1].z;
		return true;
	}

	return false;
}


void MPRPlane::SetPlaneWnd(CMPRSliceWndPtr wnd)
{
	m_PlaneWnd = wnd;
}

void MPRPlane::UpdatePlaneWnd(bool bNeedUpdateImage)
{
	/*if(bNeedUpdateImage)
	{
		m_PlaneWnd->ResliceWndImage();
	}
	else
	{
		m_PlaneWnd->RedrawWindow(NULL);
	}*/
}
