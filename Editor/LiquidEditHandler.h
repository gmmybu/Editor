#pragma once

#include "OgreRenderTargetListener.h"
#include "Terrain\OgreTerrainGroup.h"

class SceneDoc;
class Liquid;

namespace OIS
{
	class Keyboard;
	class Mouse;
}

class LiquidEditHandler : public Ogre::RenderTargetListener
{
public:
	LiquidEditHandler(SceneDoc *Owner);
	~LiquidEditHandler();

	std::vector<Liquid*> &GetLiquids() { return mLiquids; }

	void BeginEdit();
	void EndEdit();

	void OnLButtonDown(Ogre::TerrainGroup::RayResult rayResult);
	void OnLButtonUp(CPoint point) {}

	void Roaming(Ogre::Vector3 Center);
	void update(float Elapsed);

	void preRenderTargetupdate(const Ogre::RenderTargetEvent& evt);
    void postRenderTargetupdate(const Ogre::RenderTargetEvent& evt);

private:
	int GetLiquidIndex(Ogre::Terrain *Terrain, Ogre::ManualObject *Object, 
		std::vector<std::vector<int> > *Index, int *N, int X, int Y, float Height);

	SceneDoc *mOwner;
	Ogre::SceneNode *mIndicator;

	float mLiquidHeight;
	std::map<Ogre::Terrain*, std::vector<std::vector<Liquid*> >* > mLiquidMap;
	std::vector<Liquid*> mLiquids;
	Ogre::RenderTarget *mReflectionTarget, *mRefractionTarget;
};
