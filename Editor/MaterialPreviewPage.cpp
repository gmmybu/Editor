#include "stdafx.h"
#include "Editor.h"
#include "MaterialPreviewPage.h"

#include "RenderView.h"
#include "SceneDoc.h"
#include "StaticMesh.h"
#include "StringUtils.h"

#include "OgreCamera.h"
#include "OgreCompositorManager.h"
#include "OgreEntity.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreLight.h"
#include "OgreManualObject.h"
#include "OgreResourceManager.h"
#include "OgreRoot.h"
#include "OgreSceneManager.h"

#include "tinyxml.h"

IMPLEMENT_DYNAMIC(CMaterialPreviewPage, CBCGPDialog)

CMaterialPreviewPage *CMaterialPreviewPage::Current = NULL;
CMaterialPreviewPage::CMaterialPreviewPage(CWnd* pParent /*=NULL*/): 
	CBCGPDialog(CMaterialPreviewPage::IDD, pParent)
{
	Current = this;
	mSceneManager = NULL;
	mCamera = NULL;
	mView = NULL;
	mObject = NULL;
}

CMaterialPreviewPage::~CMaterialPreviewPage()
{
}

void CMaterialPreviewPage::AfterSelectResource(TiXmlElement *Elmt)
{
	ClearObject();
	KResourceManagerTreeImages Type;
	Elmt->QueryIntAttribute("Type", (int*)&Type);
	switch(Type)
	{
	case RMTI_MESH:
		{
			mObject = new StaticMesh(mSceneManager);
			mObject->create(Elmt->Attribute("Path"), Elmt->Attribute("Path"));

			mAnimList.AddString("[Reset]");
			Ogre::AnimationStateSet *States = mObject->getEntity()->getAllAnimationStates();
			if(States != NULL)
			{
				Ogre::AnimationStateIterator StateIter = States->getAnimationStateIterator();
				while(StateIter.hasMoreElements())
				{
					Ogre::AnimationState *State = StateIter.getNext();
					State->setLoop(true);
					mAnimList.AddString(State->getAnimationName().c_str());
				}
			}

			mAnimList.SetCurSel(0);

			mCameraManager.setTarget(mObject);
			mCameraManager.faceTo(mObject);
		}
	}
}

void CMaterialPreviewPage::update(float Elapsed)
{
	if(mObject != NULL)
	{
		int CurSel = mAnimList.GetCurSel();
		if(CurSel != CB_ERR)
		{
			CString Anim;
			mAnimList.GetLBText(CurSel, Anim);

			if(Anim == "[Reset]")
			{
				if(!mPrevAnim.IsEmpty())
				{
					Ogre::AnimationState *State = mObject->getEntity()->getAnimationState(Ogre::String(mPrevAnim));
					State->setEnabled(false);
				}
				mPrevAnim.Empty();
			} else

			if(mPrevAnim != Anim)
			{
				Ogre::AnimationState *State = NULL;
				if(!mPrevAnim.IsEmpty())
				{
					State = mObject->getEntity()->getAnimationState(Ogre::String(mPrevAnim));
					State->setEnabled(false);
				}

				mPrevAnim = Anim;
				State = mObject->getEntity()->getAnimationState(Ogre::String(mPrevAnim));
				State->setEnabled(true);
				State->setTimePosition(0);
			}
		}

		if(!mPrevAnim.IsEmpty())
			mObject->getEntity()->getAnimationState(Ogre::String(mPrevAnim))->addTime(Elapsed);
	}
	mView->Invalidate(FALSE);
}

