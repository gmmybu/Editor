#include "stdafx.h"
#include "MaterialGenerator.h"

#include "OgreException.h"
#include "OgreHighLevelGpuProgram.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgrePass.h"
#include "OgreStringConverter.h"
#include "OgreTechnique.h"

MaterialGenerator::MaterialGenerator():
	vsMask(0), fsMask(0), matMask(0), mImpl(0)
{
}
MaterialGenerator::~MaterialGenerator()
{
	delete mImpl;
}

const Ogre::MaterialPtr &MaterialGenerator::getMaterial(Perm permutation)
{
	/// Check if material/shader permutation already was generated
	MaterialMap::iterator i = mMaterials.find(permutation);
	if(i != mMaterials.end())
	{
		return i->second;
	}
	else
	{
		/// Create it
		Ogre::MaterialPtr templ = getTemplateMaterial(permutation & matMask);
		Ogre::GpuProgramPtr vs = getVertexShader(permutation & vsMask);
		Ogre::GpuProgramPtr fs = getFragmentShader(permutation & fsMask);
		
		/// Create material name
		Ogre::String name = materialBaseName + Ogre::StringConverter::toString(permutation);

		std::cerr << name << " " << vs->getName() << " " << fs->getName() << std::endl;
		/// Create material from template, and set shaders
		Ogre::MaterialPtr mat = templ->clone(name);
		Ogre::Technique *tech = mat->getTechnique(0);
		Ogre::Pass *pass = tech->getPass(0);
		pass->setFragmentProgram(fs->getName());
		pass->setVertexProgram(vs->getName());
	
		/// And store it
		mMaterials[permutation] = mat;
		return mMaterials[permutation];
	}
}

const Ogre::GpuProgramPtr &MaterialGenerator::getVertexShader(Perm permutation)
{
	ProgramMap::iterator i = mVs.find(permutation);
	if(i != mVs.end())
	{
		return i->second;
	}
	else
	{
		/// Create it
		mVs[permutation] = mImpl->generateVertexShader(permutation);
		return mVs[permutation];
	}
}

const Ogre::GpuProgramPtr &MaterialGenerator::getFragmentShader(Perm permutation)
{
	ProgramMap::iterator i = mFs.find(permutation);
	if(i != mFs.end())
	{
		return i->second;
	}
	else
	{
		/// Create it
		mFs[permutation] = mImpl->generateFragmentShader(permutation);
		return mFs[permutation];
	}
}

const Ogre::MaterialPtr &MaterialGenerator::getTemplateMaterial(Perm permutation)
{
	MaterialMap::iterator i = mTemplateMat.find(permutation);
	if(i != mTemplateMat.end())
	{
		return i->second;
	}
	else
	{
		/// Create it
		mTemplateMat[permutation] = mImpl->generateTemplateMaterial(permutation);
		return mTemplateMat[permutation];
	}
}

MaterialGenerator::Impl::~Impl()
{
}

