// SearchPage.cpp : implementation file
//

#include "stdafx.h"
#include "tbmessage.h"
#include "SearchPage.h"


// CSearchPage dialog

IMPLEMENT_DYNAMIC(CSearchPage, CDialog)

CSearchPage::CSearchPage(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchPage::IDD, pParent)
{

}

CSearchPage::~CSearchPage()
{
}

void CSearchPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSearchPage, CDialog)
END_MESSAGE_MAP()


// CSearchPage message handlers
