#include "stdafx.h"
#include "LightMaterialGenerator.h"

#include "OgreStringConverter.h"
#include "OgreException.h"
#include "OgreMaterialManager.h"

#include "OgrePass.h"
#include "OgreTechnique.h"

#include "OgreGpuProgramManager.h"
#include "OgreHighLevelGpuProgram.h"
#include "OgreHighLevelGpuProgramManager.h"

#include "DLight.h"

//CG
class LightMaterialGeneratorCG : public MaterialGenerator::Impl
{
public:
	typedef MaterialGenerator::Perm Perm;
	LightMaterialGeneratorCG(const Ogre::String &baseName):
	    mBaseName(baseName) 
	{

	}
	virtual ~LightMaterialGeneratorCG()
	{

	}

	virtual Ogre::GpuProgramPtr generateVertexShader(Perm permutation)
	{
        Ogre::String programName = "DeferredShading/post/";

		if (permutation & LightMaterialGenerator::MI_DIRECTIONAL)
		{
			programName += "vs";
		}
		else
		{
			programName += "LightMaterial_vs";
		}

		Ogre::GpuProgramPtr ptr = Ogre::HighLevelGpuProgramManager::getSingleton().getByName(programName);
		assert(!ptr.isNull());
		return ptr;
	}

	virtual Ogre::GpuProgramPtr generateFragmentShader(Perm permutation)
	{
		/// Create shader
		if (mMasterSource.empty())
		{
			Ogre::DataStreamPtr ptrMasterSource = Ogre::ResourceGroupManager::getSingleton().openResource(
				 "DeferredShading/post/LightMaterial_ps.cg"
				, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			assert(ptrMasterSource.isNull()==false);
			mMasterSource = ptrMasterSource->getAsString();
		}

		assert(mMasterSource.empty()==false);

		// Create name
		Ogre::String name = mBaseName+Ogre::StringConverter::toString(permutation)+"_ps";		

		// Create shader object
		Ogre::HighLevelGpuProgramPtr ptrProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
			name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			"cg", Ogre::GPT_FRAGMENT_PROGRAM);
		ptrProgram->setSource(mMasterSource);
		ptrProgram->setParameter("entry_point","main");
	    ptrProgram->setParameter("profiles","ps_2_x arbfp1");
		// set up the preprocessor defines
		// Important to do this before any call to get parameters, i.e. before the program gets loaded
		ptrProgram->setParameter("compile_arguments", getPPDefines(permutation));

		setUpBaseParameters(ptrProgram->getDefaultParameters());

		return Ogre::GpuProgramPtr(ptrProgram);
	}

	virtual Ogre::MaterialPtr generateTemplateMaterial(Perm permutation)
	{
		Ogre::String materialName = mBaseName;
	
        if(permutation & LightMaterialGenerator::MI_DIRECTIONAL)
		{   
			materialName += "Quad";
		}
		else
		{
			materialName += "Geometry";
		}

		if(permutation & LightMaterialGenerator::MI_SHADOW_CASTER)
		{
			materialName += "Shadow";
		}
		return Ogre::MaterialManager::getSingleton().getByName(materialName);
	}

	protected:
		Ogre::String mBaseName;
        Ogre::String mMasterSource;
		// Utility method
		Ogre::String getPPDefines(Perm permutation)
		{
			Ogre::String strPPD;

			//Get the type of light
			Ogre::String lightType;
			if (permutation & LightMaterialGenerator::MI_POINT)
			{
				lightType = "POINT";
			}
			else if (permutation & LightMaterialGenerator::MI_SPOTLIGHT)
			{
				lightType = "SPOT";
			}
			else if (permutation & LightMaterialGenerator::MI_DIRECTIONAL)
			{
				lightType = "DIRECTIONAL";
			}
			else
			{
				assert(false && "Permutation must have a light type");
			}
			strPPD += "-DLIGHT_TYPE=LIGHT_" + lightType + " ";

			//Optional parameters
            if (permutation & LightMaterialGenerator::MI_SPECULAR)
			{
				strPPD += "-DIS_SPECULAR ";
			}
			if (permutation & LightMaterialGenerator::MI_ATTENUATED)
			{
				strPPD += "-DIS_ATTENUATED ";
			}
			if (permutation & LightMaterialGenerator::MI_SHADOW_CASTER)
			{
				strPPD += "-DIS_SHADOW_CASTER ";
			}
			return strPPD;
		}

