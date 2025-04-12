#pragma once
#include <vector>
#include <string>
#include <windows.h>
#include "common.h"
using namespace std;
#ifdef DLL_PARONAMA_TEST
#define DLL_PARONAMA_API  _declspec(dllexport)
#else
#define  DLL_PARONAMA_API  _declspec(dllimport)
#endif

class DLL_PARONAMA_API  CProduceParonamaImage
{
public:

	 CProduceParonamaImage(void);
	 ~CProduceParonamaImage(void);
	 // npixel: the input image size in xy
	 //nslice: the number of input image slices
	 // image_data: the pointer of image data
	 bool set_image_value(int npixel, int nslice,short* image_data);
	 // nimg(panoramic images): the number of panoramic slices to generate
	 // img_thk: the thickness of each panoramic image
	 // img_dist: the distance between panoramic images in mm
	 void set_paronama_value(int nimg,float img_thk_pano, float img_dist_pano);
	 // img_thk_cs:the thickness of each cross-section image
	 // img_dist_cs :the distance between cross-section image in mm
	 // img_width: :the cross section image width in mm
	 void set_paronama_cs_value(float img_thk_cs, float img_dist_cs,float img_width);
	  // img_cen_x,img_cen_y the image center,normal set 0.0，
	 // fit_length: length of fitting,set 10.0
	 void set_paronama_pana(float img_cen_x, float img_cen_y,float fit_length);

	 // set parameters in normal
	 void initializePara();
	 // once set all parameters
	 void Set_Para(int nimg, int npixel, int nslice, float img_cen_x, float img_cen_y,float img_thk_pano, float img_dist_pano,float img_thk_cs, float img_dist_cs,float img_width,float fit_length);

	 // 获取全景图像数量nimg ，图像信息npixel和层数nslice
	 void get_value(int nimg, int npixel, int nslice);

	// 2维和3维插值函数，输入为：点的坐标和个数，输出为：拟合后点的坐标和个数
	 void interp_2d(float * pInx, float * pIny, int nInNum, float * pOutx, float * pOuty, int & nOutNum);
	 void interp_3d(float * pInX, float * pInY, float * pInZ, int nInNum, float * pOutX, float * pOutY, float * pOutZ, int & nOutNum);

	 // 等距离离散函数，对输入点进行等间距采样
	 // resample_step 等间距采样步长
	 void equal_step_resample(float * in_x, float * in_y, int n_in, float resample_step, float * out_x, float * out_y, int &n_out);
	 // 等距离拟合采样函数 = interp_2d() + equal_step_resample()
	 void initialize_line_pan(float * xi_img, float * yi_img, int count,float* xo_img,float *yo_img,int &no_img);
	
	 // 返回N条牙弓线的最大长度
	 // xi_img 输入点的X坐标
	 // yi_img 输入点的Y坐标
	// count 输入点的个数
	 int generation_arch_wire(float *xi_img, float* yi_img,int count);

	 // 根据设置的cs的距离获得cs图像的数目
	 int get_num_cs();
	 // 获取cs的的每一点法向的角度
	 void get_norm_theta_cs();
	 // 获取cross-section的线的信息，p1和p2是两个点，n_cs_line_show是显示cs线的数目
	 void get_lines_cs(int ncs, int n_cs_line_show,int cs_pos_start,float * p1_x, float * p1_y, float * p2_x, float * p2_y,float cs_tilt_angle);
	 // 获取cross-section图像
	 void get_image_cs( short* img_out,int ncs, int n_cs_line_show,int cs_pos_start, float cs_tilt_angle);
	 // 获取全景图像
	 void get_image_panoramic( short * img_out,int n_pan_max);

	//release memory while re-getting panoramic and cross section image. 
	 void release_memory();

private:
	void reverse_vector(float * vec, int size);
	bool IsSamePoint( float *pPoint1,float *pPoint2); 
	void release_memory_cs();
	void release_memory_pano();
	int GetMax(int* fArray, int iArrayLen);

private:
	int m_npixel,m_nimg,m_nslice; // 图像xy方向的个数，全景图像的个数，轴位图像的数目
	float m_img_cen_x, m_img_cen_y,m_img_thk_pano,m_img_dist_pano;//中心点的X坐标，中心点的Y坐标，图像的层厚，图像的距离
	float m_img_thk_cs,m_img_dist_cs, m_img_width; // cross-section图像的层厚，距离，宽度
	float m_fit_length;// 拟合的长度
	short* m_image_data;// 图像数据
public:	
	static float *m_contour_line_xvec_sorted;//存储牙弓线的X坐标
	static float *m_contour_line_yvec_sorted;// 存储牙弓线的Y坐标
	static int* m_contour_line_len;// 牙弓线的长度
	static float *m_cs_ovs_x;// cross-section的X坐标，Y坐标
	static float *m_cs_ovs_y;
	static float *m_cs_theta;// cross-section的离散点的法向角度
	static int m_n_cs_ovs;// 离散后的cross-section的点数
	static float *m_cs_line_x;//牙弓线法线的X坐标
	static float *m_cs_line_y;//牙弓线法线的Y坐标
	static int *m_prj_pts_pano_x;//全景图像投影线的Y坐标
	static int *m_prj_pts_pano_y;//全景图像投影线的Y坐标
	static int *m_prj_pts_pano_len;//全景图像投影线的长度
	static int *m_prj_pts_cs_x;// cross-section图像投影线的X坐标
	static int *m_prj_pts_cs_y;//Cross-section图像投影线的Y坐标
	static int *m_prj_pts_cs_len;// cross-section图像投影线的长度

	float* m_pan_line_x,*m_pan_line_y;// 等距离离散后的牙弓线的X坐标，Y坐标
	int m_n_pan;// 牙弓线等距离离散后的个数
	unsigned char m_neutral_tube_bit;// 神经管的标示位标

};	

