#pragma once

class CTerrainLayerWidget
{
public:
	CTerrainLayerWidget();
	~CTerrainLayerWidget();

	void Create(CWnd *pParentWnd, UINT nRadio, BOOL InGroup, UINT nLoadDS, UINT nLoadNH, UINT nALPrev, UINT nALNext, UINT nDL, UINT nDNL);
	void SetPosition(CPoint Position);

	BOOL IsEnable() { return mEnable; }
	void SetEnable(BOOL Enable);

	BOOL IsSideEnable() { return mSideEnable; }
	void SetSideEnable(BOOL Enable);

	CString GetDiffuseSpecular();
	void SetDiffuseSpecular(CString Filename);
	void LoadDiffuseSpecular();

	CString GetNormalHeight();
	void SetNormalHeight(CString Filename);
	void LoadNormalHeight();
	void DelNormalHeight();

private:
	CBCGPButton mRadio;
	CBCGPStatic mDiffuseSpecular;
	CString mDiffuseSpecularName; // Íâ²¿Path
	CBCGPButton mLoadDiffuseSpecular;
	CBCGPStatic mNormalHeight;
	CString mNormalHeightName;
	CBCGPButton mLoadNormalHeight;
	CBCGPButton mAddLayerPrev;
	CBCGPButton mAddLayerNext;
	CBCGPButton mDelLayer;
	CBCGPButton mDelNormalLight;

	CPoint mPosition;
	BOOL mEnable;
	BOOL mSideEnable;
};