void CMaterialPreviewPage::OnPropertyChanged(CBCGPProp *Prop)
{
	if(Prop->GetData() == MPP_BACKGROUND_COLOUR)
	{
		COLORREF BackgroundColour = Prop->GetValue();
		mView->getViewport()->setBackgroundColour(
			Ogre::ColourValue(GetRValue(BackgroundColour)/255.0f, GetGValue(BackgroundColour)/255.0f, GetBValue(BackgroundColour)/255.0f));
	} else

	if(Prop->GetData() == MPP_AMBIENT_LIGHT)
	{
		COLORREF AmbientLight = Prop->GetValue();
		mSceneManager->setAmbientLight(
			Ogre::ColourValue(GetRValue(AmbientLight)/255.0f, GetGValue(AmbientLight)/255.0f, GetBValue(AmbientLight)/255.0f));
	}

	else
	{
		int CurSel = mLights.GetCurSel();
		CString Sel;
		mLights.GetText(CurSel, Sel);

		Ogre::Light *light = mSceneManager->getLight(Ogre::String(Sel));

		switch(Prop->GetData())
		{
		case MPP_LIGHT_TYPE:
			{
				CString Type = Prop->GetValue();
				Ogre::Light::LightTypes _Type;
			
				if(Type == "Point")
				{
					_Type = Ogre::Light::LT_POINT;
				} else

				if(Type == "Directional")
				{
					_Type = Ogre::Light::LT_DIRECTIONAL;
				} else

				if(Type == "Spotlight")
				{
					_Type = Ogre::Light::LT_SPOTLIGHT;
				}

				light->setType(_Type);
			}
			break;

		case MPP_LIGHT_DIFFUSE_COLOUR:
			{
				COLORREF DiffuseColour = Prop->GetValue();
				light->setDiffuseColour(
					Ogre::ColourValue(GetRValue(DiffuseColour)/255.0f, GetGValue(DiffuseColour)/255.0f, GetBValue(DiffuseColour)/255.0f));
			}
			break;
		
		case MPP_LIGHT_SPECULAR_COLOUR:
			{
				COLORREF SpecularColour = Prop->GetValue();
				light->setSpecularColour(
					Ogre::ColourValue(GetRValue(SpecularColour)/255.0f, GetGValue(SpecularColour)/255.0f, GetBValue(SpecularColour)/255.0f));
			}
			break;

		case MPP_LIGHT_POSITION_X:
		case MPP_LIGHT_POSITION_Y:
		case MPP_LIGHT_POSITION_Z:
			{
				Ogre::Vector3 Position;

				CBCGPPropList *PropList = CPropertyWnd::Current->GetPropList();
				Position.x = PropList->FindItemByData(MPP_LIGHT_POSITION_X)->GetValue();
				Position.y = PropList->FindItemByData(MPP_LIGHT_POSITION_Y)->GetValue();
				Position.z = PropList->FindItemByData(MPP_LIGHT_POSITION_Z)->GetValue();

				light->setPosition(Position);
			}
			break;

		case MPP_LIGHT_DIRECTION_X:
		case MPP_LIGHT_DIRECTION_Y:
		case MPP_LIGHT_DIRECTION_Z:
			{
				Ogre::Vector3 Direction;

				CBCGPPropList *PropList = CPropertyWnd::Current->GetPropList();
				Direction.x = PropList->FindItemByData(MPP_LIGHT_DIRECTION_X)->GetValue();
				Direction.y = PropList->FindItemByData(MPP_LIGHT_DIRECTION_Y)->GetValue();
				Direction.z = PropList->FindItemByData(MPP_LIGHT_DIRECTION_Z)->GetValue();

				light->setDirection(Direction);
			}
			break;

		case MPP_LIGHT_VISIBLE:
			{
				bool Visible = Prop->GetValue();
				light->setVisible(Visible);
			}
		}
	}
}

BEGIN_MESSAGE_MAP(CMaterialPreviewPage, CBCGPDialog)
	ON_CBN_SELCHANGE(IDC_ANIM_LIST, &CMaterialPreviewPage::OnCbnSelchangeAnimList)
	ON_BN_CLICKED(IDC_QUAD, &CMaterialPreviewPage::OnBnClickedQuad)
	ON_BN_CLICKED(IDC_EDIT_PROGRAM, &CMaterialPreviewPage::OnBnClickedEditProgram)
	ON_BN_CLICKED(IDC_ADD_LIGHT, &CMaterialPreviewPage::OnBnClickedAddLight)
	ON_BN_CLICKED(IDC_DEL_LIGHT, &CMaterialPreviewPage::OnBnClickedDelLight)
	ON_LBN_SELCHANGE(IDC_LIGHTS, &CMaterialPreviewPage::OnLbnSelchangeLights)
	ON_BN_CLICKED(IDC_APPLY_MATERIAL, &CMaterialPreviewPage::OnBnClickedApplyMaterial)
	ON_BN_CLICKED(IDC_CLEAR_MATERIAL, &CMaterialPreviewPage::OnBnClickedClearMaterial)
	ON_BN_CLICKED(IDC_ADD_COMPOSITOR, &CMaterialPreviewPage::OnBnClickedAddCompositor)
	ON_BN_CLICKED(IDC_DEL_COMPOSITOR, &CMaterialPreviewPage::OnBnClickedDelCompositor)
	ON_BN_CLICKED(IDC_ENABLE_COMPOSITOR, &CMaterialPreviewPage::OnBnClickedEnableCompositor)
	ON_BN_CLICKED(IDC_DISABLE_COMPOSITOR, &CMaterialPreviewPage::OnBnClickedDisableCompositor)
END_MESSAGE_MAP()

