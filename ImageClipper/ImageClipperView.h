
// ImageClipperView.h : interface of the CImageClipperView class
//

#pragma once
#include <WinUser.h>
#include "ControlBar.h"
class CImageClipperView : public CScrollView
{
protected: // create from serialization only
	CImageClipperView();
	DECLARE_DYNCREATE(CImageClipperView)

// Attributes
public:
	CImageClipperDoc* GetDocument() const;
	ControlBar* pDlg;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CImageClipperView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in ImageClipperView.cpp
inline CImageClipperDoc* CImageClipperView::GetDocument() const
   { return reinterpret_cast<CImageClipperDoc*>(m_pDocument); }
#endif

