#include "stdafx.h"
#include "Editor.h"
#include "SceneView.h"
#include "SceneDoc.h"

#include "ResourceManagerTree.h"
#include "StringUtils.h"

#include "OgreStringVector.h"
#include "OgreResourceGroupManager.h"

#include "tinyxml.h"

IMPLEMENT_DYNAMIC(CResourceManagerTree, CTreeCtrl)

CResourceManagerTree::CResourceManagerTree():
	mDragging(FALSE)
{

}

CResourceManagerTree::~CResourceManagerTree()
{
}

void CResourceManagerTree::ResetImageList()
{
	CBitmap Bmp;
	if (!Bmp.LoadBitmap(IDB_RESOURCE_MANAGER_TREE))
	{
		TRACE("无法加载位图: %x\n", IDB_RESOURCE_MANAGER_TREE);
		return;
	}

	BITMAP BmpObj;
	Bmp.GetBitmap(&BmpObj);

	UINT nFlags = ILC_MASK | ILC_COLOR24;
	mImageList.Create(16, BmpObj.bmHeight, nFlags, 0, 0);
	mImageList.Add(&Bmp, RGB(255, 0, 255));

	SetImageList(&mImageList, TVSIL_NORMAL);
}

void CResourceManagerTree::RebuildTree()
{
	HTREEITEM Root = InsertItem("资源管理树", RMTI_SCENE, RMTI_SCENE);
	HTREEITEM StaticMesh = InsertItem("静态实体", RMTI_PACKAGE, RMTI_PACKAGE_OPEN, Root);
	HTREEITEM DynamicMesh = InsertItem("动态实体", RMTI_PACKAGE, RMTI_PACKAGE_OPEN, Root);
	HTREEITEM Particle = InsertItem("粒子", RMTI_PACKAGE, RMTI_PACKAGE_OPEN, Root);

	Ogre::StringVector ResGrps = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
	Ogre::StringVector::iterator ResGrpIter = ResGrps.begin();
	for(;ResGrpIter!=ResGrps.end(); ResGrpIter++)
	{
		HTREEITEM ResGrp = InsertItem((*ResGrpIter).c_str(), RMTI_PACKAGE, RMTI_PACKAGE_OPEN, StaticMesh);
		TiXmlElement *Elmt = MakeElement("ResGrp", RMTI_PACKAGE, (*ResGrpIter).c_str());
		SetItemData(ResGrp, (DWORD_PTR)Elmt);
		ListAllResource(*ResGrpIter, "mesh", "", ResGrp);

		ResGrp = InsertItem((*ResGrpIter).c_str(), RMTI_PACKAGE, RMTI_PACKAGE_OPEN, DynamicMesh);
		Elmt = MakeElement("ResGrp", RMTI_PACKAGE, (*ResGrpIter).c_str());
		SetItemData(ResGrp, (DWORD_PTR)Elmt);
		ListAllResource(*ResGrpIter, "model", "", ResGrp);

		ResGrp = InsertItem((*ResGrpIter).c_str(), RMTI_PACKAGE, RMTI_PACKAGE_OPEN, Particle);
		Elmt = MakeElement("ResGrp", RMTI_PACKAGE, (*ResGrpIter).c_str());
		SetItemData(ResGrp, (DWORD_PTR)Elmt);
		ListAllResource(*ResGrpIter, "particle", "", ResGrp);
	}

	
}

