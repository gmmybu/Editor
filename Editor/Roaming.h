#pragma once

class RenderView;

namespace OIS
{
	class InputManager;
	class Keyboard; class Mouse;
}

class Roaming
{
public:
	Roaming();
	~Roaming();

	RenderView *getActiveView();
	void setActiveView(RenderView *activeView);

	void initialize();
	void process(float Elapsed);

private:
	void createNonMainDevice(CWnd *Wnd);

	bool initialized;
	RenderView *activeView;

	OIS::Keyboard *activeKeyboard;
	OIS::Mouse *activeMouse;
	
	OIS::InputManager *mainFrmInputManager;
	OIS::Keyboard *mainFrmKeyboard; OIS::Mouse *mainFrmMouse;

	OIS::InputManager *lastInputManager;
	OIS::Keyboard *lastKeyboard; OIS::Mouse *lastMouse;
	CWnd *lastDeviceOwner;
};
