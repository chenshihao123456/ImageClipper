
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
END_MESSAGE_MAP()

// CImageClipperView construction/destruction

CImageClipperView::CImageClipperView()
{
	// TODO: add construction code here

}

CImageClipperView::~CImageClipperView()
{
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
}

void CImageClipperView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
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
