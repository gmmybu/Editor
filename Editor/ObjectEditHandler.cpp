#include "stdafx.h"
#include "ObjectEditHandler.h"

#include "MainFrm.h"
#include "SceneDoc.h"
#include "SceneView.h"
#include "SceneObject.h"
#include "SceneResourceTree.h"
#include "TerrainEditPage.h"
#include "PropertyWnd.h"

#include "OgreRoot.h"
#include "OgreEntity.h"

#include "OIS\OISKeyboard.h"
#include "OIS\OISMouse.h"

#include "OgreOpcode.h"

ObjectEditHandler::ObjectEditHandler(SceneDoc *Owner)
{
	mOwner = Owner;
	mMode = OEM_NONE;
	mAxisMode = AM_NONE;
	mTarget = NULL;
	mObjectEditNode = mOwner->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	mRayQuery = mOwner->getSceneManager()->createRayQuery(Ogre::Ray());

	mMaterials[0] = Ogre::MaterialManager::getSingletonPtr()->getByName("Editor/RedMat");
	mMaterials[1] = Ogre::MaterialManager::getSingletonPtr()->getByName("Editor/GreenMat");
	mMaterials[2] = Ogre::MaterialManager::getSingletonPtr()->getByName("Editor/BlueMat");
	mMaterials[3] = Ogre::MaterialManager::getSingletonPtr()->getByName("Editor/ObjectIndicatorWhiteMat");
	mMaterials[4] = Ogre::MaterialManager::getSingletonPtr()->getByName("Editor/ObjectIndicatorYellowMat");

	//////////////////////////////////////////////////
	// 碰撞检测
	//////////////////////////////////////////////////

	mCollisionManager = new OgreOpcode::CollisionManager(Owner->getSceneManager());
	mCollisionManager->addCollClass("Object");
	mCollisionManager->addCollType("Object", "Object", OgreOpcode::COLLTYPE_IGNORE);
	mCollisionContext = mCollisionManager->createContext("SceneObject");

	Ogre::MovableObjectFactory* Factory = 
		Ogre::Root::getSingleton().getMovableObjectFactory(Ogre::EntityFactory::FACTORY_TYPE_NAME);
	Ogre::NameValuePairList Params;

	//////////////////////////////////////////////////
	// 平移放缩指示器
	//////////////////////////////////////////////////

	mIndicatorContext = mCollisionManager->createContext("TransScale");

	mTransScaleNode = mObjectEditNode->createChildSceneNode("TransScaleIndicator");
	mTransScaleNode->setScale(200.0f, 200.0f, 200.0f);

	// x
	Ogre::SceneNode *SubNode = mTransScaleNode->createChildSceneNode();
	SubNode->roll(Ogre::Degree(-90.0f));

	Ogre::Entity *Entity = Owner->getSceneManager()->createEntity("TransScaleXLine", "MoveArrowLineVisible.mesh");
	//Entity->setQueryFlags(0); // 参与查询
	Entity->setCastShadows(false);
	Entity->setMaterial(mMaterials[0]);
	Entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);
	SubNode->attachObject(Entity);
	mTransformEntities[AM_TRANS_SCALE_X] = Entity;

	Entity = Owner->getSceneManager()->createEntity("TransScaleXCone", "MoveArrowConeVisible.mesh");
	Entity->setQueryFlags(0);
	Entity->setCastShadows(false);
	Entity->setMaterial(mMaterials[0]);
	Entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);
	SubNode->attachObject(Entity);

	Params["mesh"] = "MoveArrowCollision.mesh";
	Entity = (Ogre::Entity*)Factory->createInstance("TransScaleXCol", Owner->getSceneManager(), &Params);
	Entity->setQueryFlags(0);
	Entity->setCastShadows(false);

	Entity->setVisible(false);
	SubNode->attachObject(Entity);

	OgreOpcode::CollisionObject *CollisionObject = 
		AddCollisionEntity(mIndicatorContext, Entity);
	mCollisionObjectToAxisMode[CollisionObject] = AM_TRANS_SCALE_X;

	// y
	SubNode = mTransScaleNode;

	Entity = Owner->getSceneManager()->createEntity("TransScaleYLine", "MoveArrowLineVisible.mesh");
	//Entity->setQueryFlags(0);
	Entity->setCastShadows(false);
	Entity->setMaterial(mMaterials[1]);
	Entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);
	SubNode->attachObject(Entity);
	mTransformEntities[AM_TRANS_SCALE_Y] = Entity;

	Entity = Owner->getSceneManager()->createEntity("TransScaleYCone", "MoveArrowConeVisible.mesh");
	Entity->setQueryFlags(0);
	Entity->setCastShadows(false);
	Entity->setMaterial(mMaterials[1]);
	Entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);
	SubNode->attachObject(Entity);

	Params["mesh"] = "MoveArrowCollision.mesh";
	Entity = (Ogre::Entity*)Factory->createInstance("TransScaleYCol", Owner->getSceneManager(), &Params);
	Entity->setQueryFlags(0);
	Entity->setCastShadows(false);

	Entity->setVisible(false);
	SubNode->attachObject(Entity);

	CollisionObject = AddCollisionEntity(mIndicatorContext, Entity);
	mCollisionObjectToAxisMode[CollisionObject] = AM_TRANS_SCALE_Y;

	// z
	SubNode = mTransScaleNode->createChildSceneNode();
	SubNode->pitch(Ogre::Degree(90));

	Entity = Owner->getSceneManager()->createEntity("TransScaleZLine", "MoveArrowLineVisible.mesh");
	//Entity->setQueryFlags(0);
	Entity->setCastShadows(false);
	Entity->setMaterial(mMaterials[2]);
	Entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);
	SubNode->attachObject(Entity);
	mTransformEntities[AM_TRANS_SCALE_Z] = Entity;

	Entity = Owner->getSceneManager()->createEntity("TransScaleZCone", "MoveArrowConeVisible.mesh");
	Entity->setQueryFlags(0);
	Entity->setCastShadows(false);
	Entity->setMaterial(mMaterials[2]);
	Entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);
	SubNode->attachObject(Entity);

	Params["mesh"] = "MoveArrowCollision.mesh";
	Entity = (Ogre::Entity*)Factory->createInstance("TransScaleZCol", Owner->getSceneManager(), &Params);
	Entity->setQueryFlags(0);
	Entity->setCastShadows(false);

	Entity->setVisible(false);
	SubNode->attachObject(Entity);

	CollisionObject = AddCollisionEntity(mIndicatorContext, Entity);
	mCollisionObjectToAxisMode[CollisionObject] = AM_TRANS_SCALE_Z;

	// box
	SubNode = mTransScaleNode;

	Params["mesh"] = "Box1m.mesh";
	Entity =(Ogre::Entity*)Factory->createInstance("TransScaleAll", Owner->getSceneManager(), &Params);
	//Entity->setQueryFlags(0);
	Entity->setCastShadows(false);
	Entity->setMaterial(mMaterials[3]);
	Entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);
	SubNode->attachObject(Entity);
	mTransformEntities[AM_TRANS_SCALE_ALL] = Entity;

	CollisionObject = AddCollisionEntity(mIndicatorContext, Entity);
	mCollisionObjectToAxisMode[CollisionObject] = AM_TRANS_SCALE_ALL;

	mIndicatorContext->reset();
	mObjectEditNode->removeChild(mTransScaleNode);

	//////////////////////////////////////////////////
	// 旋转指示器
	//////////////////////////////////////////////////

	mIndicatorContext = mCollisionManager->createContext("Rotate");

	mRotateNode = mObjectEditNode->createChildSceneNode("RotateIndicator");
	mRotateNode->setScale(200.0f, 200.0f, 200.0f);

	// x
	SubNode = mRotateNode->createChildSceneNode();
	SubNode->roll(Ogre::Degree(90));

	Entity = Owner->getSceneManager()->createEntity("RotateX", "RotationRingVisible.mesh");
	//Entity->setQueryFlags(0);
	Entity->setCastShadows(false);
	Entity->setMaterial(mMaterials[0]);
	Entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);
	SubNode->attachObject(Entity);
	mTransformEntities[AM_ROTATE_X] = Entity;

	Params["mesh"] = "RotationRingCollision.mesh";
	Entity =(Ogre::Entity*)Factory->createInstance("RotateXCol", Owner->getSceneManager(), &Params);
	Entity->setQueryFlags(0);
	Entity->setCastShadows(false);

	Entity->setVisible(false);
	SubNode->attachObject(Entity);

	CollisionObject = AddCollisionEntity(mIndicatorContext, Entity);
	mCollisionObjectToAxisMode[CollisionObject] = AM_ROTATE_X;

	// y
	SubNode = mRotateNode;
	Entity = Owner->getSceneManager()->createEntity("RotateY", "RotationRingVisible.mesh");
	//Entity->setQueryFlags(0);
	Entity->setCastShadows(false);
	Entity->setMaterial(mMaterials[1]);
	Entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);
	SubNode->attachObject(Entity);
	mTransformEntities[AM_ROTATE_Y] = Entity;

	Params["mesh"] = "RotationRingCollision.mesh";
	Entity =(Ogre::Entity*)Factory->createInstance("RotateYCol", Owner->getSceneManager(), &Params);
	Entity->setQueryFlags(0);
	Entity->setCastShadows(false);

	Entity->setVisible(false);
	SubNode->attachObject(Entity);
	
	CollisionObject = AddCollisionEntity(mIndicatorContext, Entity);
	mCollisionObjectToAxisMode[CollisionObject] = AM_ROTATE_Y;

	// z
	SubNode = mRotateNode->createChildSceneNode();
	SubNode->pitch(Ogre::Degree(90.0f));

	Entity = Owner->getSceneManager()->createEntity("RotateZ", "RotationRingVisible.mesh");
	//Entity->setQueryFlags(0);
	Entity->setCastShadows(false);
	Entity->setMaterial(mMaterials[2]);
	Entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);
	SubNode->attachObject(Entity);
	mTransformEntities[AM_ROTATE_Z] = Entity;

	Params["mesh"] = "RotationRingCollision.mesh";
	Entity =(Ogre::Entity*)Factory->createInstance("RotateZCol", Owner->getSceneManager(), &Params);
	Entity->setQueryFlags(0);
	Entity->setCastShadows(false);

	Entity->setVisible(false);
	SubNode->attachObject(Entity);

	CollisionObject = AddCollisionEntity(mIndicatorContext, Entity);
	mCollisionObjectToAxisMode[CollisionObject] = AM_ROTATE_Z;

	mIndicatorContext->reset();
	mObjectEditNode->removeChild(mRotateNode);
}

