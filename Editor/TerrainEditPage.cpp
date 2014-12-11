#include "stdafx.h"
#include "afxdialogex.h"
#include "Editor.h"
#include "SceneDoc.h"

#include "TerrainEditPage.h"
#include "TerrainEditHandler.h"
#include "TerrainBrush.h"
#include "TerrainManager.h"
#include "MiscUtils.h"
#include "StringUtils.h"

IMPLEMENT_DYNAMIC(CTerrainEditPage, CBCGPDialog)

CTerrainEditPage *CTerrainEditPage::Current = NULL;
CTerrainEditPage::CTerrainEditPage(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CTerrainEditPage::IDD, pParent)
	,mTerrain(NULL)
	,mBrushText("画刷")
	,mSizeText("尺寸: 8")
	,mPowerText("强度: 8")
	,mTextureText("纹理: 0")
	,mEditLayer(0)
{
	Current = this;
}

CTerrainEditPage::~CTerrainEditPage()
{
}

void CTerrainEditPage::SetTerrain(Ogre::Terrain *Terrain)
{
	mTerrain = Terrain;
	update();
}

int CTerrainEditPage::GetBrushSize()
{
	return mSizeSlider.GetPos();
}

int CTerrainEditPage::GetBrushPower()
{
	return mPowerSlider.GetPos();
}

void CTerrainEditPage::update()
{
	if(!mBrushBar.IsWindowEnabled())
		mBrushBar.EnableWindow(TRUE);

	if(!mSizeSlider.IsWindowEnabled())
	{
		mSizeSlider.EnableWindow(TRUE);
		mSizeSlider.SetRange(1, SceneDoc::current->getTerrainManagerConfig()->TerrainSize);
		mSizeSlider.SetPos(8);
	}

	if(!mPowerSlider.IsWindowEnabled())
	{
		mPowerSlider.EnableWindow(TRUE);
		mPowerSlider.SetRange(-100, 100);
		mPowerSlider.SetPos(8);
	}

	if(!mTextureSlider.IsWindowEnabled())
	{
		mTextureSlider.EnableWindow(TRUE);
		mTextureSlider.SetRange(1, SceneDoc::current->getTerrainManagerConfig()->TerrainSize);
		mTextureSlider.SetPos(SceneDoc::current->getTerrainManagerConfig()->TerrainSize);
	}

	Ogre::uint8 LayerCount = mTerrain->getLayerCount();
	for(int i=0; i<LayerCount; i++)
	{
		mLayers[i].SetEnable(TRUE);
		Ogre::String DiffuseSpecular = mTerrain->getLayerTextureName(i, 0);
		mLayers[i].SetDiffuseSpecular(DiffuseSpecular.c_str());
		Ogre::String NormalHeight = mTerrain->getLayerTextureName(i, 1);
		mLayers[i].SetNormalHeight(NormalHeight.c_str());
	}
	for(int i=LayerCount; i<5; i++)
	{
		mLayers[i].SetEnable(FALSE);
		mLayers[i].SetDiffuseSpecular(NULL);
		mLayers[i].SetNormalHeight(NULL);
	}
	if(!mLayers[0].IsSideEnable())
		mLayers[0].SetSideEnable(TRUE); // 当第一层也被删掉时
}

