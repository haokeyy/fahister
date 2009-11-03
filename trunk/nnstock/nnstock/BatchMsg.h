#pragma once

class CBatchMsg
{
public:
	CBatchMsg();
	~CBatchMsg();

public:
	static void ProcessBatchCommand(HWND hMainWnd, CStringList commands, CMapStringToString parameters);

	static void ProcessCommand(HWND hMainWnd, CString commandLine);
};
