
// ImageClipperDoc.h : interface of the CImageClipperDoc class
//


#pragma once
#include <vector>
#include <map>
#include "MainFrm.h"

#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\highgui\highgui.hpp"



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

typedef std::map<CString, std::vector<MyRectTracker*>> images_info;
typedef std::map<CString, std::vector<MyRectTracker*>>::iterator pimages_info;

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

	//保存当前单张图片选择区域的rectTracker;
	std::vector<MyRectTracker*> m_rectTrackers;//裁剪的图片坐标
	int m_index_current_selected;              //切换图片
	std::vector<MyRectTracker*>::iterator it_current; //删除图片

	//记录下本目录下所有的的操作
	images_info m_images_clipper_result;


// Operations
public:
	void findAllImageFile(CString path, std::vector<CString>& out_files);
	void findIndexImage();
	void loadImage();

	std::string convertCString2string(CString i_str);
// Overrides
public:
//	virtual BOOL OnNewDocument();
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

	BOOL deleteRectCurren();
	// 保存当前的信息到内存中
	bool saveRectImageInfor();
	// 释放当前处理图片的空间
	void freeCurrentImageZone();
	afx_msg void OnEditCut();
};
