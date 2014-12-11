#pragma once

class CResourceLayerWidget
{
public:
	CResourceLayerWidget();
	~CResourceLayerWidget();

	void Create(CWnd *pParentWnd, UINT nLoadDiffuseSpecular, UINT nLoadNormalHeight);
	void SetPosition(CPoint Position);

	CString GetDiffuseSpecular();
	void LoadDiffuseSpecular();

	CString GetNormalHeight();
	void LoadNormalHeight();

private:
	CBCGPStatic mText;
	CBCGPStatic mDiffuseSpecular;
	CString mDiffuseSpecularName;
	CBCGPButton mLoadDiffuseSpecular;
	CBCGPStatic mNormalHeight;
	CString mNormalHeightName;
	CBCGPButton mLoadNormalHeight;

	CPoint mPosition;
};
