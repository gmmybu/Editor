#include "stdafx.h"
#include "resource.h"
#include "NewLightDlg.h"
#include "OgreMath.h"

IMPLEMENT_DYNAMIC(NewLightDlg, CDialogEx)

NewLightDlg::NewLightDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(NewLightDlg::IDD, pParent)
{

}

NewLightDlg::~NewLightDlg()
{
}

BEGIN_MESSAGE_MAP(NewLightDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

void NewLightDlg::OnBnClickedOk()
{
	for(int i=0; i<_COUNT; i++)
		values[i] = props[i]->GetValue();
	CDialog::OnOK();
}

LRESULT NewLightDlg::OnPropertyChanged(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

BOOL NewLightDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rectPropList;
	propListFrame.GetClientRect(rectPropList);
	propListFrame.MapWindowPoints(this, &rectPropList);
	propList.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER, rectPropList, this, -1);
	propList.EnableHeaderCtrl(FALSE);
	propList.MarkModifiedProperties();
	propList.SetVSDotNetLook();

	CBCGPProp *prop = NULL;

	props[TYPE] = new CBCGPProp("Type", (_variant_t)"Point", "", TYPE);
	props[TYPE]->AllowEdit(FALSE);
	props[TYPE]->AddOption("Point");
	props[TYPE]->AddOption("Directional");
	props[TYPE]->AddOption("Spotlight");
	propList.AddProperty(props[TYPE]);

	props[DIFFUSE_COLOUR] = new CBCGPColorProp("Diffuse color", RGB(0xff, 0xff, 0xff), NULL, "", DIFFUSE_COLOUR);
	((CBCGPColorProp*)props[DIFFUSE_COLOUR])->EnableOtherButton("Other");
	propList.AddProperty(props[DIFFUSE_COLOUR]);

	props[SPECULAR_COLOUR] = new CBCGPColorProp("Specular color", RGB(0, 0, 0), NULL, "", SPECULAR_COLOUR);
	((CBCGPColorProp*)props[SPECULAR_COLOUR])->EnableOtherButton("Other");
	propList.AddProperty(props[SPECULAR_COLOUR]);

	prop = new CBCGPProp("Position");
	props[POSITION_X] = new CBCGPProp("X", (_variant_t)0.0f, "", POSITION_X);
	prop->AddSubItem(props[POSITION_X]);
	props[POSITION_Y] = new CBCGPProp("Y", (_variant_t)0.0f, "", POSITION_Y);
	prop->AddSubItem(props[POSITION_Y]);
	props[POSITION_Z] = new CBCGPProp("Z", (_variant_t)0.0f, "", POSITION_Z);
	prop->AddSubItem(props[POSITION_Z]);
	propList.AddProperty(prop);

	prop = new CBCGPProp("Direction");
	props[DIRECTION_X] = new CBCGPProp("X", (_variant_t)1.0f, "", DIRECTION_X);
	prop->AddSubItem(props[DIRECTION_X]);
	props[DIRECTION_Y] = new CBCGPProp("Y", (_variant_t)-1.0f, "", DIRECTION_Y);
	prop->AddSubItem(props[DIRECTION_Y]);
	props[DIRECTION_Z] = new CBCGPProp("X", (_variant_t)-1.0f, "", DIRECTION_Z);
	prop->AddSubItem(props[DIRECTION_Z]);
	propList.AddProperty(prop);

	prop = new CBCGPProp("Attenuation");
	props[ATTENUATION_RANGE] = new CBCGPProp("Range", (_variant_t)100000.0f, "", ATTENUATION_RANGE);
	prop->AddSubItem(props[ATTENUATION_RANGE]);
	props[ATTENUATION_CONSTANT] = new CBCGPProp("Constant", (_variant_t)1.0f, "", ATTENUATION_CONSTANT);
	prop->AddSubItem(props[ATTENUATION_CONSTANT]);
	props[ATTENUATION_LINEAR] = new CBCGPProp("Linear", (_variant_t)0.0f, "", ATTENUATION_LINEAR);
	prop->AddSubItem(props[ATTENUATION_LINEAR]);
	props[ATTENUATION_QUADRATIC] = new CBCGPProp("Quadratic", (_variant_t)0.0f, "", ATTENUATION_QUADRATIC);
	prop->AddSubItem(props[ATTENUATION_QUADRATIC]);
	propList.AddProperty(prop);

	prop = new CBCGPProp("Spotlight range");
	props[SPOTLIGHT_RANGE_INNER_ANGLE] = new CBCGPProp("Inner angle", (_variant_t)Ogre::Degree(30.0f).valueRadians(), "", SPOTLIGHT_RANGE_INNER_ANGLE);
	prop->AddSubItem(props[SPOTLIGHT_RANGE_INNER_ANGLE]); // »¡¶È
	props[SPOTLIGHT_RANGE_OUTER_ANGLE] = new CBCGPProp("Outer angle", (_variant_t)Ogre::Degree(40.0f).valueRadians(), "", SPOTLIGHT_RANGE_OUTER_ANGLE);
	prop->AddSubItem(props[SPOTLIGHT_RANGE_OUTER_ANGLE]);
	props[SPOTLIGHT_RANGE_FALLOFF] = new CBCGPProp("Falloff", (_variant_t)1.0f, "", SPOTLIGHT_RANGE_FALLOFF);
	prop->AddSubItem(props[SPOTLIGHT_RANGE_FALLOFF]);
	propList.AddProperty(prop);

	return TRUE;
}

void NewLightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROPLIST_FRAME, propListFrame);
}
