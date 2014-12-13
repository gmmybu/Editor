#pragma once

#include "resource.h"

enum KNewSceneDlg
{
	NSD_SCENE_NAME,
	NSD_AMBIENT_LIGHT,
	NSD_SHADOW_TECHNIQUE,
	// Terrain global options
	NSD_LIGHT_MAP_DIRECTION_X,
	NSD_LIGHT_MAP_DIRECTION_Y,
	NSD_LIGHT_MAP_DIRECTION_Z,
	NSD_LIGHT_MAP_SIZE,
	NSD_COMPOSITE_MAP_DIFFUSE,
	NSD_COMPOSITE_MAP_DISTANCE,
	NSD_COMPOSITE_MAP_SIZE,
	// Default import settings
	NSD_FILENAME_PREFIX,
	NSD_FILENAME_EXTENSION,
	NSD_TERRAIN_SIZE,
	NSD_WORLD_SIZE,
	NSD_INPUT_SCALE,
	NSD_MIN_BATCH_SIZE,
	NSD_MAX_BATCH_SIZE,
	NSD_TEXTURE_WORLD_SIZE,
	NSD_DIFFUSE_SPECULAR,
	NSD_NORMAL_HEIGHT,
	NSD_HEIGHT_MAP,
	// Fog
	NSD_FOG_MODE,
	NSD_FOG_COLOUR,
	NSD_FOG_DENSITY,
	NSD_FOG_START,
	NSD_FOG_END,
	// Sky
	NSD_SKY_TYPE,
	NSD_SKY_MATERIAL,
	_NSD
};

class NewSceneDlg : public CDialog
{
	DECLARE_DYNAMIC(NewSceneDlg)
public:
	NewSceneDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NewSceneDlg();

	enum { IDD = IDD_NEW_SCENE };

	_variant_t GetProperty(KNewSceneDlg Type)
	{
		return (_variant_t)mValues[Type];
	}

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam);

	BOOL OnInitDialog();
	void DoDataExchange(CDataExchange* pDX);

	CStatic mPropListFrame;
	CBCGPPropList mPropList;
	CBCGPProp *mProps[_NSD];
	COleVariant mValues[_NSD];
};
