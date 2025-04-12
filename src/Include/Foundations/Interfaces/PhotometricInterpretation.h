/*
 *****************************************************************
 *                 C O P Y R I G H T  (c) 2020
 *                    All Rights Reserved
 *****************************************************************
 *
 *       THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF
 *                      Zhang Jiaqi.
 *      The copyright notice above does not evidence any
 *     actual or intended publication of such source code.
 *
 *****************************************************************
 *
 *  Author: Zhang Jiaqi
 *
 */

#ifndef PHOTOMETRIC_INTERPRETATION_4C229EBB5FF04F7E86E49CDC86F27345
#define PHOTOMETRIC_INTERPRETATION_4C229EBB5FF04F7E86E49CDC86F27345

#pragma once

/*
 * Description
 * Ó°Ïñ±àÂë·½Ê½
 */
enum class PhotometricInterpretation
{
	UnknownPhotometricInterpretation = 0,

	/*
	 * monochrome 1
	 */
	MONOCHROME1 = 1,

	/*
	 * monochrome 2
	 */
	MONOCHROME2 = 2,

	/*
	 * palette color
	 */
	PALETTE_COLOR = 3,
 
	/*
	 * RGB color
	 */
	RGB = 4,

	/*
	 * HSV color (retired)
	 */
	HSV = 5,

	/*
	 * ARGB color (retired)
	 */
	ARGB = 6,

	/*
	 * CMYK color (retired)
	 */
	CMYK = 7,

	/*
	 * YCbCr full
	 */
	YBR_FULL = 8,

	/*
	 * YCbCr full 4:2:2
	 */
	YBR_FULL_422 = 9,

	/*
	 * YCbCr partial 4:2:2
	 */
	YBR_PARTIAL_422 = 10,

	InvalidPhotometricInterpretation = 0xEFFFFF
};

inline PhotometricInterpretation ParseTextToPhotometricInterpretation(const char* strText)
{
	if (_stricmp(strText, "MONOCHROME1") == 0)
		return PhotometricInterpretation::MONOCHROME1;

	if (_stricmp(strText, "MONOCHROME2") == 0)
		return PhotometricInterpretation::MONOCHROME2;

	if (_stricmp(strText, "PALETTE_COLOR") == 0)
		return PhotometricInterpretation::PALETTE_COLOR;

	if (_stricmp(strText, "RGB") == 0)
		return PhotometricInterpretation::RGB;

	if (_stricmp(strText, "HSV") == 0)
		return PhotometricInterpretation::HSV;

	if (_stricmp(strText, "ARGB") == 0)
		return PhotometricInterpretation::ARGB;

	if (_stricmp(strText, "CMYK") == 0)
		return PhotometricInterpretation::CMYK;

	if (_stricmp(strText, "YBR_FULL") == 0)
		return PhotometricInterpretation::YBR_FULL;

	if (_stricmp(strText, "YBR_FULL_422") == 0)
		return PhotometricInterpretation::YBR_FULL_422;

	if (_stricmp(strText, "YBR_PARTIAL_422") == 0)
		return PhotometricInterpretation::YBR_PARTIAL_422;

	return PhotometricInterpretation::MONOCHROME2;
}

#endif
