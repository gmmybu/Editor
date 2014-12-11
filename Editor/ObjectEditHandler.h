#pragma once

#include "resource.h"
#include "OgreVector3.h"
#include "OgreMaterial.h"
#include "Terrain\OgreTerrainGroup.h"

class SceneDoc;
class SceneObject;

namespace OIS
{
	class Keyboard;
	class Mouse;
}

namespace OgreOpcode
{
	class CollisionManager;
	class CollisionContext;
	class CollisionObject;
}

enum KObjectEditMode
{
	OEM_NONE,
	OEM_TRANS = ID_TRANS_OBJECT,
	OEM_SCALE = ID_SCALE_OBJECT,
	OEM_ROTATE = ID_ROTATE_OBJECT,
	OEM_DELETE = ID_DEL_OBJECT
};

class CObjectEditHandler
{
public:
	CObjectEditHandler(SceneDoc *Owner);
	~CObjectEditHandler();

	BOOL IsFreeTransform() { return mFreeTransform; }
	void SetFreeTransform(BOOL FreeTransform) { mFreeTransform = FreeTransform; }

	KObjectEditMode GetMode() { return mMode; }
	void SetMode(KObjectEditMode Mode);
	
	SceneObject *GetTarget() { return mTarget; }
	void SetTarget(SceneObject *Target);

	void OnLButtonDown(CPoint point);
	void OnLButtonUp(CPoint point);
	/**
	@param Point 光标位置
	@param Center 相机到视口的射线与地形的交点
	*/
	void Roaming(CPoint Point, Ogre::TerrainGroup::RayResult rayResult, float Elapsed);
	void update(float Elapsed);

private:
	enum KAxisMode
	{
		AM_NONE,
		AM_TRANS_SCALE_X,
		AM_TRANS_SCALE_Y,
		AM_TRANS_SCALE_Z,
		AM_TRANS_SCALE_ALL,
		AM_ROTATE_X,
		AM_ROTATE_Y,
		AM_ROTATE_Z,
		_AM
	};

	OgreOpcode::CollisionObject *AddCollisionEntity(
		OgreOpcode::CollisionContext *Context, Ogre::Entity *Entity);
	void Translate(SceneObject *Object, Ogre::Vector3 vTrans, BOOL Paste);

	SceneDoc *mOwner;
	BOOL mFreeTransform;
	KObjectEditMode mMode;
	KAxisMode mAxisMode;
	SceneObject *mTarget;
	Ogre::RaySceneQuery *mRayQuery;

	Ogre::MaterialPtr mMaterials[5]; // 红绿蓝白黄

	OgreOpcode::CollisionManager *mCollisionManager;
	OgreOpcode::CollisionContext *mCollisionContext;
	OgreOpcode::CollisionContext *mIndicatorContext;

	Ogre::SceneNode *mObjectEditNode;
	Ogre::SceneNode *mTransScaleNode;
	Ogre::SceneNode *mRotateNode;

	Ogre::Entity *mTransformEntities[_AM];
	std::map<OgreOpcode::CollisionObject*, KAxisMode> mCollisionObjectToAxisMode;

	CPoint mLastPoint;
};
