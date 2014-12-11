#include "stdafx.h"
#include "NewSceneDlg.h"
#include "TextureProp.h"

IMPLEMENT_DYNAMIC(CNewSceneDlg, CDialog)

CNewSceneDlg::CNewSceneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewSceneDlg::IDD, pParent)
{
}

CNewSceneDlg::~CNewSceneDlg()
{
}

BEGIN_MESSAGE_MAP(CNewSceneDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CNewSceneDlg::OnBnClickedOk)
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

void CNewSceneDlg::OnBnClickedOk()
{
	for(int i=0; i<_NSD; i++)
		mValues[i] = mProps[i]->GetValue();
	CDialog::OnOK();
}

LRESULT CNewSceneDlg::OnPropertyChanged(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

BOOL CNewSceneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rectPropList;
	mPropListFrame.GetClientRect(rectPropList);
	mPropListFrame.MapWindowPoints(this, &rectPropList);
	mPropList.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER, rectPropList, this, -1);
	mPropList.EnableHeaderCtrl(FALSE);
	mPropList.MarkModifiedProperties();
	mPropList.SetVSDotNetLook();

	CBCGPProp *Prop = NULL, *SubProp = NULL;

	mProps[NSD_SCENE_NAME] = new CBCGPProp("Scene name", (_variant_t)"NewScene", "", NSD_SCENE_NAME);
	mPropList.AddProperty(mProps[NSD_SCENE_NAME]);

	mProps[NSD_AMBIENT_LIGHT] = new CBCGPColorProp("Ambient light", RGB(0xff, 0xff, 0xff), NULL, "", NSD_AMBIENT_LIGHT);
	((CBCGPColorProp*)mProps[NSD_AMBIENT_LIGHT])->EnableOtherButton("Other");
	mPropList.AddProperty(mProps[NSD_AMBIENT_LIGHT]);

	mProps[NSD_SHADOW_TECHNIQUE] = new CBCGPProp("Shadow technique", (_variant_t)"None", "", NSD_SHADOW_TECHNIQUE);
	mProps[NSD_SHADOW_TECHNIQUE]->AllowEdit(FALSE);
	mProps[NSD_SHADOW_TECHNIQUE]->AddOption("None");
	mProps[NSD_SHADOW_TECHNIQUE]->AddOption("Colour Shadows");
	mProps[NSD_SHADOW_TECHNIQUE]->AddOption("Depth Shadows");
	mPropList.AddProperty(mProps[NSD_SHADOW_TECHNIQUE]);

	//////////////////////////////////////////////////
	// Terrain global options
	//////////////////////////////////////////////////

	Prop = new CBCGPProp("Terrain global options");

	SubProp = new CBCGPProp("Light map direction");
	mProps[NSD_LIGHT_MAP_DIRECTION_X] = new CBCGPProp("X", (_variant_t)1.0f, "", NSD_LIGHT_MAP_DIRECTION_X);
	SubProp->AddSubItem(mProps[NSD_LIGHT_MAP_DIRECTION_X]);
	mProps[NSD_LIGHT_MAP_DIRECTION_Y] = new CBCGPProp("Y", (_variant_t)-1.0f, "", NSD_LIGHT_MAP_DIRECTION_Y);
	SubProp->AddSubItem(mProps[NSD_LIGHT_MAP_DIRECTION_Y]);
	mProps[NSD_LIGHT_MAP_DIRECTION_Z] = new CBCGPProp("Z", (_variant_t)0.0f, "", NSD_LIGHT_MAP_DIRECTION_Z);
	SubProp->AddSubItem(mProps[NSD_LIGHT_MAP_DIRECTION_Z]);
	Prop->AddSubItem(SubProp);

	mProps[NSD_LIGHT_MAP_SIZE] = new CBCGPProp("Light map size", (_variant_t)1024, "", NSD_LIGHT_MAP_SIZE);
	Prop->AddSubItem(mProps[NSD_LIGHT_MAP_SIZE]);

	mProps[NSD_COMPOSITE_MAP_DIFFUSE] = new CBCGPColorProp("Composite map diffuse", RGB(0xff, 0xff, 0xff), NULL, "", NSD_COMPOSITE_MAP_DIFFUSE);
	((CBCGPColorProp*)mProps[NSD_COMPOSITE_MAP_DIFFUSE])->EnableOtherButton("Other");
	Prop->AddSubItem(mProps[NSD_COMPOSITE_MAP_DIFFUSE]);

	mProps[NSD_COMPOSITE_MAP_DISTANCE] = new CBCGPProp("Composite map distance", (_variant_t)12800.0f, "", NSD_COMPOSITE_MAP_DISTANCE);
	Prop->AddSubItem(mProps[NSD_COMPOSITE_MAP_DISTANCE]);

	mProps[NSD_COMPOSITE_MAP_SIZE] = new CBCGPProp("Composite map size", (_variant_t)1024, "", NSD_COMPOSITE_MAP_SIZE);
	Prop->AddSubItem(mProps[NSD_COMPOSITE_MAP_SIZE]);

	mPropList.AddProperty(Prop);

	//////////////////////////////////////////////////
	// Default import settings
	//////////////////////////////////////////////////

	Prop = new CBCGPProp("Default import settings");

	SubProp = new CBCGPProp("Filename conversion");
	mProps[NSD_FILENAME_PREFIX] = new CBCGPProp("Filename prefix", (_variant_t)"../../TestMedia/Scene/NewScene", "", NSD_FILENAME_PREFIX);
	SubProp->AddSubItem(mProps[NSD_FILENAME_PREFIX]);
	mProps[NSD_FILENAME_EXTENSION] = new CBCGPProp("Filename extension", (_variant_t)"dat", "", NSD_FILENAME_EXTENSION);
	SubProp->AddSubItem(mProps[NSD_FILENAME_EXTENSION]);
	Prop->AddSubItem(SubProp);

	mProps[NSD_TERRAIN_SIZE] = new CBCGPProp("Terrain size", (_variant_t)129, "", NSD_TERRAIN_SIZE);
	Prop->AddSubItem(mProps[NSD_TERRAIN_SIZE]);
	
	mProps[NSD_WORLD_SIZE] = new CBCGPProp("World size", (_variant_t)512.0f, "", NSD_WORLD_SIZE);
	Prop->AddSubItem(mProps[NSD_WORLD_SIZE]);
	
	mProps[NSD_INPUT_SCALE] = new CBCGPProp("Input scale", (_variant_t)600, "", NSD_INPUT_SCALE);
	Prop->AddSubItem(mProps[NSD_INPUT_SCALE]);

	mProps[NSD_MIN_BATCH_SIZE] = new CBCGPProp("Min batch size", (_variant_t)33, "", NSD_MIN_BATCH_SIZE);
	Prop->AddSubItem(mProps[NSD_MIN_BATCH_SIZE]);
	
	mProps[NSD_MAX_BATCH_SIZE] = new CBCGPProp("Max batch size", (_variant_t)65, "", NSD_MAX_BATCH_SIZE);
	Prop->AddSubItem(mProps[NSD_MAX_BATCH_SIZE]);

	mProps[NSD_TEXTURE_WORLD_SIZE] = new CBCGPProp("Texture world size", (_variant_t)128.0f, "", NSD_TEXTURE_WORLD_SIZE);
	Prop->AddSubItem(mProps[NSD_TEXTURE_WORLD_SIZE]);

	mProps[NSD_DIFFUSE_SPECULAR] = new TextureProp("Diffuse specular texture", "dirt_grayrocky_diffusespecular.dds", "");
	Prop->AddSubItem(mProps[NSD_DIFFUSE_SPECULAR]);

	mProps[NSD_NORMAL_HEIGHT] = new TextureProp("Normal height texture", "dirt_grayrocky_normalheight.dds", "");
	Prop->AddSubItem(mProps[NSD_NORMAL_HEIGHT]);

	mProps[NSD_HEIGHT_MAP] = new TextureProp("Height map", "", "");
	Prop->AddSubItem(mProps[NSD_HEIGHT_MAP]);

	mPropList.AddProperty(Prop);

	//////////////////////////////////////////////////
	// Fog
	//////////////////////////////////////////////////

	Prop = new CBCGPProp("Fog");

	mProps[NSD_FOG_MODE] = new CBCGPProp("Mode", "None", "", NSD_FOG_MODE);
	mProps[NSD_FOG_MODE]->AllowEdit(FALSE);
	mProps[NSD_FOG_MODE]->AddOption("None");
	mProps[NSD_FOG_MODE]->AddOption("Linear");
	mProps[NSD_FOG_MODE]->AddOption("Exp");
	mProps[NSD_FOG_MODE]->AddOption("Exp2");
	Prop->AddSubItem(mProps[NSD_FOG_MODE]);

	mProps[NSD_FOG_COLOUR] = new CBCGPColorProp("Color", RGB(0xff, 0xff, 0xff), NULL, "", NSD_FOG_COLOUR);
	((CBCGPColorProp*)mProps[NSD_FOG_COLOUR])->EnableOtherButton("Other");
	Prop->AddSubItem(mProps[NSD_FOG_COLOUR]);

	mProps[NSD_FOG_DENSITY] = new CBCGPProp("Density", (_variant_t)0.0f, "", NSD_FOG_DENSITY);
	Prop->AddSubItem(mProps[NSD_FOG_DENSITY]);

	mProps[NSD_FOG_START] = new CBCGPProp("Start", (_variant_t)0.0f, "", NSD_FOG_START);
	Prop->AddSubItem(mProps[NSD_FOG_START]);

	mProps[NSD_FOG_END] = new CBCGPProp("End", (_variant_t)10000.0f, "", NSD_FOG_END);
	Prop->AddSubItem(mProps[NSD_FOG_END]);

	mPropList.AddProperty(Prop);

	//////////////////////////////////////////////////
	// Sky
	//////////////////////////////////////////////////

	Prop = new CBCGPProp("Sky");

	mProps[NSD_SKY_TYPE] = new CBCGPProp("Type", (_variant_t)"Box", "", NSD_SKY_TYPE);
	mProps[NSD_SKY_TYPE]->AllowEdit(FALSE);
	mProps[NSD_SKY_TYPE]->AddOption("None");
	mProps[NSD_SKY_TYPE]->AddOption("Box");
	mProps[NSD_SKY_TYPE]->AddOption("Dome");
	mProps[NSD_SKY_TYPE]->AddOption("Plane");
	Prop->AddSubItem(mProps[NSD_SKY_TYPE]);

	mProps[NSD_SKY_MATERIAL] = new CBCGPProp("Material", (_variant_t)"CloudyNoonSkyBox", "", NSD_SKY_MATERIAL);
	Prop->AddSubItem(mProps[NSD_SKY_MATERIAL]);

	mPropList.AddProperty(Prop);

	return TRUE;
}

void CNewSceneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROPLIST_FRAME, mPropListFrame);
}
