#include "pch.h"
#include "BoxIntersectionProcess.h"

#include <algorithm>

#include "GeometryFun.h"

bool VolumeInfo::PtInVolume(double pos[3])
{
	if(  (pos[0]>=(m_dXRange[0]-0.5) && pos[0]<=(m_dXRange[1]+0.5))
	  && (pos[1]>=(m_dYRange[0]-0.5) && pos[1]<=(m_dYRange[1]+0.5))
	  && (pos[2]>=(m_dZRange[0]-0.5) && pos[2]<=(m_dZRange[1]+0.5))  )
	{
		return true;
	}
	return false;
}

bool VolumeInfo::PtInVolume(Vector3d pos)
{
	if( (pos.x>=(m_dXRange[0]-0.5)) && (pos.x<=(m_dXRange[1]+0.5))
		&& (pos.y>=(m_dYRange[0]-0.5)) && (pos.y<=(m_dYRange[1]+0.5))
		&& (pos.z>=(m_dZRange[0]-0.5)) && (pos.z<=(m_dZRange[1]+0.5)) )
	{
		return true;
	}

	return false;
}


BoxIntersectionProcess::BoxIntersectionProcess(void)
{
}


BoxIntersectionProcess::~BoxIntersectionProcess(void)
{
}

//根据平面法向量和平面上的点，计算得到平面参数   ax+by+cz+d=0
Plane BoxIntersectionProcess::CalPlane(double Normal[3], double Point[3])
{
	Plane temp;
	temp.A = Normal[0];
	temp.B = Normal[1];
	temp.C = Normal[2];
	temp.D = -(Normal[0]*Point[0]+Normal[1]*Point[1]+Normal[2]*Point[2]);
	return temp;
};


