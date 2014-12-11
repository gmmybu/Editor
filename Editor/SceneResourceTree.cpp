#include "stdafx.h"
#include "Editor.h"
#include "SceneDoc.h"

#include "DynamicModel.h"
#include "PropertyWnd.h"
#include "SceneResourceTree.h"
#include "StaticMesh.h"
#include "Light.h"

#include "OgreSceneManager.h"

IMPLEMENT_DYNAMIC(CSceneResourceTree, CTreeCtrl)

CSceneResourceTree *CSceneResourceTree::Current = NULL;
CSceneResourceTree::CSceneResourceTree()
{
	Current = this;
}

CSceneResourceTree::~CSceneResourceTree()
{
}

void CSceneResourceTree::ResetImageList()
{
	CBitmap Bmp;
	if (!Bmp.LoadBitmap(IDB_SCENE_RESOURCE_TREE))
	{
		TRACE("无法加载位图: %x\n", IDB_SCENE_RESOURCE_TREE);
		return;
	}

	BITMAP BmpObj;
	Bmp.GetBitmap(&BmpObj);

	UINT nFlags = ILC_MASK | ILC_COLOR24;
	mImageList.Create(16, BmpObj.bmHeight, nFlags, 0, 0);
	mImageList.Add(&Bmp, RGB(255, 0, 255));

	SetImageList(&mImageList, TVSIL_NORMAL);
}

void CSceneResourceTree::RebuildTree()
{
	mTreeItems[SRTI_SCENE]			= InsertItem("场景",			SRTI_SCENE,			SRTI_SCENE);
	mTreeItems[SRTI_SKY]			= InsertItem("天空",			SRTI_SKY,			SRTI_SKY,			mTreeItems[SRTI_SCENE]);
	mTreeItems[SRTI_TERRAIN]		= InsertItem("地形",			SRTI_TERRAIN,		SRTI_TERRAIN,		mTreeItems[SRTI_SCENE]);
	mTreeItems[SRTI_LIGHT_SET]		= InsertItem("光源集",		SRTI_LIGHT_SET,		SRTI_LIGHT_SET,		mTreeItems[SRTI_SCENE]);
	mTreeItems[SRTI_MODEL_SET]		= InsertItem("动态实体集",	SRTI_MODEL_SET,		SRTI_MODEL_SET,		mTreeItems[SRTI_SCENE]);
	mTreeItems[SRTI_MESH_SET]		= InsertItem("静态实体集",	SRTI_MESH_SET,		SRTI_MESH_SET,		mTreeItems[SRTI_SCENE]);
	mTreeItems[SRTI_LIQUID_SET]		= InsertItem("流体集",		SRTI_LIQUID_SET,	SRTI_LIQUID_SET,	mTreeItems[SRTI_SCENE]);
	mTreeItems[SRTI_PARTICLE_SET]	= InsertItem("粒子集",		SRTI_PARTICLE_SET,	SRTI_PARTICLE_SET,	mTreeItems[SRTI_SCENE]);
}

