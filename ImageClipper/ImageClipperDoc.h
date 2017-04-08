
// ImageClipperDoc.h : interface of the CImageClipperDoc class
//


#pragma once
#include <vector>
#include "MainFrm.h"



class MyRectTracker:public CRectTracker
{
public:
	MyRectTracker() :m_rect_l(CRect(0, 0, 0, 0)), m_is_chosen(FALSE) {
		m_nStyle = CRectTracker::solidLine | CRectTracker::resizeInside;
		m_rect.NormalizeRect();
		m_nHandleSize = 8;
		CMainFrame* frame = (CMainFrame*)AfxGetMainWnd();
		CString cs;
		frame->m_combobox.GetWindowText(cs);
		m_pen_type = cs;
	}

	int checkHit(CPoint pt)
	{
		return HitTest(pt);
	}
	CRect& getRectDP()
	{
		return m_rect;
	}

public:
	CRect m_rect_l; //逻辑坐标;
	BOOL m_is_chosen; //是否被选择，用于绘制八个角
	CString m_pen_type;//矩形框的类型
};




class CImageClipperDoc : public CDocument
{
protected: // create from serialization only
	CImageClipperDoc();
	DECLARE_DYNCREATE(CImageClipperDoc)

// Attributes
public:
	CImage img;
	CString m_currentImagePath;           //当前图片的名字
	CString m_imageRootPath;
	std::vector<CString> m_imageNameList; 
	int m_index_path_image;

	//保存选择区域的rectTracker;
	std::vector<MyRectTracker*> m_rectTrackers;//裁剪的图片坐标
	int m_index_current_selected;              //切换图片
	std::vector<MyRectTracker*>::iterator it_current; //删除图片

	

// Operations
public:
	void findAllImageFile(CString path, std::vector<CString>& out_files);
	void findIndexImage();
	void loadImage();
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CImageClipperDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	bool GetImage();
	BOOL deleteRectCurren();
};