		void setUpBaseParameters(const Ogre::GpuProgramParametersSharedPtr& params)
		{
			assert(params.isNull()==false);

			struct AutoParamPair { Ogre::String name; Ogre::GpuProgramParameters::AutoConstantType type; };	

			//A list of auto params that might be present in the shaders generated
			static const AutoParamPair AUTO_PARAMS[] = {
				{ "vpWidth",			Ogre::GpuProgramParameters::ACT_VIEWPORT_WIDTH },
				{ "vpHeight",			Ogre::GpuProgramParameters::ACT_VIEWPORT_HEIGHT },
				{ "worldView",			Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX },
				{ "invProj",			Ogre::GpuProgramParameters::ACT_INVERSE_PROJECTION_MATRIX },
				{ "invView",			Ogre::GpuProgramParameters::ACT_INVERSE_VIEW_MATRIX },
				{ "flip",				Ogre::GpuProgramParameters::ACT_RENDER_TARGET_FLIPPING },
				{ "lightDiffuseColor",	Ogre::GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR },
				{ "lightSpecularColor", Ogre::GpuProgramParameters::ACT_LIGHT_SPECULAR_COLOUR },
				{ "lightFalloff",		Ogre::GpuProgramParameters::ACT_LIGHT_ATTENUATION },
				{ "lightPos",			Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_VIEW_SPACE },
				{ "lightDir",			Ogre::GpuProgramParameters::ACT_LIGHT_DIRECTION_VIEW_SPACE },
				{ "spotParams",			Ogre::GpuProgramParameters::ACT_SPOTLIGHT_PARAMS },
				{ "farClipDistance",	Ogre::GpuProgramParameters::ACT_FAR_CLIP_DISTANCE },
				{ "shadowViewProjMat",	Ogre::GpuProgramParameters::ACT_TEXTURE_VIEWPROJ_MATRIX }
			};
			int numParams = sizeof(AUTO_PARAMS) / sizeof(AutoParamPair);

			for (int i=0; i<numParams; i++)
			{
				if (params->_findNamedConstantDefinition(AUTO_PARAMS[i].name))
				{
					params->setNamedAutoConstant(AUTO_PARAMS[i].name, AUTO_PARAMS[i].type);
				}
			}
		}
};

//GLSL
class LightMaterialGeneratorGLSL : public MaterialGenerator::Impl
{
public:
	typedef MaterialGenerator::Perm Perm;
	LightMaterialGeneratorGLSL(const Ogre::String &baseName):
    mBaseName(baseName)
	{

	}
	virtual ~LightMaterialGeneratorGLSL()
	{

	}

	virtual Ogre::GpuProgramPtr generateVertexShader(Perm permutation)
	{
        Ogre::String programName = "DeferredShading/post/";

		if (permutation & LightMaterialGenerator::MI_DIRECTIONAL)
		{
			programName += "vs";
		}
		else
		{
			programName += "LightMaterial_vs";
		}

		Ogre::GpuProgramPtr ptr = Ogre::HighLevelGpuProgramManager::getSingleton().getByName(programName);
		assert(!ptr.isNull());
		return ptr;
	}

