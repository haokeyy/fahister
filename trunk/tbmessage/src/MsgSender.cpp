#include "StdAfx.h"
#include "MsgSender.h"
#include <Oleacc.h>
#include ".\Libraries\WindowHelp.h"
#include ".\Libraries\DHtmlHelper.h"

CMessageSender::CMessageSender(HWND hMainWnd)
{
    m_hMainWnd = hMainWnd;
}

CMessageSender::~CMessageSender()
{
}

void CMessageSender::SendMsg(CInstantMessage msg)
{
    this->message = msg;

    AfxBeginThread(SendMsgWorkerThread, this);
}

UINT SendMsgWorkerThread(LPVOID pvThread)
{
    CMessageSender* pSendMsgThread = (CMessageSender*)pvThread;

    return pSendMsgThread->ExecuteSendMsg();
}

UINT CMessageSender::ExecuteSendMsg()
{
    Sleep(2400);

    // 添加为好友的提示框，有些用户要求必须添加为好友
    HWND hMustAddFriend = ::FindWindow("#32770", "添加好友信息");
    if (hMustAddFriend)
    {
        ::PostMessage(hMustAddFriend, WM_CLOSE, 0, 0);
    }
    else
    {
        SendOneMsg();
    }

    CString szIndex;
    szIndex.Format("%d", message.nItemIndex);
    ::PostMessage(this->m_hMainWnd, WM_SENDMSG_COMPLETED, (WPARAM)0, (LPARAM)szIndex.GetBuffer());

    // 验证码不对，则直接关掉
    HWND hValidCodeWnd = FindTopWindowExactly("阿里旺旺 - 安全验证", "#32770");
    if (hValidCodeWnd)
    {
        ::PostMessage(hValidCodeWnd, WM_CLOSE, 0, 0);
    }

    return 0;
}


CString chkFilePath;
CString szLastUrl;
static BOOL hasInit = FALSE;

CString Recognize(CString szUrl)
{
    CString szResult("");
    if (hasInit == FALSE || chkFilePath.IsEmpty())
	{
		CString szFileName;
		DWORD nProcID = ::GetCurrentProcessId();
		GetProcessNameByProcessID(nProcID, szFileName);

		int index = szFileName.ReverseFind('\\');
		chkFilePath = szFileName.Left(index) + "\\chkcode.txt";

		hasInit = TRUE;
	}
	
    CStdioFile file;
    if (szLastUrl != szUrl)
    {
        if (file.Open(chkFilePath, CFile::modeCreate | CFile::modeReadWrite))
        {
            file.WriteString(szUrl);
            szLastUrl = szUrl;
            file.Close();
        }
    }

	CString szLine("");
    if (file.Open(chkFilePath, CFile::modeRead))
    {
        file.ReadString(szLine);
        if (szLine.GetLength() > szUrl.GetLength() + 1)
        {
            szResult = szLine.Mid(szUrl.GetLength() + 1);
        }
        file.Close();
    }
	
	return szResult;
}

