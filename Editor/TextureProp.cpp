#include "stdafx.h"
#include "Editor.h"
#include "TextureProp.h"
#include "TexturePopup.h"
#include "StringUtils.h"

TextureProp::TextureProp(const CString &strName, const CString &strResName, LPCTSTR lpszDesc):
	CBCGPProp(strName, _variant_t((LPCTSTR)strResName), lpszDesc)
{
	m_dwFlags = 1;
}


TextureProp::~TextureProp(void)
{
}

void TextureProp::ShowFileDlg()
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
			SetValue(StringUtils::internalPath(theApp.getPath("Paint"), std::string(texName)).c_str());
			m_pWndList->OnPropertyChanged(this);
		}
	}
}

void TextureProp::OnClickButton (CPoint /*point*/)
{
	m_bButtonIsDown = TRUE;
	Redraw ();

	CString path = m_varValue;
	if(!path.IsEmpty())
		path = StringUtils::externalPath(theApp.getPath("Paint"), std::string(path)).c_str();
	TexturePopup *pPopup = new TexturePopup(this, path);
	CPoint pt(m_pWndList->GetListRect().left + m_pWndList->GetLeftColumnWidth() + 1, m_rectButton.bottom + 1);
	m_pWndList->ClientToScreen(&pt);

	if(!pPopup->Create(m_pWndList, pt.x, pt.y, NULL, FALSE))
	{
		pPopup = NULL;
	}
	else
	{
		pPopup->SetFocus();
	}

	m_bButtonIsDown = FALSE;
	Redraw();

	if(m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}
}
