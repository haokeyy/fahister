#include "StdAfx.h"
#include "BatchMsg.h"
#include "Utility.h"
#include "MsgHelper.h"

CBatchMsg::CBatchMsg()
{
}

CBatchMsg::~CBatchMsg()
{
}


void CBatchMsg::ProcessBatchCommand(HWND hMainWnd, CStringList commands, CMapStringToString parameters)
{
	POSITION pos;
	for (pos = commands.GetHeadPosition(); pos != NULL;)
	{
		CString commandLine = commands.GetNext(pos);

		for (POSITION parPos = parameters.GetStartPosition(); parPos != NULL;)
		{
			CString strKey, strValue;
			parameters.GetNextAssoc(parPos, strKey, strValue);
			commandLine.Replace(strKey, strValue);
		}

		ProcessCommand(hMainWnd, commandLine);
	}
}

UINT TranslateKey(char* key)
{
	return VK_RETURN;
}

void CBatchMsg::ProcessCommand(HWND hMainWnd, CString commandLine)
{
	int sepCmd = commandLine.Find(" ");
	CString cmd = commandLine.Left(sepCmd);
	CString parameter = commandLine.Mid(sepCmd).Trim();
	char* strpar = parameter.GetBuffer();

	char *args[10];
	int argCnt = CUtility::SplitString(strpar, ",", args);
	
	if (cmd.CompareNoCase("click") == 0)
	{
		if (argCnt == 2)
		{
			CMsgHelper::SM_Click(hMainWnd, args[0], args[1]);
		}
		else if (argCnt == 4)
		{
			int x = CUtility::GetSafeInt(args[2], 0);
			int y = CUtility::GetSafeInt(args[3], 0);
			CMsgHelper::SM_Click(hMainWnd, args[0], args[1], x, y);
		}
	}
	else if (cmd == "input")
	{
		if (argCnt == 1)
		{
			CMsgHelper::SM_Text(hMainWnd, args[0]);
		}
		else if (argCnt == 3)
		{
			CMsgHelper::SM_Text(hMainWnd, args[0], args[1], args[2]);
		}
		else if (argCnt == 5)
		{
			int x = CUtility::GetSafeInt(args[2], 0);
			int y = CUtility::GetSafeInt(args[3], 0);
			CMsgHelper::SM_Text(hMainWnd, args[0], args[1], x, y, args[4]);
		}
	}
	else if (cmd == "key")
	{
		if (argCnt == 1)
		{
			UINT key = TranslateKey(args[0]);

			CMsgHelper::SM_PressKey(hMainWnd, key);
		}
		else if (argCnt == 2)
		{
			UINT key1 = TranslateKey(args[0]);
			UINT key2 = TranslateKey(args[1]);
			CMsgHelper::SM_PressKey(hMainWnd, key1, key2);
		}
	}
}