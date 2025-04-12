#pragma once

#define DeleteVector(X)	{	\
	if(X != NULL) {				\
	delete []X;				\
	X = NULL;				\
	}							\
}								\

#define EPS 1e-4 
#define FIT_LENGTH 10 // the curve length used to do the linear fit to get the norm direction, in mm
#define RESAMPLE_PIX_STEP 1.0	// the distance between sampling points on the panoramic line, the pixel step on cross line, in pixel. Usually set to 1

#define ERR_LIM 1e-10 //precision
//#define PI	3.1415926
#define square(x) ((x)*(x))

#define MAXNUM_INPUT_POINTS 200  // maximum number of input points
#define NP_PER_LINE_SEG 20    // number of points to interpolate on a line segement
#define MAX_PAN_LINE_POINTS	 4096 //maximum number of output points

#define MAX_THK_PTS 100   //maximum number of points on the projection line for both panoramic and cross section image

#define SIGN(x) (((x)>0)?1:((x)==0?0:-1))

typedef unsigned char byte;

struct FLOATVECTOR3D
{
	float x;
	float y;
	float z;
};

#pragma comment(lib, "paronama/Paronama.lib")