ObjectEditHandler::~ObjectEditHandler()
{
	delete mCollisionManager;
	
	// Was done by owner
}

void ObjectEditHandler::SetMode(KObjectEditMode Mode)
{
	if(mMode == Mode)
		return;

	mMode = Mode;
	switch(mMode) // 未选中地物时，不允许切换到其它模式；防止删除地物后，指示器通过切换模式仍可见
	{
	case OEM_NONE:
		{
			//if(mTarget)
			//	mTarget->getSceneNode()->showBoundingBox(false);
			CSceneResourceTree::Current->SelectItem(NULL);
			if(mTransScaleNode->getParentSceneNode())
				mObjectEditNode->removeChild(mTransScaleNode);
			if(mRotateNode->getParentSceneNode())
				mObjectEditNode->removeChild(mRotateNode);
		}
		break;

	case OEM_TRANS:
		{
			if(!mTarget)
				break;
			mTarget->getSceneNode()->showBoundingBox(true);

			mIndicatorContext = OgreOpcode::CollisionManager::getSingletonPtr()->getContext("TransScale");
			if(mRotateNode->getParentSceneNode())
				mObjectEditNode->removeChild(mRotateNode);
			if(!mTransScaleNode->getParentSceneNode())
				mObjectEditNode->addChild(mTransScaleNode);
			mTransScaleNode->resetOrientation();
		}
		break;

	case OEM_SCALE:
		{
			if(!mTarget)
				break;
			mTarget->getSceneNode()->showBoundingBox(true);

			mIndicatorContext = OgreOpcode::CollisionManager::getSingletonPtr()->getContext("TransScale");
			if (mRotateNode->getParentSceneNode())
				mObjectEditNode->removeChild(mRotateNode);
			if (!mTransScaleNode->getParentSceneNode())
				mObjectEditNode->addChild(mTransScaleNode);
 			mTransScaleNode->resetOrientation();
		}
		break;

	case OEM_ROTATE:
		{		
			if(!mTarget)
				break;
			mTarget->getSceneNode()->showBoundingBox(true);

			mIndicatorContext = OgreOpcode::CollisionManager::getSingletonPtr()->getContext("Rotate");
			if(mTransScaleNode->getParentSceneNode())
				mObjectEditNode->removeChild(mTransScaleNode);
			if(!mRotateNode->getParentSceneNode())
				mObjectEditNode->addChild(mRotateNode);
			mRotateNode->resetOrientation();
		}
	}
	SceneDoc::current->OnObjectEdit(mMode);
	CBCGPToolBar *ToolBar = &((CMainFrame*)AfxGetMainWnd())->mObjectEditToolBar;
	ToolBar->OnUpdateCmdUI((CFrameWnd*)ToolBar->GetOwner(), FALSE);
}

