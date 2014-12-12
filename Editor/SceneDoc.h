#pragma once

#include "OgreShadowCameraSetup.h"
#include "RenderScene.h"
#include "ResourceManagerTree.h"
#include "TerrainManager.h"
#include <vector>

class CNewSceneDlg;
class CObjectEditHandler;
class CTerrainEditHandler;
class CTerrainManager;
class DeferredShadingSystem;
class DynamicModel;
class Light;
class LiquidEditHandler;
class NewLightDlg;
class SceneObject;
class SceneView;
class StaticMesh;

class SceneDoc : public RenderScene
{
	DECLARE_DYNCREATE(SceneDoc)
public:
	SceneDoc();
	virtual ~SceneDoc();

	bool isGameMode() { return gameMode; }
	bool isPaste() { return paste; }

	CString getSceneName() { return sceneName; }
	void setSceneName(CString sceneName) { SetTitle(sceneName); this->sceneName = sceneName; }

	CString getSkyType() { return skyType; }
	CString getSkyMaterial() { return skyMaterial; }

	SceneView *getActiveView() { return activeView; }
	void setActiveView(SceneView *activeView) { this->activeView = activeView; }

	DeferredShadingSystem *getDeferredShadingSystem() { return deferredShadingSystem; }
	void setDeferredShadingSystem(DeferredShadingSystem *deferredShadingSystem) { this->deferredShadingSystem = deferredShadingSystem; }

	CTerrainManager *getTerrainManager() { return terrainManager; }
	CTerrainManagerConfig *getTerrainManagerConfig() { return &terrainManagerConfig; }

	CTerrainEditHandler *getTerrainEditHandler() { return terrainEditHandler; }
	CObjectEditHandler *getObjectEditHandler() { return objectEditHandler; }
	LiquidEditHandler *getLiquidEditHandler() { return liquidEditHandler; }

	int getNameID() { return nameID; }
	void setNameID(int nameID) { this->nameID = nameID; }

	std::vector<SceneObject*> &getObjects() { return objects; }

	/** 防止在未初始化的情况下点中SceneResourceTree而崩溃
	*/
	bool isInitialized() { return initialized; }
	void initialize(CNewSceneDlg *Dlg); void initialize(CString Filename);
	void destroy();

	StaticMesh *addStaticMesh(CString path);
	DynamicModel *addDynamicModel(CString path);
	Light *addLight();

	void selectObject(SceneObject *object);
	void removeObject(SceneObject *object);
	void configureShadows(bool enabled, bool depthShadows);

	void roaming(CPoint point, float elapsed);
	void update(float elapsed);

	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);

	static SceneDoc *current;

	afx_msg void OnObjectEdit(UINT id);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSaveScene();
	afx_msg void OnGameMode();
	afx_msg void OnUpdateGameMode(CCmdUI *pCmdUI);
	afx_msg void OnUpdateObjectEdit(CCmdUI* pCmdUI);
	afx_msg void OnAddLight();
	afx_msg void OnUpdateAddLight(CCmdUI *pCmdUI);
	afx_msg void OnObjectPaste();
	afx_msg void OnUpdateObjectPaste(CCmdUI *pCmdUI);
	afx_msg void OnShowDebugOverlay();
	afx_msg void OnUpdateShowDebugOverlay(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBrushMenu(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTextureMenu(CCmdUI* pCmdUI);

	int editMode;
	bool gameMode; bool paste; bool showDebugOverlay;
	bool deferredShading; bool ssao;

	bool initialized;
	CString sceneName;
	CString skyType; CString skyMaterial;
	
	SceneView *activeView;
	DeferredShadingSystem *deferredShadingSystem;

	CTerrainManager *terrainManager;
	CTerrainManagerConfig terrainManagerConfig;
	Ogre::ShadowCameraSetupPtr shadowCameraSetup;

	CTerrainEditHandler *terrainEditHandler;
	CObjectEditHandler *objectEditHandler;
	LiquidEditHandler *liquidEditHandler;

	int nameID;
	std::vector<SceneObject*> objects;
	afx_msg void OnDeferredShading();
	afx_msg void OnUpdateDeferredShading(CCmdUI *pCmdUI);
	afx_msg void OnSsao();
	afx_msg void OnUpdateSsao(CCmdUI *pCmdUI);
};
