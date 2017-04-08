
// ImageClipperDoc.cpp : implementation of the CImageClipperDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ImageClipper.h"
#endif

#include "ImageClipperDoc.h"

#include <propkey.h>
#include<shlwapi.h>
#pragma comment(lib,"shlwapi")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageClipperDoc

IMPLEMENT_DYNCREATE(CImageClipperDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageClipperDoc, CDocument)
END_MESSAGE_MAP()


// CImageClipperDoc construction/destruction

CImageClipperDoc::CImageClipperDoc()
{
	// TODO: add one-time construction code here
	m_index_path_image = -1;
	m_imageNameList.clear();
	m_currentImagePath = _T("");
	m_rectTrackers.clear();
	m_index_current_selected = -1;
	it_current = m_rectTrackers.begin();
}

CImageClipperDoc::~CImageClipperDoc()
{
	//释放内存
	int num_box = m_rectTrackers.size();
	for (int i = 0; i < num_box; i++)
	{
		if (m_rectTrackers[i] != NULL)
		{
			delete m_rectTrackers[i];
			m_rectTrackers[i] = NULL;
		}
	}

}

BOOL CImageClipperDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


void  CImageClipperDoc::findAllImageFile(CString path, std::vector<CString>& out_files)
{
	CFileFind ff;
	CString tmp_path;
	if (path.Right(1) != "\\") {
		//path.Format(_T("%s%s"), path.GetBuffer(), "\\");
		tmp_path.Format(_T("%s%s"), path.GetBuffer(), "\\");

	}
	tmp_path += CString("*.*");
	BOOL ret = ff.FindFile(tmp_path);
	while (ret)
	{
		ret = ff.FindNextFile();

		if (ff.IsDirectory() && !ff.IsDots())
		{
			CString path = ff.GetFilePath();
			findAllImageFile(path, out_files);
		}
		else if (!ff.IsDirectory() && !ff.IsDots())
		{
			CString name = ff.GetFileName();
			int index_dot = name.Find('.');
			int length_ext = name.GetLength() - index_dot;
			CString extension = name.Right(length_ext);

			if (extension.Compare(_T(".jpg")) || extension.Compare(_T(".png")) || extension.Compare(_T(".JPEG")))
			{
				CString path = ff.GetFilePath();
				out_files.push_back(path);
			}
		}


	}

}

void CImageClipperDoc::findIndexImage()
{
	for (int i = 0; i < m_imageNameList.size(); i++)
	{
		if (m_currentImagePath == m_imageNameList[i])
		{
			m_index_path_image = i;
			return;
		}
	}
	m_index_path_image = -1;
}

// CImageClipperDoc serialization

void CImageClipperDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		//将内存数据保存到xml中！



	}
	else
	{
		// TODO: add loading code here

		m_currentImagePath = ar.GetFile()->GetFilePath();
		m_imageRootPath = m_currentImagePath.Left(m_currentImagePath.ReverseFind('\\'));
		if (m_imageNameList.empty())
			findAllImageFile(m_imageRootPath, m_imageNameList);
		findIndexImage();
		if (!img.IsNull())
		{
			img.Destroy();
		}
		img.Load(m_currentImagePath);
	}
}

void CImageClipperDoc::loadImage()
{
	if (!img.IsNull())
	{
		img.Destroy();
	}
	if (m_index_path_image >= 0 && m_index_path_image < m_imageNameList.size())
	{
		m_currentImagePath = m_imageNameList[m_index_path_image];
		img.Load(m_currentImagePath);
		int lenName = m_currentImagePath.GetLength() - m_imageRootPath.GetLength()-1;
		CString imageName = m_currentImagePath.Right(lenName);
		AfxGetApp()->m_pMainWnd->SetWindowTextW(
			imageName
			+CString("-ImageClipper"));

		m_rectTrackers.clear();
		m_index_current_selected = -1;
		it_current = m_rectTrackers.begin();
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CImageClipperDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CImageClipperDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageClipperDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageClipperDoc diagnostics

#ifdef _DEBUG
void CImageClipperDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageClipperDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageClipperDoc commands





bool CImageClipperDoc::GetImage()
{
	if ((m_index_path_image < 0) ||
		(m_index_path_image >= m_imageNameList.size()) ||
		m_imageNameList.empty())
	{
		return false;
	}
	if (!img.IsNull())
	{
		img.Destroy();
	}
	img.Load(m_imageNameList[m_index_path_image]);
}


BOOL CImageClipperDoc::deleteRectCurren()
{
	if (m_index_current_selected == -1)
	{
		return FALSE;
	}
	if (m_rectTrackers[m_index_current_selected] != NULL)
	{
		it_current = m_rectTrackers.begin();
		delete m_rectTrackers[m_index_current_selected];
		m_rectTrackers[m_index_current_selected] = NULL;
		m_rectTrackers.erase(it_current+= m_index_current_selected);
		m_index_current_selected = -1;
	}
	return TRUE;
}