BEGIN_MESSAGE_MAP(CTerrainEditPage, CBCGPDialog)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_COMMAND(ID_BRUSH_BAR, OnBrushBar)
	ON_COMMAND(ID_LOAD_BRUSH, OnLoadBrush)
	ON_COMMAND(ID_CLEAR_BRUSH, OnClearBrush)
	ON_COMMAND(ID_RESIZE_BRUSH, OnResizeBrush)
	ON_COMMAND(ID_LOAD_DIFFUSESPECULAR, OnLoadDiffuseSpecular)
	ON_COMMAND(ID_LOAD_NORMALHEIGHT, OnLoadNormalHeight)
	ON_COMMAND(ID_LAYER1, OnLayer1)
	ON_COMMAND(ID_LAYER2, OnLayer2)
	ON_COMMAND(ID_LAYER3, OnLayer3)
	ON_COMMAND(ID_LAYER4, OnLayer4)
	ON_COMMAND(ID_LAYER5, OnLayer5)
	ON_COMMAND(ID_LAYER1_LOAD_DIFFUSE_SPECULAR, OnLayer1LoadDiffuseSpecular) // Layer1
	ON_COMMAND(ID_LAYER1_LOAD_NORMAL_HEIGHT, OnLayer1LoadNormalHeight)
	ON_COMMAND(ID_LAYER1_ADD_LAYER_PREV, OnLayer1AddLayerPrev)
	ON_COMMAND(ID_LAYER1_ADD_LAYER_NEXT, OnLayer1AddLayerNext)
	ON_COMMAND(ID_LAYER1_DEL_LAYER, OnLayer1DelLayer)
	ON_COMMAND(ID_LAYER1_DEL_NORMAL_HEIGHT, OnLayer1DelNormalHeight)

	ON_COMMAND(ID_LAYER2_LOAD_DIFFUSE_SPECULAR, OnLayer2LoadDiffuseSpecular) // Layer2
	ON_COMMAND(ID_LAYER2_LOAD_NORMAL_HEIGHT, OnLayer2LoadNormalHeight)
	ON_COMMAND(ID_LAYER2_ADD_LAYER_PREV, OnLayer2AddLayerPrev)
	ON_COMMAND(ID_LAYER2_ADD_LAYER_NEXT, OnLayer2AddLayerNext)
	ON_COMMAND(ID_LAYER2_DEL_LAYER, OnLayer2DelLayer)
	ON_COMMAND(ID_LAYER2_DEL_NORMAL_HEIGHT, OnLayer2DelNormalHeight)

	ON_COMMAND(ID_LAYER3_LOAD_DIFFUSE_SPECULAR, OnLayer3LoadDiffuseSpecular) // Layer3
	ON_COMMAND(ID_LAYER3_LOAD_NORMAL_HEIGHT, OnLayer3LoadNormalHeight)
	ON_COMMAND(ID_LAYER3_ADD_LAYER_PREV, OnLayer3AddLayerPrev)
	ON_COMMAND(ID_LAYER3_ADD_LAYER_NEXT, OnLayer3AddLayerNext)
	ON_COMMAND(ID_LAYER3_DEL_LAYER, OnLayer3DelLayer)
	ON_COMMAND(ID_LAYER3_DEL_NORMAL_HEIGHT, OnLayer3DelNormalHeight)

	ON_COMMAND(ID_LAYER4_LOAD_DIFFUSE_SPECULAR, OnLayer4LoadDiffuseSpecular) // Layer4
	ON_COMMAND(ID_LAYER4_LOAD_NORMAL_HEIGHT, OnLayer4LoadNormalHeight)
	ON_COMMAND(ID_LAYER4_ADD_LAYER_PREV, OnLayer4AddLayerPrev)
	ON_COMMAND(ID_LAYER4_ADD_LAYER_NEXT, OnLayer4AddLayerNext)
	ON_COMMAND(ID_LAYER4_DEL_LAYER, OnLayer4DelLayer)
	ON_COMMAND(ID_LAYER4_DEL_NORMAL_HEIGHT, OnLayer4DelNormalHeight)

	ON_COMMAND(ID_LAYER5_LOAD_DIFFUSE_SPECULAR, OnLayer5LoadDiffuseSpecular) // Layer5
	ON_COMMAND(ID_LAYER5_LOAD_NORMAL_HEIGHT, OnLayer5LoadNormalHeight)
	ON_COMMAND(ID_LAYER5_ADD_LAYER_PREV, OnLayer5AddLayerPrev)
	ON_COMMAND(ID_LAYER5_ADD_LAYER_NEXT, OnLayer5AddLayerNext)
	ON_COMMAND(ID_LAYER5_DEL_LAYER, OnLayer5DelLayer)
	ON_COMMAND(ID_LAYER5_DEL_NORMAL_HEIGHT, OnLayer5DelNormalHeight)

END_MESSAGE_MAP()

int CTerrainEditPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CBCGPDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect;
	rect.SetRectEmpty();

	mBrushBar.Create(rect, this, ID_BRUSH_BAR);
	mBrushBar.AddMenuItem(ID_LOAD_BRUSH, "载入画刷");
	mBrushBar.AddMenuItem(ID_CLEAR_BRUSH, "清除画刷");
	mBrushBar.AddMenuItem(ID_RESIZE_BRUSH, "切换图标尺寸");
	mBrushBar.EnableWindow(FALSE);

	mSizeSlider.Create(WS_CHILD | WS_VISIBLE, rect, this, ID_SIZE_SLIDER);
	mSizeSlider.m_bVisualManagerStyle = TRUE;
	mSizeSlider.SetPos(8);
	mSizeSlider.EnableWindow(FALSE);

	mPowerSlider.Create(WS_CHILD | WS_VISIBLE, rect, this, ID_POWER_SLIDER);
	mPowerSlider.m_bVisualManagerStyle = TRUE;
	mPowerSlider.SetPos(8);
	mPowerSlider.EnableWindow(FALSE);

	mTextureSlider.Create(WS_CHILD | WS_VISIBLE, rect, this, ID_TEXTURE_SLIDER);
	mTextureSlider.m_bVisualManagerStyle = TRUE;
	mTextureSlider.SetPos(8);
	mTextureSlider.EnableWindow(FALSE);

	mResourceLayer.Create(this, ID_LOAD_DIFFUSESPECULAR, ID_LOAD_NORMALHEIGHT);

	mLayers[0].Create(this, ID_LAYER1, TRUE, ID_LAYER1_LOAD_DIFFUSE_SPECULAR, ID_LAYER1_LOAD_NORMAL_HEIGHT, ID_LAYER1_ADD_LAYER_PREV, ID_LAYER1_ADD_LAYER_NEXT, ID_LAYER1_DEL_LAYER, ID_LAYER1_DEL_NORMAL_HEIGHT);
	mLayers[0].SetEnable(FALSE);
	mLayers[1].Create(this, ID_LAYER2, FALSE, ID_LAYER2_LOAD_DIFFUSE_SPECULAR, ID_LAYER2_LOAD_NORMAL_HEIGHT, ID_LAYER2_ADD_LAYER_PREV, ID_LAYER2_ADD_LAYER_NEXT, ID_LAYER2_DEL_LAYER, ID_LAYER2_DEL_NORMAL_HEIGHT);
	mLayers[1].SetEnable(FALSE);
	mLayers[2].Create(this, ID_LAYER3, FALSE, ID_LAYER3_LOAD_DIFFUSE_SPECULAR, ID_LAYER3_LOAD_NORMAL_HEIGHT, ID_LAYER3_ADD_LAYER_PREV, ID_LAYER3_ADD_LAYER_NEXT, ID_LAYER3_DEL_LAYER, ID_LAYER3_DEL_NORMAL_HEIGHT);
	mLayers[2].SetEnable(FALSE);
	mLayers[3].Create(this, ID_LAYER4, FALSE, ID_LAYER4_LOAD_DIFFUSE_SPECULAR, ID_LAYER4_LOAD_NORMAL_HEIGHT, ID_LAYER4_ADD_LAYER_PREV, ID_LAYER4_ADD_LAYER_NEXT, ID_LAYER4_DEL_LAYER, ID_LAYER4_DEL_NORMAL_HEIGHT);
	mLayers[3].SetEnable(FALSE);
	mLayers[4].Create(this, ID_LAYER5, FALSE, ID_LAYER5_LOAD_DIFFUSE_SPECULAR, ID_LAYER5_LOAD_NORMAL_HEIGHT, ID_LAYER5_ADD_LAYER_PREV, ID_LAYER5_ADD_LAYER_NEXT, ID_LAYER5_DEL_LAYER, ID_LAYER5_DEL_NORMAL_HEIGHT);
	mLayers[4].SetEnable(FALSE);

	return 0;
}

void CTerrainEditPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch(pScrollBar->GetDlgCtrlID())   
	{   
	case ID_SIZE_SLIDER:
		{ 
			SceneDoc::current->getTerrainEditHandler()->GetBrush()->SetSize(mSizeSlider.GetPos());
			mSizeText.Format("尺寸：%d", mSizeSlider.GetPos());
			InvalidateRect(&mSizeBox.ConvertToRect());
		} 
		break;
	case ID_POWER_SLIDER:
		{
			SceneDoc::current->getTerrainEditHandler()->GetBrush()->SetPower(mPowerSlider.GetPos());
			mPowerText.Format("强度：%d", mPowerSlider.GetPos());
			InvalidateRect(&mPowerBox.ConvertToRect());
		}
		break;

	case ID_TEXTURE_SLIDER:
		{
			mTextureText.Format("纹理:%d", mTextureSlider.GetPos());
			InvalidateRect(&mTextureBox.ConvertToRect());
		}
	}
	CBCGPDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTerrainEditPage::OnPaint()
{
	CPaintDC PaintDC(this); // device context for painting
	CBCGPMemDC MemDC(PaintDC, this);
	CDC *DC = &MemDC.GetDC();

	CRect Client;
	GetClientRect(Client);

	CBCGPVisualManager::GetInstance()->OnFillDialog(DC, this, Client);
	DC->SetBkMode(TRANSPARENT); // 否则文字将存在背景色

	CFont Font;
	Font.Attach(::GetStockObject(DEFAULT_GUI_FONT));
	CFont *OldFont = DC->SelectObject(&Font);

	DC->SetTextColor(RGB(30, 30, 30));
	DC->DrawText(mBrushText, &mBrushBox.ConvertToRect(), 0);
	DC->DrawText(mSizeText, &mSizeBox.ConvertToRect(), 0);
	DC->DrawText(mPowerText, &mPowerBox.ConvertToRect(), 0);
	DC->DrawText(mTextureText, &mTextureBox.ConvertToRect(), 0);
	DC->SelectObject(OldFont);
}

void CTerrainEditPage::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);
	int BarHeight = (cy>>2);

	mBrushBox.left = 5;
	mBrushBox.top = 5;
	mBrushBox.width = cx-10;
	mBrushBox.height = 20;

	CBox BrushBarBox;
	BrushBarBox.left = 5;
	BrushBarBox.top = mBrushBox.bottom()+3;
	BrushBarBox.width = cx-10;
	BrushBarBox.height = BarHeight;
	mBrushBar.SetWindowPos(NULL, BrushBarBox.left, BrushBarBox.top, BrushBarBox.width, BrushBarBox.height, SWP_NOACTIVATE|SWP_NOZORDER);

	mSizeBox.left = 5;
	mSizeBox.top = BrushBarBox.bottom()+3;
	mSizeBox.width = 55;
	mSizeBox.height = 20;

	CBox SizeSliderBox;
	SizeSliderBox.left = mSizeBox.right()+5;
	SizeSliderBox.top = mSizeBox.top;
	SizeSliderBox.width = cx-SizeSliderBox.left-10;
	SizeSliderBox.height = 20;
	mSizeSlider.SetWindowPos(NULL, SizeSliderBox.left, SizeSliderBox.top, SizeSliderBox.width, SizeSliderBox.height, SWP_NOACTIVATE|SWP_NOZORDER);

	mPowerBox.left = 5;
	mPowerBox.top = SizeSliderBox.bottom()+3;
	mPowerBox.width = 55;
	mPowerBox.height = 20;

	CBox PowerSliderBox;
	PowerSliderBox.left = mPowerBox.right()+5;
	PowerSliderBox.top = mPowerBox.top;
	PowerSliderBox.width = cx-PowerSliderBox.left-10;
	PowerSliderBox.height = 20;
	mPowerSlider.SetWindowPos(NULL, PowerSliderBox.left, PowerSliderBox.top, PowerSliderBox.width, PowerSliderBox.height, SWP_NOACTIVATE|SWP_NOZORDER);
		
	mTextureBox.left = 5;
	mTextureBox.top = PowerSliderBox.bottom()+3;
	mTextureBox.width = 55;
	mTextureBox.height = 20;

	CBox TextureSliderBox;
	TextureSliderBox.left = mTextureBox.right()+5;
	TextureSliderBox.top = mTextureBox.top;
	TextureSliderBox.width = cx-TextureSliderBox.left-10;
	TextureSliderBox.height = 20;
	mTextureSlider.SetWindowPos(NULL, TextureSliderBox.left, TextureSliderBox.top, TextureSliderBox.width, TextureSliderBox.height, SWP_NOACTIVATE|SWP_NOZORDER);

	CBox ResourceLayerBox;
	ResourceLayerBox.left = 5;
	ResourceLayerBox.top = TextureSliderBox.bottom()+5;
	ResourceLayerBox.width = 153;
	ResourceLayerBox.height = 84; // 小纹理与替换按钮的高度和
	mResourceLayer.SetPosition(CPoint(ResourceLayerBox.left, ResourceLayerBox.top));

	CBox Layer1Box;
	Layer1Box.left = 183; // 5+15(Radio)+5+64(1)+5+64(2)+5+15(+-)+5
	Layer1Box.top = TextureSliderBox.bottom()+5;
	Layer1Box.width = 153;
	Layer1Box.height = 84; // 小纹理与替换按钮的高度和
	mLayers[0].SetPosition(CPoint(Layer1Box.left, Layer1Box.top));

	CBox Layer2Box;
	Layer2Box.left = 5;
	Layer2Box.top = ResourceLayerBox.bottom()+5;
	Layer2Box.width = 153;
	Layer2Box.height = 84; // 小纹理与替换按钮的高度和
	mLayers[1].SetPosition(CPoint(Layer2Box.left, Layer2Box.top));

	CBox Layer3Box;
	Layer3Box.left = 183;
	Layer3Box.top = Layer1Box.bottom()+5;
	Layer3Box.width = 153;
	Layer3Box.height = 84; // 小纹理与替换按钮的高度和
	mLayers[2].SetPosition(CPoint(Layer3Box.left, Layer3Box.top));

	CBox Layer4Box;
	Layer4Box.left = 5;
	Layer4Box.top = Layer2Box.bottom()+5;
	Layer4Box.width = 153;
	Layer4Box.height = 84; // 小纹理与替换按钮的高度和
	mLayers[3].SetPosition(CPoint(Layer4Box.left, Layer4Box.top));

	CBox Layer5Box;
	Layer5Box.left = 183;
	Layer5Box.top = Layer3Box.bottom()+5;
	Layer5Box.width = 153;
	Layer5Box.height = 84; // 小纹理与替换按钮的高度和
	mLayers[4].SetPosition(CPoint(Layer5Box.left, Layer5Box.top));
}

