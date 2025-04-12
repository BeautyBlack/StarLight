#include "pch.h"
#include <Services/VR/VRParameter.h>

CVRParameter::CVRParameter()
{
	Reset();
}

void CVRParameter::Reset()
{
	GetColorSegments().clear();
	GetOpacitySegments().clear();

	double opacities[] =
		/*{ -1024, - 924, - 751, - 551, 25, 325, 172,
			572, 251, 881, 310, 1240, 385, 1603, 515,
			2027, 716, 2507, 875, 2875, 1293, 3493,
			1795, 4195, 2439, 5039, 2702, 5502, 2799, 5799, 3071, 6271
		};*/
	{ -1024, -924, -751, -551, 25, 325,
		205, 605, 264, 804, 419, 1211, 561,
		1646, 665, 2169, 703, 2503, 916, 2916,
		1251, 3451, 1565, 3965, 2246, 4846,
		2673, 5473, 2828, 5828, 3071, 6271 };
	/*{ -1024, - 924, - 751, - 551, 25, 325, 205,
		605, 264, 804, 419, 1211, 561, 1646,
		665, 2169, 703, 2503, 916, 2916, 1251,
		3451, 1565, 3965, 2246, 4846, 2673, 5473,
		2828, 5828, 3071, 6271 };*/
		/*{
	-1024, -924, -751, -551, 25, 325, 88, 488,
		176, 871, 251, 1379, 460, 1860, 745, 2345,
		837, 2637, 992, 2992, 1335, 3535, 1732,
		4132, 2422, 5022, 2648, 5448, 2974,
		5974, 3071, 6271 };*/

		//double testValues[16] = { 0 }, testOpacities[16] = { 0 };

	for (int i = 0; i < 16; ++i)
	{
		const auto y = opacities[i * 2 + 1];
		const auto x = opacities[i * 2];

		CDisplayPixelSegment opacity;

		opacity.SetPixel(x);
		opacity.SetValue(((y - x) / (i + 1) - 100.0) / 100.0);

		if (x < 2300)
			opacity.SetValue(0);
		GetOpacitySegments().push_back(opacity);
	}

	int rgbs[] =
		/*{
		-1024, 0, - 905, 4227200, - 700, 11318964,
			- 500, 13156544, - 300, 16777215,
			- 21, 13171455, 42, 7928570, 168, 1317509,
			239, 155, 297, 39639, 356, 16777215,
			975, 16777215, 1075, 16777215,
			1381, 16777215, 1600, 16777215, 3071, 16777215 };*/
	{ -1024, 0, -905, 4227200, -700, 11318964,
		-500, 13156544, -300, 16777215,
		-21, 13171455, 42, 7928570,
		239, 1317511, 335, 128, 373, 33023,
		423, 16777215, 448, 16777215,
		498, 16777215, 700, 16777215,
		1600, 16777215, 3071, 16777215 };
	/*{
-1024, 0, - 905, 4227200, - 700, 11318964,
	- 500, 13156544, - 300, 16777215,
	- 21, 13171455, 42, 7928570, 239, 1317511,
	335, 128, 373, 33023, 423, 16777215, 448, 16777215,
	498, 16777215, 700, 16777215, 1600, 16777215,
	3071, 16777215 };*/
	/*{
	-1024, 0, -905, 4227200, -700, 11318964,
	-500, 13156544, -300, 16777215, -21, 13171455,
	42, 7928570, 76, 1317511, 126, 128, 172, 33023,
	222, 16777215, 327, 16777215, 544, 16777215,
	1372, 16777215, 2477, 16777215, 3071, 16777215
};*/

	for (int i = 0; i < 16; ++i)
	{
		CDisplayPixelSegment color;

		color.SetPixel(rgbs[i * 2]);

		int t = rgbs[i * 2 + 1];
		int b = t >> 16;
		int g = (t >> 8) & 0xFF;
		int r = t & 0xFF;
		color.SetValue(r << 16 | g << 8 | b);//rgbs[i * 2 + 1]);

		GetColorSegments().push_back(color);
	}

	//return;

	/*double testRGB[][5] = { {131, 0.0, 0.66, 0.66, 0.66},
	{ 521, 0.1, 0.92, 0.92, 0.86 },
	{ 1120, 0.8, 0.93, 0.89, 0.92 },
	{ 1739, 1.0, 1.0, 1.0, 1.0 }};

	for(auto i = 0; i < 4; i++)
	{
		CDisplayPixelSegment color;

		color.SetPixel(testRGB[i][0]);

		int r = testRGB[i][2] * 100;
		int g = testRGB[i][3] * 100;
		int b = testRGB[i][4] * 100;

		color.SetValue(r<<16 | g << 8 | b);

		GetColorSegments().push_back(color);
	}*/

	/*CDisplayPixelSegment t1, t2, t3, t4;
	CDisplayPixelSegment o1, o2, o3, o4;

	t1.SetPixel(-3024);
	t1.SetValue(0);
	t1.SetMidpoint(0.5);
	t1.SetSharpness(0);

	t2.SetPixel(-16);
	t2.SetValue(732530);
	t2.SetMidpoint(0.49);
	t2.SetSharpness(0.61);

	t3.SetPixel(641);
	t3.SetValue(908256);
	t3.SetMidpoint(0.5);
	t3.SetSharpness(0);

	t4.SetPixel(3071);
	t4.SetValue(999999);
	t4.SetMidpoint(0.5);
	t4.SetSharpness(0);

	GetColorSegments().clear();

	GetColorSegments().push_back(t1);
	GetColorSegments().push_back(t2);
	GetColorSegments().push_back(t3);
	GetColorSegments().push_back(t4);

	o1.SetPixel(-3024);
	o1.SetValue(5000);

	o2.SetPixel(-16);
	o2.SetValue(4961);

	o3.SetPixel(641);
	o3.SetValue(725000);

	o4.SetPixel(3071);
	o4.SetValue(715000);

	GetOpacitySegments().clear();

	GetOpacitySegments().push_back(o1);
	GetOpacitySegments().push_back(o2);
	GetOpacitySegments().push_back(o3);
	GetOpacitySegments().push_back(o4);*/

	double test[][5] = {
		{-400, 0.0, 0.0, 0.0, 0.0},
	  {-270, 0.0, 0.0, 0.0, 0.0},
	  {50, 0.0, 0.0, 0.0, 0.0},
	  {430, 0.0, 1.0, 1.0, 0.0},
	  {1550, 0.04, 1.0, 1.0, 1.0},
	 {1856, 0.2, 1.0, 1.0, 1.0},
	  {2522, 1.0, 1.0, 1.0, 1.0},
	  {3000, 1.0, 1.0, 1.0, 1.0}
	};

	GetOpacitySegments().clear();
	GetColorSegments().clear();
	
	for(int i = 0; i < sizeof(test) / sizeof(double) / 5; i++)
	{
		CDisplayPixelSegment opacity;

		opacity.SetPixel(test[i][0]);
		opacity.SetValue(test[i][1]);
		
		GetOpacitySegments().push_back(opacity);

		CDisplayPixelSegment color;

		int r = test[i][2] * 100;
		int g = test[i][3] * 100;
		int b = test[i][4] * 100;
		
		color.SetPixel(test[i][0]);
		color.SetValue(b<<16 | g << 8 | r);

		GetColorSegments().push_back(color);
	}

	//////////////////////////
	GetOpacitySegments().clear();
	GetColorSegments().clear();
	
	double colorTrans[16 * 4] = { 0 }, opacityTrans[16*2] = {0};
	
	double colorFormer[] = {
	-500,0,0,0,1112,100,20,8,1716,234,139,16,2272,255,230,179,2925,255,255,230,3962,255,255,240
	};
	double opacityFormer[] = {
	0,0,480,0,1247,6,1500,0,2037,10,2597,94,3538,160,4074,175
	};
	const auto offset = 0;// 1024;
	
	for (int i = 0; i < sizeof(colorFormer) / sizeof(double) / 4; i++)
	{
		colorTrans[i * 4] = colorFormer[i * 4]- offset;
		colorTrans[i * 4 + 1] = (colorFormer[i * 4 + 1]);
		colorTrans[i * 4 + 2] = (colorFormer[i * 4 + 2]);
		colorTrans[i * 4 + 3] = (colorFormer[i * 4 + 3]);
	}
	for (int i = 0; i < sizeof(opacityFormer) / sizeof(double) / 2; i++)
	{
		opacityTrans[i * 2] = opacityFormer[i * 2]- offset;
		opacityTrans[2 * i + 1] = opacityFormer[i * 2 + 1] / 255.0;
	}

	for(int i = 0 ; i < 6; i++)
	{
		CDisplayPixelSegment opacity;

		opacity.SetPixel(opacityTrans[i * 2]);
		opacity.SetValue(opacityTrans[2 * i + 1]);

		GetOpacitySegments().push_back(opacity);
	}

	for(int i = 0; i < 6; i++)
	{
		CDisplayPixelSegment color;

		int r = colorTrans[i * 4 + 1];
		int g = colorTrans[i * 4 + 2];
		int b = colorTrans[i * 4 + 3];

		color.SetPixel(colorTrans[i * 4]);
		color.SetValue(
			r << 16 | g << 8 | b);
		//b << 16 | g << 8 | r);

		GetColorSegments().push_back(color);
	}
}
