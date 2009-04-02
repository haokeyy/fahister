#include "StdAfx.h"
#include "sendmsgthread.h"
#include "Utility\WindowHelp.h"
#include "Utility\Registry.h"
#include "Controls\explorer_ocx.h"


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
    if (this->m_nMsgType == 4)
    {
        ExecuteLogin(m_szSenderID, m_szMessage);
        return 0;
    }

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
 
    if (this->m_bAddFriend)
    {
        int nCount = GetFriendCount(m_szSenderID); //0; //AddToFriend(m_szSenderID.GetBuffer(0), m_szUserID.GetBuffer(0));

        if (nCount >= 100)
        {
            ::PostMessage(m_hPostWnd,WM_NOTIFY_DELETE_USER, (WPARAM)1, (LPARAM)m_szSenderID.GetBuffer(0));
            //DeleteFriend(strWndTitle.GetBuffer(0));
        }
    }

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
    if (msgType == 3)
    {
        CString strWndTitle("阿里旺旺 - ");
        strWndTitle += strSenderID;

        // 发送指定发送命令
        HWND hMainWnd = FindTopWindow(strWndTitle.GetBuffer(0), "");

        // 先选定根节点，避免总是发送给指定用户
        HWND hTree = FindWndInChildren(hMainWnd, "SysTreeView32", 2);
        HTREEITEM hR = TreeView_GetRoot(hTree);
        TreeView_SelectItem(hTree, hR);

        ::SendMessage(hMainWnd, WM_COMMAND, 0x0000804A, 0);
        Sleep(200);

        // 弹出窗口
        HWND hPopupWnd = ::FindWindow("#32770", "发送消息给指定用户");
        HWND hUserEdit = ::FindWindowEx(hPopupWnd, NULL, "Edit", "");
        HWND hCmb = ::FindWindowEx(hPopupWnd, NULL, "ComboBox", "");
        HWND hBtn = ::FindWindowEx(hPopupWnd, NULL, "Button", "发送(&S)");
        if (hPopupWnd==NULL || hUserEdit==NULL) return;

        // 选阿里巴巴
        ::SendMessage(hCmb, CB_SETCURSEL, 1, 0);
        // 输入好友名
        ::SendMessage(hUserEdit, WM_SETTEXT, NULL, (LPARAM)strUser);
        Sleep(200);

        // 点击发送
        ::SendMessage(hBtn,WM_LBUTTONDOWN,0,0);
        ::SendMessage(hBtn,WM_LBUTTONUP,0,0);
    }

    Sleep(1800);

    // 点击发送后可能弹出三种窗口,1.正常窗口,2.用户不存在,3.请求验证

    // 发送消息窗口，正常情况应该出现该窗口
    HWND hWndMsg = FindTopWindow(strUser, "StandardFrame");

    // 找到正常窗口
    if (hWndMsg != NULL)
    {
        // 添加联系人窗口
        HWND hAddFriendWnd  = NULL;
        if (this->m_bAddFriend)
        {
            // 添加为好友
            ::PostMessage(hWndMsg, WM_COMMAND, 0x00008021, 0);
            Sleep(300);

            hAddFriendWnd = ::FindWindow("#32770", "添加联系人向导");
            
            if (hAddFriendWnd)
            {
                HWND hbtnAdd = ::FindWindowEx(hAddFriendWnd, NULL, "Button", "添加");
                ::SendMessage(hbtnAdd, WM_LBUTTONDOWN,0,0);
                ::SendMessage(hbtnAdd, WM_LBUTTONUP,0,0);
            }
        }
        
        // 消息内容窗口
        HWND hSplitterBar = ::FindWindowEx(hWndMsg, NULL, "SplitterBar", "");
        HWND hHtmlEditor = FindChildWnd(hSplitterBar, "", "HtmlEditor");
        HWND hMsgEdit = FindChildWnd(hHtmlEditor, "", "Internet Explorer_Server");

        // 根据窗口句柄得到IHTMLDocument2接口
        CoInitialize( NULL );
        IHTMLDocument2 *pDoc; 
        DWORD lRes; 

        UINT MSG = RegisterWindowMessage("WM_HTML_GETOBJECT"); 
        SendMessageTimeout(hMsgEdit, MSG, 0, 0, SMTO_ABORTIFHUNG, 1000, &lRes); 
        ObjectFromLresult(lRes, IID_IHTMLDocument2, 0, (void**)&pDoc); 

        // 设置消息内容
        IHTMLElement *pBody;
        pDoc->get_body(&pBody);
        CString szMessage(strMessage);
        BSTR bstrMessage = szMessage.AllocSysString();
        pBody->put_innerHTML(bstrMessage);
        ::SysFreeString(bstrMessage);

        CoUninitialize();

        // 添加任意字符，否则“发送”按钮是灰色不可点击
        ::PostMessage(hMsgEdit, WM_KEYDOWN , '.', 0);
        ::PostMessage(hMsgEdit, WM_KEYUP , '.', 0); 
        
        Sleep(500);

        // 点击发送按钮
        HWND hbtnSend = ::FindWindowEx(hWndMsg, NULL, "StandardButton", "发送");
        ::PostMessage(hbtnSend, WM_LBUTTONDOWN , 0, 0);
        ::PostMessage(hbtnSend, WM_LBUTTONUP , 0, 0); 

        // 关闭窗口
        Sleep(400);
        
        if (this->m_bAddFriend)
        {
            // 有可能添加淘友失败
            ::PostMessage(hAddFriendWnd, WM_CLOSE, 0, 0);
        }

        ::PostMessage(hWndMsg, WM_CLOSE, 0, 0);
    }
    /*
    //else
    //{
        // 验证窗口
        HWND hValidate = ::FindWindow("#32770", "添加到联系人列表");
        if (hValidate)
        {
            // 消息内容窗口
            HWND hMsg2Edit = ::FindWindowEx(hValidate, NULL, "Edit", "");
            ::SendMessage(hMsg2Edit, WM_SETTEXT, NULL, (LPARAM)strMessage2);
            Sleep(1000);
            // 点击发送
            HWND hbtnSend2 = ::FindWindowEx(hValidate, NULL, "Button", "发送(&S)");
            ::SendMessage(hbtnSend2, WM_LBUTTONDOWN,0,0);
            ::SendMessage(hbtnSend2, WM_LBUTTONUP,0,0);
        }
    //}

    // 关闭垃圾窗口 

    // 关闭编辑联系人资料窗口
    HWND hWndRename = ::FindWindow("#32770", "编辑联系人资料");
    while (hWndRename)
    {
        ::PostMessage(hWndRename, WM_CLOSE, 0, 0);

        hWndRename = ::FindWindow("#32770", "编辑联系人资料");
    }

    // 关闭发送窗口或对方发来的消息窗口    
    CString strSendWnd;
    HWND hSendWnd;

     ////5.50.02  与 a0s4o5 的对话
    strSendWnd = "与 ";
    strSendWnd += strSenderID;
    strSendWnd += " 的对话";
    hSendWnd = FindTopWindow(strSendWnd.GetBuffer(0), "#32770");
    while (hSendWnd)
    {
        ::PostMessage(hSendWnd, WM_CLOSE, 0, 0);

        hSendWnd = NULL;//FindTopWindow(strSendWnd.GetBuffer(0), "#32770");
    } 

    ////5.5  与“a0s4o5”的对话
    strSendWnd = "与“";
    strSendWnd += strSenderID;
    strSendWnd += "”的对话";
    hSendWnd = FindTopWindow(strSendWnd.GetBuffer(0), "#32770");
    while (hSendWnd)
    {
        ::PostMessage(hSendWnd, WM_CLOSE, 0, 0);

        hSendWnd = NULL;//FindTopWindow(strSendWnd.GetBuffer(0), "#32770");
    }

    // 5.1
    strSendWnd = "与";
    strSendWnd += strSenderID;
    strSendWnd += "的对话";
    hSendWnd = FindTopWindow(strSendWnd.GetBuffer(0), "#32770");
    while (hSendWnd)
    {
        ::PostMessage(hSendWnd, WM_CLOSE, 0, 0);

        hSendWnd = NULL;//FindTopWindow(strSendWnd.GetBuffer(0), "#32770");
    }
    */


    // 关闭添加联系人成功提示窗口
    HWND hAddSuccess = ::FindWindow("#32770", "被添加为联系人提示");
    while (hAddSuccess)
    {
        ::PostMessage(hAddSuccess, WM_CLOSE, 0, 0);

        hAddSuccess = ::FindWindow("#32770", "被添加为联系人提示");
    }

    // 关闭添加联系人失败提示窗口
    HWND hAddFail = ::FindWindow("#32770", "添加请求被拒绝");
    ::PostMessage(hAddFail, WM_CLOSE, 0, 0);
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