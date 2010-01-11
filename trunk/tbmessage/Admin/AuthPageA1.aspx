<%@ Page language="c#" %>
<%@ Import Namespace="System.Data" %>
<%@ Import Namespace="System.Data.OleDb" %>
<%@ Import Namespace="Admin" %>

<script language="C#" runat="server">

    protected DataSet GetFileProperty(string checksum)
{
    string commandText = "select fileid, productid, sendlimit, checksum, adtext, titlebarurl, helpurl,commandurl from Files where status=1 and checksum='" + checksum + "'";
    return DataHelper.ExecuteDataset(CommandType.Text, commandText, null);
}

protected bool CheckReg(string machineCode, string productid)
{
    int num = 0;
    string commandText = "select count(*) from Reg_Users where status=1 and MachineCode=@machineCode and productid = @productid";
    OleDbParameter parameter = new OleDbParameter("@machineCode", OleDbType.VarChar, 50);
    OleDbParameter parameter2 = new OleDbParameter("@productid", OleDbType.VarChar, 50);
    parameter.Value = machineCode;
    parameter2.Value = productid;
    OleDbParameter[] commandParameters = new OleDbParameter[] { parameter, parameter2 };
    try
    {
        num = (int) DataHelper.ExecuteScalar(CommandType.Text, commandText, commandParameters);
    }
    catch (Exception)
    {
    }
    return (num > 0);
}

protected void WriteLog(string result, string requestUrl)
{
    //string format = "insert into logs(requesttime, result, requesturl) values('{0}', {1}, '{2}')";
    //string commandText = string.Format(format, DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss "), result, base.Request.UserHostAddress + requestUrl);
    //DataHelper.ExecuteNonQuery(CommandType.Text, commandText, null);
}

protected void Page_Load(object sender, EventArgs e)
{
    string machineCode = base.Request.QueryString["machinecode"];
    string checksum = base.Request.QueryString["checksum"];
    DataSet fileProperty = this.GetFileProperty(checksum);
    bool flag = false;
    if ((fileProperty != null) && (fileProperty.Tables[0].Rows.Count > 0))
    {
        this.ltAuthResult.Text = fileProperty.Tables[0].Rows[0]["sendlimit"].ToString();
        this.ltData1.Text = "http://shopsearch.taobao.com/browse/shop_search.htm?title=title&nick=nick";
        this.ltData2.Text = fileProperty.Tables[0].Rows[0]["commandurl"].ToString();
        this.ltData3.Text = fileProperty.Tables[0].Rows[0]["adtext"].ToString();
        this.ltData4.Text = "http://search.china.alibaba.com/search/company_search.htm?categoryId=0"; //随机数，没用
        flag = this.CheckReg(machineCode, fileProperty.Tables[0].Rows[0]["productid"].ToString());
       
        if (flag)
        {
            this.ltAuthResult.Text = "1000000";
            this.ltData3.Text = "";
        }
    }
    this.WriteLog(flag ? "1" : "0", base.Request.RawUrl);
}


</script>
<HTML>
    <HEAD>
	<meta http-equiv="Content-Type" content="text/html; charset=gb2312" /> 
    </HEAD>
    <body>
        <form runat="server" ID="Form1">
            <Span id="result">
                <asp:Literal id="ltAuthResult" runat="server" Text="0"></asp:Literal>
            </Span><Span id="data1">
                <asp:Literal id="ltData1" runat="server"></asp:Literal>
            </Span><Span id="data2">
                <asp:Literal id="ltData2" runat="server"></asp:Literal>
            </Span><Span id="data3">
                <asp:Literal id="ltData3" runat="server"></asp:Literal>
            </Span>
            <Span id="data4">
                <asp:Literal id="ltData4" runat="server"></asp:Literal>
            </Span>
        </form>
        
    </body>
</HTML>
