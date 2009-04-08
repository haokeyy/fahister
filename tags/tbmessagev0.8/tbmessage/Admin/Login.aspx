<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Login.aspx.cs" Inherits="Admin.Login" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>Login</title>
    <LINK href="./css/Default.css" type="text/css" rel="stylesheet">
</head>
<body>
    <form id="form1" runat="server">
<TABLE id="Table1" style="BORDER-LEFT-COLOR: lightgrey; BORDER-BOTTOM-COLOR: lightgrey; BORDER-TOP-STYLE: solid; BORDER-TOP-COLOR: lightgrey; BORDER-RIGHT-STYLE: solid; BORDER-LEFT-STYLE: solid; POSITION: relative; TOP: 80px; BORDER-RIGHT-COLOR: lightgrey; BORDER-BOTTOM-STYLE: solid"
                cellSpacing="0" cellPadding="1" width="320" align="center" border="0">
                <TR>
                    <TD bgColor="#000099" height="30" align="center"><FONT face="宋体" color="#ffffff">用户登录</FONT>
                    </TD>
                </TR>
                <TR>
                    <TD height="5"></TD>
                </TR>
                <TR>
                    <TD>
                        <TABLE id="TableMain" cellSpacing="2" cellPadding="2" width="363" align="center" border="0">
                            <TR>
                                <TD align="right" width="123">用户名：</TD>
                                <TD><INPUT id="tbUserID" type="text" size="16" name="tbUserID" runat="server"></TD>
                            </TR>
                            <TR>
                                <TD align="right">密码：</TD>
                                <TD>
                                    <asp:TextBox id="tbPassword" runat="server" TextMode="Password" Width="128px"></asp:TextBox></TD>
                            </TR>
                            <TR>
                                <TD align="right"><FONT face="宋体">验证码：</FONT></TD>
                                <TD>
                                    <asp:TextBox id="tbValidCode" runat="server" Width="88px" AUTOCOMPLETE="off"></asp:TextBox><IMG src="CheckCode.aspx">
                                </TD>
                            </TR>
                            <TR>
                                <TD><FONT face="宋体"></FONT></TD>
                                <TD>
                                    <asp:Button id="btnLogin" runat="server" Width="64px" Text="登录" 
                                        onclick="btnLogin_Click"></asp:Button></TD>
                            </TR>
                            <TR>
                                <TD></TD>
                                <TD>
                                    <asp:Label id="lbErrorMsg" runat="server" ForeColor="Red"></asp:Label></TD>
                            </TR>
                        </TABLE>
                    </TD>
                </TR>
            </TABLE>
    </form>
</body>
</html>
