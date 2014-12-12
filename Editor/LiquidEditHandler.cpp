#include "stdafx.h"
#include "Editor.h"
#include "SceneDoc.h"

#include "Liquid.h"
#include "LiquidEditHandler.h"
#include "SceneResourceTree.h"

#include "OgreCamera.h"
#include "OgreEntity.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreManualObject.h"
#include "OgreMeshManager.h"
#include "OgrePlane.h"
#include "OgreRoot.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreTexture.h"
#include "Terrain\OgreTerrain.h"

#include <vector>
#include <queue>

const int Direction[4][2] = 
{
	{-1,  0},	// left
	{ 0, -1},	// up
	{ 1,  0},	// right
	{ 0,  1}	// down
};

LiquidEditHandler::LiquidEditHandler(SceneDoc *Owner)
{
	mOwner = Owner;
	mIndicator = NULL;
	mLiquidHeight = 10.0f;
}


LiquidEditHandler::~LiquidEditHandler()
{
	// Was done by owner
}

void LiquidEditHandler::BeginEdit()
{
	if(!mIndicator)
	{
		float R = mOwner->getTerrainManagerConfig()->WorldSize / mOwner->getTerrainManagerConfig()->TerrainSize;
		Ogre::Vector3 Scale(R, R, R);
		Ogre::Plane Plane(Ogre::Vector3::UNIT_Y, 0);
		Ogre::MeshManager::getSingleton().createPlane("<LiquidIndicatorPlane>",
			"OgreSECore", Plane, Scale.x, Scale.z, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

		Ogre::MovableObjectFactory *Factory = 
			Ogre::Root::getSingleton().getMovableObjectFactory(Ogre::EntityFactory::FACTORY_TYPE_NAME);
		Ogre::NameValuePairList Params;
		Params["mesh"] = "<LiquidIndicatorPlane>";
		Ogre::Entity *PlaneEntity = (Ogre::Entity*)Factory->createInstance("<LiquidIndicatorPlane>", mOwner->getSceneManager(), &Params);
		PlaneEntity->setMaterialName("Editor/LiquidIndicatorMat");
		PlaneEntity->setCastShadows(false);
		PlaneEntity->setQueryFlags(0);

		mIndicator = mOwner->getSceneManager()->getRootSceneNode()->createChildSceneNode();
		mIndicator->attachObject(PlaneEntity);
		mIndicator->setScale(6, 6, 6);
	}
	mIndicator->setVisible(true);
}

void LiquidEditHandler::EndEdit()
{
	if(mIndicator)
		mIndicator->setVisible(false);
}

void LiquidEditHandler::OnLButtonDown(Ogre::TerrainGroup::RayResult rayResult)
{
	int TerrainSize = rayResult.terrain->getSize();

	Ogre::Vector3 tsPosition;
	rayResult.terrain->getTerrainPosition(rayResult.position, &tsPosition);

	CPoint S(tsPosition.x * TerrainSize, tsPosition.y * TerrainSize);
	float SHeight = -mLiquidHeight;

	std::vector<std::vector<Liquid*> > *LiquidMark; 
	if(mLiquidMap[rayResult.terrain] != NULL)
	{
		LiquidMark = mLiquidMap[rayResult.terrain];
		if((*LiquidMark)[S.x][S.y] != NULL) // 拣选流体
		{
			// Liquid *Liquid = (*LiquidMark)[S.x][S.y];
			return;
		}
	}
	else
	{
		mLiquidMap[rayResult.terrain] = new std::vector<std::vector<Liquid*> >(TerrainSize, std::vector<Liquid*>(TerrainSize, NULL));
		LiquidMark = mLiquidMap[rayResult.terrain];
	}

	Liquid *liquid = new Liquid(mOwner->getSceneManager());
	mLiquids.push_back(liquid);

	std::vector<std::vector<bool> > BfsMark(TerrainSize, std::vector<bool>(TerrainSize, false));

	std::queue<CPoint> Q;
	BfsMark[S.x][S.y] = true;
	(*LiquidMark)[S.x][S.y] = liquid;
	Q.push(S);
	while(!Q.empty())
	{
		CPoint U = Q.front(); Q.pop();
		for(int i=0; i<4; i++)
		{
			CPoint V(U.x+Direction[i][0], U.y+Direction[i][1]);
			if(V.x >= 0 && V.x < TerrainSize && V.y >= 0 && V.y < TerrainSize && !BfsMark[V.x][V.y])
			{
				BfsMark[V.x][V.y] = true;
				float VHeight = rayResult.terrain->getHeightAtPoint(V.x, V.y);
				if(VHeight <= SHeight)
				{
					(*LiquidMark)[V.x][V.y] = liquid;
					Q.push(V);
				}
			}
		}
	}

	// create our reflection & refraction render textures, and setup their render targets
	for (unsigned int i = 0; i < 2; i++)
	{
		Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().createManual(i == 0 ? "refraction" : "reflection",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, 512, 512, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);

		Ogre::RenderTarget* rtt = tex->getBuffer()->getRenderTarget();
		rtt->addViewport(mOwner->getCamera())->setOverlaysEnabled(false);
		rtt->setAutoUpdated(false);
		rtt->addListener(this);

		if (i == 0) mRefractionTarget = rtt;
		else mReflectionTarget = rtt;
	}

	Ogre::ManualObject *LiquidObject = mOwner->getSceneManager()->createManualObject("LiquidObject");
	LiquidObject->begin("Examples/FresnelReflectionRefraction", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	std::vector<std::vector<int> > Index(TerrainSize, std::vector<int>(TerrainSize, -1));
	for(int N=0, y=1; y<TerrainSize-1; y++)
	{
		for(int x=1; x<TerrainSize-1; x++)
		{
			if((*LiquidMark)[x][y])
			{
				int xM = x, yM = y;
				int xL = xM - 1, xR = xM + 1, yT = yM - 1, yB = yM + 1;

				int indexM = GetLiquidIndex(rayResult.terrain, LiquidObject, &Index, &N, xM, yM, SHeight);
				int indexT = GetLiquidIndex(rayResult.terrain, LiquidObject, &Index, &N, xM, yT, SHeight);
				int indexL = GetLiquidIndex(rayResult.terrain, LiquidObject, &Index, &N, xL, yM, SHeight);
				int indexB = GetLiquidIndex(rayResult.terrain, LiquidObject, &Index, &N, xM, yB, SHeight);
				int indexR = GetLiquidIndex(rayResult.terrain, LiquidObject, &Index, &N, xR, yM, SHeight);

				LiquidObject->triangle(indexM,indexL,indexT);
				LiquidObject->triangle(indexM,indexR,indexB);

				if(!(*LiquidMark)[xM][yT])	// 上点不属于该水面
  				{
  					size_t indexTR = GetLiquidIndex(rayResult.terrain, LiquidObject, &Index, &N, xR, yT, SHeight);
  					LiquidObject->triangle(indexM,indexT,indexTR);
  				}
  				if(!(*LiquidMark)[xM][yB])	// 下点不属于该水面
  				{
  					size_t indexBL = GetLiquidIndex(rayResult.terrain, LiquidObject, &Index, &N, xL, yB, SHeight);
  					LiquidObject->triangle(indexM,indexB,indexBL);
  				}
  				if(!(*LiquidMark)[xL][yM] && !(*LiquidMark)[xL][yB]) // 左点不属于该水面,并且左下点也不属于水面
  				{	
  					size_t indexBL = GetLiquidIndex(rayResult.terrain, LiquidObject, &Index, &N, xL, yB, SHeight);
  					LiquidObject->triangle(indexM,indexBL,indexL);
  				}
  				if(!(*LiquidMark)[xR][yM] && !(*LiquidMark)[xR][yT]) // 右点不属于该水面，并且右上点也不属于水面	 
  				{	
  					size_t indexTR = GetLiquidIndex(rayResult.terrain, LiquidObject, &Index, &N, xR, yT, SHeight);
  					LiquidObject->triangle(indexM,indexTR,indexR);
  				}
			}
		}
	}

	LiquidObject->end();

	Ogre::String LiquidName = "Liquid_" + Ogre::StringConverter::toString(mOwner->getNameID());
	Ogre::MeshPtr LiquidMesh = LiquidObject->convertToMesh(LiquidName);
	liquid->create(LiquidMesh->getName(), LiquidMesh->getName());
	mOwner->getSceneManager()->destroyManualObject(LiquidObject);
	mOwner->setNameID(mOwner->getNameID()+1);

	HTREEITEM hItem = CSceneResourceTree::Current->InsertItem(
		TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM, 
		LiquidName.c_str(), SRTI_LIQUID, SRTI_LIQUID, 0, 0, LPARAM(liquid), 
		CSceneResourceTree::Current->GetTreeItem(SRTI_LIQUID_SET), TVI_LAST);
	liquid->setUserAny(Ogre::Any(hItem));
}

void LiquidEditHandler::Roaming(Ogre::Vector3 Center)
{
	mIndicator->setPosition(Center+Ogre::Vector3(0, 3.0f, 0));
}

void LiquidEditHandler::update(float Elapsed)
{
	if(mReflectionTarget != NULL)
		mReflectionTarget->update();
	if(mRefractionTarget != NULL)
		mRefractionTarget->update();
}

void LiquidEditHandler::preRenderTargetupdate(const Ogre::RenderTargetEvent& evt)
{
	for(int i=0; i<mLiquids.size(); i++)
		mLiquids[i]->getEntity()->setVisible(false);  // hide the water

	Ogre::Plane LiquidPlane;
	LiquidPlane.d = -mLiquidHeight;
    if (evt.source == mReflectionTarget)  // for reflection, turn on camera reflection and hide submerged entities
	{
		mOwner->getCamera()->enableReflection(LiquidPlane);
	}
	else  // for refraction, hide surface entities
	{
		std::vector<SceneObject*> Objects = mOwner->getObjects();
		for(int i=0; i<Objects.size(); i++)
		{
			Objects[i]->getSceneNode()->setVisible(false);
		}
	}
}

void LiquidEditHandler::postRenderTargetupdate(const Ogre::RenderTargetEvent& evt)
{
	for(int i=0; i<mLiquids.size(); i++)
		mLiquids[i]->getEntity()->setVisible(true);  // hide the water

    if (evt.source == mReflectionTarget)  // for reflection, turn on camera reflection and hide submerged entities
	{
		mOwner->getCamera()->disableReflection();
	}
	else  // for refraction, hide surface entities
	{
		std::vector<SceneObject*> Objects = mOwner->getObjects();
		for(int i=0; i<Objects.size(); i++)
		{
			Objects[i]->getSceneNode()->setVisible(true);
		}
	}
}

int LiquidEditHandler::GetLiquidIndex(Ogre::Terrain *Terrain, Ogre::ManualObject *Object, 
	std::vector<std::vector<int> > *Index, int *N, int X, int Y, float Height)
{
	if((*Index)[X][Y] == -1)
	{
		Ogre::Vector3 tsPosition = Ogre::Vector3(float(X)/Terrain->getSize(), float(Y)/Terrain->getSize(), Height);
		Ogre::Vector3 wsPosition;
		Terrain->getPosition(tsPosition, &wsPosition);
		Object->position(wsPosition);
		Object->normal(Ogre::Vector3::UNIT_Y);
		Object->textureCoord(X*0.125, Y*0.125);
		(*Index)[X][Y] = (*N)++;
	}
	return (*Index)[X][Y];
}
