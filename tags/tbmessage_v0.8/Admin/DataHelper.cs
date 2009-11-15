using System;
using System.Web;
using System.Data;
using System.Data.OleDb;

namespace Admin
{
    public class DataHelper
    {
        private static string connectionString = System.Configuration.ConfigurationManager.ConnectionStrings["siteConnectionString"].ConnectionString;

        public static int ExecuteNonQuery(CommandType commandType, string commandText, params OleDbParameter[] commandParameters)
        {
            //create & open a OleDbConnection, and dispose of it after we are done.
            using (OleDbConnection cn = new OleDbConnection(connectionString))
            {
                //create a command and prepare it for execution
                OleDbCommand cmd = new OleDbCommand();
                PrepareCommand(cmd, cn, commandType, commandText, commandParameters);

                cn.Open();
                int retval = cmd.ExecuteNonQuery();
                cn.Close();

                // detach the OleDbParameters from the command object, so they can be used again.
                cmd.Parameters.Clear();
                return retval;
            }
        }

        public static DataSet ExecuteDataset(CommandType commandType, string commandText, params OleDbParameter[] commandParameters)
        {
            //create & open a OleDbConnection, and dispose of it after we are done.
            using (OleDbConnection cn = new OleDbConnection(connectionString))
            {
                //create a command and prepare it for execution
                OleDbCommand cmd = new OleDbCommand();
                PrepareCommand(cmd, cn, commandType, commandText, commandParameters);

                //create the DataAdapter & DataSet
                OleDbDataAdapter da = new OleDbDataAdapter(cmd);
                DataSet ds = new DataSet();

                //fill the DataSet using default values for DataTable names, etc.
                da.Fill(ds);

                // detach the OleDbParameters from the command object, so they can be used again.			
                cmd.Parameters.Clear();

                //return the dataset
                return ds;
            }
        }

        public static object ExecuteScalar(CommandType commandType, string commandText, params OleDbParameter[] commandParameters)
        {
            //create & open a OleDbConnection, and dispose of it after we are done.
            using (OleDbConnection cn = new OleDbConnection(connectionString))
            {
                //create a command and prepare it for execution
                OleDbCommand cmd = new OleDbCommand();
                PrepareCommand(cmd, cn, commandType, commandText, commandParameters);

                cn.Open();
                object retval = cmd.ExecuteScalar();
                cn.Close();

                // detach the OleDbParameters from the command object, so they can be used again.
                cmd.Parameters.Clear();
                return retval;
            }
        }

        
        #region Protected Method

        private static void AttachParameters(OleDbCommand command, OleDbParameter[] commandParameters)
        {
            foreach (OleDbParameter p in commandParameters)
            {
                //check for derived output value with no value assigned
                if ((p.Direction == ParameterDirection.InputOutput) && (p.Value == null))
                {
                    p.Value = DBNull.Value;
                }

                command.Parameters.Add(p);
            }
        }

        /// <summary>
        /// This method opens (if necessary) and assigns a connection, transaction, command type and parameters 
        /// to the provided command.
        /// </summary>
        /// <param name="command">the SqlCommand to be prepared</param>
        /// <param name="connection">a valid SqlConnection, on which to execute this command</param>
        /// <param name="transaction">a valid SqlTransaction, or 'null'</param>
        /// <param name="commandType">the CommandType (stored procedure, text, etc.)</param>
        /// <param name="commandText">the stored procedure name or T-SQL command</param>
        /// <param name="commandParameters">an array of SqlParameters to be associated with the command or 'null' if no parameters are required</param>
        private static void PrepareCommand(OleDbCommand command, OleDbConnection connection, CommandType commandType, string commandText, OleDbParameter[] commandParameters)
        {
            //associate the connection with the command
            command.Connection = connection;

            //set the command text (stored procedure name or SQL statement)
            command.CommandText = commandText;

            //set the command type
            command.CommandType = commandType;

            //attach the command parameters if they are provided
            if (commandParameters != null)
            {
                AttachParameters(command, commandParameters);
            }
        }

        #endregion
    }
}
