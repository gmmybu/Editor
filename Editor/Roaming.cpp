#include "stdafx.h"
#include "Roaming.h"
#include "RenderView.h"

#include "OIS\OISInputManager.h"
#include "OIS\OISKeyboard.h"
#include "OIS\OISMouse.h"

#include <sstream>

Roaming::Roaming(void)
{
	initialized = false;
}


Roaming::~Roaming(void)
{
}

void Roaming::initialize()
{
	OIS::ParamList parameters;
    std::ostringstream windowHandleString;
	windowHandleString << (size_t)AfxGetMainWnd()->GetSafeHwnd();
    parameters.insert(std::make_pair("WINDOW", windowHandleString.str()));
	parameters.insert(std::make_pair("w32_keyboard", "DISCL_NONEXCLUSIVE"));
	parameters.insert(std::make_pair("w32_keyboard", "DISCL_FOREGROUND"));
	parameters.insert(std::make_pair("w32_mouse", "DISCL_NONEXCLUSIVE"));
	parameters.insert(std::make_pair("w32_mouse", "DISCL_FOREGROUND"));
	mainFrmInputManager = OIS::InputManager::createInputSystem(parameters);
	mainFrmKeyboard = (OIS::Keyboard*)mainFrmInputManager->createInputObject(OIS::OISKeyboard, true);
	mainFrmMouse = (OIS::Mouse*)mainFrmInputManager->createInputObject(OIS::OISMouse, true);
	initialized = TRUE;
}

RenderView *Roaming::getActiveView()
{
	return activeView;
}

void Roaming::setActiveView(RenderView *activeView)
{
	if(!initialized)
		return;

	if(this->activeView == activeView)
		return;

	if(activeView)
	{
		CWnd *Parent = activeView;
		while(Parent = Parent->GetParent())
		{
			if(Parent->IsKindOf(RUNTIME_CLASS(CBCGPMiniFrameWnd))) // 浮动窗口的父窗口是CBCGPMiniFrameWnd
			{
				createNonMainDevice(Parent);
				activeKeyboard = lastKeyboard; activeMouse = lastMouse;
				break;
			}
		}
		if(Parent == NULL)
		{
			activeKeyboard = mainFrmKeyboard; activeMouse = mainFrmMouse;
		}
	}

	this->activeView = activeView;
}

void Roaming::process(float Elapsed)
{
	if(!initialized)
		return;

	if(activeView)
	{
		activeKeyboard->capture(); activeMouse->capture();
		activeView->roaming(activeKeyboard, activeMouse, Elapsed);
	}
}

void Roaming::createNonMainDevice(CWnd *Wnd)
{
	if(lastDeviceOwner == Wnd)
		return;

	if(lastInputManager)
	{
		lastInputManager->destroyInputObject(lastKeyboard); lastInputManager->destroyInputObject(lastMouse);
		OIS::InputManager::destroyInputSystem(lastInputManager);
	}

	OIS::ParamList parameters;
    std::ostringstream windowHandleString;
	windowHandleString << (size_t)Wnd->GetSafeHwnd();
    parameters.insert(std::make_pair("WINDOW", windowHandleString.str()));
	parameters.insert(std::make_pair("w32_keyboard", "DISCL_NONEXCLUSIVE"));
	parameters.insert(std::make_pair("w32_keyboard", "DISCL_FOREGROUND"));
	parameters.insert(std::make_pair("w32_mouse", "DISCL_NONEXCLUSIVE"));
	parameters.insert(std::make_pair("w32_mouse", "DISCL_FOREGROUND"));
	lastInputManager = OIS::InputManager::createInputSystem(parameters);
	lastKeyboard = (OIS::Keyboard*)mainFrmInputManager->createInputObject(OIS::OISKeyboard, true);
	lastMouse= (OIS::Mouse*)mainFrmInputManager->createInputObject(OIS::OISMouse, true);
	lastDeviceOwner = Wnd;
}
