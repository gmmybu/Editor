#include "stdafx.h"
#include "Editor.h"
#include "MiscUtils.h"
#include "OgreImage.h"
#include "FreeImage.h"

HBITMAP MiscUtils::loadBitmap(CString filename, int size)
{
	if(filename.GetLength() == 0 && size != 0)
	{ // 白位图
		BITMAPINFO bitmapInfo;
 		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
 		bitmapInfo.bmiHeader.biBitCount = 24;
 		bitmapInfo.bmiHeader.biPlanes = 1;
 		bitmapInfo.bmiHeader.biSizeImage = 0;
 		bitmapInfo.bmiHeader.biCompression = BI_RGB;
		bitmapInfo.bmiHeader.biWidth = size;
		bitmapInfo.bmiHeader.biHeight = size;
 		bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
 		bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
 		bitmapInfo.bmiHeader.biClrUsed = 0;
 		bitmapInfo.bmiHeader.biClrImportant = 0;
 
 		unsigned char *bitmapData = NULL;
 		HBITMAP bitmap = ::CreateDIBSection(0, &bitmapInfo, DIB_RGB_COLORS, (void**)&bitmapData, 0, 0);
		memset(bitmapData, 225, size * size * 3);

		return bitmap;
	}
	else
	{
		FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType(filename); // 由文件头分析文件类型
		if(imageFormat == FIF_UNKNOWN)
			imageFormat = FreeImage_GetFIFFromFilename(filename);

		FIBITMAP *image = FreeImage_Load(imageFormat, filename);
		if(imageFormat != FIF_BMP)
		{
			image = FreeImage_ConvertTo24Bits(image);
		}

		unsigned int imageWidth = FreeImage_GetWidth(image);
		unsigned int imageHeight = FreeImage_GetHeight(image);
		if(size != 0 && (imageWidth != size || imageHeight != size))
		{
			image = FreeImage_Rescale(image, size, size, FILTER_BOX);
		}

		BITMAPINFO bitmapInfo;
 		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
 		bitmapInfo.bmiHeader.biBitCount = 24;
 		bitmapInfo.bmiHeader.biPlanes = 1;
 		bitmapInfo.bmiHeader.biSizeImage = 0;
 		bitmapInfo.bmiHeader.biCompression = BI_RGB;
		bitmapInfo.bmiHeader.biWidth = imageWidth;
		bitmapInfo.bmiHeader.biHeight = imageHeight;
 		bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
 		bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
 		bitmapInfo.bmiHeader.biClrUsed = 0;
 		bitmapInfo.bmiHeader.biClrImportant = 0;

		if(size != 0)
		{
			bitmapInfo.bmiHeader.biWidth = size;
			bitmapInfo.bmiHeader.biHeight = size;
		}

		unsigned char *bitmapData = NULL;
 		HBITMAP bitmap = ::CreateDIBSection(0, &bitmapInfo, DIB_RGB_COLORS, (void**)&bitmapData, 0, 0);
		memcpy(bitmapData, FreeImage_GetBits(image), bitmapInfo.bmiHeader.biWidth * bitmapInfo.bmiHeader.biHeight * 3);
		FreeImage_Unload(image);
		return bitmap;
	}
	return NULL;
}