void CMaterialPreviewPage::OnCbnSelchangeAnimList()
{
	
}

void CMaterialPreviewPage::OnBnClickedQuad()
{
	ClearObject();

	Ogre::ManualObject *ManualObject = mSceneManager->createManualObject("Quad");
	ManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
			
	ManualObject->position(50.0f, -50.0, 0.0);
	ManualObject->textureCoord(0, 1);
	ManualObject->normal(Ogre::Vector3(0, 0, 1));
	ManualObject->position(-50.0f, 50.0f, 0.0);
	ManualObject->textureCoord(1, 0);
	ManualObject->normal(Ogre::Vector3(0, 0, 1));
	ManualObject->position(-50.0f, -50.0, 0.0);
	ManualObject->textureCoord(1, 1);
	ManualObject->normal(Ogre::Vector3(0, 0, 1));
	ManualObject->position(50.0f, 50.0f, 0.0);
	ManualObject->textureCoord(0, 0);
	ManualObject->normal(Ogre::Vector3(0, 0, 1));

	ManualObject->index(0);
	ManualObject->index(1);
	ManualObject->index(2);
	ManualObject->index(0);
	ManualObject->index(3);
	ManualObject->index(1);

	ManualObject->end();
	ManualObject->convertToMesh("Quad");

	mObject = new StaticMesh(mSceneManager);
	mObject->create("Quad", "Quad");

	mSceneManager->destroyManualObject(ManualObject);

	mCameraManager.setTarget(mObject);
	mCameraManager.faceTo(mObject);
}

void CMaterialPreviewPage::OnBnClickedEditProgram()
{
	CFileDialog Dlg(TRUE, NULL, NULL, OFN_NOCHANGEDIR|OFN_ALLOWMULTISELECT,
		"All Files (*.*)|*.*||", NULL);
	if(Dlg.DoModal() == IDOK)
		theApp.getDocTemplate("Material")->OpenDocumentFile(Dlg.GetPathName());
}

void CMaterialPreviewPage::OnBnClickedAddLight()
{
	Ogre::Light *light = mSceneManager->createLight(
		Ogre::StringConverter::toString(Ogre::Math::RangeRandom(0, 1000)) + ".Light");
	mLights.AddString(light->getName().c_str());
}


void CMaterialPreviewPage::OnBnClickedDelLight()
{
	int CurSel = mLights.GetCurSel();
	if(CurSel != LB_ERR && CurSel != 0)
	{
		CString Sel; mLights.GetText(CurSel, Sel);
		mLights.DeleteString(CurSel);

		mSceneManager->destroyLight(Ogre::String(Sel));
	}
}

