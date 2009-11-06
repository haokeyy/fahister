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


void CBatchMsg::ProcessBatchCommand(HWND hMainWnd, CString szCommands, NameValuePair parameters[])
{
	char* lpstrCmds = szCommands.GetBuffer();

	char *cmds[50];
	int cmdCnt = CUtility::SplitString(lpstrCmds, "\r\n", cmds);
	
	for (int i = 0; i < cmdCnt; i++)
	{
		CString commandLine(cmds[i]);
		
		int j = 0;
		while (parameters != NULL && parameters[j].Name.CompareNoCase("END") != 0 && parameters[j].Value.IsEmpty() == FALSE)
		{
			commandLine.Replace(parameters[j].Name, parameters[j].Value);
		}

		ProcessCommand(hMainWnd, commandLine);
	}
}

UINT TranslateKey(char* key)
{
	CString szKey(key);

	if (szKey.CompareNoCase("ESC")==0)
	{
		return VK_ESCAPE;
	}
	else if (szKey.CompareNoCase("F1")==0)
	{
		return VK_F1;
	}
	else if (szKey.CompareNoCase("F2")==0)
	{
		return VK_F2;
	}
	else if (szKey.CompareNoCase("F3")==0)
	{
		return VK_F3;
	}
	else if (szKey.CompareNoCase("F4")==0)
	{
		return VK_F4;
	}
	else if (szKey.CompareNoCase("F5")==0)
	{
		return VK_F5;
	}
	else if (szKey.CompareNoCase("F6")==0)
	{
		return VK_F6;
	}
	else if (szKey.CompareNoCase("F7")==0)
	{
		return VK_F7;
	}
	else if (szKey.CompareNoCase("F8")==0)
	{
		return VK_F8;
	}
	else if (szKey.CompareNoCase("F9")==0)
	{
		return VK_F9;
	}
	else if (szKey.CompareNoCase("F10")==0)
	{
		return VK_F10;
	}
	else if (szKey.CompareNoCase("F11")==0)
	{
		return VK_F11;
	}
	else if (szKey.CompareNoCase("F12")==0)
	{
		return VK_F12;
	}
	else if (szKey.CompareNoCase("ALT")==0)
	{
		return VK_MENU;
	}
	else if (szKey.CompareNoCase("CTRL")==0)
	{
		return VK_CONTROL;
	}
	else if (szKey.CompareNoCase("SHIFT")==0)
	{
		return VK_SHIFT;
	}
	else if (szKey.CompareNoCase("Enter")==0)
	{
		return VK_RETURN;
	}
	else if (szKey.CompareNoCase("Tab")==0)
	{
		return VK_TAB;
	}
	else
	{
		return szKey.GetAt(0);
	}
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
		else if (argCnt == 3)
		{
			int x = CUtility::GetSafeInt(args[1], 0);
			int y = CUtility::GetSafeInt(args[2], 0);
			CMsgHelper::SM_Click(hMainWnd, x, y);
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
		else if (argCnt == 2)
		{
			CMsgHelper::SM_Text(hMainWnd, args[0], args[1]);
		}
		else if (argCnt == 4)
		{
			int x = CUtility::GetSafeInt(args[1], 0);
			int y = CUtility::GetSafeInt(args[2], 0);
			CMsgHelper::SM_Text(hMainWnd, args[0], x, y, args[3]);
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
	else if (cmd == "sleep")
	{
		if (argCnt == 1)
		{
			int s = CUtility::GetSafeInt(args[0], 100);

			Sleep(s);
		}
	}
}