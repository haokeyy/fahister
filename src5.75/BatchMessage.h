// BatchMessage.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CBatchMessageApp:
// �йش����ʵ�֣������ BatchMessage.cpp
//

class CBatchMessageApp : public CWinApp
{
public:
	CBatchMessageApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBatchMessageApp theApp;
