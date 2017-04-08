
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
	freeCurrentImageZone();

	pimages_info it_begin = m_images_clipper_result.begin();
	pimages_info it_end = m_images_clipper_result.end();
	while (it_begin != it_end)
	{
		if (!((it_begin->second).empty()))
		{
			for (int i = 0; i < it_begin->second.size(); i++)
			{
				if (it_begin->second[i]!=NULL)
				{
					delete it_begin->second[i];
					it_begin->second[i] = NULL;
				}
			}
		}

		it_begin++;
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

	freeCurrentImageZone();
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

		CString nameImage = m_currentImagePath.Mid(m_currentImagePath.ReverseFind('\\') + 1);
		m_rectTrackers = m_images_clipper_result[nameImage];
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


// 保存当前的信息到内存中
bool CImageClipperDoc::saveRectImageInfor()
{
	//从缓存，保存到内存中!
	CString nameImage = m_currentImagePath.Mid(m_currentImagePath.ReverseFind('\\')+1);
	m_images_clipper_result[nameImage] = m_rectTrackers;
	//并且更新到文件中!

	return true;
}


// 释放当前处理图片的空间
void CImageClipperDoc::freeCurrentImageZone()
{
	//查询某个键是否存在,也就是没有保存当前的操作！，如果不存在，就释放该空间
	CString nameImage = m_currentImagePath.Mid(m_currentImagePath.ReverseFind('\\') + 1);
	if (m_images_clipper_result.count(nameImage) <= 0)
	{
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

}
