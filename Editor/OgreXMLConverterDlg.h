#pragma once
#include "afxwin.h"


// COgreXMLConverterDlg 对话框

class COgreXMLConverterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COgreXMLConverterDlg)

public:
	COgreXMLConverterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COgreXMLConverterDlg();

// 对话框数据
	enum { IDD = IDD_OGRE_XML_CONVERTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit mInputPath;
	CEdit mOutputPath;
	CEdit mArgs;
	afx_msg void OnBnClickedGetInputPath();
	afx_msg void OnBnClickedGetOutputPath();
	afx_msg void OnBnClickedXmlToBinary();
	afx_msg void OnBnClickedBinaryToXml();
};
