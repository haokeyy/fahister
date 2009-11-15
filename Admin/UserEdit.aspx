<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="UserEdit.aspx.cs" Inherits="Admin.UserEdit" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
        <title>用户编辑</title>
        <link href="./css/Default.css" type="text/css" rel="stylesheet">
    </HEAD>
    <body>
        <form id="form1" runat="server">
            <table>
                <tr>
                    <td align="center">
                        <asp:Label ID="Label5" CssClass="HeaderTitle" runat="server" Text="用户编辑"></asp:Label></td>
                </tr>
                <tr>
                    <td align="center" width="718">
                    </td>
                </tr>
                <tr>
                    <td>
                        <table>
                            <tr>
                                <td>
                                    <asp:Label ID="Label1" runat="server" Text="用户名"></asp:Label></td>
                                <td>
                                    <asp:TextBox ID="tbUserName" runat="server" Width="200px"></asp:TextBox></td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:Label ID="Label2" runat="server" Text="注册产品"></asp:Label></td>
                                <td>
                                    <asp:DropDownList ID="ddlProducts" runat="server" Width="200px"></asp:DropDownList></td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:Label ID="Label3" runat="server" Text="机器码"></asp:Label></td>
                                <td>
                                    <asp:TextBox ID="tbMachineCode" runat="server" Width="200px"></asp:TextBox></td>
                            </tr>
                            <tr>
                                <td>
                                    <asp:Label ID="Label8" runat="server" Text="注册费用"></asp:Label></td>
                                <td>
                                    <asp:TextBox ID="tbPrice" runat="server" Width="200px"></asp:TextBox></td>
                            </tr>
                            <TR>
                                <TD>
                                    <asp:Label id="Label6" runat="server">注册时间</asp:Label></TD>
                                <TD>
                                    <asp:TextBox id="tbRegTime" runat="server" Width="200px"></asp:TextBox></TD>
                            </TR>
                            <tr>
                                <td>
                                    <asp:Label ID="Label4" runat="server" Text="状态"></asp:Label></td>
                                <td>
                                    <asp:DropDownList ID="ddlStatus" runat="server" Width="200px">
                                        <asp:ListItem Value="1">正常</asp:ListItem>
                                        <asp:ListItem Value="0">禁用</asp:ListItem>
                                    </asp:DropDownList></td>
                            </tr>
                            <TR>
                                <TD>
                                    <asp:Label id="Label7" runat="server">备注</asp:Label></TD>
                                <TD>
                                    <asp:TextBox id="tbRemark" runat="server" Width="368px" Height="80px" TextMode="MultiLine"></asp:TextBox></TD>
                            </TR> 
                            <TR>
                                <TD>
                                    <asp:Label id="Label9" runat="server">创建人</asp:Label></TD>
                                <TD>
                                    <asp:Label ID="lbCreator" runat="server" Text=""></asp:Label>
                                    </TD>
                            </TR>
                            <tr>
                                <td>
                                </td>
                                <td>
                                    &nbsp;</td>
                            </tr>
                            <tr>
                                <td>
                                </td>
                                <td>
                                    <asp:Button ID="btnOK" runat="server" Text="确定" Width="45px" 
                                        onclick="btnOK_Click" />
                                    <asp:Button ID="btnCancel" runat="server" Text="返回" Width="45px" 
                                        onclick="btnCancel_Click" /></td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
        </form>
        <script language="javascript">
<!--
 function validateinput()
 {
    if (document.getElementById('tbUserName').value == '')
    {
        alert('请输入用户名');
        return false;
    } 
    if (document.getElementById('tbMachineCode').value == '')
    {
        alert('请输入机器码');
        return false;
    }    
    if (document.getElementById('tbPrice').value == '')
    {
        alert('请输入注册费用');
        return false;
    }     
    if (document.getElementById('tbRegName').value == '')
    {
        alert('请输入注册时间');
        return false;
    }  
    return true;
 }
//-->
        </script>
    </body>
</HTML>