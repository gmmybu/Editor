#pragma once


// ImageCombinator 对话框

class ImageCombinator : public CDialogEx
{
	DECLARE_DYNAMIC(ImageCombinator)

public:
	ImageCombinator(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ImageCombinator();

// 对话框数据
	enum { IDD = IDD_IMAGE_COMBINATOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString diffusePath;
	CString specularPath;
	CString diffuseSpecularPath;
	CString normalPath;
	CString heightPath;
	CString normalHeightPath;
	afx_msg void OnBnClickedLoadDiffuse();
	afx_msg void OnBnClickedLoadSpecular();
	afx_msg void OnBnClickedSaveDiffuseSpecular();
	afx_msg void OnBnClickedLoadNormal();
	afx_msg void OnBnClickedLoadHeight();
	afx_msg void OnBnClickedSaveNormalHeight();
	afx_msg void OnBnClickedGenerate();
};