UINT CMessageSender::SendOneMsg()
{    
    // 发送消息窗口，正常情况应该出现该窗口
    HWND hWndMsg = FindTopWindow(message.SendUserId.GetBuffer(), "StandardFrame");

    // 找到正常窗口
    if (hWndMsg != NULL)
    {
        // 消息内容窗口
        HWND hSplitterBar = ::FindWindowEx(hWndMsg, NULL, "SplitterBar", "");
        //HWND hHtmlEditor = FindChildWnd(hSplitterBar, "", "HtmlEditor");
        HWND hMsgEdit = FindChildWnd(hSplitterBar, "", "RichEditComponent");
		
		CString szMessage(message.MessageHtml);
		::SendMessage(hMsgEdit, WM_SETTEXT , 0, (LPARAM)szMessage.GetBuffer());

        // 添加任意字符，否则“发送”按钮是灰色不可点击
        ::PostMessage(hMsgEdit, WM_KEYDOWN , '.', 0);
        ::PostMessage(hMsgEdit, WM_KEYUP , '.', 0); 
        
        Sleep(100);

        // 点击发送按钮
        HWND hbtnSend = ::FindWindowEx(hWndMsg, NULL, "StandardButton", "发送");
        ::PostMessage(hbtnSend, WM_LBUTTONDOWN , 0, 0);
        ::PostMessage(hbtnSend, WM_LBUTTONUP , 0, 0); 

         /*
        // 检查校验码
        Sleep(2000);
        // 校验码窗口
        HWND hValidCodeWnd = FindTopWindowExactly("阿里旺旺 - 安全验证", "#32770");
        if (hValidCodeWnd)
        {
            char lpstrChkCode[32];
            memset(lpstrChkCode,0, 32);
	        HWND hValidCodeEdit = FindChildWnd(hValidCodeWnd, "", "EditComponent");
            ::SendMessage(hValidCodeEdit, WM_GETTEXT, 32, (LPARAM)lpstrChkCode);
            while (hValidCodeWnd && hValidCodeEdit && strlen(lpstrChkCode) < 5)
            {
                Sleep(1000);
                hValidCodeWnd = FindTopWindowExactly("阿里旺旺 - 安全验证", "#32770");
                hValidCodeEdit = FindChildWnd(hValidCodeWnd, "", "EditComponent");
                ::SendMessage(hValidCodeEdit, WM_GETTEXT, 32, (LPARAM)lpstrChkCode);
            }

	        HWND hValidCodeOK = FindChildWnd(hValidCodeWnd, "确定", "StandardButton");
	        ::PostMessage(hValidCodeOK, WM_LBUTTONDOWN , 0, 0);
	        ::PostMessage(hValidCodeOK, WM_LBUTTONUP , 0, 0); 

           
            HWND hValidCodeExp = FindChildWnd(hValidCodeWnd, "", "Internet Explorer_Server");

            CoInitialize( NULL );
			IHTMLDocument2 *pDoc; 
			DWORD lRes; 

			UINT MSG = RegisterWindowMessage("WM_HTML_GETOBJECT"); 
			SendMessageTimeout(hValidCodeExp, MSG, 0, 0, SMTO_ABORTIFHUNG, 1000, &lRes); 
			ObjectFromLresult(lRes, IID_IHTMLDocument2, 0, (void**)&pDoc); 

            if (pDoc)
            {
                IHTMLElement* pChkBody;
				HRESULT re = pDoc->get_body(&pChkBody);

				if (pChkBody)
				{
					BSTR bstrHtml;
					pChkBody->get_innerHTML(&bstrHtml);
					CString szHtml(bstrHtml);
                    if (!szHtml.IsEmpty())
                    {
                        CString szUrl("");
                        int nStartUrl = 0, nEndUrl =0;
                        nStartUrl = szHtml.Find("http://checkcode.alisoft.com/alisoft/checkcode?sessionID=");
                        if (nStartUrl > 0)
                        {
                            nEndUrl = szHtml.Find("\"", nStartUrl);
                        }
                        if (nEndUrl > nStartUrl && nStartUrl >= 0)
                        {
                            szUrl = szHtml.Mid(nStartUrl, nEndUrl - nStartUrl);
                        }


                        if (!szUrl.IsEmpty())
                        {
					        // 识别
					        CString checkCode = Recognize(szUrl);
        	            
                            while (checkCode.IsEmpty())
                            {
                                Sleep(1000);
                                checkCode = Recognize(szUrl);
                            }

					        HWND hValidCodeEdit = FindChildWnd(hValidCodeWnd, "", "EditComponent");
					        ::SendMessage(hValidCodeEdit, WM_SETTEXT, 0, (LPARAM)checkCode.GetBuffer());

					        HWND hValidCodeOK = FindChildWnd(hValidCodeWnd, "确定", "StandardButton");
					        ::PostMessage(hValidCodeOK, WM_LBUTTONDOWN , 0, 0);
					        ::PostMessage(hValidCodeOK, WM_LBUTTONUP , 0, 0); 
                        }
                    }
				}
            }
            CoUninitialize();
            
        }*/
        

        // 关闭窗口
        Sleep(400);
        ::PostMessage(hWndMsg, WM_CLOSE, 0, 0);        
    }

    return 0;
}

UINT CMessageSender::ExecuteLogin()
{
    CString strWndTitle("阿里旺旺");
    HWND hWnd = FindTopWindow(strWndTitle.GetBuffer(0), "");
    
    HWND hTypeEdit = FindWndInChildren(hWnd, "EditComponent", 2);
    HWND hTypeCmb = ::GetWindow(hTypeEdit, GW_HWNDNEXT);
    HWND hUidEdit = ::GetWindow(hTypeCmb, GW_HWNDNEXT); //user name
    //HWND hNext = ::GetWindow(hUidEdit, GW_HWNDNEXT);
    //hNext = ::GetWindow(hNext, GW_HWNDNEXT);
    //hNext = ::GetWindow(hNext, GW_HWNDNEXT);
    //hNext = ::GetWindow(hNext, GW_CHILD);
    //HWND hPwdEdit = ::GetWindow(hNext, GW_CHILD);

    HWND hLoginBtn = FindWndInChildren(hWnd, "登 录", 1);

    ::SendMessage(hUidEdit, WM_SETTEXT, NULL, (LPARAM)message.SendUserId.GetBuffer(0));
    //::SendMessage(hPwdEdit, WM_SETTEXT, NULL, (LPARAM)pwd.GetBuffer(0));
    ::SendMessage(hLoginBtn, WM_LBUTTONDOWN,0,0);
    ::SendMessage(hLoginBtn, WM_LBUTTONUP,0,0); 

    return 0;
}

UINT CMessageSender::AddToFriend()
{
    return 0;
}

UINT CMessageSender::DeleteAllFriend()
{
    return 0;
}

BOOL CMessageSender::UserIsLogined(CString senderId)
{
    // 用户是否已经登录
    CString strWndTitle("-阿里旺旺2009");
    strWndTitle = senderId + strWndTitle;
    HWND hMainHwnd = FindTopWindowExactly(strWndTitle.GetBuffer(), "StandardFrame");

    return hMainHwnd != NULL;
}