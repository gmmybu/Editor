#include "stdafx.h"
#include "GameListener.h"

#include "GameDoc.h"
#include "Application.h"
// #include "d3d9.h"

void CGameListener::preRenderQueues()
{
	SpeedTree::CApplication *Application = CGameDoc::Current->GetApplication();
	if(Application && Application->ReadyToRender( ))
    {
        Application->Advance( );
        Application->Cull( );
    }
}

void CGameListener::postRenderQueues()
{
}

void CGameListener::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, 
	bool& skipThisInvocation)
{
}

void CGameListener::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, 
	bool& repeatThisInvocation)
{
	//IDirect3DDevice9 *Device = CGameDoc::Current->GetDevice();
	//if(Device != NULL)
	//{
	//	// Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0xff, 0, 0), 1.0f, 0);
	//}

	bool bInitError = false;
    static bool bFirstDisplay = true;
    if (bFirstDisplay)
    {
		if (!CGameDoc::Current->GetApplication()->InitGraphics( ))
        {
            bInitError = true;
            SpeedTree::PrintSpeedTreeErrors( );
            system("pause");
            exit(-1);
        }
        bFirstDisplay = false;
    }

    if (!bInitError)
    {
        CGameDoc::Current->GetApplication()->Render( );
    }
}
