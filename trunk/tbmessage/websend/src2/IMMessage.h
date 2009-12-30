#pragma once

struct CIMMessage
{
    int     nMsgType; //0:µÇÂ¼, 1:·¢ËÍÏûÏ¢
    HWND    hWndSendFrom;
    char szSendFrom[256];
    char szPassword[256];
    char szSendTo[256];
    char szMessage[1024];
};
