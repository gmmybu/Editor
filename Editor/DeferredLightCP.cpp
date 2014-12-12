#include "stdafx.h"
#include "DeferredLightCP.h"

#include "AmbientLight.h"
#include "DLight.h"
#include "LightMaterialGenerator.h"

#include "OgreCompositorChain.h"
#include "OgreSceneManager.h"
#include "OgreTechnique.h"

//-----------------------------------------------------------------------
DeferredLightRenderOperation::DeferredLightRenderOperation(
	Ogre::CompositorInstance* instance, const Ogre::CompositionPass* pass)
{
	mViewport = instance->getChain()->getViewport();
	
	//Get the names of the GBuffer textures
	const Ogre::CompositionPass::InputTex& input0 = pass->getInput(0);
	mTexName0 = instance->getTextureInstanceName(input0.name, input0.mrtIndex);
	const Ogre::CompositionPass::InputTex& input1 = pass->getInput(1);
	mTexName1 = instance->getTextureInstanceName(input1.name, input1.mrtIndex);

	// Create lights material generator
	mLightMaterialGenerator = new LightMaterialGenerator();
	
	// Create the ambient light
	mAmbientLight = new AmbientLight();
	const Ogre::MaterialPtr& mat = mAmbientLight->getMaterial();
	mat->load();
}
//-----------------------------------------------------------------------
DLight* DeferredLightRenderOperation::createDLight(Ogre::Light* light)
{
	DLight *rv = new DLight(mLightMaterialGenerator,light);
	mLights[light] = rv;
	return rv;
}
//-----------------------------------------------------------------------
void injectTechnique(Ogre::SceneManager* sm, Ogre::Technique* tech, Ogre::Renderable* rend, const Ogre::LightList* lightList)
{
    for(unsigned short i=0; i<tech->getNumPasses(); ++i)
	{
		Ogre::Pass* pass = tech->getPass(i);
        if (lightList != 0) 
		{
			sm->_injectRenderWithPass(pass, rend, false, false, lightList);
		} 
		else
		{
			sm->_injectRenderWithPass(pass, rend, false);
		}
		
	}
}
//-----------------------------------------------------------------------
void DeferredLightRenderOperation::execute(Ogre::SceneManager *sm, Ogre::RenderSystem *rs)
{
    Ogre::Camera* cam = mViewport->getCamera();

	mAmbientLight->updateFromCamera(cam);
    Ogre::Technique* tech = mAmbientLight->getMaterial()->getBestTechnique();
	injectTechnique(sm, tech, mAmbientLight, 0);

	const Ogre::LightList& lightList = sm->_getLightsAffectingFrustum();
    for(Ogre::LightList::const_iterator it = lightList.begin(); it != lightList.end(); it++) 
	{
        Ogre::Light* light = *it;
		Ogre::LightList ll;
		ll.push_back(light);

		//if (++i != 2) continue;
        //if (light->getType() != Light::LT_DIRECTIONAL) continue;
		//if (light->getDiffuseColour() != ColourValue::Red) continue;

		LightsMap::iterator dLightIt = mLights.find(light);
		DLight* dLight = 0;
		if (dLightIt == mLights.end()) 
		{
			dLight = createDLight(light);
		}
		else 
		{
			dLight = dLightIt->second;
			dLight->updateFromParent();
		}
		dLight->updateFromCamera(cam);
		tech = dLight->getMaterial()->getBestTechnique();

		//Update shadow texture
		if (dLight->getCastChadows())
		{
			Ogre::SceneManager::RenderContext* context = sm->_pauseRendering();

			sm->prepareShadowTextures(cam, mViewport, &ll);
			sm->_resumeRendering(context);
			
			Ogre::Pass* pass = tech->getPass(0);
			Ogre::TextureUnitState* tus = pass->getTextureUnitState("ShadowMap");
			assert(tus);
			const Ogre::TexturePtr& shadowTex = sm->getShadowTexture(0);
			if (tus->_getTexturePtr() != shadowTex)
			{
				tus->_setTexturePtr(shadowTex);
			}
			
		}
		
        injectTechnique(sm, tech, dLight, &ll);
	}
}
//-----------------------------------------------------------------------
DeferredLightRenderOperation::~DeferredLightRenderOperation()
{
	for (LightsMap::iterator it = mLights.begin(); it != mLights.end(); ++it)
	{
		delete it->second;
	}
	mLights.clear();
	
	delete mAmbientLight;
	delete mLightMaterialGenerator;
}
//-----------------------------------------------------------------------
