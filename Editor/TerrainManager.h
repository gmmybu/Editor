#pragma once

#include "OgreColourValue.h"
#include "OgreString.h"
#include "OgreVector3.h"

class SceneDoc;
namespace Ogre
{
	class TerrainGlobalOptions;
	class TerrainGroup;
}

class TerrainManagerConfig
{
public:
	Ogre::Vector3	LightMapDirection;
	Ogre::uint16	LightMapSize;

	Ogre::ColourValue	CompositeMapAmbient;
	Ogre::ColourValue	CompositeMapDiffuse;
	Ogre::Real			CompositeMapDistance;
	Ogre::uint16		CompositeMapSize;

	Ogre::String FilenamePrefix;
	Ogre::String FilenameExtension;

	Ogre::Real TerrainSize;
	Ogre::Real WorldSize;
	Ogre::Real InputScale;
	Ogre::Real MinBatchSize;
	Ogre::Real MaxBatchSize;

	Ogre::uint16 LayerCount;
	std::vector<Ogre::Real> TextureWorldSizes;
	std::vector<Ogre::String> DiffuseSpeculars;
	std::vector<Ogre::String> NormalHeights;

	bool			LoadFromFile;
	Ogre::String	HeightMap;
};

class TerrainManager
{
public:
	TerrainManager(SceneDoc *Owner, TerrainManagerConfig *Config);
	~TerrainManager();

	Ogre::TerrainGlobalOptions *GetTerrainGlobals() { return mTerrainGlobals; }
	Ogre::TerrainGroup *GetTerrainGroup() { return mTerrainGroup; }

private:
	void DefineTerrain(long X, long Y, Ogre::String HeightMap, bool LoadFromFile);

	Ogre::TerrainGlobalOptions *mTerrainGlobals;
	Ogre::TerrainGroup *mTerrainGroup;
};
