#include "stdafx.h"
#include "CameraManager.h"

#include "CameraWnd.h"
#include "SceneObject.h"

#include "OgreCamera.h"
#include "OgreMath.h"
#include "OgreSceneManager.h"

#include "OIS\OISkeyboard.h"
#include "OIS\OISmouse.h"

float CameraManager::defaultCameraSpeed = 20.0f;
CameraManager::CameraManager()
{
	mode = MANUAL;
	camera = NULL; speed = defaultCameraSpeed;
	target = NULL; dragLook = false;
}

CameraManager::~CameraManager()
{
}

CameraManager::Mode CameraManager::getMode()
{
	return mode;
}

void CameraManager::setMode(Mode mode)
{
	if(this->mode != ORBIT && mode == ORBIT)
    {
        camera->setFixedYawAxis(true);
    } else 
	if(this->mode != ROAMING && mode == ROAMING)
    {
		camera->setAutoTracking(false);
        camera->setFixedYawAxis(true);
    } else 
	if(this->mode != MANUAL && mode == MANUAL)
    {
        camera->setAutoTracking(false);
    }

    this->mode = mode;
}

Ogre::Camera *CameraManager::getCamera()
{
	return camera;
}

void CameraManager::setCamera(Ogre::Camera *camera)
{
	this->camera = camera;
}

float CameraManager::getSpeed()
{
	return speed;
}

void CameraManager::setSpeed(float speed)
{
	this->speed = speed;
}

SceneObject *CameraManager::getTarget()
{
	return target;
}

void CameraManager::setTarget(SceneObject *target)
{
	if(this->target != target)
    {
        this->target = target;
        if(target)
			camera->setAutoTracking(true, target->getSceneNode());
        else
            camera->setAutoTracking(false);
    }
}

bool CameraManager::isDragLook()
{
	return dragLook;
}

void CameraManager::setDragLook(bool dragLook)
{
	if(dragLook)
		setMode(MANUAL);
    else
		setMode(ROAMING);
	this->dragLook = dragLook;
}

void CameraManager::faceTo(SceneObject *object)
{
	float camera2Object = 2 * object->getBoundingRadius() + camera->getNearClipDistance();
	camera->setPosition(0, object->getBoundingRadius() * 0.25f, camera2Object);
}

/**
	x-pitch, y-yaw, z-roll
*/
void CameraManager::roaming(OIS::Keyboard *keyboard, OIS::Mouse *mouse, float elapsed)
{
	if(dragLook)
	{
		if(GetKeyState(VK_RBUTTON) & 0x8000)
			setMode(ROAMING);
		else
			setMode(MANUAL);
	}

	switch(mode)
	{
	case ROAMING:
		{
			Ogre::Vector3 delta  = Ogre::Vector3::ZERO;
			if (keyboard->isKeyDown(OIS::KC_W) == TRUE) 
				delta += camera->getDerivedDirection();
			if (keyboard->isKeyDown(OIS::KC_S) == TRUE) 
				delta -= camera->getDerivedDirection();
			if (keyboard->isKeyDown(OIS::KC_A) == TRUE) 
				delta -= camera->getDerivedRight();
			if (keyboard->isKeyDown(OIS::KC_D) == TRUE) 
				delta += camera->getDerivedRight();
			delta.normalise();

			camera->move(delta * elapsed * speed);
			camera->yaw(-Ogre::Radian(mouse->getMouseState().X.rel * 0.005f));
			camera->pitch(-Ogre::Radian(mouse->getMouseState().Y.rel * 0.005f));
		}
		break;

	case ORBIT:
		{
			if(target != NULL)
			{
				Ogre::Real camera2Object = (camera->getPosition() - target->getSceneNode()->getPosition()).length();
				
				if(GetKeyState(VK_RBUTTON) & 0x8000)
				{
					camera->setPosition(target->getSceneNode()->_getDerivedPosition());
					camera->yaw(-Ogre::Radian(mouse->getMouseState().X.rel * 0.005f));
					camera->pitch(-Ogre::Radian(mouse->getMouseState().Y.rel * 0.005f));

					camera->moveRelative(Ogre::Vector3(0, 0, camera2Object));
				} else 
				
				if (mouse->getMouseState().Z.rel != 0)
				{
					camera->moveRelative(Ogre::Vector3(0, 0, -mouse->getMouseState().Z.rel * camera2Object * 0.0008f));
				}
			}
		}
	}

	CCameraWnd::Current->update(this);
}
