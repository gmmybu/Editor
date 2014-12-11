#pragma once

class SceneObject;

namespace Ogre
{
	class Camera;
}

namespace OIS
{
	class Keyboard;
	class Mouse;
}

class CameraManager
{
public:
	enum Mode
	{
		ROAMING,
		ORBIT,
		MANUAL
	};

	static float defaultCameraSpeed;

public:
    CameraManager();
	~CameraManager();

	Mode getMode();
	void setMode(Mode mode);

	Ogre::Camera *getCamera();
	void setCamera(Ogre::Camera *camera);

	float getSpeed();
	void setSpeed(float speed);

	SceneObject *getTarget();
	void setTarget(SceneObject *target);

	bool isDragLook();
	void setDragLook(bool dragLook);

	void faceTo(SceneObject *object);
	void roaming(OIS::Keyboard *keyboard, OIS::Mouse *mouse, float elapsed);

private:
	Mode mode;
    Ogre::Camera *camera; float speed;
    SceneObject *target; bool dragLook;
};
