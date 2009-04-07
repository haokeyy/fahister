// MemberPage.cpp : implementation file
//

#include "stdafx.h"
#include "tbmessage.h"
#include "MemberPage.h"


// CMemberPage dialog

IMPLEMENT_DYNAMIC(CMemberPage, CDialog)

CMemberPage::CMemberPage(CWnd* pParent /*=NULL*/)
	: CDialog(CMemberPage::IDD, pParent)
{

}

CMemberPage::~CMemberPage()
{
}

void CMemberPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMemberPage, CDialog)
END_MESSAGE_MAP()


// CMemberPage message handlers
