#pragma once
class CPoint;

class LineMix
{
public:
	LineMix(void);
	virtual ~LineMix(void);

	static bool Meet(const CPoint& p1,const CPoint& p2,const CPoint& p3,const CPoint& p4)  ;
	static CPoint Inter(const CPoint& p1,const CPoint& p2,const CPoint& p3,const CPoint& p4)  ;
};

