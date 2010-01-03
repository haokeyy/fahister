// TestCpp.cpp : Defines the entry point for the console application.
//

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROOT_PATH "c:\\chkcode\\code.txt"

void get_query_string(char* key, char* value)
{
    char q_key[32];
    strcpy(q_key, key);
    strcat(q_key, "=");

    char *query_strings = getenv("QUERY_STRING");//"action=read&uuid=27028683723955855001418151146000";
    char *p1, *p2;
    p1 = strstr(query_strings, q_key);
    if (p1)
    {
        p2 = strstr(p1, "&");
    }
    if (p1 && p2)
    {
        strncpy(value, p1 + strlen(q_key), p2 - p1 - strlen(q_key));
    }
    else if (p1 && !p2)
    {
        strcpy(value, p1 + strlen(q_key));
    }
    else
    {
        strcpy(value, "");
    }
}

void save_uuid(char* uuid)
{
    char fname[64];
    strcpy(fname, ROOT_PATH);
    //strcat(fname, uuid);
    FILE *pFile = fopen(fname,  "w+");
    fprintf(pFile, uuid);
    fclose(pFile);
    
    printf("Content-type:text/html\n\n"); 
    printf("save successfully.");
}

void read_chkcode(char* uuid)
{
    char chkcode[64];
    char fname[64];
    char *p = 0;
    strcpy(fname, ROOT_PATH);
    //strcat(fname, uuid);
    FILE *pFile = fopen(fname,  "r");
    while (fgets(chkcode, 64, pFile))
    {
        if (strlen(chkcode) > strlen(uuid) && strstr(chkcode, uuid))
        {
            p = chkcode + strlen(uuid) + 1;
            break;
        }
    }
    fclose(pFile);

    printf("Content-type:text/html\n\n"); 
    if (p)
    printf("%s", p);
}

void input_chkcode()
{
    char uuid[64];
    char chkcode[64];
    char fname[64];
    char *p = 0;
    strcpy(fname, ROOT_PATH);
    //strcat(fname, uuid);
    FILE *pFile = fopen(fname,  "r");
    fscanf(pFile, "%s", chkcode);
    memset(uuid, 0, 64);
    char *sep = strstr(chkcode, "-");
    if (sep)
    {
        strncpy(uuid, chkcode, sep - chkcode);
    }
    else
    {
        strcpy(uuid, chkcode);
    }
    
    printf("Content-type:text/html\n\n"); 

    printf("<html>");
    printf("<head><title> ‰»Î—È÷§¬Î</title></head>");
    printf("<body>");
    printf("<form method=\"get\" action=\"chkcode.cgi\">");
    printf("<img src=\"http://checkcode.alisoft.com/alisoft/checkcode?sessionID=");
    printf(uuid);
    printf("\">");
    printf("<input type=\"hidden\" name=\"action\" value=\"enter\"/>");
    printf("<input type=\"hidden\" name=\"uuid\" value=\"");
    printf(uuid);
    printf("\"/>");
    printf("<input type=\"text\" name=\"chkcode\"/>");
    printf("<input type=\"submit\" value=\"Submit\"/>");
    printf("</form>");
    printf("</html>");
}

void enter_chkcode(char* uuid, char* chkcode)
{
    char fname[64];
    strcpy(fname, ROOT_PATH);
    //strcat(fname, uuid);
    FILE *pFile = fopen(fname,  "w");
    fprintf(pFile, "%s-%s", uuid, chkcode);
    fclose(pFile);
    
    printf("Content-type:text/html\n\n"); 
    printf("enter successfully.");
}

int main()
{    
    char q_action[64];
    char q_uuid[64];
    char q_chkcode[64];
    memset(q_action, 0, 64);
    memset(q_uuid, 0, 64);
    memset(q_chkcode, 0, 64);
    get_query_string("action", q_action);
    get_query_string("uuid", q_uuid);
    get_query_string("chkcode", q_chkcode);

    if (strcmp(q_action, "save") == 0)
    {
        save_uuid(q_uuid);
    }
    else if (strcmp(q_action, "read") == 0)
    {
        read_chkcode(q_uuid);
    }
    else if (strcmp(q_action, "input") == 0)
    {
        input_chkcode();
    }
    else if (strcmp(q_action, "enter") == 0)
    {
        enter_chkcode(q_uuid, q_chkcode);
    }

	return 0;
}


