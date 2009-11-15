using System;
using System.Collections;
using System.Configuration;
using System.Data;
using System.Data.OleDb;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;

namespace Admin
{
    public partial class ExecSql : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsAdmin(Page.User.Identity.Name))
            {
                BtnExec.Enabled = false;
                lbErrorMsg.Text = "您无权试用此页";
            }
            lbErrorMsg.Text = "";
        }

        protected void BtnExec_Click(object sender, EventArgs e)
        {
            BindData();
        }

        private void BindData()
        {
            string strSql = TextSql.Text.Trim();
            if (listSqlType.SelectedIndex == 0)
            {
                lbErrorMsg.Text = "请先选择SQL类型";
                return;
            }
            else if (listSqlType.SelectedIndex == 1)
            {
                dgResult.DataSource = DataHelper.ExecuteDataset(CommandType.Text, strSql, null);
                dgResult.DataBind();
            }
            else
            {
                try
                {
                    int effortCnt = DataHelper.ExecuteNonQuery(CommandType.Text, strSql, null);
                    lbErrorMsg.Text = "命令执行成功，影响记录条数：" + effortCnt.ToString();
                }
                catch (Exception exp)
                {
                    lbErrorMsg.Text = "错误：" + exp.Message;
                }
            }
        }

        protected void dgResult_PageIndexChanged(object source, DataGridPageChangedEventArgs e)
        {
            string strSql = TextSql.Text.Trim();
            dgResult.DataSource = DataHelper.ExecuteDataset(CommandType.Text, strSql, null);
            dgResult.CurrentPageIndex = e.NewPageIndex;
            dgResult.DataBind();
        }

        private bool IsAdmin(string userName)
        {
            string strSql = "select IsAdmin from sysUsers where userName = ?";
            OleDbParameter par_userName = new OleDbParameter("@userName", userName);

            object obj = DataHelper.ExecuteScalar(CommandType.Text, strSql, par_userName);

            return obj.ToString() == "1" || obj.ToString().ToLower() == "true";
        }
    }
}
