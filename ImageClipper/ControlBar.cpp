// ControlBar.cpp : implementation file
//

#include "stdafx.h"
#include "ImageClipper.h"
#include "ControlBar.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ImageClipperDoc.h"
#include "ImageClipperView.h"

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
	ON_BN_CLICKED(IDC_BUTTON_PREV, &ControlBar::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &ControlBar::OnBnClickedButtonNext)
END_MESSAGE_MAP()


// ControlBar message handlers


BOOL ControlBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  Add extra initialization here
	//设置按钮图标
	CImage image;
	image.Load(_T("res\\left-2.ico"));
	leftBitMap.Attach(image.Detach());
	buttonPrev.SetBitmap(leftBitMap);
	image.Load(_T("res\\right-2.ico"));
	rightBitMap.Attach(image.Detach());
	buttonNext.SetBitmap(rightBitMap);
	
	//设置透明度
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE,
		GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) ^ 0x80000);
	HINSTANCE hInst = LoadLibrary(_T("User32.DLL"));  //加载库文件
	if (hInst)
	{
		typedef BOOL(WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD);
		MYFUNC func = NULL;	//函数指针
							//取得SetLayeredWindowAttributes函数指针 
		func = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		//使用SetLayeredWindowAttributes函数设定透明度
		if (func)func(this->GetSafeHwnd(), RGB(0, 0, 0), 150, 0x2);
		FreeLibrary(hInst);
	}

	buttonPrev.EnableWindow(FALSE);
	buttonNext.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void ControlBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
	
					   // Do not call CDialogEx::OnPaint() for painting messages
	//添加设置背景色
	//CRect   rect;
	//GetClientRect(rect);
	//dc.FillSolidRect(rect, RGB(0, 0, 0));
	

}


void ControlBar::OnBnClickedButtonPrev()
{
	// TODO: Add your control notification handler code here
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CImageClipperView* pView = (CImageClipperView*)pMain->GetActiveView();
    CImageClipperDoc* pDoc =  pView->GetDocument();
	if (pDoc->m_index_path_image<=0)
	{
		return;
	}
	pDoc->m_index_path_image--;
	pDoc->loadImage();
	pView->Invalidate();
}


void ControlBar::OnBnClickedButtonNext()
{
	// TODO: Add your control notification handler code here
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CImageClipperView* pView = (CImageClipperView*)pMain->GetActiveView();
	CImageClipperDoc* pDoc = pView->GetDocument();	
	if (pDoc->m_index_path_image > (pDoc->m_imageNameList.size()-1))
	{
		return;
	}
	pDoc->m_index_path_image++;
	pDoc->loadImage();
	pView->Invalidate();
}
