

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
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CImageClipperView construction/destruction

CImageClipperView::CImageClipperView()
{
	// TODO: add construction code here
	//m_isDraw = FALSE;
	//m_ptBegin = CPoint(0, 0);
	//m_ptEnd = CPoint(0, 0);

	//m_rectTracker.m_rect.SetRect(200, 500, 400, 300);
	//m_rectTracker.m_nStyle = CRectTracker::solidLine;
	m_isChosen = FALSE;

	pDlg = new ControlBar();
	pDlg->Create(IDD_CONTROL_BAR);
	pDlg->GetWindowRect(&m_rectControl);
}

CImageClipperView::~CImageClipperView()
{
	if (pDlg != NULL)
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
	if (!pDoc->img.IsNull())
	{
		SetScrollSizes(MM_TEXT, CSize(pDoc->img.GetWidth() + 100, pDoc->img.GetHeight() + 100));
		pDoc->img.Draw(pDC->GetSafeHdc(), 0, 0);
	}
	else
	{
		return;
	}
	//设置控制面板的状态
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
	else if ((pDoc->m_index_path_image) == (pDoc->m_imageNameList.size() - 1))
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


	//绘制矩形框

	if (!pDoc->m_rectTrackers.empty())
	{
		CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
		pDC->SelectObject(&pen);
		CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		pDC->SelectObject(pbrush);

		int numRect = pDoc->m_rectTrackers.size();
		for (int i = 0; i < numRect; i++)
		{
			MyRectTracker *pRectTracker = pDoc->m_rectTrackers[i];
			pRectTracker->getRectDP() = pRectTracker->m_rect_l;
			pDC->LPtoDP(pRectTracker->getRectDP());
			pRectTracker->Draw(pDC);
			pDC->Rectangle(pRectTracker->m_rect_l);
		}

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

	//状态栏显示
	ponints.Format(_T("(%d, %d)"), point.x, point.y);
	pMainFrame->m_wndStatusBar.SetPaneText(0, ponints);
	GetClientRect(&m_rectClient);
	//设置控制面板的装态
	if (point.y > m_rectClient.bottom*0.9)
	{
		pDlg->SetWindowPos(NULL, (m_rectClient.Width() - m_rectControl.Width()) / 2, m_rectClient.bottom, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
		pDlg->ShowWindow(SW_SHOWNORMAL);
	}
	else
	{
		pDlg->SetWindowPos(NULL, (m_rectClient.Width() - m_rectControl.Width()) / 2, m_rectClient.bottom, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE);
	}
}


void CImageClipperView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	OnPrepareDC(&dc);//进行坐标原点的匹配
	CImageClipperDoc* pDoc = GetDocument();
	int trackerCount = pDoc->m_rectTrackers.size();
	pDoc->m_index_current_selected = -1;
	MyRectTracker* pRegion = NULL;
	//判断当前的选择框列表中，是否有选中的!
	for (int i = 0; i < trackerCount; i++)
	{
		pRegion = pDoc->m_rectTrackers[i];
		//选中tracker,操作tracker!
		if (pRegion->checkHit(point) >= 0)
		{
			pDoc->m_index_current_selected = i;
			if (pRegion->Track(this, point))
			{
				pRegion->GetTrueRect(pRegion->m_rect_l);
				//操作后，重新计算，设备坐标的逻辑坐标
				dc.DPtoLP(pRegion->m_rect_l);
				pRegion->m_is_chosen = TRUE;
			}
			break;
		}
		
	}
	//没有选择的recttracker
	if (pDoc->m_index_current_selected == -1)
	{
		pRegion = new MyRectTracker();
		pRegion->TrackRubberBand(this, point);
		//如果区域太小，就忽略掉
		pRegion->m_rect_l = pRegion->getRectDP();
		if (pRegion->m_rect_l.Height() <30 || pRegion->m_rect_l.Width()<30)
		{
			delete pRegion;
			return;
		}
		dc.DPtoLP(pRegion->m_rect_l);
		pRegion->m_is_chosen = TRUE;
		pDoc->m_rectTrackers.push_back(pRegion);
	}

	Invalidate();
	CScrollView::OnLButtonDown(nFlags, point);
}



void CImageClipperView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_isChosen = FALSE;
	//m_ptEnd = point;
	CScrollView::OnLButtonUp(nFlags, point);
}


