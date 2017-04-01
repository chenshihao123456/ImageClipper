#pragma once


// ControlBar dialog

class ControlBar : public CDialogEx
{
	DECLARE_DYNAMIC(ControlBar)

public:
	ControlBar(CWnd* pParent = NULL);   // standard constructor
	virtual ~ControlBar();

// Dialog Data
	enum { IDD = IDD_CONTROL_BAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
