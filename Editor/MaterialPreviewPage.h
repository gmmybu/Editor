#pragma once

#include "CameraManager.h"
#include "PropertyWnd.h"
#include <string>
#include "afxwin.h"

class TiXmlElement;
class RenderView;
class StaticMesh;

namespace Ogre
{
	class ResourceManager;
	class SceneManager;
	class Camera; class Light;
}

enum KMaterialPreviewPage
{
	MPP_BACKGROUND_COLOUR,
	MPP_AMBIENT_LIGHT,
	MPP_LIGHT_NAME,
	MPP_LIGHT_TYPE,
	MPP_LIGHT_DIFFUSE_COLOUR,
	MPP_LIGHT_SPECULAR_COLOUR,
	MPP_LIGHT_POSITION_X,
	MPP_LIGHT_POSITION_Y,
	MPP_LIGHT_POSITION_Z,
	MPP_LIGHT_DIRECTION_X,
	MPP_LIGHT_DIRECTION_Y,
	MPP_LIGHT_DIRECTION_Z,
	MPP_LIGHT_VISIBLE
};

class CMaterialPreviewPage : public CBCGPDialog, public CPropertyListener
{
	DECLARE_DYNAMIC(CMaterialPreviewPage)
public:
	CMaterialPreviewPage(CWnd* pParent = NULL);
	virtual ~CMaterialPreviewPage();

	CString GetCurrentFile() { return mCurrentFile; }
	void SetCurrentFile(CString CurrentFile) { mCurrentFile = CurrentFile; }

	CListBox *GetMaterialList() { return &mMaterialList; }
	void ClearMaterialList() { mMaterialList.ResetContent(); }

	CListBox *GetCompositorList() { return &mCompositorList; }
	void ClearCompositorList() { mCompositorList.ResetContent(); }

	void AfterSelectResource(TiXmlElement *Elmt);
	
	void update(float Elapsed);
	void FirePropertyChanged() {}
	void OnPropertyChanged(CBCGPProp *Prop);

	static CMaterialPreviewPage *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnCbnSelchangeAnimList();
	afx_msg void OnBnClickedQuad();
	afx_msg void OnBnClickedEditProgram();
	afx_msg void OnBnClickedAddLight();
	afx_msg void OnBnClickedDelLight();
	afx_msg void OnLbnSelchangeLights();
	afx_msg void OnBnClickedApplyMaterial();
	afx_msg void OnBnClickedClearMaterial();
	afx_msg void OnBnClickedAddCompositor();
	afx_msg void OnBnClickedDelCompositor();
	afx_msg void OnBnClickedEnableCompositor();
	afx_msg void OnBnClickedDisableCompositor();

	enum { IDD = IDD_MATERIAL_PREVIEW };

	void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();
	void ClearObject();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// From http://www.ogre3d.org/tikiwiki/Reloading+materials+and+parsing+material+scripts
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void ReloadMaterial(const std::string& materialName, const std::string& groupName, const std::string& filename, bool parseMaterialScript);
	void ReloadCompositor(const std::string& compositorName, const std::string &groupName, const std::string& filename, bool parseCompositorScript);
	void UnloadMaterials(const std::string& filename);
	void UnloadCompositors(const std::string& filename);
	void UnloadVertexPrograms(const std::string& filename);
	void UnloadFragmentPrograms(const std::string& filename);
	void UnloadResource(Ogre::ResourceManager* resMgr, const std::string& resourceName);
	void LoadResource(Ogre::ResourceManager* resMgr, const std::string& resourceName, const std::string& resourceGroup);

	Ogre::SceneManager *mSceneManager;
	Ogre::Camera *mCamera; CameraManager mCameraManager;

	RenderView *mView;
	CStatic mMaterialFrame;

	CComboBox mAnimList;
	CString mPrevAnim;
	StaticMesh *mObject;

	CString mCurrentFile;
	CListBox mLights;
	CListBox mMaterialList;
	CListBox mCurrentCompositors;
	CListBox mCompositorList;
};
