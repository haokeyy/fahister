using System;
using System.Text;
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
    public partial class UserList : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!Page.IsPostBack)
            {
                BindProducts();
                BindUserList();
            }
        }

        protected void BindUserList()
        {
            StringBuilder sb = new StringBuilder("select * from RegUsers_v where 1=1");
            if (!IsAdmin(Page.User.Identity.Name))
            {
                sb.AppendFormat(" and creator = '{0}' ", Page.User.Identity.Name);
            }

            if (tbUserName.Text.Trim() != string.Empty)
            {
                sb.AppendFormat(" and userName like '%{0}%'", tbUserName.Text.Trim());
            }
            if (tbMachineCode.Text.Trim() != string.Empty)
            {
                sb.AppendFormat(" and MachineCode = '{0}'", tbMachineCode.Text.Trim());
            }
            if (ddlProducts.SelectedIndex > 0)
            {
                sb.AppendFormat(" and productid = {0}", ddlProducts.SelectedValue);
            }
            if (ddlStatus.SelectedIndex > 0)
            {
                sb.AppendFormat(" and status = {0}", ddlStatus.SelectedValue);
            }
            sb.Append(" order by regtime desc");

            DataGrid1.DataSource = DataHelper.ExecuteDataset(CommandType.Text, sb.ToString(), null);
            try
            {
                DataGrid1.DataBind();
            }
            catch
            {
                DataGrid1.CurrentPageIndex = 0;
                DataGrid1.DataBind();
            }
        }

        protected void BindProducts()
        {
            string strSql = "select * from products";

            ddlProducts.DataSource = DataHelper.ExecuteDataset(CommandType.Text, strSql, null);
            ddlProducts.DataTextField = "productname";
            ddlProducts.DataValueField = "productid";
            ddlProducts.DataBind();

            ddlProducts.Items.Insert(0, new ListItem("所有产品", "0"));
        }

        protected void btnSearch_Click(object sender, EventArgs e)
        {
            BindUserList();
        }

        protected void btnAdd_Click(object sender, EventArgs e)
        {
            Response.Redirect("UserEdit.aspx");
        }

        protected void DataGrid1_ItemCreated(object sender, DataGridItemEventArgs e)
        {
            if (e.Item.ItemIndex >= 0)
            {
                LinkButton lbtnDelete = (LinkButton)e.Item.FindControl("lbtnDelete");
                lbtnDelete.Attributes.Add("onclick", "return confirm('确实要删除该记录吗？')");
            }
        }

        protected void DataGrid1_ItemCommand(object source, DataGridCommandEventArgs e)
        {
            if (e.CommandName == "delete")
            {
                string strSql = "delete from RegUsers where id=@id";
                OleDbParameter par_id = new OleDbParameter("@id", e.CommandArgument);
                OleDbParameter[] pars = { par_id };

                DataHelper.ExecuteNonQuery(CommandType.Text, strSql, pars);

                BindUserList();
            }
        }

        protected void DataGrid1_PageIndexChanged(object source, DataGridPageChangedEventArgs e)
        {
            DataGrid1.CurrentPageIndex = e.NewPageIndex;
            BindUserList(); 
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