BEGIN_MESSAGE_MAP(CResourceManagerTree, CTreeCtrl)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CResourceManagerTree::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(mDragging)
	{	
		mDragging = false;
		ReleaseCapture();
		SelectDropTarget(NULL);

		CPoint ptScreen(point);
		ClientToScreen(&ptScreen);

		CWnd *pDropWnd = WindowFromPoint(ptScreen);
		if(pDropWnd->IsKindOf(RUNTIME_CLASS(SceneView)))
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			pDropWnd->SetFocus();
			
			TiXmlElement *Elmt = (TiXmlElement*)GetItemData(GetSelectedItem());
			if(Elmt != NULL)
			{
				KResourceManagerTreeImages Type;
				Elmt->QueryIntAttribute("Type", (int*)&Type);
				CString Path = Elmt->Attribute("Path");
				switch(Type)
				{
				case RMTI_MESH:
					{
						((SceneDoc*)((SceneView*)pDropWnd)->GetDocument())->addStaticMesh(Path);
					}
					break;

				case RMTI_MODEL:
					{
						((SceneDoc*)((SceneView*)pDropWnd)->GetDocument())->addDynamicModel(Path);
					}
				}
			}
		}
	}
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CResourceManagerTree::OnMouseMove(UINT nFlags, CPoint point)
{
	if(mDragging)
	{	
		CPoint ptScreen(point);
 		ClientToScreen(&ptScreen);
 
 		CWnd* pDropWnd = WindowFromPoint(ptScreen);
		if(pDropWnd->IsKindOf(RUNTIME_CLASS(SceneView)))
		{
			SetCursor(AfxGetApp()->LoadCursor(IDC_PLACE)); 
			return CTreeCtrl::OnMouseMove(nFlags, point);
		}

		if(pDropWnd == this)
		{
			HTREEITEM  hItem;
			UINT       flags;

			if((hItem = HitTest(point, &flags)) != NULL)
			{
				if(!(TVIS_SELECTED & GetItemState(hItem, TVIS_SELECTED)))
					SelectDropTarget(hItem);
			}
		}
		::SetCursor(LoadCursor(NULL, IDC_NO));
		return CTreeCtrl::OnMouseMove(nFlags, point);
	}

	CPoint ptScreen(point);
	ClientToScreen(&ptScreen);

	if(DragDetect(ptScreen))
	{
		mDragging = true;
		SetCapture();
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}

TiXmlElement *CResourceManagerTree::MakeElement(
	const char *Name, KResourceManagerTreeImages Type, const char *Path)
{
	TiXmlElement *Elmt = new TiXmlElement(Name);
	Elmt->SetAttribute("Type", Type);
	Elmt->SetAttribute("Path", Path);
	return Elmt;
}

void CResourceManagerTree::ListAllResource(Ogre::String ResGrp, Ogre::String Ext, Ogre::String Folder, HTREEITEM Parent)
{
	// 资源
	Ogre::String ResPattern = "*."+Ext;
	if(!Folder.empty())
	{
		ResPattern = Folder+"/"+ResPattern;
	}
	bool HasRes = false;
	Ogre::FileInfoListPtr Resources = 
		Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(ResGrp, ResPattern);
	struct ResNonCaseSensitiveCompare
	{   
		bool operator()(const Ogre::FileInfo& l, const Ogre::FileInfo& r)   
		{   
			return StringUtils::compare(l.basename, r.basename) < 0;
		}   
	};
	std::sort(Resources->begin(), Resources->end(), ResNonCaseSensitiveCompare());
	Ogre::FileInfoList::iterator ResIter = Resources->begin();
	for(;ResIter!=Resources->end(); ResIter++)
	{
		Ogre::FileInfo ResInfo = *ResIter;
		if(ResIter+1!=Resources->end() && StringUtils::compare((*ResIter).basename, (*(ResIter+1)).basename) == 0)
			continue;

		if(Ext == "mesh")
		{
			HasRes = true;
			HTREEITEM Res = InsertItem(ResInfo.basename.c_str(), RMTI_MESH, RMTI_MESH, Parent);
			TiXmlElement *Elmt = MakeElement("Res", RMTI_MESH, ResInfo.filename.c_str());
			SetItemData(Res, (DWORD_PTR)Elmt);
		} else

		if(Ext == "model")
		{
			HasRes = true;
			HTREEITEM Res = InsertItem(ResInfo.basename.c_str(), RMTI_MODEL, RMTI_MODEL, Parent);
			TiXmlElement *Elmt = MakeElement("Res", RMTI_MODEL, ResInfo.filename.c_str());
			SetItemData(Res, (DWORD_PTR)Elmt);
		} else 

		if(Ext == "particle")
		{
			HasRes = true;
			HTREEITEM Res = InsertItem(ResInfo.basename.c_str(), RMTI_PARTICLE, RMTI_PARTICLE, Parent);
			TiXmlElement *Elmt = MakeElement("Res", RMTI_PARTICLE, ResInfo.filename.c_str());
			SetItemData(Res, (DWORD_PTR)Elmt);
		}
	}
	// 目录
	Ogre::String SubFolderPattern = "*";
	if(!Folder.empty())
	{
		SubFolderPattern = Folder + "/" + SubFolderPattern;
	}
	bool HasSubFolder = false;
	Ogre::StringVectorPtr SubFolders = 
		Ogre::ResourceGroupManager::getSingleton().findResourceNames(
			ResGrp, SubFolderPattern, true);
	struct SubFolderNonCaseSensitiveCompare
	{   
		bool operator()(const std::string& l, const std::string& r)   
		{   
			return StringUtils::compare(l, r) < 0;
		}   
	};
	std::sort(SubFolders->begin(), SubFolders->end(), SubFolderNonCaseSensitiveCompare());
	Ogre::StringVector::iterator SubFolderIter = SubFolders->begin();
	for(;SubFolderIter!=SubFolders->end(); SubFolderIter++)
	{
		HasSubFolder = true;
		if(SubFolderIter+1!=SubFolders->end() && StringUtils::compare(*SubFolderIter, *(SubFolderIter+1)) == 0)
			continue;
		HTREEITEM SubFolder = InsertItem((*SubFolderIter).c_str(), RMTI_PACKAGE, RMTI_PACKAGE_OPEN, Parent);
		TiXmlElement *Elmt = MakeElement("Folder", RMTI_PACKAGE, (*SubFolderIter).c_str());
		SetItemData(SubFolder, (DWORD_PTR)Elmt);
		ListAllResource(ResGrp, Ext, *SubFolderIter, SubFolder);
	}
	if(!HasRes && !HasSubFolder)
	{
		DeleteItem(Parent);
	}
}
