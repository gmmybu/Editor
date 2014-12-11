#pragma once

#include "OgreString.h"

class TiXmlElement;

enum KResourceManagerTreeImages
{
	RMTI_SCENE,
	RMTI_PACKAGE,
	RMTI_PACKAGE_OPEN,
	RMTI_MESH,
	RMTI_MODEL,
	RMTI_PARTICLE,
	RMTI_LIGHT
};

class CResourceManagerTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CResourceManagerTree)
public:
	CResourceManagerTree();
	virtual ~CResourceManagerTree();

	void ResetImageList();
	void RebuildTree();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	TiXmlElement *MakeElement(const char *Name, KResourceManagerTreeImages Type, const char *Path);
	void ListAllResource(Ogre::String ResGrp, Ogre::String Ext, Ogre::String Folder, HTREEITEM Parent);

	CImageList mImageList;
	bool mDragging;
};
