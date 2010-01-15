#include "StdAfx.h"
#include "sendmsgthread.h"
#include "Utility\WindowHelp.h"
#include "Utility\Registry.h"


CSendMsgThread::CSendMsgThread()
{
}

CSendMsgThread::~CSendMsgThread()
{
}

void CSendMsgThread::Init(HWND hwnd, BOOL bAddFriend)
{
    m_hPostWnd = hwnd;
    m_bAddFriend = bAddFriend;
}

void CSendMsgThread::SendMsg(CString szSenderID, CString szUserID,CString szMessage, CString szMessage2, int nMsgType)
{
    m_szSenderID = szSenderID;
    m_szUserID = szUserID;
    m_szMessage = szMessage;
    m_szMessage2 = szMessage2;
    m_nMsgType = nMsgType;

    AfxBeginThread(SendMsgWorkerThread, this);
}


UINT CSendMsgThread::SendMsgWorkerThread(LPVOID pvThread)
{
    CSendMsgThread* pSendMsgThread = (CSendMsgThread*)pvThread;

    return pSendMsgThread->ExecuteSendMsg();
}

UINT CSendMsgThread::ExecuteSendMsg()
{
    //if (this->m_nMsgType == 4)
    //{
    //    ExecuteLogin(m_szSenderID, m_szMessage);
    //    return 0;
    //}

    LPTSTR strMessage2 = NULL;
    if (m_szMessage2.GetLength() > 30)
    {
        strMessage2 = m_szMessage2.GetBuffer(29);
    }
    else
    {
        strMessage2 = m_szMessage2.GetBuffer(0);
    }

    //CString szSendWndTitle = "与" + m_szSenderID + "的对话";

    SendOneMessage(m_szSenderID.GetBuffer(0), m_szMessage.GetBuffer(0), strMessage2, m_szUserID.GetBuffer(0), m_nMsgType);
 
    BOOL bResult = TRUE;
    LPTSTR szUserID = m_szUserID.GetBuffer(0);

    ::PostMessage(m_hPostWnd,WM_SENDMSGCOMPLETED, (WPARAM)bResult, (LPARAM)szUserID);
 
    //if (this->m_bAddFriend)
    //{
    //    int nCount = GetFriendCount(m_szSenderID); //0; //AddToFriend(m_szSenderID.GetBuffer(0), m_szUserID.GetBuffer(0));

    //    if (nCount >= 100)
    //    {
    //        ::PostMessage(m_hPostWnd,WM_NOTIFY_DELETE_USER, (WPARAM)1, (LPARAM)m_szSenderID.GetBuffer(0));
    //        //DeleteFriend(strWndTitle.GetBuffer(0));
    //    }
    //}

    return 0;
}

UINT CSendMsgThread::ExecuteLogin(CString uid, CString pwd)
{
    CString szExePath;// = "C:\\Program Files\\Alisoft\\WangWang\\WangWang.exe";

    CRegistry reg(HKEY_CLASSES_ROOT);
	BOOL bReturn;
	bReturn=reg.Open ("Alitalk\\Shell\\Open\\Command");
    bReturn=reg.Read ("",szExePath);
    reg.Close();

    szExePath = szExePath.Left(szExePath.GetLength()-3);
    WinExec(szExePath, SW_SHOWNORMAL);
    Sleep(1000);

    CString strWndTitle("阿里旺旺");
    HWND hWnd = FindTopWindow(strWndTitle.GetBuffer(0), "");
    
    HWND hUserCmb = FindWndInChildren(hWnd, "ComboBox", 2);
    HWND hPwdEdit = ::GetWindow(hUserCmb, GW_HWNDNEXT);
    HWND hHideLogin = FindWndInChildren(hWnd, "隐身登录", 1);
    HWND hLoginBtn = ::GetWindow(hHideLogin, GW_HWNDNEXT);

    ::SendMessage(hUserCmb, WM_SETTEXT, NULL, (LPARAM)uid.GetBuffer(0));
    ::SendMessage(hPwdEdit, WM_SETTEXT, NULL, (LPARAM)pwd.GetBuffer(0));
    ::SendMessage(hLoginBtn, WM_LBUTTONDOWN,0,0);
    ::SendMessage(hLoginBtn, WM_LBUTTONUP,0,0); 

    return 0;
}

