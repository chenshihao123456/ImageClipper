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


bool CIO::saveXML(const CString& path, images_info infor_rect)
{
#if 0
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");
	TiXmlElement * ImageDBElement = new TiXmlElement("ImagesDB");
	


	for (pimages_info it_begin = infor_rect.begin(); it_begin != infor_rect.end(); it_begin++)
	{
		if (it_begin->second.size()>0)
		{
			CString fileName = it_begin->first;
			CT2A ascii(fileName);
			TiXmlElement * ImageElement = new TiXmlElement("Image");
			
			TiXmlElement* name = new TiXmlElement("name");
			ImageElement->InsertEndChild(*name)->InsertEndChild(TiXmlText(ascii.m_psz));		
			name = new TiXmlElement("objectPosition");
	

			ImageElement->InsertEndChild(*name);			
			ImageDBElement->LinkEndChild(ImageElement);
		}
	}


	doc.LinkEndChild(decl);
	doc.LinkEndChild(ImageDBElement);

	CStringA charstr(path);
	doc.SaveFile((const char*)charstr);
#endif

	//创建一个XML的文档对象。
	TiXmlDocument *myDocument = new TiXmlDocument();
	//创建一个根元素并连接。
	TiXmlElement *RootElement = new TiXmlElement("ImageDB");
	myDocument->LinkEndChild(RootElement);
	//创建一个Person元素并连接。
	TiXmlElement *LastImage = new TiXmlElement("LastImage");
	LastImage->LinkEndChild(new TiXmlText("3.jpg"));
	RootElement->LinkEndChild(LastImage);

	TiXmlElement* Images = new TiXmlElement("Images");
	RootElement->LinkEndChild(Images);


	for (pimages_info it_begin = infor_rect.begin(); it_begin != infor_rect.end(); it_begin++)
	{
		if (it_begin->second.size()>0)
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
				ROI->SetAttribute("type", "1");
				std::stringstream ss;
				ss.str("");
				ss << (rects[i]->m_rect_l).TopLeft().x <<" " << (rects[i]->m_rect_l).TopLeft().y 
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

bool CIO::readXML(const CString& path)
{


	return true;
}