void CMaterialPreviewPage::OnLbnSelchangeLights()
{
	int CurSel = mLights.GetCurSel();
	if(CurSel != LB_ERR)
	{
		CPropertyWnd::Current->SetListener(this);

		CBCGPPropList *PropList = CPropertyWnd::Current->GetPropList();
		PropList->RemoveAll();

		CBCGPProp *Prop = NULL, *SubProp = NULL;

		CString Sel;
		mLights.GetText(CurSel, Sel);

		if(Sel == "[Ambient]")
		{
			Ogre::ColourValue BackgroundColour = mView->getViewport()->getBackgroundColour();
			COLORREF _BackgroundColour = RGB(BackgroundColour.r*255, BackgroundColour.g*255, BackgroundColour.b*255); 
			Prop = new CBCGPColorProp("Background colour", _BackgroundColour, NULL, "", MPP_BACKGROUND_COLOUR);
			((CBCGPColorProp*)Prop)->EnableOtherButton("Other");
			PropList->AddProperty(Prop);

			Ogre::ColourValue AmbientLight = mSceneManager->getAmbientLight();
			COLORREF _AmbientLight = RGB(AmbientLight.r*255, AmbientLight.g*255, AmbientLight.b*255); 
			Prop = new CBCGPColorProp("Ambient light", _AmbientLight, NULL, "", MPP_AMBIENT_LIGHT);
			((CBCGPColorProp*)Prop)->EnableOtherButton("Other");
			PropList->AddProperty(Prop);
		}
		else
		{
			Ogre::Light *light = mSceneManager->getLight(Ogre::String(Sel));

			//////////////////////////////////////////////////
			// Name
			//////////////////////////////////////////////////

			const char *Name = light->getName().c_str();
			Prop = new CBCGPProp("Name", (_variant_t)Name, "", MPP_LIGHT_NAME);
			Prop->AllowEdit(FALSE);
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Type
			//////////////////////////////////////////////////

			Ogre::Light::LightTypes Type = light->getType();
			CString _Type;
			switch(Type)
			{
			case Ogre::Light::LT_POINT:
				{
					_Type = "Point";
				}
				break;

			case Ogre::Light::LT_DIRECTIONAL:
				{
					_Type = "Directional";
				}
				break;

			case Ogre::Light::LT_SPOTLIGHT:
				{
					_Type = "Spotlight";
				}
			}
			Prop = new CBCGPProp("Type", (_variant_t)_Type, "", MPP_LIGHT_TYPE);
			Prop->AddOption("Point");
			Prop->AddOption("Directional");
			Prop->AddOption("Spotlight");
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Diffuse colour
			//////////////////////////////////////////////////

			Ogre::ColourValue DiffuseColour = light->getDiffuseColour();
			COLORREF _DiffuseColour = RGB(DiffuseColour.r*255, DiffuseColour.g*255, DiffuseColour.b*255); 
			Prop = new CBCGPColorProp("Diffuse colour", _DiffuseColour, NULL, "", MPP_LIGHT_DIFFUSE_COLOUR);
			((CBCGPColorProp*)Prop)->EnableOtherButton("Other");
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Specular colour
			//////////////////////////////////////////////////

			Ogre::ColourValue SpecularColour = light->getSpecularColour();
			COLORREF _SpecularColour = RGB(SpecularColour.r*255, SpecularColour.g*255, SpecularColour.b*255); 
			Prop = new CBCGPColorProp("Specular colour", _SpecularColour, NULL, "", MPP_LIGHT_SPECULAR_COLOUR);
			((CBCGPColorProp*)Prop)->EnableOtherButton("Other");
			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Position
			//////////////////////////////////////////////////

			Prop = new CBCGPProp("Position");

			Ogre::Vector3 Position = light->getPosition();
			
			SubProp = new CBCGPProp("X", (_variant_t)Position.x, "", MPP_LIGHT_POSITION_X);
			Prop->AddSubItem(SubProp);

			SubProp = new CBCGPProp("Y", (_variant_t)Position.y, "", MPP_LIGHT_POSITION_Y);
			Prop->AddSubItem(SubProp);

			SubProp = new CBCGPProp("Z", (_variant_t)Position.z, "", MPP_LIGHT_POSITION_Z);
			Prop->AddSubItem(SubProp);

			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Direction
			//////////////////////////////////////////////////

			Prop = new CBCGPProp("Direction");

			Ogre::Vector3 Direction = light->getDirection();
			
			SubProp = new CBCGPProp("X", (_variant_t)Direction.x, "", MPP_LIGHT_DIRECTION_X);
			Prop->AddSubItem(SubProp);

			SubProp = new CBCGPProp("Y", (_variant_t)Direction.y, "", MPP_LIGHT_DIRECTION_Y);
			Prop->AddSubItem(SubProp);

			SubProp = new CBCGPProp("Z", (_variant_t)Direction.z, "", MPP_LIGHT_DIRECTION_Z);
			Prop->AddSubItem(SubProp);

			PropList->AddProperty(Prop);

			//////////////////////////////////////////////////
			// Visible
			//////////////////////////////////////////////////
					
			bool Visible = light->getVisible();
			Prop = new CBCGPProp("Visible", (_variant_t)Visible, "", MPP_LIGHT_VISIBLE);
			PropList->AddProperty(Prop);
		}
	}
}

void CMaterialPreviewPage::OnBnClickedApplyMaterial()
{
	int CurrSel = mMaterialList.GetCurSel();
	if(CurrSel != LB_ERR)
	{
		CString Material;
		mMaterialList.GetText(CurrSel, Material);

		CString InternalPath = StringUtils::internalPath(theApp.getPath("Material"), std::string(mCurrentFile)).c_str();
		ReloadMaterial(Material.GetString(), "General", InternalPath.GetString(), true);
		if(mObject != NULL)
			mObject->getEntity()->setMaterialName(Ogre::String(Material), "General");
	}
}

void CMaterialPreviewPage::OnBnClickedClearMaterial()
{
	mObject->getEntity()->setMaterialName("BaseWhiteNoLight", "General");
}

