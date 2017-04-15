#pragma once


#include "ImageClipperDoc.h"
#include "tinyXml\include\tinyxml.h"
#include "tinyXml\include\tinystr.h"

extern class MyRectTracker;
extern images_info;
extern pimages_info;


class CIO
{
public:
	CIO();
	~CIO();
	static void  CStringToChar(CString i_cstring , char*& o_p);
	static bool saveXML(const CString& path, images_info infor_rect);
	static bool readXML(const CString& path, images_info& infor_rect);
};

