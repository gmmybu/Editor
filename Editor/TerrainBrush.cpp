#include "stdafx.h"
#include "Editor.h"
#include "TerrainBrush.h"

#include "SceneDoc.h"
#include "TerrainManager.h"
#include "TerrainEditPage.h"
#include "StringUtils.h"

#include "OgreManualObject.h"
#include "OgreSceneManager.h"
#include "Terrain\OgreTerrain.h"

CTerrainBrush::CTerrainBrush(SceneDoc *Owner)
{
	mOwner = Owner;

	mSize = 0; mPower = 0;
	mBuff = NULL;
	mIndicator = mOwner->getSceneManager()->createManualObject("<TerrainBrushIndicator>");
	mIndicator->setQueryFlags(0);
	mIndicator->setCastShadows(false);
	mIndicator->setDynamic(true);
	mIndicator->setRenderQueueGroup(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_2);
	mIndicator->setVisible(false);
	mOwner->getSceneManager()->getRootSceneNode()->createChildSceneNode()->attachObject(mIndicator);
}

CTerrainBrush::~CTerrainBrush()
{
	if(mBuff != NULL)
		delete[] mBuff;
	
	// Was done by owner
}

int CTerrainBrush::GetSize()
{
	return mSize;
}

void CTerrainBrush::SetSize(int Size)
{
	if(mSize == Size)
		return;
	mSize = Size;
	
	if(mBuff != NULL)
		delete[] mBuff;
	
	mBuff = new float[mSize * mSize];
	Load(mBrushTexture);

	RebuildIndicator();
}

bool CTerrainBrush::IsVisible()
{
	return mIndicator->isVisible();
}

void CTerrainBrush::SetVisible(bool Visible)
{
	mIndicator->setVisible(Visible);
}

void CTerrainBrush::Load(CString Filename)
{
	mBrushTexture = Filename;
	if(mBrushTexture.GetLength() == 0)
	{
		memset(mBuff, 0, mSize * mSize * sizeof(float));
	}
	else
	{
		Ogre::String Path = StringUtils::internalPath(theApp.getPath("Brush"), std::string(mBrushTexture));

		Ogre::Image Image;

		Image.load(Ogre::String(Path), "General");
		Image.resize(mSize, mSize);

		int BuffLen = mSize*mSize;
		
		int Bpp = Image.getSize()/BuffLen;
		int Lmt = (1 << (Bpp * 8)) - 1;

		const unsigned char *ImageData = Image.getData();
		for(int i=0; i<BuffLen; i++)
		{
			int Value = 0;
			memcpy(&Value, ImageData, Bpp);
			mBuff[i] = float(Value) / Lmt;
			ImageData += Bpp;
		}
	}
}

float CTerrainBrush::At(float X, float Y)
{
	if(X >= 0 && X < 1.0f && Y >= 0 && Y < 1.0f)
	{
		int _X = X * mSize;
		int _Y = Y * mSize;
		return At(_X, _Y);
	}
	return 0;
}

float CTerrainBrush::At(int X, int Y)
{
	if(X >= 0 && X < mSize && Y >= 0 && Y < mSize)
		return mBuff[X+Y*mSize];
	return 0;
}

