using System;
using System.Data.OleDb;
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
    public partial class Login : System.Web.UI.Page
    {
        protected void Page_Load(object sender, System.EventArgs e)
        {
            if (Request.QueryString["action"] == "logout")
            {
                Logout();
            }
        }

        protected void Logout()
        {
            Session.RemoveAll();
            Session.Abandon();

            //注销
            System.Web.Security.FormsAuthentication.SignOut();
        }

        protected void btnLogin_Click(object sender, System.EventArgs e)
        {
            string userName = tbUserID.Value;
            string password = tbPassword.Text;
            string checkCode = tbValidCode.Text;

            if (userName.Length == 0)
            {
                lbErrorMsg.Text = "请输入用户名";
                return;
            }
            if (password.Length == 0)
            {
                lbErrorMsg.Text = "请输入密码";
                return;
            }
            if (checkCode.Length == 0)
            {
                lbErrorMsg.Text = "请输入验证码";
                return;
            }

            //验证验证码
            if (Request.Cookies["CHECK_CODE"] == null)
            {
                lbErrorMsg.Text = "请正确输入验证码信息。";
                return;
            }
            string strCheckCode = Request.Cookies["CHECK_CODE"].Value;
            if (tbValidCode.Text.Trim().ToLower() != strCheckCode.ToLower())
            {
                lbErrorMsg.Text = "请正确输入验证码信息。";
                return;
            }


            if (IsValidUser(userName, password))
            {
                //登录
                FormsAuthentication.SetAuthCookie(userName, false);
                Response.Redirect("UserList.aspx");
            }
            else
            {
                lbErrorMsg.Text = "用户名或密码不正确。";
            }
        }

        protected bool IsValidUser(string userName, string password)
        {
            string strSql2 = "select count(*) from SysUsers where status=1";
            object cnt = DataHelper.ExecuteScalar(CommandType.Text, strSql2, null);
            int ncnt = Convert.ToInt32(cnt);
            if (ncnt > 1)
            {
                return false;
            }

            string strSql = "select count(*) from SysUsers where status=1 and userName = @userName and password=@password";
            OleDbParameter par_userName = new OleDbParameter("@userName", userName);
            OleDbParameter par_password = new OleDbParameter("@password", password);
            OleDbParameter[] pars = { par_userName, par_password };

            object obj = DataHelper.ExecuteScalar(CommandType.Text, strSql, pars);
            if (obj != null && Convert.ToInt32(obj) > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
