using System;
using System.Collections;
using System.Configuration;
using System.Data;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;

namespace Admin
{
    public partial class Link : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string commandText;
            string fileId = Request.QueryString["id"];
            if (Request.QueryString["type"] == "help")
            {
                commandText = "select helpurl from Files where status=1 and fileId='" + fileId + "'";
            }
            else
            {
                commandText = "select titlebarurl from Files where status=1 and fileId='" + fileId + "'";
            }
            string url = (string)DataHelper.ExecuteScalar(CommandType.Text, commandText, null);

            Response.Redirect(url);
        }
    }
}