void CTerrainBrush::Roaming(Ogre::Vector3 Center)
{
	if(CTerrainEditPage::Current->GetTerrain() == NULL)
		return;

	int brushWorldSize = mOwner->getTerrainManagerConfig()->WorldSize / 
		mOwner->getTerrainManagerConfig()->TerrainSize * mSize;
	float inverseSize = 1.0f / brushWorldSize;

	POINT LeftTop;
	LeftTop.x = Center.x - brushWorldSize/2;
	LeftTop.y = Center.z - brushWorldSize/2;

	// 网格
	//size_t sectionIndex(0);
	//for(int i = 0; i <= BrushWorldSize; i += BrushWorldSize)
	//{
	//	int posX = LeftTop.x + i;
	//	mIndicator->beginUpdate(sectionIndex++);
	//	for (int j = 0; j <= BrushWorldSize; j += BrushWorldSize)
	//	{
	//		int posZ = LeftTop.y + (int)j;
	//		float TerrainHeight = CTerrainEditPage::Current->GetTerrain()->getHeightAtWorldPosition(
	//			Ogre::Vector3(posX, 0, posZ));
	//		float Height = TerrainHeight + 1.0f;
	//		mIndicator->position(posX, Height, posZ);
	//	}
	//	mIndicator->end();
	//}
	//for (int j = 0; j <= BrushWorldSize; j += BrushWorldSize)
	//{
	//	int posZ = LeftTop.y + (int)j;
	//	mIndicator->beginUpdate(sectionIndex++);
	//	for (int i = 0; i <= BrushWorldSize; i += BrushWorldSize)
	//	{
	//		int posX = LeftTop.x + (int)i;
	//		float TerrainHeight = CTerrainEditPage::Current->GetTerrain()->getHeightAtWorldPosition(
	//			Ogre::Vector3(posX, 0, posZ));
	//		float Height = TerrainHeight + 1.0f;
	//		mIndicator->position(posX, Height, posZ);
	//	}
	//	mIndicator->end();
	//}

	// 刷子
	mIndicator->beginUpdate(0);
	mIndicator->position(LeftTop.x, 1.0f, LeftTop.y);
	mIndicator->textureCoord(0, 0);

	mIndicator->position(LeftTop.x, 1.0f, LeftTop.y + brushWorldSize);
	mIndicator->textureCoord(0, 1.0f);

	mIndicator->position(LeftTop.x + brushWorldSize, 1.0f, LeftTop.y);
	mIndicator->textureCoord(1.0f, 0);

	mIndicator->position(LeftTop.x + brushWorldSize, 1.0f, LeftTop.y + brushWorldSize);
	mIndicator->textureCoord(1.0f, 1.0f);

	mIndicator->index(1);
	mIndicator->index(3);
	mIndicator->index(2);

	mIndicator->index(1);
	mIndicator->index(2);
	mIndicator->index(0);
	mIndicator->end();
}

void CTerrainBrush::RebuildIndicator()
{
	int brushWorldSize = mOwner->getTerrainManagerConfig()->WorldSize / 
		mOwner->getTerrainManagerConfig()->TerrainSize * mSize;
	float inverseSize = 1.0f / brushWorldSize;

	// 网格
	//mIndicator->clear();	
	//for(int i = 0; i <= BrushWorldSize; i += BrushWorldSize)
	//{
	//	mIndicator->begin("Editor/BrushIndicatorMat", Ogre::RenderOperation::OT_LINE_STRIP);
	//	for(int j = 0; j <= BrushWorldSize; j += BrushWorldSize)
	//		mIndicator->position(i, 0, j);
	//	mIndicator->end();
	//}
	//for(int j = 0; j <= BrushWorldSize; j  += BrushWorldSize)
	//{
	//	mIndicator->begin("Editor/BrushIndicatorMat", Ogre::RenderOperation::OT_LINE_STRIP);
	//	for(int i = 0; i <= BrushWorldSize; i  += BrushWorldSize)
	//		mIndicator->position(i, 0, j);
	//	mIndicator->end();
	//}

	// 刷子
	mIndicator->clear();
	mIndicator->begin("Editor/BrushIndicatorMat", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	mIndicator->position(0, 1.0f, 0);
	mIndicator->textureCoord(0, 0);

	mIndicator->position(0, 1.0f, brushWorldSize);
	mIndicator->textureCoord(0, 1.0f);

	mIndicator->position(brushWorldSize, 1.0f, 0);
	mIndicator->textureCoord(1.0f, 0);

	mIndicator->position(brushWorldSize, 1.0f, brushWorldSize);
	mIndicator->textureCoord(1.0f, 1.0f);

	mIndicator->index(1);
	mIndicator->index(3);
	mIndicator->index(2);

	mIndicator->index(1);
	mIndicator->index(2);
	mIndicator->index(0);
	mIndicator->end();
}