void CTerrainEditPage::OnBrushBar()
{
	mBrushBar.FillPropertyWnd();
	SceneDoc::current->getTerrainEditHandler()->GetBrush()->Load(mBrushBar.GetSelectedItem());
}

void CTerrainEditPage::OnLoadBrush()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_NOCHANGEDIR|OFN_ALLOWMULTISELECT,
		"All Image Files (*.bmp;*.dds;*.jpg;*.jpeg;*.png;*.tga)|*.bmp;*.dds;*.jpg;*.jpeg;*.png;*.tga|\
		Windows BMP (*.bmp)|*.bmp|\
		DirectDraw Surface Bitmaps (*.dds)|*.dds|\
		JPEG Images (*.jpg;*.jpeg)|*.jpg;*.jpeg|\
		Portable Network Graphic (*.png)|*.png|\
		TARGA Images (*.tga;*.targa)|*.tga;*.targa|\
		All Files (*.*)|*.*||", NULL);
	
	TCHAR pathBuffer[MAX_PATH];
	::GetFullPathName(theApp.getPath("Brush").c_str(), MAX_PATH, pathBuffer, NULL);
	dlg.m_ofn.lpstrInitialDir = pathBuffer;

	const int MIN_FILE_NUMBER = 100;
	dlg.m_ofn.lpstrFile = new TCHAR[_MAX_PATH * MIN_FILE_NUMBER];         
	memset(dlg.m_ofn.lpstrFile, 0, _MAX_PATH * MIN_FILE_NUMBER);
	dlg.m_ofn.nMaxFile = _MAX_PATH * MIN_FILE_NUMBER;

	if(IDOK == dlg.DoModal())
	{
		int baseLen = lstrlen(pathBuffer);

		CStringArray sa;
		POSITION pos = dlg.GetStartPosition();
		while(NULL != pos)
		{
			CString fileName = dlg.GetNextPathName(pos);
			if(fileName.Left(baseLen) == pathBuffer) // 过滤掉Ogre未加载的
			{
				sa.Add(fileName);
			}
			else
				break;	
		}
		if(!sa.IsEmpty())
		{
			mBrushBar.AddImages(sa);
		}
	}

	delete[] dlg.m_ofn.lpstrFile;
}

