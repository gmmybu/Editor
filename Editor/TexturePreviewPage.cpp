#include "stdafx.h"
#include "afxdialogex.h"
#include "Editor.h"
#include "TexturePreviewPage.h"
#include "MiscUtils.h"

#include "Box.h"

IMPLEMENT_DYNAMIC(CTexturePreviewPage, CDialogEx)

CTexturePreviewPage::CTexturePreviewPage(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CTexturePreviewPage::IDD, pParent)
{

}

CTexturePreviewPage::~CTexturePreviewPage()
{
}

BEGIN_MESSAGE_MAP(CTexturePreviewPage, CBCGPDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_TEXTURE_BAR, OnTextureBar)
	ON_COMMAND(ID_LOAD_TEXTURE, OnLoadTexture)
	ON_COMMAND(ID_CLEAR_TEXTURE, OnClearTexture)
END_MESSAGE_MAP()

int CTexturePreviewPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CBCGPDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect;
	rect.SetRectEmpty();

	mTextureBar.Create(rect, this, ID_TEXTURE_BAR);
	mTextureBar.AddMenuItem(ID_LOAD_TEXTURE, "载入纹理");
	mTextureBar.AddMenuItem(ID_CLEAR_TEXTURE, "清空纹理"); // 纹理散落在不同的文件夹中，因此不能每次载入的时候就清空

	mImage.Create(rect, this, ID_TEXTURE_PREVIEW);
	return 0;
}

void CTexturePreviewPage::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);
	
	int BarHeight = cy>>3;

	CBox TextureBarBox;
	TextureBarBox.left = 5;
	TextureBarBox.top = 5;
	TextureBarBox.width = cx-10;
	TextureBarBox.height = BarHeight;
	mTextureBar.SetWindowPos(NULL, TextureBarBox.left, TextureBarBox.top, TextureBarBox.width, TextureBarBox.height, SWP_NOACTIVATE|SWP_NOZORDER);

	CBox TexturePreviewBox;
	TexturePreviewBox.left = 5;
	TexturePreviewBox.top = TextureBarBox.bottom() + 5;
	TexturePreviewBox.width = cx-10;
	TexturePreviewBox.height = cy - TexturePreviewBox.top;
	mImage.SetWindowPos(NULL, TexturePreviewBox.left, TexturePreviewBox.top, TexturePreviewBox.width, TexturePreviewBox.height, SWP_NOACTIVATE|SWP_NOZORDER);
}

void CTexturePreviewPage::OnTextureBar()
{
	mTextureBar.FillPropertyWnd();
	HBITMAP Bitmap = MiscUtils::loadBitmap(mTextureBar.GetSelectedItem(), 0);
	if(Bitmap != NULL)
		mImage.SetImage(Bitmap, NULL, TRUE);
}

void CTexturePreviewPage::OnLoadTexture()
{
	CFileDialog dlg( TRUE, NULL, NULL, OFN_NOCHANGEDIR|OFN_ALLOWMULTISELECT,
		"All Image Files (*.bmp;*.dds;*.jpg;*.jpeg;*.png;*.tga)|*.bmp;*.dds;*.jpg;*.jpeg;*.png;*.tga|\
		Windows BMP (*.bmp)|*.bmp|\
		DirectDraw Surface Bitmaps (*.dds)|*.dds|\
		JPEG Images (*.jpg;*.jpeg)|*.jpg;*.jpeg|\
		Portable Network Graphic (*.png)|*.png|\
		TARGA Images (*.tga;*.targa)|*.tga;*.targa|\
		All Files (*.*)|*.*||", NULL);
	
	const int MIN_FILE_NUMBER = 100;
	dlg.m_ofn.lpstrFile = new TCHAR[_MAX_PATH * MIN_FILE_NUMBER];         
	memset(dlg.m_ofn.lpstrFile, 0, _MAX_PATH * MIN_FILE_NUMBER);
	dlg.m_ofn.nMaxFile = _MAX_PATH * MIN_FILE_NUMBER;

	if(IDOK == dlg.DoModal())
	{
		CStringArray SA;

		POSITION Pos = dlg.GetStartPosition();
		while(Pos != NULL)
		{
			CString Filename = dlg.GetNextPathName(Pos);
			SA.Add(Filename);
		}

		if(!SA.IsEmpty())
		{
			mTextureBar.AddImages(SA);
		}
	}

	delete[] dlg.m_ofn.lpstrFile;
}

void CTexturePreviewPage::OnClearTexture()
{
	mTextureBar.ClearImages();
}

void CTexturePreviewPage::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}