void CSceneResourceTree::FirePropertyChanged()
{
	HTREEITEM SelectedItem = GetSelectedItem();

	KSceneResourceTreeImages nImage; int nSelectedImage;
	CSceneResourceTree::Current->GetItemImage(SelectedItem, (int&)nImage, nSelectedImage);

	CBCGPPropList *PropList = CPropertyWnd::Current->GetPropList(); CBCGPProp *Prop = NULL;
	switch(nImage)
	{
	case SRTI_LIGHT:
		{
			Light *Object = (Light*)CSceneResourceTree::Current->GetItemData(SelectedItem);

			//////////////////////////////////////////////////
			// Position
			//////////////////////////////////////////////////

			const Ogre::Vector3 &Position = Object->getSceneNode()->getPosition();
			
			Prop = PropList->FindItemByData(SRTI_LIGHT_POSITION_X);
			Prop->SetValue((_variant_t)Position.x);

			Prop = PropList->FindItemByData(SRTI_LIGHT_POSITION_Y);
			Prop->SetValue((_variant_t)Position.y);

			Prop = PropList->FindItemByData(SRTI_LIGHT_POSITION_Z);
			Prop->SetValue((_variant_t)Position.z);

			//////////////////////////////////////////////////
			// Direction
			//////////////////////////////////////////////////

			const Ogre::Vector3 &Direction = Object->getLight()->getDirection();
			
			Prop = PropList->FindItemByData(SRTI_LIGHT_DIRECTION_X);
			Prop->SetValue((_variant_t)Direction.x);

			Prop = PropList->FindItemByData(SRTI_LIGHT_DIRECTION_Y);
			Prop->SetValue((_variant_t)Direction.y);

			Prop = PropList->FindItemByData(SRTI_LIGHT_DIRECTION_Z);
			Prop->SetValue((_variant_t)Direction.z);
		}
		break;

	case SRTI_MODEL:
	case SRTI_MESH:
		{
			SceneObject *Object = (SceneObject*)CSceneResourceTree::Current->GetItemData(SelectedItem);

			//////////////////////////////////////////////////
			// Position
			//////////////////////////////////////////////////

			const Ogre::Vector3 &Position = Object->getSceneNode()->getPosition();
			
			Prop = PropList->FindItemByData(SRTI_MESH_POSITION_X);
			Prop->SetValue((_variant_t)Position.x);

			Prop = PropList->FindItemByData(SRTI_MESH_POSITION_Y);
			Prop->SetValue((_variant_t)Position.y);

			Prop = PropList->FindItemByData(SRTI_MESH_POSITION_Z);
			Prop->SetValue((_variant_t)Position.z);

			//////////////////////////////////////////////////
			// Scale
			//////////////////////////////////////////////////

			const Ogre::Vector3 &Scale = Object->getSceneNode()->getScale();
			
			Prop = PropList->FindItemByData(SRTI_MESH_SCALE_X);
			Prop->SetValue((_variant_t)Scale.x);

			Prop = PropList->FindItemByData(SRTI_MESH_SCALE_Y);
			Prop->SetValue((_variant_t)Scale.y);

			Prop = PropList->FindItemByData(SRTI_MESH_SCALE_Z);
			Prop->SetValue((_variant_t)Scale.z);

			//////////////////////////////////////////////////
			// Direction
			//////////////////////////////////////////////////

			const Ogre::Quaternion &Direction = Object->getSceneNode()->getOrientation();
			
			Prop = PropList->FindItemByData(SRTI_MESH_DIRECTION_X);
			Prop->SetValue((_variant_t)Direction.x);

			Prop = PropList->FindItemByData(SRTI_MESH_DIRECTION_Y);
			Prop->SetValue((_variant_t)Direction.y);

			Prop = PropList->FindItemByData(SRTI_MESH_DIRECTION_Z);
			Prop->SetValue((_variant_t)Direction.z);
		}
		break;

	case SRTI_LIQUID:
		{
			SceneObject *Object = (SceneObject*)CSceneResourceTree::Current->GetItemData(SelectedItem);

			//////////////////////////////////////////////////
			// Position
			//////////////////////////////////////////////////

			const Ogre::Vector3 &Position = Object->getSceneNode()->getPosition();
			
			Prop = PropList->FindItemByData(SRTI_LIQUID_POSITION_X);
			Prop->SetValue((_variant_t)Position.x);

			Prop = PropList->FindItemByData(SRTI_LIQUID_POSITION_Y);
			Prop->SetValue((_variant_t)Position.y);

			Prop = PropList->FindItemByData(SRTI_LIQUID_POSITION_Z);
			Prop->SetValue((_variant_t)Position.z);
		}
	}
}

