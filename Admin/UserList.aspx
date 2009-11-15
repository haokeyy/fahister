<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="UserList.aspx.cs" Inherits="Admin.UserList" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
        <title>用户管理</title>
        <link href="./css/Default.css" type="text/css" rel="stylesheet">
</head>
<body>
    <form id="form1" runat="server">
<table width="100%">
                <tr>
                    <td align="center" width="700"><asp:label id="Label5" runat="server" Text="用户管理" CssClass="HeaderTitle"></asp:label></td>
                    <td><a href="Login.aspx?action=logout">注销</a></td>
                </tr>
                <tr>
                    <td align="center" width="100%" colSpan="2"><FONT face="宋体"></FONT></td>
                </tr>
                <tr>
                    <td width="700" colSpan="2">
                        <table width="700">
                            <tr>
                                <td><asp:label id="Label1" runat="server" Text="用户名"></asp:label></td>
                                <td><asp:textbox id="tbUserName" runat="server"></asp:textbox></td>
                                <td><asp:label id="Label2" runat="server" Text="注册产品"></asp:label></td>
                                <td><asp:dropdownlist id="ddlProducts" runat="server" Width="180px"></asp:dropdownlist></td>
                                <td>&nbsp;</td>
                                <td></td>
                            </tr>
                            <tr>
                                <td><asp:label id="Label3" runat="server" Text="机器码"></asp:label></td>
                                <td><asp:textbox id="tbMachineCode" runat="server"></asp:textbox></td>
                                <td><asp:label id="Label4" runat="server" Text="状态"></asp:label></td>
                                <td><asp:dropdownlist id="ddlStatus" runat="server" Width="180px">
                                        <asp:ListItem Value="1000">全部</asp:ListItem>
                                        <asp:ListItem Value="1">正常</asp:ListItem>
                                        <asp:ListItem Value="0">禁用</asp:ListItem>
                                    </asp:dropdownlist></td>
                                <td><asp:button id="btnSearch" runat="server" Text="查找" Width="45px" 
                                        onclick="btnSearch_Click"></asp:button></td>
                                <td>
                                    <asp:Button ID="btnAdd" runat="server" onclick="btnAdd_Click" Text="新增" />
                                </td>
                            </tr>
                            <tr>
                                <td></td>
                                <td></td>
                                <td></td>
                                <td></td>
                                <td></td>
                                <td></td>
                            </tr>
                        </table>
                    </td>
                </tr>
                <tr>
                    <td width="100%" colSpan="2"><asp:datagrid id="DataGrid1" runat="server" 
                            width="100%" PageSize="100" AutoGenerateColumns="False"
                            AllowPaging="True" GridLines="Vertical" CellPadding="3" BorderWidth="1px" 
                            BorderStyle="None" BorderColor="#999999"
                            BackColor="White" onitemcommand="DataGrid1_ItemCommand" 
                            onitemcreated="DataGrid1_ItemCreated" 
                            onpageindexchanged="DataGrid1_PageIndexChanged">
                            <FooterStyle BackColor="#CCCCCC" Font-Bold="False" Font-Italic="False" Font-Overline="False"
                                Font-Strikeout="False" Font-Underline="False" ForeColor="Black" />
                            <SelectedItemStyle BackColor="#008A8C" Font-Bold="True" ForeColor="White" />
                            <PagerStyle BackColor="#999999" Font-Bold="False" Font-Italic="False" Font-Overline="False"
                                Font-Strikeout="False" Font-Underline="False" ForeColor="Black" HorizontalAlign="Right"
                                Mode="NumericPages" />
                            <AlternatingItemStyle BackColor="Gainsboro" />
                            <ItemStyle BackColor="#EEEEEE" ForeColor="Black" />
                            <HeaderStyle BackColor="#000084" Font-Bold="True" Font-Italic="False" Font-Overline="False" Font-Strikeout="False"
                                Font-Underline="False" ForeColor="White" />
                            <Columns>
                                <asp:TemplateColumn HeaderText="用户名">
                                    <ItemTemplate>
                                        <a href='<%# "UserEdit.aspx?id=" + DataBinder.Eval(Container, "DataItem.id") %>'>
                                            <%# DataBinder.Eval(Container, "DataItem.userName") %>
                                        </a>
                                    </ItemTemplate>
                                </asp:TemplateColumn>
                                <asp:BoundColumn DataField="productname" HeaderText="注册产品"></asp:BoundColumn>
                                <asp:BoundColumn DataField="MachineCode" HeaderText="机器码"></asp:BoundColumn>
                                <asp:BoundColumn DataField="price" HeaderText="注册费用"></asp:BoundColumn>
                                <asp:BoundColumn DataField="remark" HeaderText="备注"></asp:BoundColumn>
                                <asp:BoundColumn DataField="statusname" HeaderText="状态"></asp:BoundColumn>
                                <asp:BoundColumn DataField="regtime" HeaderText="注册时间"></asp:BoundColumn>
                                <asp:BoundColumn DataField="creator" HeaderText="创建人"></asp:BoundColumn>
                                <asp:TemplateColumn HeaderText="操作">
                                    <ItemTemplate>
                                        <asp:LinkButton ID="lbtnDelete" CommandName="delete" runat="server" CommandArgument='<%# DataBinder.Eval(Container, "DataItem.id") %>' Text="删除">
                                        </asp:LinkButton>
                                    </ItemTemplate>
                                </asp:TemplateColumn>
                            </Columns>
                        </asp:datagrid></td>
                </tr>
            </table>
    </form>
</body>
</html>
