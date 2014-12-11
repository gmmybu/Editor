#include "stdafx.h"
#include "Editor.h"
#include "SceneDoc.h"

#include "TerrainEditHandler.h"
#include "TerrainBrush.h"
#include "TerrainEditPage.h"

#include "OgreSceneManager.h"
#include "OgreManualObject.h"
#include "Terrain\OgreTerrain.h"

#include "OIS\OISKeyboard.h"
#include "OIS\OISMouse.h"

CTerrainEditHandler::CTerrainEditHandler(SceneDoc *Owner)
{
	mOwner = Owner;
	mMode = TEM_NONE;

	mBrush = new CTerrainBrush(Owner);
	mBrush->SetSize(8);
	mBrush->SetPower(8);

	mHeightUpdateRate = 1.0f / 20.0f; // Update terrain at max 20fps
	mHeightUpdateCountDown = 0;
}


CTerrainEditHandler::~CTerrainEditHandler()
{
	// Was done by owner
}

void CTerrainEditHandler::Roaming(Ogre::TerrainGroup::RayResult rayResult, float Elapsed)
{
	if(mMode == TEM_NONE)
		return;

	if(mMode == TEM_SELECT && GetKeyState(VK_LBUTTON) & 0x8000)
	{
		CTerrainEditPage::Current->SetTerrain(rayResult.terrain);
		return;
	}

	mBrush->Roaming(rayResult.position);
	if(!(GetKeyState(VK_LBUTTON) & 0x8000))
		return;

	Ogre::Vector3 Center;
	rayResult.terrain->getTerrainPosition(rayResult.position, &Center);

	float BrushSize = 
		(float)mBrush->GetSize() / rayResult.terrain->getSize();

	if(mMode == TEM_DEFORM)
	{
		Ogre::Real TerrainSize = (rayResult.terrain->getSize() - 1);
		long StartX = (Center.x - BrushSize/2) * TerrainSize;
		long StartY = (Center.y - BrushSize/2) * TerrainSize;
		long EndX = (Center.x + BrushSize/2) * TerrainSize;
		long EndY= (Center.y + BrushSize/2) * TerrainSize;
		StartX = std::max(StartX, 0L);
		StartY = std::max(StartY, 0L);
		EndX = std::min(EndX, (long)TerrainSize);
		EndY = std::min(EndY, (long)TerrainSize);

		for(long Y = StartY; Y <= EndY; ++Y)
		{
			for (long X = StartX; X <= EndX; ++X)
			{
				float HeightDelta = mBrush->At(X - StartX, Y - StartY) * mBrush->GetPower() * Elapsed;
				float NewHeight;
				if(GetKeyState(VK_LBUTTON) & 0x8000)
					NewHeight = rayResult.terrain->getHeightAtPoint(X, Y) + HeightDelta;
				rayResult.terrain->setHeightAtPoint(X, Y, NewHeight);
			}
		}

		if(mHeightUpdateCountDown == 0)
			mHeightUpdateCountDown = mHeightUpdateRate;
	}
	else if(mMode = TEM_BLEND)
	{
		int EditLayer = CTerrainEditPage::Current->GetEditLayer();
		if(EditLayer <= 1)
			return;
		Ogre::TerrainLayerBlendMap *Layer = rayResult.terrain->getLayerBlendMap(EditLayer-1);

		Ogre::Real ImgSize = rayResult.terrain->getLayerBlendMapSize();
		long StartX = (Center.x - BrushSize/2) * ImgSize;
		long StartY = (Center.y - BrushSize/2) * ImgSize;
		long EndX = (Center.x + BrushSize/2) * ImgSize;
		long EndY= (Center.y + BrushSize/2) * ImgSize;
		StartX = std::max(StartX, 1L);
		StartY = std::max(StartY, 1L);
		EndX = std::min(EndX, (long)ImgSize);
		EndY = std::min(EndY, (long)ImgSize);

		for(long Y = StartY; Y < EndY; ++Y)
		{
			for (long X = StartX; X < EndX; ++X)
			{
				float xRate = float(X - StartX) / (EndX - StartX);
				float yRate = float(Y - StartY) / (EndY - StartY);
				float AlphaDelta = mBrush->At(xRate, yRate) * (mBrush->GetPower()/100.0f) * Elapsed;
				size_t ImgY = ImgSize - Y;
				float NewAlpha;
				if(GetKeyState(VK_LBUTTON) & 0x8000)
					NewAlpha = Layer->getBlendValue(X, ImgY) + AlphaDelta;
				NewAlpha = Ogre::Math::Clamp(NewAlpha, 0.0f, 1.0f);
				Layer->setBlendValue(X, ImgY, NewAlpha);
			}
		}

		Layer->update();
	}
}

void CTerrainEditHandler::SetMode(KTerrainEditMode Mode)
{
	mMode = Mode;
	switch(mMode)
	{
	case TEM_DEFORM:
	case TEM_BLEND:
		mBrush->SetVisible(true);
		break;

	default:
		mBrush->SetVisible(false);
	}
}

void CTerrainEditHandler::update(float Elapsed)
{
	if(mHeightUpdateCountDown > 0)
	{
		mHeightUpdateCountDown -= Elapsed;
		if(mHeightUpdateCountDown <= 0)
		{
			mOwner->getTerrainManager()->GetTerrainGroup()->update();
			mHeightUpdateCountDown = 0;
		}
	}
}
