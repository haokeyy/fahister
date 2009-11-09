#pragma once

struct NameValuePair
{
public:
    CString Name;
    CString Value;
public:
    NameValuePair(CString strName, CString strValue)
    {
        Value   = strValue;
        Name    = strName;
    }
};

class CUtility
{
public:
	static int SplitString(char* s, char* separator, char *substr[]);
	
	static int GetSafeInt(char* s, int defaultValue);

    static CString CUtility::GetPageDirect(CString rAddress);
};
