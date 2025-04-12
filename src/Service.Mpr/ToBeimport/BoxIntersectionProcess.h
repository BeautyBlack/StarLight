#pragma once
#include <vector>

struct Vector3d
{
	double x,y,z;
};

struct VolumeInfo
{
	int  m_nCX;
	int  m_nCY;
	int  m_nCZ;

	double m_dOriginal[3];
	double m_dSpace[3];

	Vector3d m_AxialDirection[3];
	Vector3d m_BoxPoints[8];

	double m_dXRange[2];
	double m_dYRange[2];
	double m_dZRange[2];

public:
	bool PtInVolume(double pos[3]);
	bool PtInVolume(Vector3d pos);
};

struct Plane
{
	double A,B,C,D;
};


class BoxIntersectionProcess
{
public:
	BoxIntersectionProcess(void);
	virtual ~BoxIntersectionProcess(void);

	void InitVolume(double dOriginal[3], double dx[3], double dy[3], double dz[3], 
		double dspace[3], 
		int cx, int cy, int cz);
	
	//根据平面法向量和平面上的点，计算得到平面参数   ax+by+cz+d=0
	Plane CalPlane(double Normal[3], double Point[3]);

	//计算平面和Volume 12条边的交点坐标
	void IntersectPlane(Plane plane, std::vector<Vector3d> &InterPoints);

	bool IntersectPlaneLine(Plane plane,  Vector3d end0, Vector3d  end1, Vector3d *vIntersect, double *alpha);

	//计算box和任意条直线的交点
	bool IntersectLineWithBox(double pt[3], double vector[3], std::vector<Vector3d> &InterPoints);
	
	bool PtInVolume(double pt[3]);

protected:
	VolumeInfo m_VolmueBox;
};

