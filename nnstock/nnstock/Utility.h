#pragma once

class CUtility
{
public:
	static int SplitString(char* s, char* separator, char *substr[]);
	
	static int GetSafeInt(char* s, int defaultValue);
};
