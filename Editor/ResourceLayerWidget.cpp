#include "stdafx.h"
#include "Editor.h"
#include "ResourceLayerWidget.h"
#include "MiscUtils.h"

CResourceLayerWidget::CResourceLayerWidget()
{
}

CResourceLayerWidget::~CResourceLayerWidget()
{
}

void CResourceLayerWidget::Create(CWnd *pParentWnd, UINT nLoadDiffuseSpecular, UINT nLoadNormalHeight)
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	mDiffuseSpecular.Create("", WS_VISIBLE|WS_CHILD|SS_BITMAP, rectDummy, pParentWnd);
	HBITMAP WhiteBmp = MiscUtils::loadBitmap(NULL, 64);
	mDiffuseSpecular.SetBitmap(WhiteBmp);
	mLoadDiffuseSpecular.Create("载入", WS_CHILD|WS_VISIBLE, rectDummy, pParentWnd, nLoadDiffuseSpecular);
	mNormalHeight.Create("", WS_VISIBLE|WS_CHILD|SS_BITMAP, rectDummy, pParentWnd);
	mNormalHeight.SetBitmap(WhiteBmp);
	mLoadNormalHeight.Create("载入", WS_CHILD|WS_VISIBLE, rectDummy, pParentWnd, nLoadNormalHeight);
	// ::DeleteObject(WhiteBmp); // 否则将看不见Bmp
}

void CResourceLayerWidget::SetPosition(CPoint Position)
{
	mPosition = Position;
	mDiffuseSpecular.SetWindowPos(NULL, mPosition.x+20, mPosition.y, 64, 64, SWP_NOACTIVATE|SWP_NOZORDER);
	mLoadDiffuseSpecular.SetWindowPos(NULL, mPosition.x+20, mPosition.y+64, 64, 20, SWP_NOACTIVATE|SWP_NOZORDER);
	mNormalHeight.SetWindowPos(NULL, mPosition.x+64+25, mPosition.y, 64, 64, SWP_NOACTIVATE|SWP_NOZORDER);
	mLoadNormalHeight.SetWindowPos(NULL, mPosition.x+64+25, mPosition.y+64, 64, 20, SWP_NOACTIVATE|SWP_NOZORDER);
}

CString CResourceLayerWidget::GetDiffuseSpecular()
{
	return mDiffuseSpecularName;
}

void CResourceLayerWidget::LoadDiffuseSpecular()
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
		int baseLen = lstrlen(pathBuffer);
		CString texName = dlg.GetPathName();
		if(texName.Left(baseLen) == pathBuffer)
		{
			HBITMAP hBitmap = MiscUtils::loadBitmap(texName, 64);
			HBITMAP PrevBitmap = mDiffuseSpecular.SetBitmap(hBitmap);
			::DeleteObject(PrevBitmap);
			mDiffuseSpecularName = texName;
		}
	}
}

CString CResourceLayerWidget::GetNormalHeight()
{
	return mNormalHeightName;
}

void CResourceLayerWidget::LoadNormalHeight()
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
		int baseLen = lstrlen(pathBuffer);
		CString texName = dlg.GetPathName();
		if(texName.Left(baseLen) == pathBuffer)
		{
			HBITMAP hBitmap = MiscUtils::loadBitmap(texName, 64);
			HBITMAP PrevBitmap = mNormalHeight.SetBitmap(hBitmap);
			::DeleteObject(PrevBitmap);
			mNormalHeightName = texName;
		}
	}
}
