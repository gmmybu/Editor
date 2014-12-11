#include "stdafx.h"
#include "CharacterController.h"

#include "CameraWnd.h"
#include "SceneDoc.h"
#include "TerrainManager.h"

#include "OgreAnimationState.h"
#include "OgreCamera.h"
#include "OgreEntity.h"
#include "OgreRibbonTrail.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "Terrain\OgreTerrain.h"
#include "Terrain\OgreTerrainGroup.h"

#include "OIS\OISKeyboard.h"
#include "OIS\OISMouse.h"


#define CHAR_HEIGHT 5          // height of character's center of mass above ground
#define CAM_HEIGHT 2           // height of camera above character's center of mass
#define RUN_SPEED 17           // character running speed in units per second
#define TURN_SPEED 500.0f      // character turning in degrees per second
#define ANIM_FADE_SPEED 7.5f   // animation crossfade speed in % of full weight per second
#define JUMP_ACCEL 30.0f       // character jump acceleration in upward units per squared second
#define GRAVITY 90.0f          // gravity in downward units per squared second

CharacterController::CharacterController(SceneDoc *Owner)
{
	mOwner = Owner;
	SetupBody();
	SetupAnimations();
	SetupCamera();
}

CharacterController::~CharacterController()
{
	// Was done by owner
}

void CharacterController::SetupBody()
{
	Ogre::Terrain *Terrain = mOwner->getTerrainManager()->GetTerrainGroup()->getTerrain(0, 0);
	float TerrainHeight = Terrain->getHeightAtWorldPosition(Ogre::Vector3(0, 0, 0));
	
	mBodyNode = mOwner->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	mBodyNode->setPosition(Ogre::Vector3(0, TerrainHeight + CHAR_HEIGHT, 0));
	mBodyEnt = mOwner->getSceneManager()->createEntity("SinbadBody", "Sinbad.mesh");
	mBodyNode->attachObject(mBodyEnt);

	mSword1 = mOwner->getSceneManager()->createEntity("SinbadSword1", "Sword.mesh");
	mSword2 = mOwner->getSceneManager()->createEntity("SinbadSword2", "Sword.mesh");
	mBodyEnt->attachObjectToBone("Sheath.L", mSword1);
	mBodyEnt->attachObjectToBone("Sheath.R", mSword2);

	Ogre::NameValuePairList params;
	params["numberOfChains"] = "2";
	params["maxElements"] = "80";
	mSwordTrail = (Ogre::RibbonTrail*)mOwner->getSceneManager()->createMovableObject("RibbonTrail", &params);
	mSwordTrail->setMaterialName("Examples/LightRibbonTrail");
	mSwordTrail->setTrailLength(20);
	mSwordTrail->setVisible(false);
	mOwner->getSceneManager()->getRootSceneNode()->attachObject(mSwordTrail);

	for(int i = 0; i < 2; i++)
	{
		mSwordTrail->setInitialColour(i, 1, 0.8, 0);
		mSwordTrail->setColourChange(i, 0.75, 1.25, 1.25, 1.25);
		mSwordTrail->setWidthChange(i, 1);
		mSwordTrail->setInitialWidth(i, 0.5);
	}

	mKeyDirection = Ogre::Vector3::ZERO;
	mVerticalVelocity = 0;
}

