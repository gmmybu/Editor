#pragma once

#include "OgreVector3.h"

class SceneDoc;

namespace OIS
{
	class Keyboard; 
	class Mouse;
}

#define NUM_ANIMS 13 // number of animations the character has

class CharacterController
{
public:
	CharacterController(SceneDoc *Owner);
	~CharacterController();

	void SetupBody();
	void SetupAnimations();
	void SetupCamera();

	void update(float Elapsed);
	void Roaming(OIS::Keyboard *Keyboard, OIS::Mouse *Mouse, float Elapsed);

private:
	enum AnimID
	{
		ANIM_IDLE_BASE,
		ANIM_IDLE_TOP,
		ANIM_RUN_BASE,
		ANIM_RUN_TOP,
		ANIM_HANDS_CLOSED,
		ANIM_HANDS_RELAXED,
		ANIM_DRAW_SWORDS,
		ANIM_SLICE_VERTICAL,
		ANIM_SLICE_HORIZONTAL,
		ANIM_DANCE,
		ANIM_JUMP_START,
		ANIM_JUMP_LOOP,
		ANIM_JUMP_END,
		ANIM_NONE
	};

	void UpdateBody(float deltaTime);
	void UpdateAnimations(float deltaTime);
	void UpdateCamera(float deltaTime);
	void UpdateCameraGoal(float deltaYaw, float deltaPitch, float deltaZoom);
	
	void SetBaseAnimation(AnimID id, bool reset = false);
	void SetTopAnimation(AnimID id, bool reset = false);
	void FadeAnimations(float deltaTime);

	SceneDoc *mOwner;

	Ogre::SceneNode* mBodyNode;
	Ogre::Entity* mBodyEnt;
	Ogre::Entity* mSword1;
	Ogre::Entity* mSword2;
	Ogre::RibbonTrail *mSwordTrail;
	Ogre::AnimationState *mAnims[NUM_ANIMS];
	AnimID mBaseAnimID;
	AnimID mTopAnimID;
	bool mFadingIn[NUM_ANIMS];
	bool mFadingOut[NUM_ANIMS];
	bool mSwordsDrawn;
	Ogre::Vector3 mKeyDirection;
	Ogre::Vector3 mGoalDirection;
	float mVerticalVelocity;
	float mTimer;
};
