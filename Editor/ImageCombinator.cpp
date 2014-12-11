// ImageCombinator.cpp : 实现文件
//

#include "stdafx.h"
#include "Editor.h"
#include "ImageCombinator.h"
#include "afxdialogex.h"
#include <fstream>
#include "StringUtils.h"
#include "OgreDataStream.h"
#include "OgreImage.h"
#include "d3d9.h"
#include "d3dx9.h"

// ImageCombinator 对话框

IMPLEMENT_DYNAMIC(ImageCombinator, CDialogEx)

ImageCombinator::ImageCombinator(CWnd* pParent /*=NULL*/)
	: CDialogEx(ImageCombinator::IDD, pParent)
	, diffusePath(_T(""))
	, specularPath(_T(""))
	, diffuseSpecularPath(_T(""))
	, normalPath(_T(""))
	, heightPath(_T(""))
	, normalHeightPath(_T(""))
{

}

ImageCombinator::~ImageCombinator()
{
}

void ImageCombinator::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, diffusePath);
	DDX_Text(pDX, IDC_EDIT2, specularPath);
	DDX_Text(pDX, IDC_EDIT3, diffuseSpecularPath);
	DDX_Text(pDX, IDC_EDIT4, normalPath);
	DDX_Text(pDX, IDC_EDIT5, heightPath);
	DDX_Text(pDX, IDC_EDIT6, normalHeightPath);
}


BEGIN_MESSAGE_MAP(ImageCombinator, CDialogEx)
	ON_BN_CLICKED(IDC_LOAD_DIFFUSE, &ImageCombinator::OnBnClickedLoadDiffuse)
	ON_BN_CLICKED(IDC_LOAD_SPECULAR, &ImageCombinator::OnBnClickedLoadSpecular)
	ON_BN_CLICKED(IDC_SAVE_DIFFUSE_SPECULAR, &ImageCombinator::OnBnClickedSaveDiffuseSpecular)
	ON_BN_CLICKED(IDC_LOAD_NORMAL, &ImageCombinator::OnBnClickedLoadNormal)
	ON_BN_CLICKED(IDC_LOAD_HEIGHT, &ImageCombinator::OnBnClickedLoadHeight)
	ON_BN_CLICKED(IDC_SAVE_NORMAL_HEIGHT, &ImageCombinator::OnBnClickedSaveNormalHeight)
	ON_BN_CLICKED(IDC_GENERATE, &ImageCombinator::OnBnClickedGenerate)
END_MESSAGE_MAP()


// ImageCombinator 消息处理程序


void ImageCombinator::OnBnClickedLoadDiffuse()
{
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		diffusePath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void ImageCombinator::OnBnClickedLoadSpecular()
{
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		specularPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void ImageCombinator::OnBnClickedSaveDiffuseSpecular()
{
	CFileDialog dlg(FALSE);
	if(dlg.DoModal() == IDOK)
	{
		diffuseSpecularPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void ImageCombinator::OnBnClickedLoadNormal()
{
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		normalPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void ImageCombinator::OnBnClickedLoadHeight()
{
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		heightPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void ImageCombinator::OnBnClickedSaveNormalHeight()
{
	CFileDialog dlg(FALSE);
	if(dlg.DoModal() == IDOK)
	{
		normalHeightPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void ImageCombinator::OnBnClickedGenerate()
{
	if(	diffusePath.IsEmpty() ||
		specularPath.IsEmpty() ||
		diffuseSpecularPath.IsEmpty() ||
		normalPath.IsEmpty() ||
		heightPath.IsEmpty() ||
		normalHeightPath.IsEmpty())
	{
		AfxMessageBox("必须填充所有路径");
		return;
	}

	std::ifstream ifs, _ifs;
	ifs.open(diffusePath, std::ios::binary|std::ios::in);
	_ifs.open(specularPath, std::ios::binary|std::ios::in);

	std::string extension, _extension;
	extension = StringUtils::extension(std::string(diffusePath));
	_extension = StringUtils::extension(std::string(specularPath));

	Ogre::Image image;
	image.loadTwoImagesAsRGBA(
		Ogre::DataStreamPtr(new Ogre::FileStreamDataStream(Ogre::String(diffusePath), &ifs, false)),
		Ogre::DataStreamPtr(new Ogre::FileStreamDataStream(Ogre::String(specularPath), &_ifs, false)));
	image.save(Ogre::String(diffuseSpecularPath));

	// => DDS
	IDirect3DDevice9 *device = theApp.getDevice(); IDirect3DTexture9 *texture = NULL;
	D3DXCreateTextureFromFile(device, diffuseSpecularPath, &texture);
	D3DXSaveTextureToFile((StringUtils::filename(std::string(diffuseSpecularPath))+".dds").c_str(), D3DXIFF_DDS, texture, NULL);

	ifs.close(); ifs.clear(); _ifs.close(); _ifs.clear();
	ifs.open(normalPath, std::ios::binary|std::ios::in);
	_ifs.open(heightPath, std::ios::binary|std::ios::in);

	extension = StringUtils::extension(std::string(normalPath));
	_extension = StringUtils::extension(std::string(heightPath));

	image.loadTwoImagesAsRGBA(
		Ogre::DataStreamPtr(new Ogre::FileStreamDataStream(Ogre::String(normalPath), &ifs, false)),
		Ogre::DataStreamPtr(new Ogre::FileStreamDataStream(Ogre::String(heightPath), &_ifs, false)));
	image.save(Ogre::String(normalHeightPath));

	// => DDS
	D3DXCreateTextureFromFile(device, normalHeightPath, &texture);
	D3DXSaveTextureToFile((StringUtils::filename(std::string(normalHeightPath))+".dds").c_str(), D3DXIFF_DDS, texture, NULL);

	AfxMessageBox("生成成功");
}
