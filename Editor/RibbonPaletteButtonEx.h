#pragma once
class CRibbonPaletteButtonEx : public CBCGPRibbonPaletteButton
{
	friend class CRibbonBarEx;
public:
	CRibbonPaletteButtonEx(UINT nID, LPCTSTR lpszText, int nSmallImageIndex, int nLargeImageIndex, CBCGPToolBarImages &imagesPalette);
	~CRibbonPaletteButtonEx(void);
};
