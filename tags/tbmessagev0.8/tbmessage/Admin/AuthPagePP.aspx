<%@ Page language="c#" %>
<%@ Import Namespace="System.Data" %>
<%@ Import Namespace="System.Data.OleDb" %>
<%@ Import Namespace="Admin" %>

<script language="C#" runat="server">

protected DataSet GetFileProperty(string fileId)
{
    string commandText = "select fileid, productid, sendlimit, checksum, adtext, titlebarurl, helpurl from Files where status=1 and fileId='" + fileId + "'";
    return DataHelper.ExecuteDataset(CommandType.Text, commandText, null);
}

protected bool CheckReg(string machineCode, string userId)
{
    int num = 0;
    string commandText = "select count(*) from RegUsers where status=1 and MachineCode=@machineCode and userName = @userId";
    OleDbParameter parameter = new OleDbParameter("@machineCode", OleDbType.VarChar, 50);
    OleDbParameter parameter2 = new OleDbParameter("@userId", OleDbType.VarChar, 50);
    parameter.Value = machineCode;
    parameter2.Value = userId;
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
    string userId = base.Request.QueryString["userid"];
    string fileId = base.Request.QueryString["fileid"];
    string checksum = base.Request.QueryString["checksum"];
    DataSet fileProperty = this.GetFileProperty(fileId);
    bool flag = false;
    if ((fileProperty != null) && (fileProperty.Tables[0].Rows.Count > 0))
    {
        if (fileProperty.Tables[0].Rows[0]["checksum"].ToString() == checksum)
        {
            this.ltData1.Text = "http://search1.paipai.com/shop/";
            this.ltData2.Text = "{C992686E-91E4-43a2-B0DF-0EFE1E593739}"; //随机数，没用
 	    this.ltData3.Text = "{0E035C4B-0048-4575-8AE3-F14D0D139857}"; //随机数，没用
            this.ltData4.Text = "javascript:var x=";
            flag = this.CheckReg(machineCode, userId);
        }
        if (flag)
        {
            this.ltAuthResult.Text = "1000000";
            this.ltData3.Text = "";
        }
        else
        {
            this.ltAuthResult.Text = fileProperty.Tables[0].Rows[0]["sendlimit"].ToString();
            this.ltData3.Text = fileProperty.Tables[0].Rows[0]["adtext"].ToString();
        }
    }
    this.WriteLog(flag ? "1" : "0", base.Request.RawUrl);
}


</script>
<HTML>
    <HEAD>
	<title>authpagepp</title>
    </HEAD>
    <body>
        <form runat="server" ID="Form1">
            <Span id="result">
                <asp:Literal id="ltAuthResult" runat="server" Text="0"></asp:Literal>
            </Span>
            <Span id="data1">
                <asp:Literal id="ltData1" runat="server"></asp:Literal>
            </Span>
            <Span id="data2">
                <asp:Literal id="ltData2" runat="server"></asp:Literal>
            </Span>
            <Span id="data3">
                <asp:Literal id="ltData3" runat="server"></asp:Literal>
            </Span>
            <Span id="data4">
                <asp:Literal id="ltData4" runat="server"></asp:Literal>
            </Span>
        </form>
    </body>
</HTML>