void ObjectEditHandler::SetTarget(SceneObject *Target)
{
	if(mTarget != NULL)
	{
		mTarget->getSceneNode()->showBoundingBox(false);
	}
	mTarget = Target;
	if(mTarget != NULL)
	{
		mTarget->getSceneNode()->showBoundingBox(true);
		mObjectEditNode->setPosition(mTarget->getSceneNode()->getPosition());
		for(int i=AM_TRANS_SCALE_X; i<_AM; i++)
		{
			mTransformEntities[i]->setUserAny(Ogre::Any(mTarget));
		}
	}
}

void ObjectEditHandler::OnLButtonDown(CPoint point)
{
	if(mMode == OEM_NONE)
		return;

	mLastPoint = point;
	if(mFreeTransform)
	{
		mFreeTransform = FALSE;
		return;
	}

	if(mMode == OEM_TRANS && mAxisMode == AM_TRANS_SCALE_ALL)
	{
		mFreeTransform = TRUE;
	}
}

void ObjectEditHandler::OnLButtonUp(CPoint point)
{
	if(mTarget)
		return;

	float x = point.x / (float)SceneView::current->getWidth();
	float y = point.y / (float)SceneView::current->getHeight();

	Ogre::Ray pickRay = mOwner->getCamera()->getCameraToViewportRay(x, y);
	mRayQuery->setRay(pickRay);
	mRayQuery->setSortByDistance(true);
	Ogre::RaySceneQueryResult &result = mRayQuery->execute();
	if(result.empty())
		return;
	/** NOTE 当地形的包围盒大于地物时(采用了高度图时可能会出现这种情形)，则result的begin为地形，
		因此应迭代，直到拾取到地物后break，防止删除多个物体。
	*/
	Ogre::RaySceneQueryResult::iterator resultIter = result.begin();
	do
	{
		Ogre::RaySceneQueryResultEntry entry = *resultIter;
		const Ogre::Any &Any = entry.movable->getUserAny();
		if(!Any.isEmpty()) // Terrain亦可被选中，但其不携带Any
		{
			SceneObject *Object = Any.operator()<SceneObject*>();
			HTREEITEM hItem = Object->getUserAny().operator()<HTREEITEM>();
			if(mMode == OEM_DELETE)
			{
				CSceneResourceTree::Current->DeleteItem(hItem);
				CSceneResourceTree::Current->SelectItem(NULL);
				SceneDoc::current->removeObject(Object);
			} else 
			if(mTarget != Object)
			{
				CSceneResourceTree::Current->SelectItem(NULL);
				CSceneResourceTree::Current->SelectItem(hItem);
			}
			break;
		}
	}while(++resultIter != result.end());
}