void CharacterController::SetupAnimations()
{
	// this is very important due to the nature of the exported animations
	mBodyEnt->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	Ogre::String animNames[] =
	{"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
	"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};

	for(int i = 0; i < NUM_ANIMS; i++)
	{
		mAnims[i] = mBodyEnt->getAnimationState(animNames[i]);
		mAnims[i]->setLoop(true);
		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	SetBaseAnimation(ANIM_IDLE_BASE);
	SetTopAnimation(ANIM_IDLE_TOP);
	mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);

	mSwordsDrawn = false;
}

void CharacterController::SetupCamera()
{
	mOwner->getCamera()->setPosition(mBodyNode->getPosition() + Ogre::Vector3(0, 30, 30));
	mOwner->getCamera()->setAutoTracking(true, mBodyNode);
}

void CharacterController::update(Ogre::Real Elapsed)
{
	UpdateBody(Elapsed);
	UpdateAnimations(Elapsed);
	UpdateCamera(Elapsed);
}

void CharacterController::Roaming(OIS::Keyboard *Keyboard, OIS::Mouse *Mouse, float Elapsed)
{
	if(Keyboard->isKeyDown(OIS::KC_Q) && (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP))
	{ // 持刀
		SetTopAnimation(ANIM_DRAW_SWORDS, true);
		mTimer = 0;
	} else 
	
	if(Keyboard->isKeyDown(OIS::KC_E) && !mSwordsDrawn)
	{ // 起舞
		if (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP)
		{
			SetBaseAnimation(ANIM_DANCE, true);
			SetTopAnimation(ANIM_NONE);
			mAnims[ANIM_HANDS_RELAXED]->setEnabled(false);
		} else 
			
		if (mBaseAnimID == ANIM_DANCE)
		{
			SetBaseAnimation(ANIM_IDLE_BASE);
			SetTopAnimation(ANIM_IDLE_TOP);
			mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
		}
	}

	mKeyDirection = Ogre::Vector3::ZERO;
	if(Keyboard->isKeyDown(OIS::KC_W)) 
		mKeyDirection.z = -1;
	if(Keyboard->isKeyDown(OIS::KC_A)) 
		mKeyDirection.x = -1;
	if(Keyboard->isKeyDown(OIS::KC_S)) 
		mKeyDirection.z = 1;
	if(Keyboard->isKeyDown(OIS::KC_D)) 
		mKeyDirection.x = 1;

	if(Keyboard->isKeyDown(OIS::KC_SPACE) && (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP))
	{ // 跳跃
		SetBaseAnimation(ANIM_JUMP_START, true);
		SetTopAnimation(ANIM_NONE);
		mTimer = 0;
	}

	Ogre::Real Cam2Obj = (mOwner->getCamera()->getPosition() - mBodyEnt->getParentSceneNode()->getPosition()).length();

	if(!mKeyDirection.isZeroLength() && mBaseAnimID == ANIM_IDLE_BASE)
	{ // 移动
		SetBaseAnimation(ANIM_RUN_BASE, true);
		if(mTopAnimID == ANIM_IDLE_TOP) 
			SetTopAnimation(ANIM_RUN_TOP, true);
		mOwner->getCamera()->setPosition(mBodyNode->getPosition());
		mOwner->getCamera()->moveRelative(Ogre::Vector3(0, 0, Cam2Obj));
	} else 
		
	if(mKeyDirection.isZeroLength() && mBaseAnimID == ANIM_RUN_BASE)
	{ // 不动
		SetBaseAnimation(ANIM_IDLE_BASE);
		if(mTopAnimID == ANIM_RUN_TOP) 
			SetTopAnimation(ANIM_IDLE_TOP);
	}

	if(mSwordsDrawn && (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP))
	{ // 挥刀
		if(GetKeyState(VK_LBUTTON) & 0x8000)
		{
			SetTopAnimation(ANIM_SLICE_VERTICAL, true);
		} else 
			
		if(GetKeyState(VK_RBUTTON) & 0x8000)
		{
			SetTopAnimation(ANIM_SLICE_HORIZONTAL, true);
		}
		mTimer = 0;
	}

	//////////////////////////////////////////////////
	// Camera
	//////////////////////////////////////////////////

	if(GetKeyState(VK_RBUTTON) & 0x8000)
	{ // 轨道
		mOwner->getCamera()->setPosition(mBodyNode->getPosition());
		mOwner->getCamera()->yaw(-Ogre::Radian(Mouse->getMouseState().X.rel * 0.005f));
		mOwner->getCamera()->pitch(-Ogre::Radian(Mouse->getMouseState().Y.rel * 0.005f));
		mOwner->getCamera()->moveRelative(Ogre::Vector3(0, 0, Cam2Obj));
	} else 
				
	if (Mouse->getMouseState().Z.rel != 0)
	{ // 放缩
		mOwner->getCamera()->moveRelative(Ogre::Vector3(0, 0, -Mouse->getMouseState().Z.rel * Cam2Obj * 0.0008f));
	}
}

void CharacterController::UpdateBody(Ogre::Real deltaTime)
{
	mGoalDirection = Ogre::Vector3::ZERO;   // we will calculate this

	if(mKeyDirection != Ogre::Vector3::ZERO && mBaseAnimID != ANIM_DANCE)
	{
		// calculate actually goal direction in world based on player's key directions
		mGoalDirection += mKeyDirection.z * mOwner->getCamera()->getOrientation().zAxis();
		mGoalDirection += mKeyDirection.x * mOwner->getCamera()->getOrientation().xAxis();
		mGoalDirection.y = 0;
		mGoalDirection.normalise();

		Ogre::Quaternion toGoal = mBodyNode->getOrientation().zAxis().getRotationTo(mGoalDirection);

		// calculate how much the character has to turn to face goal direction
		Ogre::Real yawToGoal = toGoal.getYaw().valueDegrees();
		// this is how much the character CAN turn this frame
		Ogre::Real yawAtSpeed = yawToGoal / Ogre::Math::Abs(yawToGoal) * deltaTime * TURN_SPEED;
		// reduce "turnability" if we're in midair
		if(mBaseAnimID == ANIM_JUMP_LOOP)
			yawAtSpeed *= 0.2f;

		// turn as much as we can, but not more than we need to
		if(yawToGoal < 0)
		{
			yawToGoal = std::min<Ogre::Real>(0, std::max<Ogre::Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, yawAtSpeed, 0);
		} else 
		
		if(yawToGoal > 0)
		{
			yawToGoal = std::max<Ogre::Real>(0, std::min<Ogre::Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, 0, yawAtSpeed);
		}

		mBodyNode->yaw(Ogre::Degree(yawToGoal));

		// move in current body direction (not the goal direction)
		mBodyNode->translate(0, 0, deltaTime * RUN_SPEED * mAnims[mBaseAnimID]->getWeight(),
			Ogre::Node::TS_LOCAL);

		Ogre::Vector3 Position = mBodyNode->getPosition();
		Ogre::Terrain *Terrain = mOwner->getTerrainManager()->GetTerrainGroup()->getTerrain(0, 0);
		float MinHeight = Terrain->getHeightAtWorldPosition(Ogre::Vector3(Position.x, 0, Position.z)) + CHAR_HEIGHT;
		mBodyNode->setPosition(Position.x, MinHeight, Position.z);
	}

	if(mBaseAnimID == ANIM_JUMP_LOOP)
	{
		// if we're jumping, add a vertical offset too, and apply gravity
		mBodyNode->translate(0, mVerticalVelocity * deltaTime, 0, Ogre:: Node::TS_LOCAL);
		mVerticalVelocity -= GRAVITY * deltaTime;
			
		Ogre::Vector3 Position = mBodyNode->getPosition();
		Ogre::Terrain *Terrain = mOwner->getTerrainManager()->GetTerrainGroup()->getTerrain(0, 0);
		float MinHeight = Terrain->getHeightAtWorldPosition(Ogre::Vector3(Position.x, 0, Position.z)) + CHAR_HEIGHT;
		if(Position.y <= MinHeight)
		{
			// if we've hit the ground, change to landing state
			Position.y = MinHeight;
			mBodyNode->setPosition(Position);
			SetBaseAnimation(ANIM_JUMP_END, true);
			mTimer = 0;
		}
	}
}

void CharacterController::UpdateAnimations(Ogre::Real deltaTime)
{
	Ogre::Real baseAnimSpeed = 1;
	Ogre::Real topAnimSpeed = 1;

	mTimer += deltaTime;

	if(mTopAnimID == ANIM_DRAW_SWORDS)
	{
		// flip the draw swords animation if we need to put it back
		topAnimSpeed = mSwordsDrawn ? -1 : 1;

		// half-way through the animation is when the hand grasps the handles...
		if(	mTimer >= mAnims[mTopAnimID]->getLength() / 2 &&
			mTimer - deltaTime < mAnims[mTopAnimID]->getLength() / 2)
		{
			// so transfer the swords from the sheaths to the hands
			mBodyEnt->detachAllObjectsFromBone();
			mBodyEnt->attachObjectToBone(mSwordsDrawn ? "Sheath.L" : "Handle.L", mSword1);
			mBodyEnt->attachObjectToBone(mSwordsDrawn ? "Sheath.R" : "Handle.R", mSword2);
			// change the hand state to grab or let go
			mAnims[ANIM_HANDS_CLOSED]->setEnabled(!mSwordsDrawn);
			mAnims[ANIM_HANDS_RELAXED]->setEnabled(mSwordsDrawn);

			// toggle sword trails
			if (mSwordsDrawn)
			{
				mSwordTrail->setVisible(false);
				mSwordTrail->removeNode(mSword1->getParentNode());
				mSwordTrail->removeNode(mSword2->getParentNode());
			}
			else
			{
				mSwordTrail->setVisible(true);
				mSwordTrail->addNode(mSword1->getParentNode());
				mSwordTrail->addNode(mSword2->getParentNode());
			}
		}

		if (mTimer >= mAnims[mTopAnimID]->getLength())
		{
			// animation is finished, so return to what we were doing before
			if (mBaseAnimID == ANIM_IDLE_BASE) SetTopAnimation(ANIM_IDLE_TOP);
			else
			{
				SetTopAnimation(ANIM_RUN_TOP);
				mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
			}
			mSwordsDrawn = !mSwordsDrawn;
		}
	}
	else if (mTopAnimID == ANIM_SLICE_VERTICAL || mTopAnimID == ANIM_SLICE_HORIZONTAL)
	{
		if (mTimer >= mAnims[mTopAnimID]->getLength())
		{
			// animation is finished, so return to what we were doing before
			if (mBaseAnimID == ANIM_IDLE_BASE) SetTopAnimation(ANIM_IDLE_TOP);
			else
			{
				SetTopAnimation(ANIM_RUN_TOP);
				mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
			}
		}

		// don't sway hips from side to side when slicing. that's just embarrassing.
		if (mBaseAnimID == ANIM_IDLE_BASE) baseAnimSpeed = 0;
	}
	else if (mBaseAnimID == ANIM_JUMP_START)
	{
		if (mTimer >= mAnims[mBaseAnimID]->getLength())
		{
			// takeoff animation finished, so time to leave the ground!
			SetBaseAnimation(ANIM_JUMP_LOOP, true);
			// apply a jump acceleration to the character
			mVerticalVelocity = JUMP_ACCEL;
		}
	}
	else if (mBaseAnimID == ANIM_JUMP_END)
	{
		if (mTimer >= mAnims[mBaseAnimID]->getLength())
		{
			// safely landed, so go back to running or idling
			if (mKeyDirection == Ogre::Vector3::ZERO)
			{
				SetBaseAnimation(ANIM_IDLE_BASE);
				SetTopAnimation(ANIM_IDLE_TOP);
			}
			else
			{
				SetBaseAnimation(ANIM_RUN_BASE, true);
				SetTopAnimation(ANIM_RUN_TOP, true);
			}
		}
	}

	// increment the current base and top animation times
	if (mBaseAnimID != ANIM_NONE) mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);
	if (mTopAnimID != ANIM_NONE) mAnims[mTopAnimID]->addTime(deltaTime * topAnimSpeed);

	// apply smooth transitioning between our animations
	FadeAnimations(deltaTime);
}