int CSendMsgThread::AddToFriend(LPTSTR szSenderID, LPTSTR strUser)
{    
    CString strWndTitle("淘宝旺旺 - ");
    
    strWndTitle += szSenderID;

    HWND hWnd = FindTopWindow(strWndTitle.GetBuffer(0), "");

    // 由于有两个"添加淘友"的按钮，需要找两次
    HWND hBtn1 = FindWndInChildren(hWnd, "我想...", 1);
    HWND hBtnWnd = FindWndInChildren(hBtn1, "添加淘友", 1);
    
    ::PostMessage(hBtnWnd,WM_LBUTTONDOWN,0,0);
    ::SendMessage(hBtnWnd,WM_LBUTTONDOWN,0,0);
    ::PostMessage(hBtnWnd,WM_LBUTTONUP,0,0);
    ::SendMessage(hBtnWnd,WM_LBUTTONUP,0,0);

    Sleep(800);

    HWND hPopupWnd = ::FindWindow("#32770", "淘宝旺旺--添加淘友");
    HWND hEdit = ::FindWindowEx(hPopupWnd, NULL, "Edit", "");
    
    if (hPopupWnd==NULL || hEdit==NULL)return 0;

    ::SendMessage(hEdit, WM_SETTEXT, NULL, (LPARAM)strUser);

    HWND hbtnSend = ::FindWindowEx(hPopupWnd, NULL, "BUTTON", "");
    ::SendMessage(hbtnSend, WM_LBUTTONDOWN,0,0);
    ::SendMessage(hbtnSend, WM_LBUTTONUP,0,0);  

    Sleep(400);

    HWND hWndRename = ::FindWindow("#32770", "编辑淘友信息");
    while (hWndRename)
    {
        ::SendMessage(hWndRename, WM_CLOSE, 0, 0);
        hWndRename = ::FindWindow("#32770", "编辑淘友信息");
    }
    ::SendMessage(hPopupWnd, WM_CLOSE, 0, 0);
    Sleep(500);
    
    int n = GetFriendCount(szSenderID);
    return n;
}

void CSendMsgThread::DeleteFriend(LPTSTR szSenderID)
{
    m_szSenderID = szSenderID;
    AfxBeginThread(DeleteFriendThread, this);    
}

UINT CSendMsgThread::DeleteFriendThread(LPVOID pvThread)
{
    CSendMsgThread* pThread = (CSendMsgThread*)pvThread;

    CString szSender = pThread->m_szSenderID;

    DeleteAllFriend(szSender);

    ::PostMessage(pThread->m_hPostWnd,WM_NOTIFY_DELETE_USER, (WPARAM)0, (LPARAM)szSender.GetBuffer(0));

    return 0;
}

void DeleteAllFriend(CString szSenderID)
{
    CString strWndTitle("阿里旺旺 - ");
    
    strWndTitle += szSenderID;

    HWND hWnd = FindTopWindow(strWndTitle.GetBuffer(0), "");
    // 由于有两个"添加淘友"的按钮，需要找两次
    HWND hTree = FindWndInChildren(hWnd, "SysTreeView32", 2);
    
    HTREEITEM hR = TreeView_GetRoot(hTree);
    HTREEITEM hM = TreeView_GetChild(hTree, hR);
    while (hM)
    {
        HTREEITEM hM2 = TreeView_GetNextSibling(hTree, hM);

        TreeView_SelectItem(hTree, hM);

        ::PostMessage(hWnd,WM_COMMAND,0x00008073, 0x00000000);

        Sleep(1000);
             
        // 确定
        HWND hPopupWnd = ::FindWindow("#32770", "阿里旺旺");
        HWND hBtnOK = ::FindWindowEx(hPopupWnd, NULL, "Button", "是(&Y)");
        
        ::SendMessage(hBtnOK, WM_LBUTTONDOWN, 0, 0);
        ::SendMessage(hBtnOK, WM_LBUTTONUP, 0, 0);  
        
        Sleep(1000);
        hM = hM2;
    }
}

