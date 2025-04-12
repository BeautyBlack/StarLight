#pragma once

#include <memory>
#include <vtk-9.0/vtkMatrix4x4.h>
#include <vtk-9.0/vtkSmartPointer.h>

class MPRPlane;
typedef std::shared_ptr<MPRPlane> MPRPlanePtr;

class BoxIntersectionProcess;
typedef std::shared_ptr<BoxIntersectionProcess> BoxIntersectionProcessPtr;

class CMPRSliceWnd;
typedef std::shared_ptr<CMPRSliceWnd> CMPRSliceWndPtr;


enum SlapSliceMergerMode{SLAP_MIN, SLAP_MAX, SLAP_MEAN};

class MPRPlane
{
public:
	MPRPlane(void);
	MPRPlane(double X[3], double Y[3], 	double CrossLine1[3], double CrossLine2[3], double CrossPoint[3],
		double dSpace[2]);

	virtual ~MPRPlane(void);

	void SetPlaneWnd(CMPRSliceWndPtr);

	void UpdatePlaneWnd(bool bNeedUpdateImage);

	void SetBoxIntersectionProcess(BoxIntersectionProcessPtr boxProcess);

	virtual void UpdateMPR();
	vtkSmartPointer<vtkMatrix4x4> GetMPRMatrix();

	virtual void RotalLine1Plane(double degree);
	virtual void RotalLine2Plane(double degree);

	virtual void RotalAllVector(double RotalAxial[3], double degree);
	virtual void GetOriginal(double dOriginal[3]);

	void SetCenterPoint(double dCenter[3], bool bUpdate=true);
	void SetCrossLine1(double dCrossLine1[3], bool bUpdate=true);
	void SetCrossLine2(double dCrossLine2[3], bool bUpdate=true);
	void SetXDirection(double X[3], bool bUpdate=true);
	void SetYDirection(double Y[3], bool bUpdate=true);
	void SetSpacing(double dSpace[2], bool bUpdate=true);

	void GetCenterPoint(double dCenter[3]);
	void GetCrossLine1(double dCrossLine1[3]);
	void GetCrossLine2(double dCrossLine2[3]);
	
	void GetLine1Pts(double pt1[3], double pt2[3]);
	void GetLine2Pts(double pt1[3], double pt2[3]);
	void GetPlaneCornerPts(double pt1[3], double pt2[3], double pt3[3], double pt4[3]);
	void GetPlaneOriginal(double pt[3]);

	void GetXDirection(double X[3]);
	void GetYDirection(double Y[3]);
	void GetNormal(double dNormal[3]);

	void GetSpacing(double dSpace[2]);

	int GetPlaneCX();
	int GetPlaneCY();

	void SetSlapThickness(double dThickness); 
	double GetSlapThickness();

	void SetSlapSliceNum(int nSliceNum);
	int  GetSlapSliceNum();

	void SetSlapMode(SlapSliceMergerMode nMode);
	SlapSliceMergerMode GetSlapMode();

	bool GetNewCrossline1Pts(double newCrossPoint[3], double pt1[3], double pt2[3]);
	bool GetNewCrossline2Pts(double newCrossPoint[3], double pt1[3], double pt2[3]);

	void SetLine1CrossPlane(MPRPlanePtr LineCrossPlane);
	void SetLine2CrossPlane(MPRPlanePtr LineCrossPlane);

	MPRPlanePtr GetLine1CrossPlane();
	MPRPlanePtr GetLine2CrossPlane();

	void UpdateCrossLine();

	bool IsVolumeBoxValid();

protected:
	void Rotal(double oldVector[3], double RotalAxial[3], double degree);

protected:
	double m_dSpace[2];
	int m_CX;
	int m_CY;

	//平面坐标轴
	double  m_X[3];
	double  m_Y[3];
	double  m_Normal[3];

	//与A平面的交线
	double  m_CrossLine1[3];
	double  m_CrossLine1Pt1[3];
	double  m_CrossLine1Pt2[3];

	//与B平面的交线
	double  m_CrossLine2[3];
	double  m_CrossLine2Pt1[3];
	double  m_CrossLine2Pt2[3];

	//三个平面的交点
	double  m_CrossPoint[3];

	//平面四个角的坐标，通过计算MPR平面与Volume的交，然后根据X，Y的方向，得到相应的坐标系
	double  m_A[3];    //对应MPR的实际原点坐标
	double  m_B[3];
	double  m_C[3];
	double  m_D[3];


	CMPRSliceWndPtr m_PlaneWnd;

	int     m_nSlapSliceNumber;
	SlapSliceMergerMode m_SlapMode;

	vtkSmartPointer<vtkMatrix4x4> m_matrix;

	BoxIntersectionProcessPtr m_VolumeBox;

	//CString m_strPlaneName;

	MPRPlanePtr m_Line1CrossPlane;
	MPRPlanePtr m_Line2CrossPlane;

};

