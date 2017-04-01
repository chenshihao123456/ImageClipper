// ControlBar.cpp : implementation file
//

#include "stdafx.h"
#include "ImageClipper.h"
#include "ControlBar.h"
#include "afxdialogex.h"


// ControlBar dialog

IMPLEMENT_DYNAMIC(ControlBar, CDialogEx)

ControlBar::ControlBar(CWnd* pParent /*=NULL*/)
	: CDialogEx(ControlBar::IDD, pParent)
{

}

ControlBar::~ControlBar()
{
}

void ControlBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ControlBar, CDialogEx)
END_MESSAGE_MAP()


// ControlBar message handlers
