#pragma once


class GeometryFun
{
public:
	GeometryFun(void);
	~GeometryFun(void);

	static double VectorDot(double p1[3], double p2[3]);

	static double VectorCross(double p1[2], double p2[2]);
	static double CalLength(double p1[2], double p2[2]);
	static double CalDisPointToLine(double tempPoint[2], double beginPoint[2], double endPoint[2]);

	static double CalDisPointToInnerLine(double tempPoint[2], double beginPoint[2], double endPoint[2]);

	static double CalLength(CPoint pt1, CPoint pt2);
	static double CalDisPointToLine(CPoint tempPoint, CPoint beginPoint, CPoint endPoint);
	static double CalDisPointToInnerLine(CPoint tempPoint, CPoint beginPoint, CPoint endPoint);

	static bool   CalPointToLineProjection(double pt[3], double vec[3], double orgpt[3], double newpt[3]);


	static inline void scaleV3d(double s, double one[3])
	{
		one[0] *= s;
		one[1] *= s;
		one[2] *= s;
	}

	static inline void addV3d(double out[3], double inl[3], double inr[3])
	{
		out[0] = inl[0] + inr[0];
		out[1] = inl[1] + inr[1];
		out[2] = inl[2] + inr[2];
	}

	static inline double dotV3d(double one[3], double two[3])
	{
		return one[0]*two[0] + one[1]*two[1] + one[2]*two[2];
	}
};
