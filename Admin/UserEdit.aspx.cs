using System;
using System.Collections;
using System.Configuration;
using System.Data;
using System.Web;
using System.Data.OleDb;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;

namespace Admin
{
    public partial class UserEdit : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!Page.IsPostBack)
            {
                btnOK.Attributes.Add("onclick", "return validateinput()");
                BindProducts();

                if (Request.QueryString["id"] != null)
                {
                    LoadUser(Request.QueryString["id"]);
                }
                else
                {
                    tbRegTime.Text = DateTime.Now.ToString("yyyy-MM-dd");
                }
            }
        }

        private void LoadUser(string id)
        {
            string strSql = "select * from RegUsers where id=@id";
            OleDbParameter par_id = new OleDbParameter("@id", id);
            OleDbParameter[] pars = { par_id };

            DataSet ds = DataHelper.ExecuteDataset(CommandType.Text, strSql, pars);

            if (ds.Tables[0].Rows.Count > 0)
            {
                tbUserName.Text = ds.Tables[0].Rows[0]["userName"].ToString();
                tbMachineCode.Text = ds.Tables[0].Rows[0]["MachineCode"].ToString();
                ddlProducts.SelectedValue = ds.Tables[0].Rows[0]["productId"].ToString();
                ddlStatus.SelectedValue = ds.Tables[0].Rows[0]["status"].ToString();
                tbRegTime.Text = ds.Tables[0].Rows[0]["regTime"].ToString();
                tbRemark.Text = ds.Tables[0].Rows[0]["remark"].ToString();
                tbPrice.Text = ds.Tables[0].Rows[0]["price"].ToString();
                lbCreator.Text = ds.Tables[0].Rows[0]["creator"].ToString();
            }
        }

        private void BindProducts()
        {
            string strSql = "select * from products";

            ddlProducts.DataSource = DataHelper.ExecuteDataset(CommandType.Text, strSql, null);
            ddlProducts.DataTextField = "productname";
            ddlProducts.DataValueField = "productid";
            ddlProducts.DataBind();
        }

        protected void btnOK_Click(object sender, EventArgs e)
        {
            if (!IsAdmin(Page.User.Identity.Name))
            {
                if (lbCreator.Text != string.Empty && lbCreator.Text != Page.User.Identity.Name)
                {
                    Page.ClientScript.RegisterClientScriptBlock(this.GetType(), "alertwin", "alert('您无权编辑此用户。');", true);
                    return;
                }
            }

            if (Request.QueryString["id"] != null)
            {
                string strSql1 = "delete from RegUsers where id=@id";
                OleDbParameter par_id = new OleDbParameter("@id", Request.QueryString["id"]);
                OleDbParameter[] pars1 = { par_id };

                DataHelper.ExecuteNonQuery(CommandType.Text, strSql1, pars1);
            }

            int limitCnt = GetUserLimit(Page.User.Identity.Name);
            int currentCnt = GetUserCount(Page.User.Identity.Name);
            if (currentCnt >= limitCnt)
            {
                Page.ClientScript.RegisterClientScriptBlock(this.GetType(), "alertwin", "alert('您无权继续添加用户。');", true);
                return;
            }

            string strSql = "insert into RegUsers(userName,productid,machinecode,status,regTime, remark,price, creator, valid) values(@userName, @productId, @machineCode, @status, @regTime, @remark, @price, @creator, 1)";

            OleDbParameter par_userName = new OleDbParameter("@userName", OleDbType.VarChar, 50);
            OleDbParameter par_productID = new OleDbParameter("@productId", OleDbType.VarChar, 50);
            OleDbParameter par_machineCode = new OleDbParameter("@machineCode", OleDbType.VarChar, 50);
            OleDbParameter par_status = new OleDbParameter("@status", OleDbType.Integer);
            OleDbParameter par_RegTime = new OleDbParameter("@RegTime", OleDbType.VarChar, 50);
            OleDbParameter par_Remark = new OleDbParameter("@Remark", OleDbType.VarChar, 255);
            OleDbParameter par_Price = new OleDbParameter("@price", OleDbType.VarChar, 50);
            OleDbParameter par_creator = new OleDbParameter("@creator", OleDbType.VarChar, 50);

            par_userName.Value = tbUserName.Text;
            par_productID.Value = ddlProducts.SelectedValue;
            par_machineCode.Value = tbMachineCode.Text;
            par_status.Value = ddlStatus.SelectedValue;
            par_RegTime.Value = tbRegTime.Text;
            par_Remark.Value = tbRemark.Text.Length > 0 ? tbRemark.Text : " ";
            par_Price.Value = tbPrice.Text;
            par_creator.Value = lbCreator.Text == string.Empty ? Page.User.Identity.Name : lbCreator.Text;

            OleDbParameter[] pars = { par_userName, par_productID, par_machineCode, par_status, par_RegTime, par_Remark, par_Price, par_creator };

            DataHelper.ExecuteNonQuery(CommandType.Text, strSql, pars);

            //string scripts = "<script>with(opener){__doPostBack('lbtnSearch','');}window.close();</script>";
            //Page.RegisterClientScriptBlock("winclose", scripts);

            Response.Redirect("UserList.aspx");
        }

        protected void btnCancel_Click(object sender, EventArgs e)
        {
            Response.Redirect("UserList.aspx");    
        }

        private bool IsAdmin(string userName)
        {
            string strSql = "select IsAdmin from sysUsers where userName = ?";
            OleDbParameter par_userName = new OleDbParameter("@userName", userName);

            object obj = DataHelper.ExecuteScalar(CommandType.Text, strSql, par_userName);

            return obj.ToString() == "1" || obj.ToString().ToLower() == "true";
        }

        private int GetUserLimit(string userName)
        {
            string strSql = "select limit from sysUsers where userName = ?";
            OleDbParameter par_userName = new OleDbParameter("@userName", userName);

            object limit = DataHelper.ExecuteScalar(CommandType.Text, strSql, par_userName);

            return Convert.ToInt32(limit);
        }

        private int GetUserCount(string userName)
        {
            string strSql = "select count(*) from regUsers where creator = ?";
            OleDbParameter par_userName = new OleDbParameter("@userName", userName);

            object count = DataHelper.ExecuteScalar(CommandType.Text, strSql, par_userName);

            return Convert.ToInt32(count);
        }
    }
}
