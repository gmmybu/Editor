#include "stdafx.h"
#include "Editor.h"
#include "TerrainLayerWidget.h"
#include "MiscUtils.h"
#include "StringUtils.h"

CTerrainLayerWidget::CTerrainLayerWidget()
{
}

CTerrainLayerWidget::~CTerrainLayerWidget()
{
}

void CTerrainLayerWidget::Create(CWnd *pParentWnd, UINT nRadio, BOOL InGroup, UINT nLoadDS, UINT nLoadNH, UINT nALPrev, UINT nALNext, UINT nDL, UINT nDNL)
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	DWORD dwStyle = WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON;
	if(InGroup)
		dwStyle |= WS_GROUP;
	mRadio.Create("", dwStyle, rectDummy, pParentWnd, nRadio);
	mDiffuseSpecular.Create("", WS_VISIBLE|WS_CHILD|SS_BITMAP, rectDummy, pParentWnd);
	HBITMAP WhiteBmp = MiscUtils::loadBitmap(NULL, 64);
	mDiffuseSpecular.SetBitmap(WhiteBmp);
	mLoadDiffuseSpecular.Create("替换", WS_CHILD|WS_VISIBLE, rectDummy, pParentWnd, nLoadDS);
	mNormalHeight.Create("", WS_VISIBLE|WS_CHILD|SS_BITMAP, rectDummy, pParentWnd);
	mNormalHeight.SetBitmap(WhiteBmp);
	mLoadNormalHeight.Create("替换", WS_CHILD|WS_VISIBLE, rectDummy, pParentWnd, nLoadNH);
	mAddLayerPrev.Create("<", WS_CHILD|WS_VISIBLE, rectDummy, pParentWnd, nALPrev);
	mAddLayerNext.Create(">", WS_CHILD|WS_VISIBLE, rectDummy, pParentWnd, nALNext);
	mDelLayer.Create("X", WS_CHILD|WS_VISIBLE, rectDummy, pParentWnd, nDL);
	mDelNormalLight.Create("/", WS_CHILD|WS_VISIBLE, rectDummy, pParentWnd, nDNL);
	// ::DeleteObject(WhiteBmp); // 否则将看不见Bmp
}

void CTerrainLayerWidget::SetPosition(CPoint Position)
{
	mPosition = Position;
	mRadio.SetWindowPos(NULL, mPosition.x, mPosition.y+30, 15, 15, SWP_NOACTIVATE|SWP_NOZORDER);
	mDiffuseSpecular.SetWindowPos(NULL, mPosition.x+15+5, mPosition.y, 64, 64, SWP_NOACTIVATE|SWP_NOZORDER);
	mLoadDiffuseSpecular.SetWindowPos(NULL, mPosition.x+15+5, mPosition.y+64, 64, 20, SWP_NOACTIVATE|SWP_NOZORDER);
	mNormalHeight.SetWindowPos(NULL, mPosition.x+15+5+64+5, mPosition.y, 64, 64, SWP_NOACTIVATE|SWP_NOZORDER);
	mLoadNormalHeight.SetWindowPos(NULL, mPosition.x+15+5+64+5, mPosition.y+64, 64, 20, SWP_NOACTIVATE|SWP_NOZORDER);
	mAddLayerPrev.SetWindowPos(NULL, mPosition.x+15+5+64+5+64+5, mPosition.y, 15, 21, SWP_NOACTIVATE|SWP_NOZORDER);
	mAddLayerNext.SetWindowPos(NULL, mPosition.x+15+5+64+5+64+5, mPosition.y+21, 15, 21, SWP_NOACTIVATE|SWP_NOZORDER);
	mDelLayer.SetWindowPos(NULL, mPosition.x+15+5+64+5+64+5, mPosition.y+42, 15, 22, SWP_NOACTIVATE|SWP_NOZORDER);
	mDelNormalLight.SetWindowPos(NULL, mPosition.x+15+5+64+5+64+5, mPosition.y+63, 15, 22, SWP_NOACTIVATE|SWP_NOZORDER);
}

void CTerrainLayerWidget::SetEnable(BOOL Enable)
{
	mEnable = Enable;
	mRadio.EnableWindow(Enable);
	mLoadDiffuseSpecular.EnableWindow(mEnable);
	mLoadNormalHeight.EnableWindow(mEnable);
	SetSideEnable(Enable);
}

void CTerrainLayerWidget::SetSideEnable(BOOL Enable)
{
	mSideEnable = Enable;
	mAddLayerPrev.EnableWindow(mSideEnable);
	mAddLayerNext.EnableWindow(mSideEnable);
	mDelLayer.EnableWindow(mSideEnable);
	mDelNormalLight.EnableWindow(mSideEnable);
}