void ObjectEditHandler::Roaming(CPoint Point, Ogre::TerrainGroup::RayResult rayResult, float Elapsed)
{
	if(mMode == OEM_NONE)
		return;

	if(mFreeTransform)
	{
		Ogre::Vector3 newPosition;
		if(!(GetKeyState(VK_LCONTROL) & 0x8000))
		{
			newPosition = Ogre::Vector3(rayResult.position.x, mTarget->getSceneNode()->getPosition().y, rayResult.position.z);
			if(mOwner->isPaste())
				newPosition.y = rayResult.position.y;
			mTarget->getSceneNode()->setPosition(newPosition);
			CPropertyWnd::Current->FirePropertyChanged();
		}
		mObjectEditNode->setPosition(newPosition);
	} else

	if(GetKeyState(VK_LBUTTON) & 0x8000)
	{
		if(Point == mLastPoint)
			return;

		CPoint Move = Point - mLastPoint;
		mLastPoint = Point;

		if(mMode == OEM_TRANS)
		{
			Ogre::Vector3 vCamXAxis, vCamYAxis, vCamZAxis;
			mOwner->getCamera()->getDerivedOrientation().ToAxes(vCamXAxis, vCamYAxis, vCamZAxis);
			Ogre::Vector3 vTrans(0,0,0);
			switch(mAxisMode)
			{
			case AM_TRANS_SCALE_X:
				{
					float xyPlane = (vCamZAxis.y < 0 ? -1 : 1);
					float yAxis = (vCamXAxis.z < 0 ? 1 : -1);
					vTrans.x = Move.x * vCamXAxis.x + Move.y * (1 - vCamXAxis.x) * xyPlane * yAxis;
				}
				break;

			case AM_TRANS_SCALE_Y:
				{
					vTrans.y = -Move.y * (vCamYAxis.y > 0 ? 1 : -1);
				}
				break;

			case AM_TRANS_SCALE_Z:
				{
					float yAxis = (vCamZAxis.z < 0 ? -1 : 1);
					vTrans.z = Move.x * vCamXAxis.z + Move.y * (1.0f - vCamXAxis.z) * yAxis;
				}
				break;

			case AM_TRANS_SCALE_ALL:
				{
					float xyPlane = (vCamZAxis.y < 0 ? -1 : 1);
					float yAxis = (vCamXAxis.z < 0 ? 1 : -1);
					vTrans.x = Move.x * vCamXAxis.x + Move.y * (1 - vCamXAxis.x) * xyPlane * yAxis;
					vTrans.z = Move.x * vCamXAxis.z + Move.y * (1.0f - vCamXAxis.z) * (vCamZAxis.z < 0 ? -1 : 1);
					vTrans.y = -Move.y * (vCamYAxis.y > 0 ? 1 : -1);
				}
			}
			if(!(GetKeyState(VK_LCONTROL) & 0x8000))
			{
				mTarget->getSceneNode()->translate(vTrans);
				CPropertyWnd::Current->FirePropertyChanged();
			}
			mObjectEditNode->translate(vTrans);
		} else 
			
		if(mMode == OEM_SCALE)
		{
			Ogre::Vector3 vScaleInc(0, 0, 0);
			if(mAxisMode == AM_TRANS_SCALE_ALL)
			{
				vScaleInc.x = Move.x * 0.01f;
				vScaleInc.y = vScaleInc.x;
				vScaleInc.z = vScaleInc.x;
			}
			else
			{
				Ogre::Vector3 vNodeXAxis, vNodeYAxis, vNodeZAxis;
				mObjectEditNode->getOrientation().ToAxes(vNodeXAxis, vNodeYAxis, vNodeZAxis);
				const Ogre::Matrix4 &ViewMatrix = mOwner->getCamera()->getViewMatrix();
				switch(mAxisMode)
				{
				case AM_TRANS_SCALE_X:
					{
						vNodeXAxis = ViewMatrix * vNodeXAxis;
						vScaleInc.x = (Move.x * vNodeXAxis.x - Move.y * vNodeXAxis.y) * 0.001f;
					}
					break;

				case AM_TRANS_SCALE_Y:
					{
						vNodeYAxis = ViewMatrix * vNodeYAxis;
						vScaleInc.y = (Move.x * vNodeYAxis.x - Move.y * vNodeYAxis.y) * 0.001f;
					}
					break;

				case AM_TRANS_SCALE_Z:
					{					
						vNodeZAxis = ViewMatrix * vNodeZAxis;
						vScaleInc.z = (Move.x * vNodeZAxis.x - Move.y * vNodeZAxis.y) * 0.001f;
					}
				}
				mTarget->getSceneNode()->setScale(
					mTarget->getSceneNode()->getScale() + vScaleInc);
				CPropertyWnd::Current->FirePropertyChanged();
			}
		} else

		if(mMode == OEM_ROTATE)
		{
			Ogre::Quaternion qRotate;
			switch(mAxisMode)
			{
			case AM_ROTATE_X:
				qRotate.FromAngleAxis(Ogre::Radian(Move.y * 0.01f), mRotateNode->getOrientation().xAxis());
				break;
			case AM_ROTATE_Y:
				qRotate.FromAngleAxis(Ogre::Radian(Move.x * 0.01f), mRotateNode->getOrientation().yAxis());
				break;
			case AM_ROTATE_Z:
				qRotate.FromAngleAxis(Ogre::Radian(-Move.y * 0.01f), mRotateNode->getOrientation().zAxis());
				break;
			}
			if (!(GetKeyState(VK_LCONTROL) & 0x8000))
			{
				mTarget->getSceneNode()->rotate(qRotate, Ogre::Node::TS_PARENT);
				CPropertyWnd::Current->FirePropertyChanged();
			}
			mRotateNode->rotate(qRotate, Ogre::Node::TS_PARENT);
		}
	}
	else
	{
		mIndicatorContext->collide();
		OgreOpcode::CollisionPair **CollisionPair = NULL;
		float Distance = 
			mOwner->getCamera()->getFarClipDistance();
		if(Distance < 0.1f) // 近似比较float与0
			Distance = Ogre::Math::POS_INFINITY;

		Ogre::Ray pickRay = mOwner->getCamera()->getCameraToViewportRay(float(Point.x) / mOwner->getActiveView()->getWidth(), 
			float(Point.y) / mOwner->getActiveView()->getHeight());

		if(mIndicatorContext->rayCheck(pickRay, Distance,
			OgreOpcode::COLLTYPE_EXACT,
			OgreOpcode::COLLTYPE_ALWAYS_EXACT,
			CollisionPair))
		{
			OgreOpcode::CollisionObject *CollisionObject = 
				CollisionPair[0]->this_object;
			KAxisMode AxisMode = mCollisionObjectToAxisMode[CollisionObject];

			if(mAxisMode == AM_NONE)
			{
				mAxisMode = mCollisionObjectToAxisMode[CollisionObject];
				mTransformEntities[mAxisMode]->setMaterial(mMaterials[4]);
				return;
			} else 
			
			if(mAxisMode == AxisMode)
				return;
		}
		switch(mAxisMode) // 恢复指示器材质
		{
		case AM_NONE:
			return;
		case AM_TRANS_SCALE_X:
		case AM_ROTATE_X:
			mTransformEntities[mAxisMode]->setMaterial(mMaterials[0]);
			break;
		case AM_TRANS_SCALE_Y:
		case AM_ROTATE_Y:
			mTransformEntities[mAxisMode]->setMaterial(mMaterials[1]);
			break;
		case AM_TRANS_SCALE_Z:
		case AM_ROTATE_Z:
			mTransformEntities[mAxisMode]->setMaterial(mMaterials[2]);
			break;
		case AM_TRANS_SCALE_ALL:
			mTransformEntities[mAxisMode]->setMaterial(mMaterials[3]);
			break;
		}
		mAxisMode = AM_NONE;
	}
}

void ObjectEditHandler::update(float Elapsed)
{
}

OgreOpcode::CollisionObject *ObjectEditHandler::AddCollisionEntity(
	OgreOpcode::CollisionContext *Context, Ogre::Entity *Entity)
{
	OgreOpcode::CollisionObject *Object = NULL;

	OgreOpcode::EntityCollisionShape *CollisionShape = 
		mCollisionManager->createEntityCollisionShape(Entity->getName());
	CollisionShape->load(Entity);
	
	Object = Context->createObject(Entity->getName());
	Context->addObject(Object);

	Object->setCollClass("Object");
	Object->setShape(CollisionShape);
	
	return Object;
}

void ObjectEditHandler::Translate(SceneObject *Object, Ogre::Vector3 vTrans, BOOL Paste)
{
	Ogre::Vector3 Position = Object->getSceneNode()->getPosition();
	Position.x += vTrans.x;
	Position.z += vTrans.z;
	if(Paste)
	{
		float TerrainHeight = CTerrainEditPage::Current->GetTerrain()->getHeightAtWorldPosition(
			Ogre::Vector3(Position.x, 0, Position.z));
		Position.y = TerrainHeight;
	}
	Object->getSceneNode()->setPosition(Position);
}
