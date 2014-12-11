#pragma once

#include "Box.h"
#include "RibbonBarEx.h"
#include "ResourceLayerWidget.h"
#include "TerrainLayerWidget.h"

namespace Ogre
{
	class Terrain;
}

class CTerrainEditPage : public CBCGPDialog
{
	DECLARE_DYNAMIC(CTerrainEditPage)
public:
	CTerrainEditPage(CWnd* pParent = NULL);
	virtual ~CTerrainEditPage();

	Ogre::Terrain *GetTerrain() { return mTerrain; }
	void SetTerrain(Ogre::Terrain *Terrain);

	int GetBrushSize();
	int GetBrushPower();

	void update();
	int GetEditLayer() { return mEditLayer; }

	static CTerrainEditPage *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBrushBar();
	afx_msg void OnLoadBrush();
	afx_msg void OnClearBrush();
	afx_msg void OnResizeBrush();

	afx_msg void OnLoadDiffuseSpecular();
	afx_msg void OnLoadNormalHeight();

	afx_msg void OnLayer1(); // Radio
	afx_msg void OnLayer2();
	afx_msg void OnLayer3();
	afx_msg void OnLayer4();
	afx_msg void OnLayer5();

	afx_msg void OnLayer1LoadDiffuseSpecular();
	afx_msg void OnLayer1LoadNormalHeight();
	afx_msg void OnLayer1AddLayerPrev();
	afx_msg void OnLayer1AddLayerNext();
	afx_msg void OnLayer1DelLayer();
	afx_msg void OnLayer1DelNormalHeight();

	afx_msg void OnLayer2LoadDiffuseSpecular();
	afx_msg void OnLayer2LoadNormalHeight();
	afx_msg void OnLayer2AddLayerPrev();
	afx_msg void OnLayer2AddLayerNext();
	afx_msg void OnLayer2DelLayer();
	afx_msg void OnLayer2DelNormalHeight();

	afx_msg void OnLayer3LoadDiffuseSpecular();
	afx_msg void OnLayer3LoadNormalHeight();
	afx_msg void OnLayer3AddLayerPrev();
	afx_msg void OnLayer3AddLayerNext();
	afx_msg void OnLayer3DelLayer();
	afx_msg void OnLayer3DelNormalHeight();

	afx_msg void OnLayer4LoadDiffuseSpecular();
	afx_msg void OnLayer4LoadNormalHeight();
	afx_msg void OnLayer4AddLayerPrev();
	afx_msg void OnLayer4AddLayerNext();
	afx_msg void OnLayer4DelLayer();
	afx_msg void OnLayer4DelNormalHeight();

	afx_msg void OnLayer5LoadDiffuseSpecular();
	afx_msg void OnLayer5LoadNormalHeight();
	afx_msg void OnLayer5AddLayerPrev();
	afx_msg void OnLayer5AddLayerNext();
	afx_msg void OnLayer5DelLayer();
	afx_msg void OnLayer5DelNormalHeight();

	enum { IDD = IDD_TERRAIN_EDIT };

	void DoDataExchange(CDataExchange* pDX);
	void LoadDiffuseSpecular(int Layer);
	void LoadNormalHeight(int Layer);
	void AddLayerPrev(int Layer);
	void AddLayerNext(int Layer);
	void DelLayer(int Layer);
	void DelNormalHeight(int Layer);

	Ogre::Terrain *mTerrain;

	CBox mBrushBox; // Ñ¡Ôñ»­Ë¢
	CString mBrushText;
	CRibbonBarEx mBrushBar;

	CBox mSizeBox; // »­Ë¢³ß´ç
	CString mSizeText;
	CBCGPSliderCtrl mSizeSlider;

	CBox mPowerBox; // »­Ë¢Ç¿¶È
	CString mPowerText;
	CBCGPSliderCtrl mPowerSlider;

	CBox mTextureBox;
	CString mTextureText;
	CBCGPSliderCtrl mTextureSlider;

	CResourceLayerWidget mResourceLayer;
	CTerrainLayerWidget mLayers[5];
	int mEditLayer;
};