void CTerrainEditPage::OnClearBrush()
{
	mBrushBar.ClearImages();
}

void CTerrainEditPage::OnResizeBrush()
{
	if(mBrushBar.GetIconSize() != 32)
		mBrushBar.ResizeIcon(32);
	else
		mBrushBar.ResizeIcon(64);
}

void CTerrainEditPage::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}

void CTerrainEditPage::OnLoadDiffuseSpecular()
{
	mResourceLayer.LoadDiffuseSpecular();
}

void CTerrainEditPage::OnLoadNormalHeight()
{
	mResourceLayer.LoadNormalHeight();
}

void CTerrainEditPage::OnLayer1()
{
	mEditLayer = 1;
}

void CTerrainEditPage::OnLayer2()
{
	mEditLayer = 2;
}

void CTerrainEditPage::OnLayer3()
{
	mEditLayer = 3;
}

void CTerrainEditPage::OnLayer4()
{
	mEditLayer = 4;
}

void CTerrainEditPage::OnLayer5()
{
	mEditLayer = 5;
}

// Layer1
void CTerrainEditPage::OnLayer1LoadDiffuseSpecular()
{
	LoadDiffuseSpecular(1);
}

void CTerrainEditPage::OnLayer1LoadNormalHeight()
{
	LoadNormalHeight(1);
}

void CTerrainEditPage::OnLayer1AddLayerPrev()
{
	AddLayerPrev(1);
}

void CTerrainEditPage::OnLayer1AddLayerNext()
{
	AddLayerNext(1);
}

void CTerrainEditPage::OnLayer1DelLayer()
{
	DelLayer(1);
}

void CTerrainEditPage::OnLayer1DelNormalHeight()
{
	DelNormalHeight(1);
}

// Layer2
void CTerrainEditPage::OnLayer2LoadDiffuseSpecular()
{
	LoadDiffuseSpecular(2);
}

void CTerrainEditPage::OnLayer2LoadNormalHeight()
{
	LoadNormalHeight(2);
}

void CTerrainEditPage::OnLayer2AddLayerPrev()
{
	AddLayerPrev(2);
}

void CTerrainEditPage::OnLayer2AddLayerNext()
{
	AddLayerNext(2);
}

void CTerrainEditPage::OnLayer2DelLayer()
{
	DelLayer(2);
}

void CTerrainEditPage::OnLayer2DelNormalHeight()
{
	DelNormalHeight(2);
}

// Layer3
void CTerrainEditPage::OnLayer3LoadDiffuseSpecular()
{
	LoadDiffuseSpecular(3);
}

void CTerrainEditPage::OnLayer3LoadNormalHeight()
{
	LoadNormalHeight(3);
}

void CTerrainEditPage::OnLayer3AddLayerPrev()
{
	AddLayerPrev(3);
}

void CTerrainEditPage::OnLayer3AddLayerNext()
{
	AddLayerNext(3);
}

void CTerrainEditPage::OnLayer3DelLayer()
{
	DelLayer(3);
}

void CTerrainEditPage::OnLayer3DelNormalHeight()
{
	DelNormalHeight(3);
}

// Layer4
void CTerrainEditPage::OnLayer4LoadDiffuseSpecular()
{
	LoadDiffuseSpecular(4);
}

void CTerrainEditPage::OnLayer4LoadNormalHeight()
{
	LoadNormalHeight(4);
}

void CTerrainEditPage::OnLayer4AddLayerPrev()
{
	AddLayerPrev(4);
}

void CTerrainEditPage::OnLayer4AddLayerNext()
{
	AddLayerNext(4);
}

void CTerrainEditPage::OnLayer4DelLayer()
{
	DelLayer(4);
}

void CTerrainEditPage::OnLayer4DelNormalHeight()
{
	DelNormalHeight(4);
}

