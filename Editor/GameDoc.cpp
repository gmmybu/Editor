#include "stdafx.h"
#include "Editor.h"
#include "GameDoc.h"
#include "GameView.h"

#include "SharedData.h"

#include "OgreCamera.h"
#include "OgreRoot.h"
#include "OgreSceneManager.h"

IMPLEMENT_DYNCREATE(CGameDoc, CDocument)

CGameDoc *CGameDoc::Current = NULL;
CGameDoc::CGameDoc()
{
	Current = this;
}

CGameDoc::~CGameDoc()
{
}

void CGameDoc::Initialize()
{
	new SharedData();

	sceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC);
	sceneManager->setAmbientLight(Ogre::ColourValue(0.15, 0.00, 0.00));

	// Create main, static light
	Light* l1 = sceneManager->createLight();
    l1->setType(Light::LT_DIRECTIONAL);
    l1->setDiffuseColour(0.5f, 0.45f, 0.1f);
	l1->setDirection(1, -0.5, -0.2);
	l1->setShadowFarClipDistance(250);
	l1->setShadowFarDistance(75);
	// Turn this on to have the directional light cast shadows
	l1->setCastShadows(false);

	camera = sceneManager->createCamera(
		Ogre::StringConverter::toString(Ogre::Math::RangeRandom(0, 1000)) + ".Camera");
	camera->setPosition(25, 5, 0);
	camera->lookAt(0, 0, 0);
	camera->setNearClipDistance(0.5f);
	camera->setFarClipDistance(1000.0f);
	cameraManager.setCamera(camera);
	// cameraManager.SetSpeed(20.0f);
	cameraManager.setDragLook(true);

	//Create the scene
	// Create "root" node
	Ogre::SceneNode* rootNode = sceneManager->getRootSceneNode()->createChildSceneNode();
        
    // Create the cathedral - this will be the static scene
	Ogre::Entity* cathedralEnt = sceneManager->createEntity("Cathedral", "sibenik.mesh");
    Ogre::SceneNode* cathedralNode = rootNode->createChildSceneNode();
    cathedralNode->attachObject(cathedralEnt);
	
	createAtheneScene(rootNode);
	createKnotScene(rootNode);
	createObjects(rootNode);
}

void CGameDoc::Destroy()
{
	if(sceneManager != NULL)
		Ogre::Root::getSingleton().destroySceneManager(sceneManager);
}

void CGameDoc::update(float Elapsed)
{
	SharedData::getSingleton().iLastFrameTime = Elapsed;
        
	if (SharedData::getSingleton().mMLAnimState)
		SharedData::getSingleton().mMLAnimState->addTime(Elapsed);

	POSITION ViewPos = GetFirstViewPosition();
	while(ViewPos != NULL)
	{
		((CGameView*)GetNextView(ViewPos))->Invalidate(FALSE);
	}
}

BEGIN_MESSAGE_MAP(CGameDoc, CDocument)
END_MESSAGE_MAP()

void setEntityHeight(Ogre::Entity* ent, Ogre::Real newHeight)
{
    Ogre::Real curHeight = ent->getMesh()->getBounds().getSize().y;
    Ogre::Real scaleFactor = newHeight / curHeight;
        
    Ogre::SceneNode* parentNode = ent->getParentSceneNode();
    parentNode->setScale(scaleFactor, scaleFactor, scaleFactor);
}