void BoxIntersectionProcess::InitVolume(double dOriginal[3], double dx[3], double dy[3], double dz[3], 
				double dspace[3], 
				int cx, int cy, int cz)
{
	m_VolmueBox.m_dOriginal[0] = dOriginal[0];
	m_VolmueBox.m_dOriginal[1] = dOriginal[1];
	m_VolmueBox.m_dOriginal[2] = dOriginal[2];


	m_VolmueBox.m_AxialDirection[0].x = dx[0];
	m_VolmueBox.m_AxialDirection[0].y = dx[1];
	m_VolmueBox.m_AxialDirection[0].z = dx[2];

	m_VolmueBox.m_AxialDirection[1].x = dy[0];
	m_VolmueBox.m_AxialDirection[1].y = dy[1];
	m_VolmueBox.m_AxialDirection[1].z = dy[2];

	m_VolmueBox.m_AxialDirection[2].x = dz[0];
	m_VolmueBox.m_AxialDirection[2].y = dz[1];
	m_VolmueBox.m_AxialDirection[2].z = dz[2];

	m_VolmueBox.m_dSpace[0] = dspace[0];
	m_VolmueBox.m_dSpace[1] = dspace[1];
	m_VolmueBox.m_dSpace[2] = dspace[2];

	m_VolmueBox.m_nCX = cx;
	m_VolmueBox.m_nCY = cy;
	m_VolmueBox.m_nCZ = cz;


	double dCX = m_VolmueBox.m_nCX*m_VolmueBox.m_dSpace[0];
	double dCY = m_VolmueBox.m_nCY*m_VolmueBox.m_dSpace[1];
	double dCZ = m_VolmueBox.m_nCZ*m_VolmueBox.m_dSpace[2];


	//计算8个顶点三维坐标
	m_VolmueBox.m_BoxPoints[0].x = dOriginal[0];
	m_VolmueBox.m_BoxPoints[0].y = dOriginal[1];
	m_VolmueBox.m_BoxPoints[0].z = dOriginal[2];

	m_VolmueBox.m_BoxPoints[1].x = dOriginal[0]+dCX*m_VolmueBox.m_AxialDirection[0].x;
	m_VolmueBox.m_BoxPoints[1].y = dOriginal[1]+dCX*m_VolmueBox.m_AxialDirection[0].y;
	m_VolmueBox.m_BoxPoints[1].z = dOriginal[2]+dCX*m_VolmueBox.m_AxialDirection[0].z;

	m_VolmueBox.m_BoxPoints[2].x = dOriginal[0]+dCX*m_VolmueBox.m_AxialDirection[0].x
											   +dCY*m_VolmueBox.m_AxialDirection[1].x;

	m_VolmueBox.m_BoxPoints[2].y = dOriginal[1]+dCX*m_VolmueBox.m_AxialDirection[0].y
											   +dCY*m_VolmueBox.m_AxialDirection[1].y;

	m_VolmueBox.m_BoxPoints[2].z = dOriginal[2]+dCX*m_VolmueBox.m_AxialDirection[0].z
											   +dCY*m_VolmueBox.m_AxialDirection[1].z;

	m_VolmueBox.m_BoxPoints[3].x = dOriginal[0]+dCY*m_VolmueBox.m_AxialDirection[1].x;
	m_VolmueBox.m_BoxPoints[3].y = dOriginal[1]+dCY*m_VolmueBox.m_AxialDirection[1].y;
	m_VolmueBox.m_BoxPoints[3].z = dOriginal[2]+dCY*m_VolmueBox.m_AxialDirection[1].z;

	double dZoffsetx = dCZ*m_VolmueBox.m_AxialDirection[2].x;
	double dZoffsety = dCZ*m_VolmueBox.m_AxialDirection[2].y;
	double dZoffsetz = dCZ*m_VolmueBox.m_AxialDirection[2].z;
	
	for(int i=0; i<4; i++)
	{
		m_VolmueBox.m_BoxPoints[4+i].x = m_VolmueBox.m_BoxPoints[i].x + dZoffsetx;
		m_VolmueBox.m_BoxPoints[4+i].y = m_VolmueBox.m_BoxPoints[i].y + dZoffsety;
		m_VolmueBox.m_BoxPoints[4+i].z = m_VolmueBox.m_BoxPoints[i].z + dZoffsetz;
	}

	m_VolmueBox.m_dXRange[0] = m_VolmueBox.m_BoxPoints[0].x;
	m_VolmueBox.m_dXRange[1] = m_VolmueBox.m_BoxPoints[0].x;

	m_VolmueBox.m_dYRange[0] = m_VolmueBox.m_BoxPoints[0].y;
	m_VolmueBox.m_dYRange[1] = m_VolmueBox.m_BoxPoints[0].y;
	
	m_VolmueBox.m_dZRange[0] = m_VolmueBox.m_BoxPoints[0].z;
	m_VolmueBox.m_dZRange[1] = m_VolmueBox.m_BoxPoints[0].z;

	for(int i=1; i<8; i++)
	{
		m_VolmueBox.m_dXRange[0] = std::min(m_VolmueBox.m_dXRange[0], m_VolmueBox.m_BoxPoints[i].x);
		m_VolmueBox.m_dXRange[1] = std::max(m_VolmueBox.m_dXRange[1], m_VolmueBox.m_BoxPoints[i].x);

		m_VolmueBox.m_dYRange[0] = std::min(m_VolmueBox.m_dYRange[0], m_VolmueBox.m_BoxPoints[i].y);
		m_VolmueBox.m_dYRange[1] = std::max(m_VolmueBox.m_dYRange[1], m_VolmueBox.m_BoxPoints[i].y);

		m_VolmueBox.m_dZRange[0] = std::min(m_VolmueBox.m_dZRange[0], m_VolmueBox.m_BoxPoints[i].z);
		m_VolmueBox.m_dZRange[1] = std::max(m_VolmueBox.m_dZRange[1], m_VolmueBox.m_BoxPoints[i].z);
	}
}

const int lineIndex[12][2]=
{
	{0,1},{1, 2}, {2, 3}, {3, 0}, {4,5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6},{3, 7} 
};

//计算平面和Volume 12条边的交点坐标
void BoxIntersectionProcess::IntersectPlane(Plane plane, std::vector<Vector3d> &InterPoints)
{
	Vector3d insertPoint;
	double alpha=0;
	for ( int i = 0; i < 12; i++)
	{
		if (IntersectPlaneLine(plane, m_VolmueBox.m_BoxPoints[lineIndex[i][0]],
			m_VolmueBox.m_BoxPoints[lineIndex[i][1]], &insertPoint, &alpha))
		{
			InterPoints.push_back(insertPoint);
		}
	}
}

