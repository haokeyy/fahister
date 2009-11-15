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
    Sleep(1800);

    if (message.AddToFriend)
    {
        AddToFriend();
    }

    // 用户是否已经登录
    CString strWndTitle("-阿里旺旺 2009");
    strWndTitle = message.SendUserId + strWndTitle;
    HWND hMainHwnd = FindTopWindowExactly(strWndTitle.GetBuffer(), "");
    if (hMainHwnd == NULL && message.AutoLogin) // 没有登录
    {
        ExecuteLogin();
    }

    SendOneMsg();

    CString szIndex;
    szIndex.Format("%d", message.nItemIndex);
    ::PostMessage(this->m_hMainWnd, WM_SENDMSG_COMPLETED, (WPARAM)0, (LPARAM)szIndex.GetBuffer());

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

    return 0;
}


typedef int (WINAPI *fn_loadcode)(void* code, long length, char* address, char* password);

typedef char* (WINAPI *fn_Recognition)(int itemNo, int picIn, int length, char* address1, char* address2, int* lppicout, int* lplength, int* cLength);
fn_loadcode _loadcode = NULL;
fn_Recognition _Recognition = NULL;
static BOOL hasInit = FALSE;

CString Recognize(CString szUrl)
{
	if (hasInit == FALSE || _loadcode == NULL || _Recognition == NULL)
	{
		CString szFileName;
		DWORD nProcID = ::GetCurrentProcessId();
		GetProcessNameByProcessID(nProcID, szFileName);

		int index = szFileName.ReverseFind('\\');
		CString chkFilePath = szFileName.Left(index) + "\\4383.fc";

		HINSTANCE hInstance = ::LoadLibrary("ycode.dll");
		
		_loadcode = (fn_loadcode)::GetProcAddress(hInstance, "loadcode");
		_Recognition = (fn_Recognition)::GetProcAddress(hInstance, "Recognition");   

		char *path = chkFilePath.GetBuffer();
		
		CFile file(path, CFile::modeRead|CFile::typeBinary);
		int len = file.GetLength();
		BYTE *buf = (BYTE*)malloc(len);
		file.Read(buf, len);

		_loadcode(buf, len, "", "cv2222");

		hasInit = TRUE;
	}
	
	int n1 = 0, n2 = 0, n3 = 0;
	char* address = szUrl.GetBuffer();
	char* code =  _Recognition(1, 0, 0, address, "", &n1, &n2, &n3);

	CString szResult(code);
	
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

        // 根据窗口句柄得到IHTMLDocument2接口
        //IHTMLDocument2 *pDoc; 
        //DWORD lRes; 

        //UINT MSG = RegisterWindowMessage("WM_HTML_GETOBJECT"); 
        //SendMessageTimeout(hMsgEdit, MSG, 0, 0, SMTO_ABORTIFHUNG, 1000, &lRes); 
        //ObjectFromLresult(lRes, IID_IHTMLDocument2, 0, (void**)&pDoc); 

        //if (pDoc != NULL)
        //{
        //    // 设置消息内容
        //    IHTMLElement *pBody;
        //    pDoc->get_body(&pBody);
        //    CString szMessage(message.MessageHtml);
        //    BSTR bstrMessage = szMessage.AllocSysString();
        //    pBody->put_innerHTML(bstrMessage);
        //    ::SysFreeString(bstrMessage);
        //}
		
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

        // 检查校验码
        Sleep(1000);
        // 校验码窗口
        HWND hValidCodeWnd = FindTopWindow("阿里旺旺 - 安全验证", "#32770");
        if (hValidCodeWnd)
        {
            HWND hValidCodeExp = FindChildWnd(hValidCodeWnd, "", "Internet Explorer_Server");

			IHTMLDocument2 *pDoc; 
			DWORD lRes; 

			UINT MSG = RegisterWindowMessage("WM_HTML_GETOBJECT"); 
			SendMessageTimeout(hValidCodeExp, MSG, 0, 0, SMTO_ABORTIFHUNG, 1000, &lRes); 
			ObjectFromLresult(lRes, IID_IHTMLDocument2, 0, (void**)&pDoc); 

            if (pDoc)
            {
                IHTMLElement* pChkBody;
				HRESULT re = pDoc->get_body(&pChkBody);
                //GetDocumentBody(pChkDoc, &pChkBody);

				if (pChkBody)
				{
					BSTR bstrHtml;
					pChkBody->get_innerHTML(&bstrHtml);
					CString szHtml(bstrHtml);
					CString szUrl = szHtml.Mid(29, 89);

					// 识别
					CString checkCode = Recognize(szUrl);
	            
					HWND hValidCodeEdit = FindChildWnd(hValidCodeWnd, "", "EditComponent");
					::SendMessage(hValidCodeEdit, WM_SETTEXT, 0, (LPARAM)checkCode.GetBuffer());

					HWND hValidCodeOK = FindChildWnd(hValidCodeWnd, "确定", "StandardButton");
					::PostMessage(hValidCodeOK, WM_LBUTTONDOWN , 0, 0);
					::PostMessage(hValidCodeOK, WM_LBUTTONUP , 0, 0); 
				}
            }
        }

        // 关闭窗口
        Sleep(400);
        ::PostMessage(hWndMsg, WM_CLOSE, 0, 0);        
    }

    return 0;
}

UINT CMessageSender::ExecuteLogin()
{
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
    CString strWndTitle("-阿里旺旺 2009");
    strWndTitle = senderId + strWndTitle;
    HWND hMainHwnd = FindTopWindowExactly(strWndTitle.GetBuffer(), "StandardFrame");

    return hMainHwnd != NULL;
}