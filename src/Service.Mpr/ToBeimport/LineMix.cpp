#include "pch.h"
#include "LineMix.h"

#include <algorithm>
#include <math.h>

const double eps = 1e-6;  
const double Pi = acos(-1.0);  

LineMix::LineMix(void)
{
}


LineMix::~LineMix(void)
{
}


int sgn(double x)  
{  
	return x<-eps ? -1 : (x>eps);  
}  

double Cross(const CPoint& p1,const CPoint& p2,const CPoint& p3,const CPoint& p4)  
{  
	return (p2.x-p1.x)*(p4.y-p3.y) - (p2.y-p1.y)*(p4.x-p3.x);  
}  

double Area(const CPoint& p1,const CPoint& p2,const CPoint& p3)  
{  
	return Cross(p1,p2,p1,p3);  
}  

double fArea(const CPoint& p1,const CPoint& p2,const CPoint& p3)  
{  
	return fabs(Area(p1,p2,p3));  
}  


bool LineMix::Meet(const CPoint& p1,const CPoint& p2,const CPoint& p3,const CPoint& p4)  
{  
	return std::max(std::min(p1.x,p2.x), std::min(p3.x,p4.x)) <= std::min(std::max(p1.x,p2.x), std::max(p3.x,p4.x))  
		&& std::max(std::min(p1.y,p2.y), std::min(p3.y,p4.y)) <= std::min(std::max(p1.y,p2.y), std::max(p3.y,p4.y))  
		&& sgn(Cross(p3,p2,p3,p4) * Cross(p3,p4,p3,p1)) >= 0  
		&& sgn(Cross(p1,p4,p1,p2) * Cross(p1,p2,p1,p3)) >= 0;  
} 

CPoint LineMix::Inter(const CPoint& p1,const CPoint& p2,const CPoint& p3,const CPoint& p4)  
{  
	double k = fArea(p1,p2,p3) / fArea(p1,p2,p4);  
	return CPoint((p3.x + k*p4.x)/(1+k),(p3.y + k*p4.y)/(1+k));  
}  