	virtual Ogre::GpuProgramPtr generateFragmentShader(Perm permutation)
	{
		/// Create shader
		if (mMasterSource.empty())
		{
			Ogre::DataStreamPtr ptrMasterSource;
            if(Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
                ptrMasterSource = Ogre::ResourceGroupManager::getSingleton().openResource("DeferredShading/post/LightMaterial_ps.glsles",
                                                                                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
            else
                ptrMasterSource = Ogre::ResourceGroupManager::getSingleton().openResource("DeferredShading/post/LightMaterial_ps.glsl",
                                                                                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

			assert(ptrMasterSource.isNull()==false);
			mMasterSource = ptrMasterSource->getAsString();
		}

		assert(mMasterSource.empty()==false);

		// Create name
		Ogre::String name = mBaseName+Ogre::StringConverter::toString(permutation)+"_ps";

		// Create shader object
		Ogre::HighLevelGpuProgramPtr ptrProgram;
        if(Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
        {
            ptrProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                                  "glsles", Ogre::GPT_FRAGMENT_PROGRAM);
            ptrProgram->setParameter("profiles", "glsles");
        }
        else
        {
            ptrProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                                  "glsl", Ogre::GPT_FRAGMENT_PROGRAM);
            ptrProgram->setParameter("profiles", "glsl150");
        }
        ptrProgram->setSource(mMasterSource);
		// set up the preprocessor defines
		// Important to do this before any call to get parameters, i.e. before the program gets loaded
		ptrProgram->setParameter("preprocessor_defines", getPPDefines(permutation));

		setUpBaseParameters(ptrProgram->getDefaultParameters());

        // Bind samplers
		Ogre::GpuProgramParametersSharedPtr params = ptrProgram->getDefaultParameters();
        int numSamplers = 0;
        params->setNamedConstant("Tex0", (int)numSamplers++);
        params->setNamedConstant("Tex1", (int)numSamplers++);

        if(permutation & LightMaterialGenerator::MI_SHADOW_CASTER)
            params->setNamedConstant("ShadowTex", (int)numSamplers++);

		return Ogre::GpuProgramPtr(ptrProgram);
	}

	virtual Ogre::MaterialPtr generateTemplateMaterial(Perm permutation)
	{
		Ogre::String materialName = mBaseName;

        if(permutation & LightMaterialGenerator::MI_DIRECTIONAL)
		{
			materialName += "Quad";
		}
		else
		{
			materialName += "Geometry";
		}

		if(permutation & LightMaterialGenerator::MI_SHADOW_CASTER)
		{
			materialName += "Shadow";
		}
		return Ogre::MaterialManager::getSingleton().getByName(materialName);
	}

protected:
    Ogre::String mBaseName;
    Ogre::String mMasterSource;
    // Utility method
    Ogre::String getPPDefines(Perm permutation)
    {
        Ogre::String strPPD;

        //Get the type of light
        Ogre::uint lightType = 0;
        if (permutation & LightMaterialGenerator::MI_POINT)
        {
            lightType = 1;
        }
        else if (permutation & LightMaterialGenerator::MI_SPOTLIGHT)
        {
            lightType = 2;
        }
        else if (permutation & LightMaterialGenerator::MI_DIRECTIONAL)
        {
            lightType = 3;
        }
        else
        {
            assert(false && "Permutation must have a light type");
        }
        strPPD += "LIGHT_TYPE=" + Ogre::StringConverter::toString(lightType);

        //Optional parameters
        if (permutation & LightMaterialGenerator::MI_SPECULAR)
        {
            strPPD += ",IS_SPECULAR=1";
        }
        if (permutation & LightMaterialGenerator::MI_ATTENUATED)
        {
            strPPD += ",IS_ATTENUATED=1";
        }
        if (permutation & LightMaterialGenerator::MI_SHADOW_CASTER)
        {
            strPPD += ",IS_SHADOW_CASTER=1";
        }
        return strPPD;
    }

    void setUpBaseParameters(const Ogre::GpuProgramParametersSharedPtr& params)
    {
        assert(params.isNull()==false);

        struct AutoParamPair { Ogre::String name; Ogre::GpuProgramParameters::AutoConstantType type; };

        //A list of auto params that might be present in the shaders generated
        static const AutoParamPair AUTO_PARAMS[] = {
            { "vpWidth",            Ogre::GpuProgramParameters::ACT_VIEWPORT_WIDTH },
            { "vpHeight",           Ogre::GpuProgramParameters::ACT_VIEWPORT_HEIGHT },
            { "worldView",          Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX },
            { "invProj",            Ogre::GpuProgramParameters::ACT_INVERSE_PROJECTION_MATRIX },
            { "invView",            Ogre::GpuProgramParameters::ACT_INVERSE_VIEW_MATRIX },
            { "flip",               Ogre::GpuProgramParameters::ACT_RENDER_TARGET_FLIPPING },
            { "lightDiffuseColor",  Ogre::GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR },
            { "lightSpecularColor", Ogre::GpuProgramParameters::ACT_LIGHT_SPECULAR_COLOUR },
            { "lightFalloff",       Ogre::GpuProgramParameters::ACT_LIGHT_ATTENUATION },
            { "lightPos",           Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_VIEW_SPACE },
            { "lightDir",           Ogre::GpuProgramParameters::ACT_LIGHT_DIRECTION_VIEW_SPACE },
            { "spotParams",         Ogre::GpuProgramParameters::ACT_SPOTLIGHT_PARAMS },
            { "farClipDistance",    Ogre::GpuProgramParameters::ACT_FAR_CLIP_DISTANCE },
            { "shadowViewProjMat",  Ogre::GpuProgramParameters::ACT_TEXTURE_VIEWPROJ_MATRIX }
        };
        int numParams = sizeof(AUTO_PARAMS) / sizeof(AutoParamPair);
        
        for (int i=0; i<numParams; i++)
        {
            if (params->_findNamedConstantDefinition(AUTO_PARAMS[i].name))
            {
                params->setNamedAutoConstant(AUTO_PARAMS[i].name, AUTO_PARAMS[i].type);
            }
        }
    }
};

LightMaterialGenerator::LightMaterialGenerator()
{
	vsMask = 0x00000004;
	fsMask = 0x0000003F;
	matMask =	LightMaterialGenerator::MI_DIRECTIONAL | 
				LightMaterialGenerator::MI_SHADOW_CASTER;
	
	materialBaseName = "DeferredShading/LightMaterial/";
    if ((Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl") || Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles")) &&
        !(Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_2_x") || Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("arbfp1")))
        mImpl = new LightMaterialGeneratorGLSL("DeferredShading/LightMaterial/");
    else
        mImpl = new LightMaterialGeneratorCG("DeferredShading/LightMaterial/");
}

LightMaterialGenerator::~LightMaterialGenerator()
{

}
