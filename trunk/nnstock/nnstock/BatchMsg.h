#pragma once

#include "Utility.h"

class CBatchMsg
{
public:
	CBatchMsg();
	~CBatchMsg();

public:
	static void ProcessBatchCommand(HWND hMainWnd, CString commands, NameValuePair parameters[]);

	static void ProcessCommand(HWND hMainWnd, CString commandLine);
};