void CSendMsgThread::SendOneMessage(LPTSTR strSenderID, LPTSTR strMessage, LPTSTR strMessage2, LPTSTR strUser, int msgType)
{
    Sleep(1000);

	HWND hWndMsg = NULL;

	// 发送消息窗口，正常情况应该出现该窗口
	hWndMsg = FindTopWindow("与来自拍拍网的", "#32770");
    if (hWndMsg == NULL)
    {
        hWndMsg = FindTopWindow("TCP直连新TCP中转", "#32770");
    }

	// 找到正常窗口
	if (hWndMsg != NULL)
	{        
		// 消息内容窗口
		HWND hWnd1 = FindChildWnd(hWndMsg, "", "RichEdit20A");
        HWND hWnd2 = ::GetWindow(hWnd1, GW_HWNDNEXT);
        HWND hMsgEdit = FindChildWnd(hWnd2, "", "RichEdit20A");

        int size = 0;        
        LPWORD p = StringToWords(strMessage, size);
        WORD pp[1024];
        memset(pp, 0, 1024*sizeof(WORD));
        size = size > 1024 ? 1024 : size;
        memcpy(pp, p, size*sizeof(WORD));

        for(int i = 0; i < size; i++)
        {
            ::SendMessage(hMsgEdit, WM_IME_CHAR, pp[i], 0);
        }
	    
		Sleep(1000);
        HWND hBtnSend = FindChildWnd(hWndMsg, "发送(S)", "Button");
		// 发送
		::PostMessage(hBtnSend, WM_LBUTTONDOWN, 0, 0);
		::PostMessage(hBtnSend, WM_LBUTTONUP, 0, 0); 

		// 关闭窗口
		Sleep(1000);

		::PostMessage(hWndMsg, WM_CLOSE, 0, 0);

		HWND hWndPrompt = FindTopWindow("有消息正在发送", "#32770");
		if (hWndPrompt != NULL)
		{
			HWND hBtnYes = FindChildWnd(hWndPrompt, "是(&Y)", "Button");
			// 发送
			::PostMessage(hBtnYes, WM_LBUTTONDOWN, 0, 0);
			::PostMessage(hBtnYes, WM_LBUTTONUP, 0, 0); 
		}
	}

	// 关闭多余窗口
	/*hWndMsg = FindTopWindow("(买卖通IM商友) -对话", "#32770");
	while(hWndMsg)
	{
		::PostMessage(hWndMsg, WM_CLOSE, 0, 0);
		hWndMsg = FindTopWindow("(买卖通IM商友) -对话", "#32770");
	}*/
}

int GetFriendCount(CString szSenderID)
{
    //CString strWndTitle("阿里旺旺 - ");    
    //strWndTitle += strSender;

    //HWND hWnd = FindTopWindow(strWndTitle.GetBuffer(0), "");
    //HWND hChild = FindChildWnd(hWnd, strSender.GetBuffer(0), "Static");
    //if (hChild)
    //{
    //    char strTemp[256];
    //    ::GetWindowText(hChild, strTemp, 255);
    //    CString szText(strTemp);
    //    int s = szText.Find('/', 0)+1;
    //    int e = szText.Find(')', s);
    //    CString ss = szText.Mid(s, e-s);

    //    int n = atoi(ss.GetBuffer(0));
    //    return n;
    //}

    int i = 0;

    CString strWndTitle("阿里旺旺 - ");
    
    strWndTitle += szSenderID;

    HWND hWnd = FindTopWindow(strWndTitle.GetBuffer(0), "");
    // 由于有两个"添加淘友"的按钮，需要找两次
    HWND hTree = FindWndInChildren(hWnd, "SysTreeView32", 2);
    
    HTREEITEM hR = TreeView_GetRoot(hTree);
    HTREEITEM hM = TreeView_GetChild(hTree, hR);
    while (hM)
    {
        hM = TreeView_GetNextSibling(hTree, hM);
        i++;
    }

    return i;
}