void CMaterialPreviewPage::OnBnClickedAddCompositor()
{
	int CurSel = mCompositorList.GetCurSel();
	if(CurSel != LB_ERR)
	{
		CString Compositor;
		mCompositorList.GetText(CurSel, Compositor);

		if(mCurrentCompositors.FindString(0, Compositor) != LB_ERR) // 不能重复添加同一组合器
			return;

		CString InternalPath = StringUtils::internalPath(theApp.getPath("Material"), std::string(mCurrentFile)).c_str();
		ReloadCompositor(Compositor.GetString(), "General", InternalPath.GetString(), true);
		// Material preview
		Ogre::CompositorManager::getSingleton().addCompositor(
			mCamera->getViewport(), Ogre::String(Compositor));
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(
			mCamera->getViewport(), Ogre::String(Compositor), true);
		mCurrentCompositors.InsertString(mCurrentCompositors.GetCount(), Compositor);
		
		// Scene
		if(SceneDoc::current != NULL)
		{
			Ogre::CompositorManager::getSingleton().addCompositor(
				SceneDoc::current->getCamera()->getViewport(), Ogre::String(Compositor));
			Ogre::CompositorManager::getSingleton().setCompositorEnabled(
				SceneDoc::current->getCamera()->getViewport(), Ogre::String(Compositor), true);
		}
	}
}

void CMaterialPreviewPage::OnBnClickedDelCompositor()
{
	int CurSel = mCurrentCompositors.GetCurSel();
	if(CurSel != LB_ERR)
	{
		CString Compositor;
		mCurrentCompositors.GetText(CurSel, Compositor);
		Ogre::CompositorManager::getSingleton().removeCompositor(
			mCamera->getViewport(), Ogre::String(Compositor));
		mCurrentCompositors.DeleteString(CurSel);

		// Scene
		if(SceneDoc::current != NULL)
		{
			Ogre::CompositorManager::getSingleton().removeCompositor(
				SceneDoc::current->getCamera()->getViewport(), Ogre::String(Compositor));
		}
	}
}

void CMaterialPreviewPage::OnBnClickedEnableCompositor()
{
	int CurSel = mCurrentCompositors.GetCurSel();
	if(CurSel != LB_ERR)
	{
		CString Compositor;
		mCurrentCompositors.GetText(CurSel, Compositor);
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(
			mCamera->getViewport(), Ogre::String(Compositor), true);

		// Scene
		if(SceneDoc::current != NULL)
		{
			Ogre::CompositorManager::getSingleton().setCompositorEnabled(
				SceneDoc::current->getCamera()->getViewport(), Ogre::String(Compositor), true);
		}
	}
}


void CMaterialPreviewPage::OnBnClickedDisableCompositor()
{
	int CurSel = mCurrentCompositors.GetCurSel();
	if(CurSel != LB_ERR)
	{
		CString Compositor;
		mCurrentCompositors.GetText(CurSel, Compositor);
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(
			mCamera->getViewport(), Ogre::String(Compositor), false);

		// Scene
		if(SceneDoc::current != NULL)
		{
			Ogre::CompositorManager::getSingleton().setCompositorEnabled(
				SceneDoc::current->getCamera()->getViewport(), Ogre::String(Compositor), false);
		}
	}
}

