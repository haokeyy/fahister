
// tbmessage.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CtbmsgApp:
// �йش����ʵ�֣������ tbmessage.cpp
//

class CtbmsgApp : public CWinApp
{
public:
	CtbmsgApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CtbmsgApp theApp;
