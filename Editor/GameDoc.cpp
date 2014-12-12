#include "stdafx.h"
#include "Editor.h"
#include "GameDoc.h"
#include "GameView.h"

#include "OgreCamera.h"
#include "OgreRoot.h"
#include "OgreSceneManager.h"

IMPLEMENT_DYNCREATE(CGameDoc, CDocument)

CGameDoc *CGameDoc::Current = NULL;
CGameDoc::CGameDoc()
{
	Current = this;
}

CGameDoc::~CGameDoc()
{
}

void CGameDoc::Initialize()
{
}

void CGameDoc::Destroy()
{
}

void CGameDoc::update(float Elapsed)
{
	POSITION ViewPos = GetFirstViewPosition();
	while(ViewPos != NULL)
	{
		((CGameView*)GetNextView(ViewPos))->Invalidate(FALSE);
	}
}

BEGIN_MESSAGE_MAP(CGameDoc, CDocument)
END_MESSAGE_MAP()
