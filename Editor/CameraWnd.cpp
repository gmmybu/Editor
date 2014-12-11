#include "stdafx.h"
#include "CameraWnd.h"
#include "CameraManager.h"

#include "OgreCamera.h"
#include "OgreStringConverter.h"

IMPLEMENT_DYNAMIC(CCameraWnd, CBCGPDockingControlBar)

CCameraWnd *CCameraWnd::Current = NULL;
CCameraWnd::CCameraWnd():
	mCameraManager(NULL)
{
	Current = this;
}

CCameraWnd::~CCameraWnd()
{
}

void CCameraWnd::update(CameraManager *CameraManager)
{
	mCameraManager = CameraManager;
	Ogre::Camera *Camera = mCameraManager->getCamera();

	const Ogre::Vector3 &CameraPos = Camera->getDerivedPosition();
	mProps[CW_XPOS]->SetValue(CameraPos.x);
	mProps[CW_YPOS]->SetValue(CameraPos.y);
	mProps[CW_ZPOS]->SetValue(CameraPos.z);

	Ogre::Vector3 CameraDir = Camera->getDerivedDirection();
	mProps[CW_XDIR]->SetValue(CameraDir.x);
	mProps[CW_YDIR]->SetValue(CameraDir.y);
	mProps[CW_ZDIR]->SetValue(CameraDir.z);
}

BEGIN_MESSAGE_MAP(CCameraWnd, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, &CCameraWnd::OnPropertyChanged )
END_MESSAGE_MAP()

int CCameraWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	mPropList.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER, rectDummy, this, 0);
	mPropList.EnableHeaderCtrl(FALSE);
	mPropList.SetVSDotNetLook();

	CBCGPProp *Tmp = NULL;

	mProps[CW_SPEED] = new CBCGPProp("Speed", (_variant_t)CameraManager::defaultCameraSpeed, "", CW_SPEED);
	mPropList.AddProperty(mProps[CW_SPEED]);

	mProps[CW_POLYGON_MODE] = new CBCGPProp(
		"PolygonMode", (_variant_t)"Solid", "", CW_POLYGON_MODE);
	mProps[CW_POLYGON_MODE]->AllowEdit(FALSE);
	mProps[CW_POLYGON_MODE]->AddOption("Points");
	mProps[CW_POLYGON_MODE]->AddOption("Wireframe");
	mProps[CW_POLYGON_MODE]->AddOption("Solid");
	mPropList.AddProperty(mProps[CW_POLYGON_MODE]);

	Tmp = new CBCGPProp("Position");
	mProps[CW_XPOS] = new CBCGPProp("X", (_variant_t)0.0f, "", CW_XPOS);
	mProps[CW_XPOS]->AllowEdit(FALSE);
	Tmp->AddSubItem(mProps[CW_XPOS]);
	mProps[CW_YPOS] = new CBCGPProp("Y", (_variant_t)0.0f, "", CW_YPOS);
	mProps[CW_YPOS]->AllowEdit(FALSE);
	Tmp->AddSubItem(mProps[CW_YPOS]);
	mProps[CW_ZPOS] = new CBCGPProp("Z", (_variant_t)0.0f, "", CW_ZPOS);
	mProps[CW_ZPOS]->AllowEdit(FALSE);
	Tmp->AddSubItem(mProps[CW_ZPOS]);
	mPropList.AddProperty(Tmp);

	Tmp = new CBCGPProp("Direction");
	mProps[CW_XDIR] = new CBCGPProp("X", (_variant_t)0.0f, "", CW_XDIR);
	mProps[CW_XDIR]->AllowEdit(FALSE);
	Tmp->AddSubItem(mProps[CW_XDIR]);
	mProps[CW_YDIR] = new CBCGPProp("Y", (_variant_t)0.0f, "", CW_YDIR);
	mProps[CW_YDIR]->AllowEdit(FALSE);
	Tmp->AddSubItem(mProps[CW_YDIR]);
	mProps[CW_ZDIR] = new CBCGPProp("Z", (_variant_t)0.0f, "", CW_ZDIR);
	mProps[CW_ZDIR]->AllowEdit(FALSE);
	Tmp->AddSubItem(mProps[CW_ZDIR]);
	mPropList.AddProperty(Tmp);

	mPropList.ExpandAll();
	AdjustLayout();

	return 0;
}

void CCameraWnd::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout();
}

LRESULT CCameraWnd::OnPropertyChanged(WPARAM wparam, LPARAM lparam)
{
	if(mCameraManager == NULL)
		return 0;

	CBCGPProp *Prop = (CBCGPProp*)lparam;
	switch(Prop->GetData())
	{
	case CW_SPEED:
		{
			float Speed = mProps[CW_SPEED]->GetValue();
			mCameraManager->setSpeed(Speed);
		}
		break;

	case CW_POLYGON_MODE:
		CString strPolygonMode = mProps[CW_POLYGON_MODE]->GetValue();
		if(strPolygonMode == "Solid")
		{
			mCameraManager->getCamera()->setPolygonMode(Ogre::PM_SOLID);
		} else 
		if(strPolygonMode == "Wireframe")
		{
			mCameraManager->getCamera()->setPolygonMode(Ogre::PM_WIREFRAME);
		} else 
		if(strPolygonMode == "Points")
		{
			mCameraManager->getCamera()->setPolygonMode(Ogre::PM_POINTS);
		}
	}

	return 0;
}

void CCameraWnd::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	mPropList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}
