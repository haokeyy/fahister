using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Web;
using System.Web.SessionState;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;

namespace Admin
{
	
	public class CheckCode : System.Web.UI.Page
	{
		private void Page_Load(object sender, System.EventArgs e)
		{
			this.CreateCheckCodeImage(GenerateCheckCode());
		}

		#region Web 窗体设计器生成的代码
		override protected void OnInit(EventArgs e)
		{
			//
			// CODEGEN: 该调用是 ASP.NET Web 窗体设计器所必需的。
			//
			InitializeComponent();
			base.OnInit(e);
		}
  
		/// <summary>
		/// 设计器支持所需的方法 - 不要使用代码编辑器修改
		/// 此方法的内容。
		/// </summary>
		private void InitializeComponent()
		{    
			this.Load += new System.EventHandler(this.Page_Load);
		}
		#endregion

		private string GenerateCheckCode()
		{
			int number;
			char code;
			string checkCode = String.Empty;

			System.Random random = new Random();

			for(int i=0; i<4; i++)
			{
				number = random.Next();

				//if(number % 3 == 0)
				//	code = (char)('A' + (char)(number % 26));
				//else
				code = (char)(48 + (number % 10));

				checkCode += code.ToString();
			}

			Response.Cookies.Add(new HttpCookie("CHECK_CODE", checkCode));

			return checkCode;
		}

		private void CreateCheckCodeImage(string checkCode)
		{
			if(checkCode == null || checkCode.Trim() == String.Empty)
				return;

			System.Drawing.Bitmap image = new System.Drawing.Bitmap(54, 20);
			Graphics g = Graphics.FromImage(image);

			try
			{
				//生成随机生成器
				Random random = new Random();

				//清空图片背景色
				g.Clear(Color.White);

				//画图片的背景噪音线
//				for(int i=0; i<10; i++)
//				{
//					int x1 = random.Next(image.Width);
//					int x2 = random.Next(image.Width);
//					int y1 = random.Next(image.Height);
//					int y2 = random.Next(image.Height);
//
//					g.DrawLine(new Pen(Color.Silver,0.5f), x1, y1, x2, y2);
//				}

				Font font = new System.Drawing.Font("Arial", 12, System.Drawing.FontStyle.Bold);
				System.Drawing.Drawing2D.LinearGradientBrush brush = new System.Drawing.Drawing2D.LinearGradientBrush(new Rectangle(0, 0, image.Width, image.Height), Color.Blue, Color.DarkRed, 1.2f, true);
				g.DrawString(checkCode.Substring(0,1), font, brush, 2, 2);
				g.DrawString(checkCode.Substring(1,1), font, brush, 14, 2);
				g.DrawString(checkCode.Substring(2,1), font, brush, 26, 2);
				g.DrawString(checkCode.Substring(3,1), font, brush, 38, 2);

				//画图片的前景噪音点
				for(int i=0; i<50; i++)
				{
					int x = random.Next(image.Width);
					int y = random.Next(image.Height);

					image.SetPixel(x, y, Color.FromArgb(random.Next()));
				}

				//画图片的边框线
				g.DrawRectangle(new Pen(Color.Silver), 0, 0, image.Width, image.Height);

				System.IO.MemoryStream ms = new System.IO.MemoryStream();
				image.Save(ms, System.Drawing.Imaging.ImageFormat.Gif);
				Response.ClearContent();
				Response.ContentType = "image/Gif";
				Response.BinaryWrite(ms.ToArray());
			}
			finally
			{
				g.Dispose();
				image.Dispose();
			}
		}
	}
}
