// ControlBar.cpp : implementation file
//

#include "stdafx.h"
#include "ImageClipper.h"
#include "ControlBar.h"
#include "afxdialogex.h"


// ControlBar dialog

IMPLEMENT_DYNAMIC(ControlBar, CDialogEx)

ControlBar::ControlBar(CWnd* pParent /*=NULL*/)
	: CDialogEx(ControlBar::IDD, pParent)
{
}

ControlBar::~ControlBar()
{
}

void ControlBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_PREV, buttonPrev);
	DDX_Control(pDX, IDC_BUTTON_NEXT, buttonNext);
}


BEGIN_MESSAGE_MAP(ControlBar, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// ControlBar message handlers


BOOL ControlBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  Add extra initialization here
	//���ð�ťͼ��
	CImage image;
	image.Load(_T("res\\left-2.ico"));
	leftBitMap.Attach(image.Detach());
	buttonPrev.SetBitmap(leftBitMap);
	image.Load(_T("res\\right-2.ico"));
	rightBitMap.Attach(image.Detach());
	buttonNext.SetBitmap(rightBitMap);
	
	//����͸����
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE,
		GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) ^ 0x80000);
	HINSTANCE hInst = LoadLibrary(_T("User32.DLL"));  //���ؿ��ļ�
	if (hInst)
	{
		typedef BOOL(WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD);
		MYFUNC func = NULL;	//����ָ��
							//ȡ��SetLayeredWindowAttributes����ָ�� 
		func = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		//ʹ��SetLayeredWindowAttributes�����趨͸����
		if (func)func(this->GetSafeHwnd(), RGB(0, 0, 0), 150, 0x2);
		FreeLibrary(hInst);
	}

	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void ControlBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
	
					   // Do not call CDialogEx::OnPaint() for painting messages
	//������ñ���ɫ
	//CRect   rect;
	//GetClientRect(rect);
	//dc.FillSolidRect(rect, RGB(0, 0, 0));
	

}
