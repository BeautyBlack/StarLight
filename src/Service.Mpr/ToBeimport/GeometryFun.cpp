#include "pch.h"
#include "GeometryFun.h"

#include <atltypes.h>
#include <math.h>


GeometryFun::GeometryFun(void)
{
}


GeometryFun::~GeometryFun(void)
{
}

double GeometryFun::VectorCross(double p1[2], double p2[2])
{
	return 	(p1[0] * p2[1] - p1[1] * p2[0]);
}

// 计算两点间长度
double GeometryFun::CalLength(double p1[2], double p2[2])
{
	return sqrt((p1[0] - p2[0]) * (p1[0]- p2[0]) + (p1[1] - p2[1]) * (p1[1] - p2[1])) ;
}

// 计算顶点到直线的距离
double GeometryFun::CalDisPointToLine(double tempPoint[2], double beginPoint[2], double endPoint[2])
{
	double cross, length;
	double p1[2], p2[2];
	p1[0] = tempPoint[0] - beginPoint[0];
	p1[1] = tempPoint[1] - beginPoint[1];

	p2[0] = tempPoint[0] - endPoint[0];
	p2[1] = tempPoint[1] - endPoint[1];

	cross = VectorCross(p1, p2);
	length = CalLength(beginPoint, endPoint);

	if(length < 1E-6)
		return 1E-6;
	else
		return fabs(cross/length);
}

// 计算顶点到线段的距离
double GeometryFun::CalDisPointToInnerLine(double tempPoint[2], double beginPoint[2], double endPoint[2])
{
	double a, b, c1, c2;
	a = beginPoint[0] - endPoint[0];
	b = beginPoint[1] - endPoint[1];
	c1 = beginPoint[0]*endPoint[0] + beginPoint[1]*endPoint[1];
	c2 = pow(endPoint[0],2) + pow(endPoint[1],2) - c1;
	c1 = c1 - pow(beginPoint[0],2) - pow(beginPoint[1],2);

	double d1 = tempPoint[0]*a+tempPoint[1]*b+c1;
	double d2 = endPoint[0]*a+endPoint[1]*b+c1;
	if(d1*d2 < 0)
		return CalLength(tempPoint, beginPoint);

	d1 = d1 - c1 + c2;
	d2 = beginPoint[0]*a+beginPoint[1]*b+c2;
	if(d1*d2 < 0)
		return CalLength(tempPoint, endPoint);

	return CalDisPointToLine(tempPoint, beginPoint, endPoint);
}


double GeometryFun::CalLength(CPoint pt1, CPoint pt2)
{
	return sqrt(double(pt1.x - pt2.x) * (pt1.x- pt2.x) + double(pt1.y - pt2.y) * (pt1.y - pt2.y)) ;
}

double GeometryFun::CalDisPointToLine(CPoint tempPoint, CPoint beginPoint, CPoint endPoint)
{
	double dt[2];
	double db[2];
	double de[2];

	dt[0] = tempPoint.x;
	dt[1] = tempPoint.y;

	db[0] = beginPoint.x;
	db[1] = beginPoint.y;

	de[0] = endPoint.x;
	de[1] = endPoint.y;

	return CalDisPointToLine(dt, db, de);
}


double GeometryFun::CalDisPointToInnerLine(CPoint tempPoint, CPoint beginPoint, CPoint endPoint)
{
	double dt[2];
	double db[2];
	double de[2];

	dt[0] = tempPoint.x;
	dt[1] = tempPoint.y;

	db[0] = beginPoint.x;
	db[1] = beginPoint.y;

	de[0] = endPoint.x;
	de[1] = endPoint.y;

	return CalDisPointToInnerLine(dt, db, de);
}




double GeometryFun::VectorDot(double p1[3], double p2[3])
{
	return p1[0]*p2[0]+p1[1]*p2[1]+p1[2]*p2[2];
}

//Pt1 and Vec 表示直线
//orgpt 待投影点
//newpt 

bool   GeometryFun::CalPointToLineProjection(double pt1[3], double vec[3], double orgpt[3], double newpt[3])
{
	double vec2[3];
	vec2[0] = orgpt[0]-pt1[0];
	vec2[1] = orgpt[1]-pt1[1];
	vec2[2] = orgpt[2]-pt1[2];

	double t = VectorDot(vec, vec2)/VectorDot(vec, vec);

	newpt[0] = pt1[0] + vec[0]*t;
	newpt[1] = pt1[1] + vec[1]*t;
	newpt[2] = pt1[2] + vec[2]*t;

	return true;
}