void CharacterController::FadeAnimations(Ogre::Real deltaTime)
{
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		if (mFadingIn[i])
		{
			// slowly fade this animation in until it has full weight
			Ogre::Real newWeight = mAnims[i]->getWeight() + deltaTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
			if (newWeight >= 1) mFadingIn[i] = false;
		}
		else if (mFadingOut[i])
		{
			// slowly fade this animation out until it has no weight, and then disable it
			Ogre::Real newWeight = mAnims[i]->getWeight() - deltaTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
			if (newWeight <= 0)
			{
				mAnims[i]->setEnabled(false);
				mFadingOut[i] = false;
			}
		}
	}
}

void CharacterController::UpdateCamera(Ogre::Real deltaTime)
{
}

void CharacterController::UpdateCameraGoal(Ogre::Real deltaYaw, Ogre::Real deltaPitch, Ogre::Real deltaZoom)
{
}

void CharacterController::SetBaseAnimation(AnimID id, bool reset)
{
	if (mBaseAnimID >= 0 && mBaseAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mBaseAnimID] = false;
		mFadingOut[mBaseAnimID] = true;
	}

	mBaseAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}

void CharacterController::SetTopAnimation(AnimID id, bool reset)
{
	if (mTopAnimID >= 0 && mTopAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mTopAnimID] = false;
		mFadingOut[mTopAnimID] = true;
	}

	mTopAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}
