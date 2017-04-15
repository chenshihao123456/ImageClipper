#include "stdafx.h"
#include "IO.h"
#include <string>
#include <atlstr.h>
#include <sstream>


CIO::CIO()
{
}


CIO::~CIO()
{
}



void CIO::CStringToChar(CString i_cstring, char*& o_p)
{
	USES_CONVERSION;
	o_p = T2A(i_cstring);
}



bool CIO::saveXML(const CString& path, images_info infor_rect)
{
	//创建一个XML的文档对象。
	TiXmlDocument *myDocument = new TiXmlDocument();
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "gbk2312", "");
	myDocument->LinkEndChild(decl);
	//创建一个根元素并连接。
	TiXmlElement *RootElement = new TiXmlElement("ImageDB");
	myDocument->LinkEndChild(RootElement);
	//创建一个Person元素并连接。
	TiXmlElement *LastImage = new TiXmlElement("LastImage");
	pimages_info it_last = infor_rect.end();
	it_last--;
	CString lastFileName = it_last->first;
	CT2A lastName(lastFileName);
	LastImage->LinkEndChild(new TiXmlText(lastName.m_psz));
	RootElement->LinkEndChild(LastImage);
	it_last++;

	TiXmlElement* Images = new TiXmlElement("Images");
	RootElement->LinkEndChild(Images);
	for (pimages_info it_begin = infor_rect.begin(); it_begin != it_last; it_begin++)
	{
		if (it_begin->second.size() > 0)
		{
			CString fileName = it_begin->first;
			CT2A ascii(fileName);
			TiXmlElement* Image = new TiXmlElement("Image");
			Image->SetAttribute("name", ascii.m_psz);
			Images->LinkEndChild(Image);
			std::vector<MyRectTracker*> rects = it_begin->second;
			for (int i = 0; i < rects.size(); i++)
			{
				TiXmlElement* ROI = new TiXmlElement("ROI");
				CString rectType = rects[i]->m_pen_type;
				USES_CONVERSION;
				char* p = T2A(rectType);
				ROI->SetAttribute("type", p);
				std::stringstream ss;
				ss.str("");
				ss << (rects[i]->m_rect_l).TopLeft().x << " " << (rects[i]->m_rect_l).TopLeft().y
					<< " " << (rects[i]->m_rect_l).Width() << " " << (rects[i]->m_rect_l).Height();
				ROI->SetAttribute("xywh", ss.str().c_str());
				Image->LinkEndChild(ROI);
			}
		}
	}

	CStringA charstr(path);
	myDocument->SaveFile((const char*)charstr);

	return true;
}

bool CIO::readXML(const CString& path, images_info& infor_rect)
{
	CStringA charstr(path);
	TiXmlDocument doc((const char*)charstr);
	if (!doc.LoadFile()) return FALSE;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);

	pElem = hDoc.FirstChildElement().Element();
	// should always have a valid root but handle gracefully if it does
	if (!pElem) return FALSE;
	// save this for later
	hRoot = TiXmlHandle(pElem);

	TiXmlElement* pImagesElement = hRoot.FirstChild("Images").FirstChild().Element();
	for (pImagesElement; pImagesElement; pImagesElement = pImagesElement->NextSiblingElement())
	{
		const char* image = pImagesElement->Attribute("name");
		TiXmlHandle imageHandle(0);
		imageHandle = TiXmlHandle(pImagesElement);
		TiXmlElement* rectsElement = imageHandle.FirstChild("ROI").Element();
		std::vector<MyRectTracker*> rects;
		for (rectsElement; rectsElement; rectsElement = rectsElement->NextSiblingElement())
		{
			const char* typeRect = rectsElement->Attribute("type");
			const char* xywh = rectsElement->Attribute("xywh");
			//封包数据
			MyRectTracker* pRegion = NULL;
			pRegion = new MyRectTracker();
			
			std::stringstream ss;
			std::string pos;
			pos = xywh;
			ss << pos;
			int x = 0, y = 0, w = 0, h = 0;
			ss >> x >> y >> w >> h;
			pRegion->m_pen_type = typeRect;
			pRegion->m_rect_l.SetRect(x, y, x+w, y+h);
			rects.push_back(pRegion);
		}
		CString imageName;
		USES_CONVERSION;
		imageName = A2W(image);
		infor_rect[imageName] = rects;
	}

	return TRUE;
}
