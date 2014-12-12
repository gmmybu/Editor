#include "stdafx.h"
#include "TerrainManager.h"

#include "SceneDoc.h"
#include "TerrainMaterialGeneratorD.h"

#include "Terrain\OgreTerrain.h"
#include "Terrain\OgreTerrainGroup.h"

CTerrainManager::CTerrainManager(SceneDoc *Owner, CTerrainManagerConfig *Config)
{
	//////////////////////////////////////////////////
	// Terrain global options
	//////////////////////////////////////////////////

	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
	mTerrainGlobals->setDefaultMaterialGenerator(Ogre::TerrainMaterialGeneratorPtr(new Ogre::TerrainMaterialGeneratorD));

    mTerrainGlobals->setMaxPixelError(8);

	mTerrainGlobals->setLightMapDirection(Config->LightMapDirection);
	mTerrainGlobals->setLightMapSize(Config->LightMapSize);

	mTerrainGlobals->setCompositeMapAmbient(Config->CompositeMapAmbient);
	mTerrainGlobals->setCompositeMapDiffuse(Config->CompositeMapDiffuse);
	mTerrainGlobals->setCompositeMapDistance(Config->CompositeMapDistance);
	mTerrainGlobals->setCompositeMapSize(Config->CompositeMapSize);

	//////////////////////////////////////////////////
	// Default import settings
	//////////////////////////////////////////////////

	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(
		Owner->getSceneManager(), Ogre::Terrain::ALIGN_X_Z, Config->TerrainSize, Config->WorldSize);

	mTerrainGroup->setResourceGroup("Scene");
    mTerrainGroup->setFilenameConvention(Config->FilenamePrefix, Config->FilenameExtension);
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

    Ogre::Terrain::ImportData& DefaultImp = mTerrainGroup->getDefaultImportSettings();
	DefaultImp.terrainSize	= Config->TerrainSize;
	DefaultImp.worldSize	= Config->WorldSize;
    DefaultImp.inputScale	= Config->InputScale;
	DefaultImp.minBatchSize	= Config->MinBatchSize;
	DefaultImp.maxBatchSize = Config->MaxBatchSize;

	DefaultImp.layerList.resize(Config->LayerCount);
	for(int i=0; i<DefaultImp.layerList.size(); i++)
	{
		DefaultImp.layerList[i].worldSize = Config->TextureWorldSizes[i];
		DefaultImp.layerList[i].textureNames.push_back(Config->DiffuseSpeculars[i]);
		DefaultImp.layerList[i].textureNames.push_back(Config->NormalHeights[i]);
	}
	
	DefineTerrain(0, 0, Config->HeightMap, Config->LoadFromFile);

    mTerrainGroup->loadAllTerrains(true);
    mTerrainGroup->freeTemporaryResources();
}

CTerrainManager::~CTerrainManager()
{
	OGRE_DELETE mTerrainGroup;
	OGRE_DELETE mTerrainGlobals;
}

void GetTerrainImage(bool FlipX, bool FlipY, Ogre::String HeightMap, Ogre::Image &Image)
{
    Image.load(HeightMap, "General");
    if(FlipX)
        Image.flipAroundY();
    if(FlipY)
        Image.flipAroundX();
}

void CTerrainManager::DefineTerrain(long X, long Y, Ogre::String HeightMap, bool LoadFromFile)
{
	// if a file is available, use it
	// if not, generate file from import
	if(LoadFromFile)
	{
		Ogre::String Filename = mTerrainGroup->generateFilename(X, Y);
		if(Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), Filename))
		{
			mTerrainGroup->defineTerrain(X, Y);
			return;
		}
	}

	if(HeightMap.empty())
	{
		mTerrainGroup->defineTerrain(X, Y, 0.0f);
	}
	else
	{
		Ogre::Image Image;
		GetTerrainImage(X % 2 != 0, Y % 2 != 0, HeightMap, Image);
		mTerrainGroup->defineTerrain(X, Y, &Image);
	}
}
