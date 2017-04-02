

// ImageClipperView.cpp : implementation of the CImageClipperView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ImageClipper.h"
#endif

#include "ImageClipperDoc.h"
#include "ImageClipperView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageClipperView

IMPLEMENT_DYNCREATE(CImageClipperView, CScrollView)

BEGIN_MESSAGE_MAP(CImageClipperView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CImageClipperView construction/destruction

CImageClipperView::CImageClipperView()
{
	// TODO: add construction code here
	
	pDlg = new ControlBar();
	pDlg->Create(IDD_CONTROL_BAR);
	pDlg->GetWindowRect(&m_rectControl);
}

CImageClipperView::~CImageClipperView()
{
	if (pDlg!=NULL)
	{
		delete pDlg;
	}
}

BOOL CImageClipperView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CImageClipperView drawing

void CImageClipperView::OnDraw(CDC* pDC)
{
	CImageClipperDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	if(!pDoc->img.IsNull())
	{
		SetScrollSizes(MM_TEXT, CSize(pDoc->img.GetWidth()+100, pDoc->img.GetHeight()+100));
		pDoc->img.Draw(pDC->GetSafeHdc(), 0, 0);
	}
	else
	{
		return;
	}
	if (pDoc->m_index_path_image == -1 || pDoc->m_imageNameList.empty())
	{
		//初始化状态
		pDlg->buttonPrev.EnableWindow(FALSE);
		pDlg->buttonNext.EnableWindow(FALSE);
	}
	else if ((pDoc->m_index_path_image) == 0)
	{
		//左向按钮不可用;
		pDlg->buttonPrev.EnableWindow(FALSE);	
		pDlg->buttonNext.EnableWindow(TRUE);
	}
	else if ((pDoc->m_index_path_image) == (pDoc->m_imageNameList.size()-1))
	{
		//右向按钮不可用;
		pDlg->buttonPrev.EnableWindow(TRUE);
		pDlg->buttonNext.EnableWindow(FALSE);
	}
	else
	{
		pDlg->buttonPrev.EnableWindow(TRUE);
		pDlg->buttonNext.EnableWindow(TRUE);
	}
}

void CImageClipperView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	GetClientRect(&m_rectClient);
}


// CImageClipperView printing

BOOL CImageClipperView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageClipperView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageClipperView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CImageClipperView diagnostics

#ifdef _DEBUG
void CImageClipperView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageClipperView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageClipperDoc* CImageClipperView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageClipperDoc)));
	return (CImageClipperDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageClipperView message handlers


void CImageClipperView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnMouseMove(nFlags, point);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CString ponints;
	ponints.Format(_T("(%d, %d)"), point.x, point.y);
	pMainFrame->m_wndStatusBar.SetPaneText(0, ponints);
	GetClientRect(&m_rectClient);
	//获取视图窗口的大小
	if (point.y > m_rectClient.bottom*0.9)
	{
		//ControlBar* pDlg = new ControlBar();
		//pDlg->Create(IDD_DIALOG_CONTROL);
		pDlg->SetWindowPos(NULL, (m_rectClient.Width()-m_rectControl.Width())/2, m_rectClient.bottom,0,0,SWP_SHOWWINDOW|SWP_NOSIZE);
		pDlg->ShowWindow(SW_SHOWNORMAL);

	}
	else
	{		
		//pDlg->RunModalLoop();		
		pDlg->SetWindowPos(NULL, (m_rectClient.Width() - m_rectControl.Width()) / 2, m_rectClient.bottom,0,0,SWP_HIDEWINDOW|SWP_NOSIZE);
		//pDlg->ShowWindow(SW_SHOWNORMAL);
	}
	
}
