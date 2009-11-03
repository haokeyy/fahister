#include "StdAfx.h"
#include "Utility.h"

int CUtility::GetSafeInt(char* s, int defaultValue)
{
	BOOL isnumber = TRUE;
	char *p = s;
	while (*p)
	{
		if (!isalnum(*p))
		{
			isnumber = FALSE;
		}
		p++;
	}
	if (!isnumber)
	{
		return defaultValue;
	}
	else
	{
		return atoi(s);
	}
}

int CUtility::SplitString(char* s, char* separator, char *substr[])
{
	int cnt = 0;
	int seplen = strlen(separator);

	char *p, *q;
	p = q = s;
	while (*p)
	{
		if (strncmp(p, separator, seplen) == 0)
		{
			*p = 0;
			if (strlen(q) > 0)
			{
				substr[cnt++] = q;
			}

			p += seplen;
			q = p;
		}
		else
		{
			p++;
		}
	}
	if (strlen(q) > 0)
	{
		substr[cnt++] = q;
	} 
	return cnt;
}