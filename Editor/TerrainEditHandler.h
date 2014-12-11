#pragma once

#include "resource.h"
#include "Terrain\OgreTerrainGroup.h"

class SceneDoc;
class CTerrainBrush;

enum KTerrainEditMode
{	
	TEM_NONE,
	TEM_SELECT = ID_SELECT_TERRAIN,
	TEM_DEFORM = ID_DEFORM_TERRAIN,
	TEM_BLEND  = ID_BLEND_TERRAIN
};

class CTerrainEditHandler
{
public:
	CTerrainEditHandler(SceneDoc *Owner);
	~CTerrainEditHandler();

	KTerrainEditMode GetMode() { return mMode; }
	CTerrainBrush *GetBrush() { return mBrush; }

	void Roaming(Ogre::TerrainGroup::RayResult rayResult, float Elapsed);
	void SetMode(KTerrainEditMode Mode);
	void update(float Elapsed);

private:
	SceneDoc *mOwner;
	KTerrainEditMode mMode;
	CTerrainBrush *mBrush;

	float mHeightUpdateRate;
	float mHeightUpdateCountDown;
};
