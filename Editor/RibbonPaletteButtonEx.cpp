#include "stdafx.h"
#include "RibbonPaletteButtonEx.h"

CRibbonPaletteButtonEx::CRibbonPaletteButtonEx(UINT nID, LPCTSTR lpszText, int nSmallImageIndex, int nLargeImageIndex, CBCGPToolBarImages &imagesPalette):
	CBCGPRibbonPaletteButton(nID, lpszText, nSmallImageIndex, nLargeImageIndex, imagesPalette)
{
}

CRibbonPaletteButtonEx::~CRibbonPaletteButtonEx(void)
{
}