void CSceneResourceTree::OnPropertyChanged(CBCGPProp *Prop)
{
	if(!SceneDoc::current)
		return;
	CBCGPPropList *PropList = CPropertyWnd::Current->GetPropList();
	switch(Prop->GetData())
	{
	case SRTI_SCENE_NAME:
		{
			CString SceneName = Prop->GetValue();
			SceneDoc::current->setSceneName(SceneName);
		}
		break;

	case SRTI_SCENE_AMBIENT_LIGHT:
		{
			COLORREF AmbientLight = Prop->GetValue();
			SceneDoc::current->getSceneManager()->setAmbientLight(
				Ogre::ColourValue(GetRValue(AmbientLight)/255.0f, GetGValue(AmbientLight)/255.0f, GetBValue(AmbientLight)/255.0f));
		}
		break;

	case SRTI_SCENE_SHADOW_TECHNIQUE:
		{
			CString ShadowTechnique = Prop->GetValue();
			SceneDoc::current->configureShadows(ShadowTechnique != "None", ShadowTechnique == "Depth Shadows");
		}
		break;

	case SRTI_SCENE_FOG_MODE:
	case SRTI_SCENE_FOG_COLOR:
	case SRTI_SCENE_FOG_DENSITY:
	case SRTI_SCENE_FOG_START:
	case SRTI_SCENE_FOG_END:
		{
			COLORREF FogColor =PropList->FindItemByData(SRTI_SCENE_FOG_COLOR)->GetValue();
			float FogDensity = PropList->FindItemByData(SRTI_SCENE_FOG_DENSITY)->GetValue();
			float FogStart = PropList->FindItemByData(SRTI_SCENE_FOG_START)->GetValue();
			float FogEnd = PropList->FindItemByData(SRTI_SCENE_FOG_END)->GetValue();

			CString FogMode = PropList->FindItemByData(SRTI_SCENE_FOG_MODE)->GetValue();
			if(FogMode == "None")
			{
				SceneDoc::current->getSceneManager()->setFog(
					Ogre::FOG_NONE, Ogre::ColourValue(GetRValue(FogColor)/255.0f, GetGValue(FogColor)/255.0f, GetBValue(FogColor)/255.0f), FogDensity, FogStart, FogEnd);
			} else 
			if(FogMode == "Linear")
			{
				SceneDoc::current->getSceneManager()->setFog(
					Ogre::FOG_LINEAR, Ogre::ColourValue(GetRValue(FogColor)/255.0f, GetGValue(FogColor)/255.0f, GetBValue(FogColor)/255.0f), FogDensity, FogStart, FogEnd);
			} else 
			if(FogMode == "Exp")
			{
				SceneDoc::current->getSceneManager()->setFog(
					Ogre::FOG_EXP, Ogre::ColourValue(GetRValue(FogColor)/255.0f, GetGValue(FogColor)/255.0f, GetBValue(FogColor)/255.0f), FogDensity, FogStart, FogEnd);
			} else 
			if(FogMode == "Exp2")
			{
				SceneDoc::current->getSceneManager()->setFog(
					Ogre::FOG_EXP2, Ogre::ColourValue(GetRValue(FogColor)/255.0f, GetGValue(FogColor)/255.0f, GetBValue(FogColor)/255.0f), FogDensity, FogStart, FogEnd);
			}
		}
		break;

	case SRTI_LIGHT_TYPE:
	case SRTI_LIGHT_DIFFUSE_COLOUR:
	case SRTI_LIGHT_SPECULAR_COLOUR:
	case SRTI_LIGHT_POSITION_X:
	case SRTI_LIGHT_POSITION_Y:
	case SRTI_LIGHT_POSITION_Z:
	case SRTI_LIGHT_DIRECTION_X:
	case SRTI_LIGHT_DIRECTION_Y:
	case SRTI_LIGHT_DIRECTION_Z:
	case SRTI_LIGHT_ATTENUATION_RANGE:
	case SRTI_LIGHT_ATTENUATION_CONSTANT:
	case SRTI_LIGHT_ATTENUATION_LINEAR:
	case SRTI_LIGHT_ATTENUATION_QUADRATIC:
	case SRTI_LIGHT_SPOTLIGHT_RANGE_INNER_ANGLE:
	case SRTI_LIGHT_SPOTLIGHT_RANGE_OUTER_ANGLE:
	case SRTI_LIGHT_SPOTLIGHT_RANGE_FALLOFF:
		{
			Light *light = (Light*)CSceneResourceTree::Current->GetItemData(GetSelectedItem());
			
			CString Type = PropList->FindItemByData(SRTI_LIGHT_TYPE)->GetValue();
			if(Type == "Point")
			{
				light->getLight()->setType(Ogre::Light::LT_POINT);
			} else
			if(Type == "Directional")
			{
				light->getLight()->setType(Ogre::Light::LT_DIRECTIONAL);
			} else
			if(Type == "Spotlight")
			{
				light->getLight()->setType(Ogre::Light::LT_SPOTLIGHT);
			}

			COLORREF DiffuseColour = PropList->FindItemByData(SRTI_LIGHT_DIFFUSE_COLOUR)->GetValue();
			light->getLight()->setDiffuseColour(
				Ogre::ColourValue(GetRValue(DiffuseColour)/255.0f, GetGValue(DiffuseColour)/255.0f, GetBValue(DiffuseColour)/255.0f));

			COLORREF SpecularColour = PropList->FindItemByData(SRTI_LIGHT_SPECULAR_COLOUR)->GetValue();
			light->getLight()->setSpecularColour(
				Ogre::ColourValue(GetRValue(SpecularColour)/255.0f, GetGValue(SpecularColour)/255.0f, GetBValue(SpecularColour)/255.0f));

			float PositionX = PropList->FindItemByData(SRTI_LIGHT_POSITION_X)->GetValue();
			float PositionY = PropList->FindItemByData(SRTI_LIGHT_POSITION_Y)->GetValue();
			float PositionZ = PropList->FindItemByData(SRTI_LIGHT_POSITION_Z)->GetValue();
			light->getSceneNode()->setPosition(Ogre::Vector3(PositionX, PositionY, PositionZ));

			float DirectionX = PropList->FindItemByData(SRTI_LIGHT_DIRECTION_X)->GetValue();
			float DirectionY = PropList->FindItemByData(SRTI_LIGHT_DIRECTION_Y)->GetValue();
			float DirectionZ = PropList->FindItemByData(SRTI_LIGHT_DIRECTION_Z)->GetValue();
			light->getLight()->setDirection(Ogre::Vector3(DirectionX, DirectionY, DirectionZ));

			float AttenuationRange = PropList->FindItemByData(SRTI_LIGHT_ATTENUATION_RANGE)->GetValue();
			float AttenuationConstant = PropList->FindItemByData(SRTI_LIGHT_ATTENUATION_CONSTANT)->GetValue();
			float AttenuationLinear = PropList->FindItemByData(SRTI_LIGHT_ATTENUATION_LINEAR)->GetValue();
			float AttenuationQuadratic = PropList->FindItemByData(SRTI_LIGHT_ATTENUATION_QUADRATIC)->GetValue();
			light->getLight()->setAttenuation(AttenuationRange, AttenuationConstant, AttenuationLinear, AttenuationQuadratic);

			float SpotlightInnerAngle = PropList->FindItemByData(SRTI_LIGHT_SPOTLIGHT_RANGE_INNER_ANGLE)->GetValue();
			float SpotlightOuterAngle = PropList->FindItemByData(SRTI_LIGHT_SPOTLIGHT_RANGE_OUTER_ANGLE)->GetValue();
			float SpotlightFalloff = PropList->FindItemByData(SRTI_LIGHT_SPOTLIGHT_RANGE_FALLOFF)->GetValue();
			light->getLight()->setSpotlightRange(Ogre::Radian(SpotlightInnerAngle),  Ogre::Radian(SpotlightOuterAngle), SpotlightFalloff);
		}
	}
}