void CGameDoc::createAtheneScene(Ogre::SceneNode* rootNode)
{
	// Prepare athene mesh for normalmapping
    Ogre::MeshPtr pAthene = Ogre::MeshManager::getSingleton().load("athene.mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    unsigned short src, dest;
    if (!pAthene->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
        pAthene->buildTangentVectors(VES_TANGENT, src, dest);
        
	//Create an athena statue
    Ogre::Entity* athena = sceneManager->createEntity("Athena", "athene.mesh");
	athena->setMaterialName("DeferredDemo/DeferredAthena");
	Ogre::SceneNode *aNode = rootNode->createChildSceneNode();
	aNode->attachObject( athena );
	aNode->setPosition(-8.5, 4.5, 0);
    setEntityHeight(athena, 4.0);
    aNode->yaw(Ogre::Degree(90));
	// Create some happy little lights to decorate the athena statue
	createSampleLights();
}

void CGameDoc::createSampleLights()
{
	// Create some lights		
	vector<Light*>::type lights;
	SceneNode *parentNode = sceneManager->getRootSceneNode()->createChildSceneNode("LightsParent");
	// Create light nodes
	vector<Node*>::type nodes;
        
    Vector4 attParams = Vector4(4,1,0,7);
    Real lightRadius = 25;
        
	Light *a = sceneManager->createLight();
	SceneNode *an = parentNode->createChildSceneNode();
	an->attachObject(a);
	a->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
	//a->setAttenuation(1.0f, 0.000f, 0.000f);
	an->setPosition(0,0,lightRadius);
	a->setDiffuseColour(1,0,0);
	//a->setSpecularColour(0.5,0,0);
	lights.push_back(a);
	nodes.push_back(an);
        
	Light *b = sceneManager->createLight();
	SceneNode *bn = parentNode->createChildSceneNode();
	bn->attachObject(b);
	b->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
	bn->setPosition(lightRadius,0,0);
	b->setDiffuseColour(1,1,0);
	//b->setSpecularColour(0.5,0.5,0);
	lights.push_back(b);
	nodes.push_back(bn);
        
	Light *c = sceneManager->createLight();
	SceneNode *cn = parentNode->createChildSceneNode();
	cn->attachObject(c);
	c->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
	cn->setPosition(0,0,-lightRadius);
	c->setDiffuseColour(0,1,1);
	c->setSpecularColour(0.25,1.0,1.0); // Cyan light has specular component
	lights.push_back(c);
	nodes.push_back(cn);
        
	Light *d = sceneManager->createLight();
	SceneNode *dn = parentNode->createChildSceneNode();
	dn->attachObject(d);
	d->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
	dn->setPosition(-lightRadius,0,0);
	d->setDiffuseColour(1,0,1);
	d->setSpecularColour(0.0,0,0.0);
	lights.push_back(d);
	nodes.push_back(dn);
        
	Light *e = sceneManager->createLight();
	SceneNode *en = parentNode->createChildSceneNode();
	en->attachObject(e);
	e->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
	en->setPosition(lightRadius,0,lightRadius);
	e->setDiffuseColour(0,0,1);
	e->setSpecularColour(0,0,0);
	lights.push_back(e);
	nodes.push_back(en);
		
	Light *f = sceneManager->createLight();
	SceneNode *fn = parentNode->createChildSceneNode();
	fn->attachObject(f);
	f->setAttenuation(attParams.x, attParams.y, attParams.z, attParams.w);
	fn->setPosition(-lightRadius,0,-lightRadius);
	f->setDiffuseColour(0,1,0);
	f->setSpecularColour(0,0.0,0.0);
	lights.push_back(f);
	nodes.push_back(fn);
        
	// Create marker meshes to show user where the lights are
	Entity *ent;
	GeomUtils::createSphere("PointLightMesh", 0.05f, 5, 5, true, true);
	for(vector<Light*>::type::iterator i=lights.begin(); i!=lights.end(); ++i)
	{
		Light* light = *i;
		ent = sceneManager->createEntity(light->getName()+"v", "PointLightMesh");
		String matname = light->getName()+"m";
		// Create coloured material
		MaterialPtr mat = MaterialManager::getSingleton().create(matname,
																	ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        Pass* pass = mat->getTechnique(0)->getPass(0);
        pass->setDiffuse(0.0f,0.0f,0.0f,1.0f);
		pass->setAmbient(0.0f,0.0f,0.0f);
		pass->setSelfIllumination(light->getDiffuseColour());
            
		ent->setMaterialName(matname);
		//ent->setRenderQueueGroup(light->getRenderQueueGroup());
		ent->setRenderQueueGroup(DeferredShadingSystem::POST_GBUFFER_RENDER_QUEUE);
		static_cast<SceneNode*>(light->getParentNode())->attachObject(ent);
		ent->setVisible(true);
	}		
        
	// Store nodes for hiding/showing
	SharedData::getSingleton().mLightNodes = nodes;
        
	// Do some animation for node a-f
	// Generate helix structure
	float seconds_per_station = 1.0f;
	float r = 1.0;
	//Vector3 base(0,-30,0);
	Vector3 base(-8.75, 3.5, 0);
        
	float h=3;
	const size_t s_to_top = 16;
	const size_t stations = s_to_top*2-1;
	float ascend = h/((float)s_to_top);
	float stations_per_revolution = 3.5f;
	size_t skip = 2; // stations between lights
	Vector3 station_pos[stations];
	for(size_t x=0; x<s_to_top; ++x)
	{
		float theta = ((float)x/stations_per_revolution)*2.0f*Math::PI;
		station_pos[x] = base+Vector3(Math::Sin(theta)*r, ascend*x, Math::Cos(theta)*r);
	}
	for(size_t x=s_to_top; x<stations; ++x)
	{
		float theta = ((float)x/stations_per_revolution)*2.0f*Math::PI;
		station_pos[x] = base+Vector3(Math::Sin(theta)*r, h-ascend*(x-s_to_top), Math::Cos(theta)*r);
	}
	// Create a track for the light swarm
	Animation* anim = sceneManager->createAnimation("LightSwarmTrack", stations*seconds_per_station);
	// Spline it for nice curves
	anim->setInterpolationMode(Animation::IM_SPLINE);
	for(unsigned int x=0; x<nodes.size(); ++x)
	{
		// Create a track to animate the camera's node
		NodeAnimationTrack* track = anim->createNodeTrack(x, nodes[x]);
		for(size_t y=0; y<=stations; ++y)
		{
			// Setup keyframes
			TransformKeyFrame* key = track->createNodeKeyFrame(y*seconds_per_station); // A start position
			key->setTranslate(station_pos[(x*skip+y)%stations]);
			// Make sure size of light doesn't change
			key->setScale(nodes[x]->getScale());
		}
	}
	// Create a new animation state to track this
	SharedData::getSingleton().mMLAnimState = sceneManager->createAnimationState("LightSwarmTrack");
	SharedData::getSingleton().mMLAnimState->setEnabled(true);
        
	/*light* spotLight = mSceneMgr->createLight("Spotlight1");
        spotlight->setType(Light::LT_SPOTLIGHT);
        spotlight->setAttenuation(200, 1.0f, 0, 0);
        spotlight->setSpotlightRange(Degree(30.0), Degree(45.0), 0.8);
        spotlight->setPosition(0,120,0);
        spotlight->setDirection(0, -1, 0);
        spotlight->setDiffuseColour(1,1,1);
        spotlight->setSpecularColour(1,1,1);*/
}

void CGameDoc::createKnotScene(SceneNode* rootNode)
{
	// Prepare knot mesh for normal mapping
	MeshPtr pKnot = MeshManager::getSingleton().load("knot.mesh", 
                                                        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	unsigned short src, dest;
    if (!pKnot->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
        pKnot->buildTangentVectors(VES_TANGENT, src, dest);
        
	// Create a bunch of knots with spotlights hanging from above
	Entity* knotEnt = sceneManager->createEntity("Knot", "knot.mesh");
	knotEnt->setMaterialName("DeferredDemo/RockWall");
	//knotEnt->setMeshLodBias(0.25f);
    Vector3 knotStartPos(25.5, 2, 5.5);
    Vector3 knotDiff(-3.7, 0, 0);
    for (int i=0; i < 5; i++)
    {
        char cloneName[16];
		sprintf_s(cloneName, "Knot%d", i);
        Entity* cloneKnot = knotEnt->clone(cloneName);
        Vector3 clonePos = knotStartPos + knotDiff*i;
		SceneNode* cloneNode = rootNode->createChildSceneNode(clonePos);
        cloneNode->attachObject(cloneKnot);
        setEntityHeight(cloneKnot, 3);
        cloneNode->yaw(Degree(i*17));
        cloneNode->roll(Degree(i*31));
            
        sprintf_s(cloneName, "KnotLight%d", i);
        Light* knotLight = sceneManager->createLight(cloneName);
        knotLight->setType(Light::LT_SPOTLIGHT);
        knotLight->setDiffuseColour(SAMPLE_COLORS[i]);
        knotLight->setSpecularColour(ColourValue::White);
        knotLight->setPosition(clonePos + Vector3(0,3,0));
        knotLight->setDirection(Vector3::NEGATIVE_UNIT_Y);
        knotLight->setSpotlightRange(Degree(25), Degree(45), 1);
        knotLight->setAttenuation(6, 1, 0.2, 0);
    }
}

void CGameDoc::createObjects(SceneNode* rootNode)
{
	// Create ogre heads to decorate the wall
	Entity* ogreHead = sceneManager->createEntity("Head", "ogrehead.mesh");
	//rootNode->createChildSceneNode( "Head" )->attachObject( ogreHead );
    Vector3 headStartPos[2] = { Vector3(25.25,11,3), Vector3(25.25,11,-3) };
    Vector3 headDiff(-3.7,0,0);
    for (int i=0; i < 12; i++) 
    {
        char cloneName[16];
		sprintf_s(cloneName, "OgreHead%d", i);
        Entity* cloneHead = ogreHead->clone(cloneName);
        Vector3 clonePos = headStartPos[i%2] + headDiff*(i/2);
        if ((i/2) >= 4) clonePos.x -= 0.75;
		SceneNode* cloneNode = rootNode->createChildSceneNode(clonePos);
        cloneNode->attachObject(cloneHead);
        setEntityHeight(cloneHead, 1.5);
        if (i % 2 == 0)
        {
            cloneNode->yaw(Degree(180));
        }
    }
        
	// Create a pile of wood pallets
    Entity* woodPallet = sceneManager->createEntity("Pallet", "WoodPallet.mesh");
    Vector3 woodStartPos(10, 0.5, -5.5);
    Vector3 woodDiff(0, 0.3, 0);
    for (int i=0; i < 5; i++)
    {
        char cloneName[16];
		sprintf_s(cloneName, "WoodPallet%d", i);
        Entity* clonePallet = woodPallet->clone(cloneName);
        Vector3 clonePos = woodStartPos + woodDiff*i;
		SceneNode* cloneNode = rootNode->createChildSceneNode(clonePos);
        cloneNode->attachObject(clonePallet);
        setEntityHeight(clonePallet, 0.3);
        cloneNode->yaw(Degree(i*20));
    }
        
}
