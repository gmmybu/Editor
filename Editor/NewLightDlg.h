#pragma once

class NewLightDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NewLightDlg)

public:
	enum { IDD = IDD_NEW_LIGHT };

	enum
	{
		TYPE,
		DIFFUSE_COLOUR,
		SPECULAR_COLOUR,
		POSITION_X,
		POSITION_Y,
		POSITION_Z,
		DIRECTION_X,
		DIRECTION_Y,
		DIRECTION_Z,
		ATTENUATION_RANGE,
		ATTENUATION_CONSTANT,
		ATTENUATION_LINEAR,
		ATTENUATION_QUADRATIC,
		SPOTLIGHT_RANGE_INNER_ANGLE,
		SPOTLIGHT_RANGE_OUTER_ANGLE,
		SPOTLIGHT_RANGE_FALLOFF,
		_COUNT
	};

public:
	NewLightDlg(CWnd* pParent = NULL);
	virtual ~NewLightDlg();

	_variant_t getProperty(int type)
	{
		return (_variant_t)values[type];
	}

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam);

	BOOL OnInitDialog();
	void DoDataExchange(CDataExchange* pDX);

	CStatic propListFrame; CBCGPPropList propList;
	CBCGPProp *props[_COUNT]; COleVariant values[_COUNT];
};
