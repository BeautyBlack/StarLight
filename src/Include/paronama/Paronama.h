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
	  // img_cen_x,img_cen_y the image center,normal set 0.0��
	 // fit_length: length of fitting,set 10.0
	 void set_paronama_pana(float img_cen_x, float img_cen_y,float fit_length);

	 // set parameters in normal
	 void initializePara();
	 // once set all parameters
	 void Set_Para(int nimg, int npixel, int nslice, float img_cen_x, float img_cen_y,float img_thk_pano, float img_dist_pano,float img_thk_cs, float img_dist_cs,float img_width,float fit_length);

	 // ��ȡȫ��ͼ������nimg ��ͼ����Ϣnpixel�Ͳ���nslice
	 void get_value(int nimg, int npixel, int nslice);

	// 2ά��3ά��ֵ����������Ϊ���������͸��������Ϊ����Ϻ�������͸���
	 void interp_2d(float * pInx, float * pIny, int nInNum, float * pOutx, float * pOuty, int & nOutNum);
	 void interp_3d(float * pInX, float * pInY, float * pInZ, int nInNum, float * pOutX, float * pOutY, float * pOutZ, int & nOutNum);

	 // �Ⱦ�����ɢ���������������еȼ�����
	 // resample_step �ȼ���������
	 void equal_step_resample(float * in_x, float * in_y, int n_in, float resample_step, float * out_x, float * out_y, int &n_out);
	 // �Ⱦ�����ϲ������� = interp_2d() + equal_step_resample()
	 void initialize_line_pan(float * xi_img, float * yi_img, int count,float* xo_img,float *yo_img,int &no_img);
	
	 // ����N�������ߵ���󳤶�
	 // xi_img ������X����
	 // yi_img ������Y����
	// count �����ĸ���
	 int generation_arch_wire(float *xi_img, float* yi_img,int count);

	 // �������õ�cs�ľ�����csͼ�����Ŀ
	 int get_num_cs();
	 // ��ȡcs�ĵ�ÿһ�㷨��ĽǶ�
	 void get_norm_theta_cs();
	 // ��ȡcross-section���ߵ���Ϣ��p1��p2�������㣬n_cs_line_show����ʾcs�ߵ���Ŀ
	 void get_lines_cs(int ncs, int n_cs_line_show,int cs_pos_start,float * p1_x, float * p1_y, float * p2_x, float * p2_y,float cs_tilt_angle);
	 // ��ȡcross-sectionͼ��
	 void get_image_cs( short* img_out,int ncs, int n_cs_line_show,int cs_pos_start, float cs_tilt_angle);
	 // ��ȡȫ��ͼ��
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
	int m_npixel,m_nimg,m_nslice; // ͼ��xy����ĸ�����ȫ��ͼ��ĸ�������λͼ�����Ŀ
	float m_img_cen_x, m_img_cen_y,m_img_thk_pano,m_img_dist_pano;//���ĵ��X���꣬���ĵ��Y���꣬ͼ��Ĳ��ͼ��ľ���
	float m_img_thk_cs,m_img_dist_cs, m_img_width; // cross-sectionͼ��Ĳ�񣬾��룬���
	float m_fit_length;// ��ϵĳ���
	short* m_image_data;// ͼ������
public:	
	static float *m_contour_line_xvec_sorted;//�洢�����ߵ�X����
	static float *m_contour_line_yvec_sorted;// �洢�����ߵ�Y����
	static int* m_contour_line_len;// �����ߵĳ���
	static float *m_cs_ovs_x;// cross-section��X���꣬Y����
	static float *m_cs_ovs_y;
	static float *m_cs_theta;// cross-section����ɢ��ķ���Ƕ�
	static int m_n_cs_ovs;// ��ɢ���cross-section�ĵ���
	static float *m_cs_line_x;//�����߷��ߵ�X����
	static float *m_cs_line_y;//�����߷��ߵ�Y����
	static int *m_prj_pts_pano_x;//ȫ��ͼ��ͶӰ�ߵ�Y����
	static int *m_prj_pts_pano_y;//ȫ��ͼ��ͶӰ�ߵ�Y����
	static int *m_prj_pts_pano_len;//ȫ��ͼ��ͶӰ�ߵĳ���
	static int *m_prj_pts_cs_x;// cross-sectionͼ��ͶӰ�ߵ�X����
	static int *m_prj_pts_cs_y;//Cross-sectionͼ��ͶӰ�ߵ�Y����
	static int *m_prj_pts_cs_len;// cross-sectionͼ��ͶӰ�ߵĳ���

	float* m_pan_line_x,*m_pan_line_y;// �Ⱦ�����ɢ��������ߵ�X���꣬Y����
	int m_n_pan;// �����ߵȾ�����ɢ��ĸ���
	unsigned char m_neutral_tube_bit;// �񾭹ܵı�ʾλ��

};	

