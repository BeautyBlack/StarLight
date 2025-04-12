#include "pch.h"
#include <Services/VR/DisplayPixelSegment.h>

CDisplayPixelSegment::CDisplayPixelSegment()
{
	SetPixel(0);
	SetValue(0);
	SetMidpoint(0);
	SetSharpness(0);
}

CDisplayPixelSegment::CDisplayPixelSegment(const CDisplayPixelSegment& copySrc)
{
	SetPixel(copySrc.m_Pixel);
	SetValue(copySrc.m_Value);
	SetMidpoint(copySrc.m_Midpoint);
	SetSharpness(copySrc.m_Sharpness);
}