CString CTerrainLayerWidget::GetDiffuseSpecular()
{
	return mDiffuseSpecularName;
}

void CTerrainLayerWidget::SetDiffuseSpecular(CString Filename)
{
	CString Path(Filename);
	if(Filename.GetLength() != 0)
	{
		Path = StringUtils::externalPath(theApp.getPath("Paint"), std::string(Filename)).c_str();
	}
	HBITMAP Bitmap = MiscUtils::loadBitmap(Path, 64);
	HBITMAP PrevBitmap = mDiffuseSpecular.SetBitmap(Bitmap);
	::DeleteObject(PrevBitmap);
	mDiffuseSpecularName = Path;
}

void CTerrainLayerWidget::LoadDiffuseSpecular()
{
	CFileDialog dlg( TRUE, NULL, NULL, OFN_NOCHANGEDIR|OFN_ALLOWMULTISELECT,
		"All Image Files (*.bmp;*.dds;*.jpg;*.jpeg;*.png;*.tga)|*.bmp;*.dds;*.jpg;*.jpeg;*.png;*.tga|\
		Windows BMP (*.bmp)|*.bmp|\
		DirectDraw Surface Bitmaps (*.dds)|*.dds|\
		JPEG Images (*.jpg;*.jpeg)|*.jpg;*.jpeg|\
		Portable Network Graphic (*.png)|*.png|\
		TARGA Images (*.tga;*.targa)|*.tga;*.targa|\
		All Files (*.*)|*.*||", NULL);

	TCHAR pathBuffer[MAX_PATH];
	::GetFullPathName(theApp.getPath("Paint").c_str(), MAX_PATH, pathBuffer, NULL);
	dlg.m_ofn.lpstrInitialDir = pathBuffer;

	if(IDOK == dlg.DoModal())
	{
		CString texName = dlg.GetPathName();
		if(texName.Left(lstrlen(pathBuffer)) == pathBuffer)
		{
			HBITMAP hBitmap = MiscUtils::loadBitmap(texName, 64);
			HBITMAP PrevBitmap = mDiffuseSpecular.SetBitmap(hBitmap);
			::DeleteObject(PrevBitmap);
			mDiffuseSpecularName = texName;
		}
	}
}

CString CTerrainLayerWidget::GetNormalHeight()
{
	return mNormalHeightName;
}

void CTerrainLayerWidget::SetNormalHeight(CString Filename)
{
	CString Path(Filename);
	if(Filename.GetLength() != 0)
	{
		Path = StringUtils::externalPath(theApp.getPath("Paint"), std::string(Filename)).c_str();
	}
	HBITMAP Bitmap = MiscUtils::loadBitmap(Path, 64);
	HBITMAP PrevBitmap = mNormalHeight.SetBitmap(Bitmap);
	::DeleteObject(PrevBitmap);
	mNormalHeightName = Path;
}

void CTerrainLayerWidget::LoadNormalHeight()
{
	CFileDialog dlg( TRUE, NULL, NULL, OFN_NOCHANGEDIR|OFN_ALLOWMULTISELECT,
		"All Image Files (*.bmp;*.dds;*.jpg;*.jpeg;*.png;*.tga)|*.bmp;*.dds;*.jpg;*.jpeg;*.png;*.tga|\
		Windows BMP (*.bmp)|*.bmp|\
		DirectDraw Surface Bitmaps (*.dds)|*.dds|\
		JPEG Images (*.jpg;*.jpeg)|*.jpg;*.jpeg|\
		Portable Network Graphic (*.png)|*.png|\
		TARGA Images (*.tga;*.targa)|*.tga;*.targa|\
		All Files (*.*)|*.*||", NULL);

	TCHAR pathBuffer[MAX_PATH];
	::GetFullPathName(theApp.getPath("Paint").c_str(), MAX_PATH, pathBuffer, NULL);
	dlg.m_ofn.lpstrInitialDir = pathBuffer;

	if(IDOK == dlg.DoModal())
	{
		CString texName = dlg.GetPathName();
		if(texName.Left(lstrlen(pathBuffer)) == pathBuffer)
		{
			HBITMAP hBitmap = MiscUtils::loadBitmap(texName, 64);
			HBITMAP PrevBitmap = mNormalHeight.SetBitmap(hBitmap);
			::DeleteObject(PrevBitmap);
			mNormalHeightName = texName;
		}
	}
}

void CTerrainLayerWidget::DelNormalHeight()
{
	HBITMAP hBitmap = MiscUtils::loadBitmap("", 64);
	HBITMAP PrevBitmap = mNormalHeight.SetBitmap(hBitmap);
	::DeleteObject(PrevBitmap);
	mNormalHeightName = "";
}