// Layer5
void CTerrainEditPage::OnLayer5LoadDiffuseSpecular()
{
	LoadDiffuseSpecular(5);
}

void CTerrainEditPage::OnLayer5LoadNormalHeight()
{
	LoadNormalHeight(5);
}

void CTerrainEditPage::OnLayer5AddLayerPrev()
{
	AddLayerPrev(5);
}

void CTerrainEditPage::OnLayer5AddLayerNext()
{
	AddLayerNext(5);
}

void CTerrainEditPage::OnLayer5DelLayer()
{
	DelLayer(5);
}

void CTerrainEditPage::OnLayer5DelNormalHeight()
{
	DelNormalHeight(5);
}

void CTerrainEditPage::LoadDiffuseSpecular(int Layer)
{
	mLayers[Layer-1].LoadDiffuseSpecular();
	CString DS = mLayers[Layer-1].GetDiffuseSpecular();
	DS = StringUtils::internalPath(theApp.getPath("Paint"), std::string(DS)).c_str();
	mTerrain->setLayerTextureName(
		Layer-1, 0, Ogre::String(DS));
	SceneDoc::current->getTerrainManager()->GetTerrainGroup()->update();
}

void CTerrainEditPage::LoadNormalHeight(int Layer)
{
	mLayers[Layer-1].LoadNormalHeight();
	CString NH = mLayers[Layer-1].GetNormalHeight();
	NH = StringUtils::internalPath(theApp.getPath("Paint"), std::string(NH)).c_str();
	mTerrain->setLayerTextureName(
		Layer-1, 1, Ogre::String(NH));
	SceneDoc::current->getTerrainManager()->GetTerrainGroup()->update();
}

void CTerrainEditPage::AddLayerPrev(int Layer)
{
	CString DiffuseSpecular = 
		mResourceLayer.GetDiffuseSpecular();
	DiffuseSpecular = StringUtils::internalPath(theApp.getPath("Paint"), std::string(DiffuseSpecular)).c_str();
	CString NormalHeight = 
		mResourceLayer.GetNormalHeight();
	NormalHeight = StringUtils::internalPath(theApp.getPath("Paint"), std::string(NormalHeight)).c_str();
	if(	DiffuseSpecular.GetLength() != 0 &&
		NormalHeight.GetLength() != 0)
	{
		if(mTerrain->getLayerCount() < 5)
		{
			Ogre::StringVector SV;
			SV.push_back(Ogre::String(DiffuseSpecular));
			SV.push_back(Ogre::String(NormalHeight));
			mTerrain->addLayer(Layer-1, mTextureSlider.GetPos(), &SV);
			mTerrain->update();
			update();
		}
		else
			AfxMessageBox("层数达到最大");
	}
}

void CTerrainEditPage::AddLayerNext(int Layer)
{
	CString DiffuseSpecular = 
		mResourceLayer.GetDiffuseSpecular();
	DiffuseSpecular = StringUtils::internalPath(theApp.getPath("Paint"), std::string(DiffuseSpecular)).c_str();
	CString NormalHeight = 
		mResourceLayer.GetNormalHeight();
	NormalHeight = StringUtils::internalPath(theApp.getPath("Paint"), std::string(NormalHeight)).c_str();
	if(	DiffuseSpecular.GetLength() != 0 &&
		NormalHeight.GetLength() != 0)
	{
		if(mTerrain->getLayerCount() < 5)
		{
			Ogre::StringVector SV;
			SV.push_back(Ogre::String(DiffuseSpecular));
			SV.push_back(Ogre::String(NormalHeight));
			mTerrain->addLayer(Layer, mTextureSlider.GetPos(), &SV);
			mTerrain->update();
			update();
		}
		else
			AfxMessageBox("层数达到最大");
	}
}

void CTerrainEditPage::DelLayer(int Layer)
{
	mTerrain->removeLayer(Layer-1);
	mTerrain->update();
	update();
}

void CTerrainEditPage::DelNormalHeight(int Layer)
{
	mLayers[Layer-1].DelNormalHeight();
	mTerrain->setLayerTextureName(
		Layer-1, 1, "");
	SceneDoc::current->getTerrainManager()->GetTerrainGroup()->update();
}
