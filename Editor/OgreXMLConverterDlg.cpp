#include "stdafx.h"
#include "Editor.h"
#include "OgreXMLConverterDlg.h"
#include "OgreXMLMeshSerializer.h"
#include "OgreXMLSkeletonSerializer.h"
#include "StringUtils.h"
#include "OgreMeshManager.h"
#include "OgreMeshSerializer.h"
#include "OgreSkeletonManager.h"
#include "OgreSkeletonSerializer.h"
#include "OgreXMLSkeletonSerializer.h"

IMPLEMENT_DYNAMIC(COgreXMLConverterDlg, CDialogEx)

COgreXMLConverterDlg::COgreXMLConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COgreXMLConverterDlg::IDD, pParent)
{

}

COgreXMLConverterDlg::~COgreXMLConverterDlg()
{
}

void COgreXMLConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT_PATH, mInputPath);
	// DDX_Control(pDX, IDC_OUTPUT_PATH, mOutputPath);
	DDX_Control(pDX, IDC_ARGS, mArgs);
}

BEGIN_MESSAGE_MAP(COgreXMLConverterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_GET_INPUT_PATH, &COgreXMLConverterDlg::OnBnClickedGetInputPath)
	ON_BN_CLICKED(IDC_GET_OUTPUT_PATH, &COgreXMLConverterDlg::OnBnClickedGetOutputPath)
	ON_BN_CLICKED(IDC_XML_TO_BINARY, &COgreXMLConverterDlg::OnBnClickedXmlToBinary)
	ON_BN_CLICKED(IDC_BINARY_TO_XML, &COgreXMLConverterDlg::OnBnClickedBinaryToXml)
END_MESSAGE_MAP()

void COgreXMLConverterDlg::OnBnClickedGetInputPath()  
{  
    CFileDialog Dlg(TRUE, NULL, NULL, OFN_NOCHANGEDIR,   
        "All Files (*.*)|*.*||", NULL);  
    if(Dlg.DoModal() == IDOK)  
        mInputPath.SetWindowText(Dlg.GetPathName());  
}

void COgreXMLConverterDlg::OnBnClickedGetOutputPath()
{
	CFileDialog Dlg(FALSE, NULL, NULL, OFN_NOCHANGEDIR, 
		"XML (*.xml)|*.xml|\
		All Files (*.*)|*.*||", NULL);
	if(Dlg.DoModal() == IDOK)
		mOutputPath.SetWindowText(Dlg.GetPathName());
}

void COgreXMLConverterDlg::OnBnClickedBinaryToXml()  
{  
    CString InputPath; mInputPath.GetWindowText(InputPath);  
    if(InputPath.IsEmpty())  
        return;  
	CString Extension = StringUtils::extension(std::string(InputPath)).c_str();  
    if(Extension == "xml")  
    {  
        AfxMessageBox("输入文件并非Mesh或Skeleton！");  
        return;  
    }  
  
    CString OutputPath = (InputPath + ".xml");  
    std::ifstream ifs; ifs.open(InputPath, std::ios_base::in | std::ios_base::binary);  
    Ogre::DataStreamPtr stream(new Ogre::FileStreamDataStream(Ogre::String(InputPath), &ifs, false));  
  
    if(Extension == "mesh")  
    {  
        Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().create("conversion",   
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);  
        Ogre::MeshSerializer MeshSerializer;  
        MeshSerializer.importMesh(stream, mesh.getPointer());  
        Ogre::XMLMeshSerializer XMLMeshSerializer;  
        XMLMeshSerializer.exportMesh(mesh.getPointer(), Ogre::String(OutputPath));   
        AfxMessageBox("Mesh -> XML 成功！");  
    } else  
    if(Extension == "skeleton")  
    {  
        Ogre::SkeletonPtr skel = Ogre::SkeletonManager::getSingleton().create("conversion",   
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);  
        Ogre::SkeletonSerializer SkeletonSerializer;  
        SkeletonSerializer.importSkeleton(stream, skel.getPointer());  
        Ogre::XMLSkeletonSerializer XMLSkeletonSerializer;  
        XMLSkeletonSerializer.exportSkeleton(skel.getPointer(), Ogre::String(OutputPath));  
        AfxMessageBox("Skeleton -> XML 成功！");  
    }  
    else  
    {  
        AfxMessageBox("输入文件并非Mesh或Skeleton！");  
    }  
}  
  
void COgreXMLConverterDlg::OnBnClickedXmlToBinary()  
{  
    CString InputPath; mInputPath.GetWindowText(InputPath);  
    if(InputPath.IsEmpty())  
        return;  
    CString Extension = StringUtils::extension(std::string(InputPath)).c_str();  
    if(Extension != "xml")  
    {  
        AfxMessageBox("输入文件并非Mesh或Skeleton的XML！");  
        return;  
    }  
  
	CString OutputPath = StringUtils::filename(std::string(InputPath)).c_str();  
    TiXmlDocument* doc = new TiXmlDocument(InputPath); doc->LoadFile();  
    TiXmlElement* root = doc->RootElement();  
  
    if (!stricmp(root->Value(), "mesh"))  
    {  
        delete doc;  
        Ogre::MeshPtr newMesh = Ogre::MeshManager::getSingleton().createManual("conversion",   
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);  
        Ogre::VertexElementType colourElementType = Ogre::VET_COLOUR_ARGB;  
        Ogre::XMLMeshSerializer XMLMeshSerializer;  
        XMLMeshSerializer.importMesh(Ogre::String(InputPath), colourElementType, newMesh.getPointer());  
        newMesh->_determineAnimationTypes();  
        Ogre::MeshSerializer MeshSerializer;  
        MeshSerializer.exportMesh(newMesh.getPointer(), Ogre::String(OutputPath));  
        Ogre::MeshManager::getSingleton().remove("conversion");  
        AfxMessageBox("Mesh XML -> Mesh 成功！");  
    }  
    else if (!stricmp(root->Value(), "skeleton"))  
    {  
        delete doc;  
        Ogre::SkeletonPtr newSkel = Ogre::SkeletonManager::getSingleton().create("conversion",   
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);  
        Ogre::XMLSkeletonSerializer XMLSkeletonSerializer;  
        XMLSkeletonSerializer.importSkeleton(Ogre::String(InputPath), newSkel.getPointer());  
        Ogre::SkeletonSerializer SkeletonSerializer;  
        SkeletonSerializer.exportSkeleton(newSkel.getPointer(), Ogre::String(OutputPath), Ogre::SKELETON_VERSION_LATEST);  
        Ogre::SkeletonManager::getSingleton().remove("conversion");  
        AfxMessageBox("Skeleton XML -> Skeleton 成功！");  
    }  
    else  
    {  
        delete doc;  
        AfxMessageBox("输入文件并非Mesh或Skeleton的XML！");  
    }  
}
