<%@ Page Language="C#" AutoEventWireup="true" EnableViewState="false" CodeBehind="ExecSql.aspx.cs" Inherits="Admin.ExecSql" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>执行SQL</title>
    <link href="./css/Default.css" type="text/css" rel="stylesheet">
</head>
<body>
    <form id="form1" runat="server">
            <table width="100%" border="0">
                <tr>
                    <td vAlign="top" noWrap>
                        <!-- begin of your code here -->
                        <table id="table1" cellSpacing="0" cellPadding="0" width="100%" border="0">
                            <tr>
                                <td>
                                    <asp:label id="lbTitle" runat="server" CssClass="NCWhiteLabel">SQL查询分析器</asp:label>
                                </td>
                            </tr>
                            <tr>
                                <td width="100%">&nbsp;<asp:label id="lbErrorMsg" runat="server" CssClass="ErrorTxt"></asp:label></td>
                            </tr>
                            <tr>
                                <td width="100%">
                                    <!-- The Form Content -->
                                    <TABLE id="Table2" cellSpacing="0" cellPadding="1" border="0">
                                        <tr>
                                            <td>
                                                <asp:Label id="Label1" runat="server">SQL示例</asp:Label></td>
                                        </tr>
                                        <TR>
                                            <TD><asp:ListBox id="ListBox1" runat="server" Height="112px" Width="648px">
                                                    <asp:ListItem Value="select name from sysobjects where type='u'">查找表名:select name from sysobjects where type='u'</asp:ListItem>
                                                    <asp:ListItem Value="select * from [TableName]">查询表:select * from [TableName]</asp:ListItem>
                                                    <asp:ListItem Value="Update [TableName] set [ColumnName]=[ColumnValue] where [ColumnName]=[ColumnValue]">更新表:Update [TableName] set [ColumnName]=[ColumnValue] where [ColumnName]=[ColumnValue]</asp:ListItem>
                                                    <asp:ListItem Value="Insert into [TableName]([CloumnName1],[ColumnName2]) values([ColumnValue1],[ColumnValue2])">插入记录:Insert into [TableName]([CloumnName1],[ColumnName2]) values([ColumnValue1],[ColumnValue2])</asp:ListItem>
                                                    <asp:ListItem Value="Delete from [TableName] where [ColumnName]=[ColumnValue]">删除记录:Delete from [TableName] where [ColumnName]=[ColumnValue]</asp:ListItem>
                                                </asp:ListBox></TD>
                                        </TR>
                                        <tr>
                                            <td>
                                                <asp:Label id="Label2" runat="server">SQL类型</asp:Label>
                                                <asp:DropDownList id="listSqlType" runat="server" Width="80px">
                                                    <asp:ListItem Value="0">请选择</asp:ListItem>
                                                    <asp:ListItem Value="1">查询</asp:ListItem>
                                                    <asp:ListItem Value="2">更新</asp:ListItem>
                                                </asp:DropDownList></td>
                                        </tr>
                                        <TR>
                                            <TD><asp:TextBox id="TextSql" runat="server" Width="100%" Height="104px" TextMode="MultiLine"></asp:TextBox></TD>
                                        </TR>
                                        <tr>
                                            <td vAlign="bottom"><asp:Button id="BtnExec" runat="server" Text="执行" Width="56px" 
                                                    onclick="BtnExec_Click"></asp:Button></td>
                                        </tr>
                                    </TABLE>
                                    <!-- End The Form Content --></td>
                            </tr>
                            <TR>
                                <td width="100%">
                                    <!-- The Grid Content -->
                                    <asp:datagrid id="dgResult" runat="server" CssClass="GridItemSmall" 
                                        BorderWidth="1px" CellPadding="3"
                                        PageSize="50" Width="99%" AllowPaging="True" BackColor="White" 
                                        BorderColor="#999999" BorderStyle="None"
                                        GridLines="Vertical" onpageindexchanged="dgResult_PageIndexChanged">
                                        <SelectedItemStyle Font-Bold="True" ForeColor="White" BackColor="#008A8C"></SelectedItemStyle>
                                        <AlternatingItemStyle BackColor="#DCDCDC"></AlternatingItemStyle>
                                        <ItemStyle ForeColor="Black" BackColor="#EEEEEE"></ItemStyle>
                                        <HeaderStyle Font-Bold="True" ForeColor="White" CssClass="GridHeader" BackColor="#000084"></HeaderStyle>
                                        <FooterStyle ForeColor="Black" BackColor="#CCCCCC"></FooterStyle>
                                        <PagerStyle HorizontalAlign="Right" ForeColor="Black" BackColor="#999999" Mode="NumericPages"></PagerStyle>
                                    </asp:datagrid>
                                    <!-- End The Grid Content --></td>
                            </TR>
                            <tr>
                                <td height="5"></td>
                            </tr>
                        </table>
                        <!-- end of your code here --></td>
                </tr>
            </table>
    </form>
</body>
</html>