bool BoxIntersectionProcess::IntersectPlaneLine(Plane plane,  Vector3d end0, Vector3d  end1, Vector3d *vIntersect, double *alpha)
{
	Vector3d N;
	N.x = plane.A; N.y = plane.B; N.z = plane.C;
	double fEnd0, fEnd1;
	fEnd0 = GeometryFun::dotV3d((double *)&end0, (double *)&N);
	fEnd1 = GeometryFun::dotV3d((double *)&end1, (double *)&N);
	* alpha = (fEnd1 + plane.D) / (fEnd1 - fEnd0);
	if ((*alpha >=0) && (*alpha <= 1))
	{
		Vector3d v0, v1;
		v0 = end0;					   
		v1 = end1;

		GeometryFun::scaleV3d(*alpha, (double *)&v0);
		GeometryFun::scaleV3d(1- *alpha, (double *)&v1);

		GeometryFun::addV3d((double *)vIntersect, (double *)&v0, (double *)&v1);

		return true;
	}
	return false;
}


//计算box和任意条直线的交点
bool BoxIntersectionProcess::IntersectLineWithBox(double pt[3], double vector[3], std::vector<Vector3d> &InterPoints)
{
	//计算六个面和该直线的交点，并且交点,交点范围必须符合要求

	Vector3d pt1;
	Vector3d pt2;

	double dMaxLeng =2000;// sqrt(m_VolmueBox.m_nCX*m_VolmueBox.m_nCX+m_VolmueBox.m_nCY*m_VolmueBox.m_nCY+
		//m_VolmueBox.m_nCZ*m_VolmueBox.m_nCZ);

	pt1.x = pt[0] - dMaxLeng*vector[0];
	pt1.y = pt[1] - dMaxLeng*vector[1];
	pt1.z = pt[2] - dMaxLeng*vector[2];

	pt2.x = pt[0] + dMaxLeng*vector[0];
	pt2.y = pt[1] + dMaxLeng*vector[1];
	pt2.z = pt[2] + dMaxLeng*vector[2];

	Plane panel[6];

	Vector3d insertPoint;
	double alpha=0;

	double direction[3];
	double original[3];

	//=========================================================
	direction[0] = m_VolmueBox.m_AxialDirection[0].x;
	direction[1] = m_VolmueBox.m_AxialDirection[0].y;
	direction[2] = m_VolmueBox.m_AxialDirection[0].z;

	original[0] = m_VolmueBox.m_BoxPoints[0].x;
	original[1] = m_VolmueBox.m_BoxPoints[0].y;
	original[2] = m_VolmueBox.m_BoxPoints[0].z;
	panel[0]=CalPlane(direction, original);

	original[0] = m_VolmueBox.m_BoxPoints[6].x;
	original[1] = m_VolmueBox.m_BoxPoints[6].y;
	original[2] = m_VolmueBox.m_BoxPoints[6].z;
	panel[1]=CalPlane(direction, original);


	//=============================================================
	direction[0] = m_VolmueBox.m_AxialDirection[1].x;
	direction[1] = m_VolmueBox.m_AxialDirection[1].y;
	direction[2] = m_VolmueBox.m_AxialDirection[1].z;

	original[0] = m_VolmueBox.m_BoxPoints[0].x;
	original[1] = m_VolmueBox.m_BoxPoints[0].y;
	original[2] = m_VolmueBox.m_BoxPoints[0].z;
	panel[2]=CalPlane(direction, original);

	original[0] = m_VolmueBox.m_BoxPoints[6].x;
	original[1] = m_VolmueBox.m_BoxPoints[6].y;
	original[2] = m_VolmueBox.m_BoxPoints[6].z;
	panel[3]=CalPlane(direction, original);

	//===================================================================
	direction[0] = m_VolmueBox.m_AxialDirection[2].x;
	direction[1] = m_VolmueBox.m_AxialDirection[2].y;
	direction[2] = m_VolmueBox.m_AxialDirection[2].z;

	original[0] = m_VolmueBox.m_BoxPoints[0].x;
	original[1] = m_VolmueBox.m_BoxPoints[0].y;
	original[2] = m_VolmueBox.m_BoxPoints[0].z;
	panel[4]=CalPlane(direction, original);

	original[0] = m_VolmueBox.m_BoxPoints[6].x;
	original[1] = m_VolmueBox.m_BoxPoints[6].y;
	original[2] = m_VolmueBox.m_BoxPoints[6].z;
	panel[5]=CalPlane(direction, original);

	for(int i=0; i<6; i++)
	{
		if (IntersectPlaneLine(panel[i], pt1, pt2, &insertPoint, &alpha))
		{
			if(m_VolmueBox.PtInVolume(insertPoint))
			{
				InterPoints.push_back(insertPoint);
			}
		}
	}

	return InterPoints.size()==2;
}


bool BoxIntersectionProcess::PtInVolume(double pt[3])
{
	return m_VolmueBox.PtInVolume(pt);
}
