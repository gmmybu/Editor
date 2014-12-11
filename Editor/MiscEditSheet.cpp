#include "stdafx.h"
#include "Editor.h"
#include "MiscEditSheet.h"

#include "TexturePreviewPage.h"
#include "MaterialPreviewPage.h"
#include "TerrainEditPage.h"

IMPLEMENT_DYNAMIC(CMiscEditSheet, CBCGPTabWnd)

CMiscEditSheet::CMiscEditSheet()
{

}

CMiscEditSheet::~CMiscEditSheet()
{
}

BEGIN_MESSAGE_MAP(CMiscEditSheet, CBCGPTabWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CMiscEditSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CBCGPTabWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	mTabs[MES_TEXTURE_PREVIEW] = new CTexturePreviewPage;
	mTabs[MES_TEXTURE_PREVIEW]->Create(IDD_TEXTURE_PREVIEW, this);
	InsertTab(mTabs[MES_TEXTURE_PREVIEW], "ÎÆÀíÔ¤ÀÀ", MES_TEXTURE_PREVIEW);

	mTabs[MES_MATERIAL_PREVIEW] = new CMaterialPreviewPage;
	mTabs[MES_MATERIAL_PREVIEW]->Create(IDD_MATERIAL_PREVIEW, this);
	InsertTab(mTabs[MES_MATERIAL_PREVIEW], "²ÄÖÊÔ¤ÀÀ", MES_MATERIAL_PREVIEW);

	mTabs[MES_TERRAIN_EDIT] = new CTerrainEditPage;
	mTabs[MES_TERRAIN_EDIT]->Create(IDD_TERRAIN_EDIT, this);
	InsertTab(mTabs[MES_TERRAIN_EDIT], "µØÐÎ±à¼­", MES_TERRAIN_EDIT);

	return 0;
}