BEGIN_MESSAGE_MAP(CSceneResourceTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
END_MESSAGE_MAP()

void CSceneResourceTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	if(!SceneDoc::current)
		return;

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	SceneObject *Object = NULL;
	if(hItem != NULL)
	{
		Object = (SceneObject*)GetItemData(hItem);

		SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
		AfterSelectTreeItem(hItem);
	}
	SceneDoc::current->selectObject(Object);
}

void CSceneResourceTree::AfterSelectTreeItem(HTREEITEM hItem)
{
	CBCGPPropList *PropList = CPropertyWnd::Current->GetPropList();
	PropList->RemoveAll();

	if(!SceneDoc::current || !SceneDoc::current->isInitialized())
		return;

	CPropertyWnd::Current->SetListener(this);

	KSceneResourceTreeImages nImage; int nSelectedImage;
	GetItemImage(hItem, (int&)nImage, nSelectedImage);

	CBCGPProp *Prop = NULL, *SubProp = NULL;
	switch(nImage)
	{
	case SRTI_SCENE:
		{
			//////////////////////////////////////////////////
			// Scene name
			//////////////////////////////////////////////////

			CString SceneName = SceneDoc::current->getSceneName();
			Prop = new CBCGPProp("Scene name", (_variant_t)SceneName, "", SRTI_SCENE_NAME);
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Ambient light
			//////////////////////////////////////////////////

			Ogre::ColourValue AmbientLight = 
				SceneDoc::current->getSceneManager()->getAmbientLight();
			Prop = new CBCGPColorProp("Ambient Light", RGB(AmbientLight.r*255, AmbientLight.g*255, AmbientLight.b*255), NULL, NULL, SRTI_SCENE_AMBIENT_LIGHT);
			((CBCGPColorProp*)Prop)->EnableOtherButton("Other");
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Shadow technique
			//////////////////////////////////////////////////

			Prop = new CBCGPProp("Shadow technique", (_variant_t)"None", "", SRTI_SCENE_SHADOW_TECHNIQUE);
			Prop->AllowEdit(FALSE);
			Prop->AddOption("None");
			Prop->AddOption("Colour Shadows");
			Prop->AddOption("Depth Shadows");
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Fog
			//////////////////////////////////////////////////

			Prop = new CBCGPProp("Fog");

			Ogre::FogMode FogMode = SceneDoc::current->getSceneManager()->getFogMode();
			switch(FogMode)
			{
			case Ogre::FOG_NONE:
				SubProp = new CBCGPProp("Mode", (_variant_t)"None", NULL, SRTI_SCENE_FOG_MODE);
				break;

			case Ogre::FOG_LINEAR:
				SubProp = new CBCGPProp("Mode", (_variant_t)"Linear", NULL, SRTI_SCENE_FOG_MODE);
				break;

			case Ogre::FOG_EXP:
				SubProp = new CBCGPProp("Mode", (_variant_t)"Exp", NULL, SRTI_SCENE_FOG_MODE);
				break;

			case Ogre::FOG_EXP2:
				SubProp = new CBCGPProp("Mode", (_variant_t)"Exp2", NULL, SRTI_SCENE_FOG_MODE);
			}
			SubProp->AllowEdit(FALSE);
			SubProp->AddOption("None");
			SubProp->AddOption("Linear");
			SubProp->AddOption("Exp");
			SubProp->AddOption("Exp2");
			Prop->AddSubItem(SubProp);

			Ogre::ColourValue FogColor = 
				SceneDoc::current->getSceneManager()->getFogColour();
			SubProp = new CBCGPColorProp("Color", RGB(FogColor.r*255, FogColor.g*255, FogColor.b*255), NULL, NULL, SRTI_SCENE_FOG_COLOR);
			((CBCGPColorProp*)SubProp)->EnableOtherButton("Other");
			Prop->AddSubItem(SubProp);

			float Density = SceneDoc::current->getSceneManager()->getFogDensity();
			SubProp = new CBCGPProp("Density", Density, NULL, SRTI_SCENE_FOG_DENSITY);
			Prop->AddSubItem(SubProp);

			float Start = SceneDoc::current->getSceneManager()->getFogStart();
			SubProp = new CBCGPProp("Start", Start, NULL, SRTI_SCENE_FOG_START);
			Prop->AddSubItem(SubProp);

			float End = SceneDoc::current->getSceneManager()->getFogEnd();
			SubProp =  new CBCGPProp("End", End, NULL, SRTI_SCENE_FOG_END);
			Prop->AddSubItem(SubProp);

			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Fog
			//////////////////////////////////////////////////

			Prop = new CBCGPProp("Sky");

			SubProp = new CBCGPProp("Type", (_variant_t)SceneDoc::current->getSkyType(), "", SRTI_SCENE_SKY_TYPE);
			SubProp->AllowEdit(FALSE);
			SubProp->AddOption("None");
			SubProp->AddOption("Box");
			SubProp->AddOption("Dome");
			SubProp->AddOption("Plane");
			Prop->AddSubItem(SubProp);

			PropList->AddProperty(Prop);
			// SubProp = new CBCGPProp("Material", (_variant_t)

		}
		break;

	case SRTI_SKY:
		{
		}
		break;

	case SRTI_TERRAIN:
		{
		}
		break;

	case SRTI_LIGHT:
		{
			Light *Object = (Light*)GetItemData(hItem);
			Ogre::Light *light = ((Light*)Object)->getLight();

			//////////////////////////////////////////////////
			// Name
			//////////////////////////////////////////////////

			const char *Name = Object->getSceneNode()->getName().c_str();
			Prop = new CBCGPProp("Name", (_variant_t)Name, "");
			Prop->AllowEdit(FALSE);
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Type
			//////////////////////////////////////////////////

			Ogre::Light::LightTypes Type = light->getType();
			switch(Type)
			{
			case Ogre::Light::LT_POINT:
				{
					Prop = new CBCGPProp("Type", (_variant_t)"Point", "", SRTI_LIGHT_TYPE);
				}
				break;

			case Ogre::Light::LT_DIRECTIONAL:
				{
					Prop = new CBCGPProp("Type", (_variant_t)"Directional", "", SRTI_LIGHT_TYPE);
				}
				break;

			case Ogre::Light::LT_SPOTLIGHT:
				{
					Prop = new CBCGPProp("Type", (_variant_t)"Spotlight", "", SRTI_LIGHT_TYPE);
				}
			}
			Prop->AddOption("Point");
			Prop->AddOption("Directional");
			Prop->AddOption("Spotlight");
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Diffuse colour
			//////////////////////////////////////////////////

			Ogre::ColourValue DiffuseColour = light->getDiffuseColour();
			Prop = new CBCGPColorProp("Diffuse colour", RGB(DiffuseColour.r*255, DiffuseColour.g*255, DiffuseColour.b*255), NULL, "", SRTI_LIGHT_DIFFUSE_COLOUR);
			((CBCGPColorProp*)Prop)->EnableOtherButton("Other");
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Specular colour
			//////////////////////////////////////////////////

			Ogre::ColourValue SpecularColour = light->getSpecularColour();
			Prop = new CBCGPColorProp("Specular colour", RGB(SpecularColour.r*255, SpecularColour.g*255, SpecularColour.b*255), NULL, "", SRTI_LIGHT_SPECULAR_COLOUR);
			((CBCGPColorProp*)Prop)->EnableOtherButton("Other");
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Position
			//////////////////////////////////////////////////

			Prop = new CBCGPProp("Position");
			Ogre::Vector3 Position = Object->getSceneNode()->getPosition();
			SubProp = new CBCGPProp("X", (_variant_t)Position.x, "", SRTI_LIGHT_POSITION_X);
			Prop->AddSubItem(SubProp);
			SubProp = new CBCGPProp("Y", (_variant_t)Position.y, "", SRTI_LIGHT_POSITION_Y);
			Prop->AddSubItem(SubProp);
			SubProp = new CBCGPProp("Z", (_variant_t)Position.z, "", SRTI_LIGHT_POSITION_Z);
			Prop->AddSubItem(SubProp);
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Direction
			//////////////////////////////////////////////////

			Prop = new CBCGPProp("Direction");
			Ogre::Vector3 Direction = Object->getLight()->getDirection();
			SubProp = new CBCGPProp("X", (_variant_t)Direction.x, "", SRTI_LIGHT_DIRECTION_X);
			Prop->AddSubItem(SubProp);
			SubProp = new CBCGPProp("Y", (_variant_t)Direction.y, "", SRTI_LIGHT_DIRECTION_Y);
			Prop->AddSubItem(SubProp);
			SubProp = new CBCGPProp("Z", (_variant_t)Direction.z, "", SRTI_LIGHT_DIRECTION_Z);
			Prop->AddSubItem(SubProp);
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Attenuation
			//////////////////////////////////////////////////

			Prop = new CBCGPProp("Attenuation");
			SubProp = new CBCGPProp("Range", (_variant_t)light->getAttenuationRange(), "", SRTI_LIGHT_ATTENUATION_RANGE);
			Prop->AddSubItem(SubProp);
			SubProp = new CBCGPProp("Constant", (_variant_t)light->getAttenuationConstant(), "", SRTI_LIGHT_ATTENUATION_CONSTANT);
			Prop->AddSubItem(SubProp);
			SubProp = new CBCGPProp("Linear", (_variant_t)light->getAttenuationLinear(), "", SRTI_LIGHT_ATTENUATION_LINEAR);
			Prop->AddSubItem(SubProp);
			SubProp = new CBCGPProp("Quadratic", (_variant_t)light->getAttenuationQuadric(), "", SRTI_LIGHT_ATTENUATION_QUADRATIC);
			Prop->AddSubItem(SubProp);
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Spotlight range
			//////////////////////////////////////////////////

			Prop = new CBCGPProp("Spotlight range");
			SubProp = new CBCGPProp("Inner angle", (_variant_t)light->getSpotlightInnerAngle().valueRadians(), "", SRTI_LIGHT_SPOTLIGHT_RANGE_INNER_ANGLE);
			Prop->AddSubItem(SubProp); // 弧度
			SubProp = new CBCGPProp("Outer angle", (_variant_t)light->getSpotlightOuterAngle().valueRadians(), "", SRTI_LIGHT_SPOTLIGHT_RANGE_OUTER_ANGLE);
			Prop->AddSubItem(SubProp);
			SubProp = new CBCGPProp("Falloff", (_variant_t)light->getSpotlightFalloff(), "", SRTI_LIGHT_SPOTLIGHT_RANGE_FALLOFF);
			Prop->AddSubItem(SubProp);
			PropList->AddProperty(Prop);
		}
		break;

	case SRTI_MODEL:
	case SRTI_MESH:
		{
			SceneObject *Object = (SceneObject*)GetItemData(hItem);

			CString Name = Object->getSceneNode()->getName().c_str();
			Prop = new CBCGPProp("Name", (_variant_t)Name);
			Prop->AllowEdit(FALSE);
			PropList->AddProperty(Prop);

			Prop = new CBCGPProp("Shadow", (_variant_t)"FALSE", "", SRTI_MESH_SHADOW);
			Prop->AllowEdit(FALSE);
			Prop->AddOption("TRUE");
			Prop->AddOption("FALSE");
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Position
			//////////////////////////////////////////////////

			Prop = new CBCGPProp("Position");

			Ogre::Vector3 Position = Object->getSceneNode()->getPosition();
			
			SubProp = new CBCGPProp("X", (_variant_t)Position.x, "", SRTI_MESH_POSITION_X);
			Prop->AddSubItem(SubProp);

			SubProp = new CBCGPProp("Y", (_variant_t)Position.y, "", SRTI_MESH_POSITION_Y);
			Prop->AddSubItem(SubProp);

			SubProp = new CBCGPProp("Z", (_variant_t)Position.z, "", SRTI_MESH_POSITION_Z);
			Prop->AddSubItem(SubProp);

			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Scale
			//////////////////////////////////////////////////

			Prop = new CBCGPProp("Scale");

			Ogre::Vector3 Scale = Object->getSceneNode()->getScale();
			
			SubProp = new CBCGPProp("X", (_variant_t)Scale.x, "", SRTI_MESH_SCALE_X);
			Prop->AddSubItem(SubProp);

			SubProp = new CBCGPProp("Y", (_variant_t)Scale.y, "", SRTI_MESH_SCALE_Y);
			Prop->AddSubItem(SubProp);

			SubProp = new CBCGPProp("Z", (_variant_t)Scale.z, "", SRTI_MESH_SCALE_Z);
			Prop->AddSubItem(SubProp);

			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Direction
			//////////////////////////////////////////////////

			Prop = new CBCGPProp("Direction");

			Ogre::Quaternion Direction = Object->getSceneNode()->getOrientation();
			
			SubProp = new CBCGPProp("X", (_variant_t)Direction.x, "", SRTI_MESH_DIRECTION_X);
			Prop->AddSubItem(SubProp);

			SubProp = new CBCGPProp("Y", (_variant_t)Direction.y, "", SRTI_MESH_DIRECTION_Y);
			Prop->AddSubItem(SubProp);

			SubProp = new CBCGPProp("Z", (_variant_t)Direction.z, "", SRTI_MESH_DIRECTION_Z);
			Prop->AddSubItem(SubProp);

			PropList->AddProperty(Prop);
		}
		break;

	case SRTI_LIQUID:
		{
			SceneObject *Object = (SceneObject*)GetItemData(hItem);

			CString Name = Object->getSceneNode()->getName().c_str();
			Prop = new CBCGPProp("Name", (_variant_t)Name);
			Prop->AllowEdit(FALSE);
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Position
			//////////////////////////////////////////////////

			Prop = new CBCGPProp("Position");

			Ogre::Vector3 Position = Object->getSceneNode()->getPosition();
			
			SubProp = new CBCGPProp("X", (_variant_t)Position.x, "", SRTI_LIQUID_POSITION_X);
			Prop->AddSubItem(SubProp);

			SubProp = new CBCGPProp("Y", (_variant_t)Position.y, "", SRTI_LIQUID_POSITION_Y);
			Prop->AddSubItem(SubProp);

			SubProp = new CBCGPProp("Z", (_variant_t)Position.z, "", SRTI_LIQUID_POSITION_Z);
			Prop->AddSubItem(SubProp);

			PropList->AddProperty(Prop);
		}
		break;

	case SRTI_PARTICLE:
		{
		}
	}
}