void CMaterialPreviewPage::ReloadMaterial(const std::string& materialName, const std::string& groupName, const std::string& filename, bool parseMaterialScript)
{
    if (materialName.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Material name is empty.");
        return;
    }
 
    if (groupName.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Group name is empty.");
        return;
    }
 
    if (filename.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Filename is empty.");
        return;
    }
 
    UnloadMaterials(filename);
    UnloadVertexPrograms(filename);
    UnloadFragmentPrograms(filename);
 
    if (parseMaterialScript)
    {
        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
        if(!stream.isNull())
        {
            try
            {
                Ogre::MaterialManager::getSingleton().parseScript(stream, groupName);
                Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingleton().getByName(materialName);
                if (!materialPtr.isNull())
                {
                    materialPtr->compile();
                    materialPtr->load();
                }
            }
            catch (Ogre::Exception &e)
            {
                Ogre::StringUtil::StrStreamType msg;
                msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
                Ogre::LogManager::getSingleton().logMessage(msg.str());
            }
        }
        stream->close();
 
        ///
        /// RELOAD MATERIAL SCRIPT CONTENTS
        ///
        stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
        if(!stream.isNull())
        {
            try
            {
                ///
                /// RELOAD ALL MATERIAL CONTENTS IN FILE
                ///
                while(!stream->eof())
                {
                    std::string line = stream->getLine();
                    Ogre::StringUtil::trim(line);
                    ///
                    /// RELOAD MATERIALS
                    ///
                    if (Ogre::StringUtil::startsWith(line, "material "))
                    {
						Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line, "\t\r\n: ");
						bool skipFirst = true, canPrint = true;
						std::string match, _match;
						for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
						{
							if (skipFirst)
							{
								skipFirst = false;
								continue;
							}

							std::string match = (*it);

							if(match.find_first_of('\"') != std::string::npos)
							{
								if(canPrint)
								{
									canPrint = false;
								}
								else
								{
									canPrint = true;
									match = _match + match;
								}
							}
				
							if(canPrint)
							{
								Ogre::StringUtil::trim(match);
								match.erase(0, match.find_first_not_of('\"'));  
								match.erase(match.find_last_not_of('\"')+1);

								if(!match.empty())
								{
									LoadResource(Ogre::MaterialManager::getSingletonPtr(), match, groupName);
									break;
								}
							}
							else
							{
								_match += (match + " ");
							}
						}
                    }
                    ///
                    /// RELOAD VERTEX PROGRAMS
                    ///
                    if (Ogre::StringUtil::startsWith(line, "vertex_program") && !Ogre::StringUtil::startsWith(line, "vertex_program_ref"))
                    {
                        Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t\r\n");
                        bool skipFirst = true;
                        for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                        {
                            if (skipFirst)
                            {
                                skipFirst = false;
                                continue;
                            }
                            std::string match = (*it);
                            Ogre::StringUtil::trim(match);
                            if (!match.empty())
                            {
                                LoadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match, groupName);
                                break;
                            }
                        }
                    }
                    ///
                    /// RELOAD FRAGMENT PROGRAMS
                    ///
                    if (Ogre::StringUtil::startsWith(line, "fragment_program") && !Ogre::StringUtil::startsWith(line, "fragment_program_ref"))
                    {
                        Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t\r\n");
                        bool skipFirst = true;
                        for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                        {
                            if (skipFirst)
                            {
                                skipFirst = false;
                                continue;
                            }
                            std::string match = (*it);
                            Ogre::StringUtil::trim(match);
                            if (!match.empty())
                            {
                                LoadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match, groupName);
                                break;
                            }
                        }
                    }
                }
            }
            catch (Ogre::Exception &e)
            {
                Ogre::StringUtil::StrStreamType msg;
                msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
                Ogre::LogManager::getSingleton().logMessage(msg.str());
            }
        }
        stream->close();
 
        try
        {
            // Do a render test if it fails, leave materials unloaded
            Ogre::Root::getSingleton().renderOneFrame();
            return;
        }
        catch (Ogre::Exception &e)
        {
            UnloadVertexPrograms(filename);
 
            Ogre::StringUtil::StrStreamType msg;
            msg << "Render test failed. Unloading vertex programs." << std::endl;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
 
        try
        {
            // Do a render test if it fails, leave materials unloaded
            Ogre::Root::getSingleton().renderOneFrame();
        }
        catch (Ogre::Exception &e)
        {
            // Don't load the script this time
            ReloadMaterial(materialName, groupName, filename, false);
 
            Ogre::StringUtil::StrStreamType msg;
            msg << "Render test failed. Unloading materials." << std::endl;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
}

void CMaterialPreviewPage::ReloadCompositor(const std::string& compositorName, const std::string &groupName, const std::string& filename, bool parseCompositorScript)
{
    if (compositorName.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Material name is empty.");
        return;
    }
 
    if (groupName.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Group name is empty.");
        return;
    }
 
    if (filename.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Filename is empty.");
        return;
    }
 
    UnloadCompositors(filename);

	if (parseCompositorScript)
    {
        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
        if(!stream.isNull())
        {
            try
            {
				Ogre::CompositorManager::getSingleton().parseScript(stream, groupName);
				Ogre::CompositorPtr compositorPtr = Ogre::CompositorManager::getSingleton().getByName(compositorName);
                if (!compositorPtr.isNull())
                {
					compositorPtr->load();
                }
            }
            catch (Ogre::Exception &e)
            {
                Ogre::StringUtil::StrStreamType msg;
                msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
                Ogre::LogManager::getSingleton().logMessage(msg.str());
            }
        }
        stream->close();
 
        ///
        /// RELOAD COMPOSITOR SCRIPT CONTENTS
        ///
        stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
        if(!stream.isNull())
        {
            try
            {
                ///
                /// RELOAD ALL COMPOSITOR CONTENTS IN FILE
                ///
                while(!stream->eof())
                {
                    std::string line = stream->getLine();
                    Ogre::StringUtil::trim(line);
                    ///
                    /// RELOAD COMPOSITORS
                    ///
                    if (Ogre::StringUtil::startsWith(line, "compositor "))
                    {
						Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line, "\t\r\n: ");
						bool skipFirst = true, canPrint = true;
						std::string match, _match;
						for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
						{
							if (skipFirst)
							{
								skipFirst = false;
								continue;
							}

							std::string match = (*it);

							if(match.find_first_of('\"') != std::string::npos)
							{
								if(canPrint)
								{
									canPrint = false;
								}
								else
								{
									canPrint = true;
									match = _match + match;
								}
							}
				
							if(canPrint)
							{
								Ogre::StringUtil::trim(match);
								match.erase(0, match.find_first_not_of('\"'));  
								match.erase(match.find_last_not_of('\"')+1);

								if(!match.empty())
								{
									LoadResource(Ogre::CompositorManager::getSingletonPtr(), match, groupName);
									break;
								}
							}
							else
							{
								_match += (match + " ");
							}
						}
                    }
                }
            }
            catch (Ogre::Exception &e)
            {
                Ogre::StringUtil::StrStreamType msg;
                msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
                Ogre::LogManager::getSingleton().logMessage(msg.str());
            }
        }
        stream->close();
    }
}

void CMaterialPreviewPage::UnloadMaterials(const std::string& filename)
{
    if (filename.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Filename is empty.");
        return;
    }
 
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
    if(!stream.isNull())
    {
        try
        {
            while(!stream->eof())
            {
                std::string line = stream->getLine();
                Ogre::StringUtil::trim(line);
 
                ///
                /// UNLOAD MATERIALS
                ///
                if (Ogre::StringUtil::startsWith(line, "material "))
                {
					Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line, "\t\r\n: ");
					bool skipFirst = true, canPrint = true;
					std::string match, _match;
					for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
					{
						if (skipFirst)
						{
							skipFirst = false;
							continue;
						}

						std::string match = (*it);

						if(match.find_first_of('\"') != std::string::npos)
						{
							if(canPrint)
							{
								canPrint = false;
							}
							else
							{
								canPrint = true;
								match = _match + match;
							}
						}
				
						if(canPrint)
						{
							Ogre::StringUtil::trim(match);
							match.erase(0, match.find_first_not_of('\"'));  
							match.erase(match.find_last_not_of('\"')+1);

							if(!match.empty())
							{
								UnloadResource(Ogre::MaterialManager::getSingletonPtr(), match);
								break;
							}
						}
						else
						{
							_match += (match + " ");
						}
					}
                }
            }
        }
        catch (Ogre::Exception &e)
        {
            Ogre::StringUtil::StrStreamType msg;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
    stream->close();
}

void CMaterialPreviewPage::UnloadCompositors(const std::string& filename)
{
    if (filename.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Filename is empty.");
        return;
    }
 
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
    if(!stream.isNull())
    {
        try
        {
            while(!stream->eof())
            {
                std::string line = stream->getLine();
                Ogre::StringUtil::trim(line);
 
                ///
                /// UNLOAD COMPOSITORS
                ///
                if (Ogre::StringUtil::startsWith(line, "compositor "))
                {
					Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line, "\t\r\n: ");
					bool skipFirst = true, canPrint = true;
					std::string match, _match;
					for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
					{
						if (skipFirst)
						{
							skipFirst = false;
							continue;
						}

						std::string match = (*it);

						if(match.find_first_of('\"') != std::string::npos)
						{
							if(canPrint)
							{
								canPrint = false;
							}
							else
							{
								canPrint = true;
								match = _match + match;
							}
						}
				
						if(canPrint)
						{
							Ogre::StringUtil::trim(match);
							match.erase(0, match.find_first_not_of('\"'));  
							match.erase(match.find_last_not_of('\"')+1);

							if(!match.empty() && mCurrentCompositors.FindString(0, match.c_str()) == LB_ERR)
							{
								UnloadResource(Ogre::CompositorManager::getSingletonPtr(), match);
								break;
							}
						}
						else
						{
							_match += (match + " ");
						}
					}
                }
            }
        }
        catch (Ogre::Exception &e)
        {
            Ogre::StringUtil::StrStreamType msg;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
    stream->close();
}

void CMaterialPreviewPage::UnloadResource(Ogre::ResourceManager* resMgr, const std::string& resourceName)
{
    Ogre::ResourcePtr rPtr = resMgr->getResourceByName(resourceName);
    if (rPtr.isNull())
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
        return;
    }
 
    rPtr->unload();
    if (rPtr->isLoaded())
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was not unloaded: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was unloaded: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
 
    resMgr->remove(resourceName);
    rPtr = resMgr->getResourceByName(resourceName);
    if (rPtr.isNull())
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was removed: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was not removed: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
 
    if (resMgr->resourceExists(resourceName))
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource still exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
}
 
void CMaterialPreviewPage::LoadResource(Ogre::ResourceManager* resMgr, const std::string& resourceName, const std::string& resourceGroup)
{
    Ogre::ResourcePtr rPtr = resMgr->load(resourceName, resourceGroup);
    if (rPtr.isNull())
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
        return;
    }
 
    rPtr->reload();
    if (rPtr->isLoaded())
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was reloaded: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was not reloaded: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
 
    if (resMgr->resourceExists(resourceName))
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource still exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
    else
    {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
}
 
void CMaterialPreviewPage::UnloadVertexPrograms(const std::string& filename)
{
    if (filename.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Material filename is empty.");
        return;
    }
 
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
    if(!stream.isNull())
    {
        try
        {
            while(!stream->eof())
            {
                std::string line = stream->getLine();
                Ogre::StringUtil::trim(line);
 
                ///
                /// UNLOAD VERTEX PROGRAMS
                ///
                if (Ogre::StringUtil::startsWith(line, "vertex_program") && !Ogre::StringUtil::startsWith(line, "vertex_program_ref"))
                {
                    Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t");
                    bool skipFirst = true;
                    for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                    {
                        if (skipFirst)
                        {
                            skipFirst = false;
                            continue;
                        }
                        std::string match = (*it);
                        Ogre::StringUtil::trim(match);
                        if (!match.empty())
                        {
                            UnloadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match);
                            break;
                        }
                    }
                }
            }
        }
        catch (Ogre::Exception &e)
        {
            Ogre::StringUtil::StrStreamType msg;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
    stream->close();
}
 
void CMaterialPreviewPage::UnloadFragmentPrograms(const std::string& filename)
{
    if (filename.empty())
    {
        Ogre::LogManager::getSingleton().logMessage("Material filename is empty.");
        return;
    }
 
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
    if(!stream.isNull())
    {
        try
        {
            while(!stream->eof())
            {
                std::string line = stream->getLine();
                Ogre::StringUtil::trim(line);
 
                ///
                /// UNLOAD FRAGMENT PROGRAMS
                ///
                if (Ogre::StringUtil::startsWith(line, "fragment_program") && !Ogre::StringUtil::startsWith(line, "fragment_program_ref"))
                {
                    Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t");
                    bool skipFirst = true;
                    for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
                    {
                        if (skipFirst)
                        {
                            skipFirst = false;
                            continue;
                        }
                        std::string match = (*it);
                        Ogre::StringUtil::trim(match);
                        if (!match.empty())
                        {
                            UnloadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match);
                            break;
                        }
                    }
                }
            }
        }
        catch (Ogre::Exception &e)
        {
            Ogre::StringUtil::StrStreamType msg;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
    stream->close();
}

void CMaterialPreviewPage::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIM_LIST, mAnimList);
	DDX_Control(pDX, IDC_LIGHTS, mLights);
	DDX_Control(pDX, IDC_MATERIAL_FRAME, mMaterialFrame);
	DDX_Control(pDX, IDC_MATERIAL_LIST, mMaterialList);
	DDX_Control(pDX, IDC_CURRENT_COMPOSITORS, mCurrentCompositors);
	DDX_Control(pDX, IDC_COMPOSITOR_LIST, mCompositorList);
}


BOOL CMaterialPreviewPage::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	//////////////////////////////////////////////////
	// Core
	//////////////////////////////////////////////////

	mSceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC);

	mCamera = mSceneManager->createCamera(
		Ogre::StringConverter::toString(Ogre::Math::RangeRandom(0, 1000)) + ".Camera");
	mCamera->setNearClipDistance(0.1f);
	mCamera->setFarClipDistance(1000.0f);
	mCamera->setAspectRatio(225.0f / 150.0f);
	mCameraManager.setCamera(mCamera);
	mCameraManager.setMode(CameraManager::ORBIT);

	//////////////////////////////////////////////////
	// View
	//////////////////////////////////////////////////

	CRect rectMaterial;
	mMaterialFrame.GetClientRect(rectMaterial);
	mMaterialFrame.MapWindowPoints(this, &rectMaterial);

	mView = (RenderView*)RUNTIME_CLASS(RenderView)->CreateObject();
  	mView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, rectMaterial, this, IDC_MATERIAL_FRAME);
	mView->setCameraManager(&mCameraManager);

	//mMeshList.SetCurSel(0);
	//OnCbnSelchangeMeshList();

	//mCameraManager.setTarget(mObject);
	//mCameraManager.faceTo(mObject);

	//////////////////////////////////////////////////
	// Light
	//////////////////////////////////////////////////
	
	mLights.AddString("[Ambient]");

	return TRUE;
}

void CMaterialPreviewPage::ClearObject()
{
	if(mObject != NULL)
	{
		mCameraManager.setTarget(NULL);

		mObject->destroy();
		delete mObject;
		mObject = NULL;

		mAnimList.ResetContent();
		mPrevAnim.Empty();
	}
}
