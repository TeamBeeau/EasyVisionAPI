using System;
using System.Data;
using System.Data.SqlClient;
using System.IO;


namespace BeeAPI
{

    public class DB
    {
        string connectionString = @"Server=(LocalDB)\MSSQLLocalDB;AttachDbFilename=" +
        System.IO.Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, "DATA.mdf") +
        ";Integrated Security=true;";

        public void ConnectToDatabase()
        {
            using (SqlConnection connection = new SqlConnection(connectionString))
            {
                try
                {
                    connection.Open();
                    Console.WriteLine("Connection to the 'DATA' database successful!");
                }
                catch (Exception ex)
                {
                    Console.WriteLine("Error connecting to the database: " + ex.Message);
                }
            }
        }
        public void InsertData(string dataModel, string dataPO)
        {
            string checkQuery = "SELECT COUNT(*) FROM DATA WHERE DATA_PO = @DataPO";
            string insertQuery = @"INSERT INTO DATA (DATA_MODEL, DATA_PO, DATA_COUNTER, DATA_PATH, DATA_SIZE) 
                           VALUES (@DataModel, @DataPO, NULL, NULL, NULL)";

            using (SqlConnection connection = new SqlConnection(connectionString))
            {
                try
                {
                    connection.Open();

                    using (SqlCommand checkCommand = new SqlCommand(checkQuery, connection))
                    {
                        checkCommand.Parameters.AddWithValue("@DataPO", dataPO);
                        int existingCount = (int)checkCommand.ExecuteScalar();

                        if (existingCount > 0)
                        {
                            Console.WriteLine($"DATA_PO '{dataPO}' already exists in the database. No data inserted.");
                            return;
                        }
                    }

                    using (SqlCommand insertCommand = new SqlCommand(insertQuery, connection))
                    {
                        insertCommand.Parameters.AddWithValue("@DataModel", dataModel);
                        insertCommand.Parameters.AddWithValue("@DataPO", dataPO);

                        int rowsAffected = insertCommand.ExecuteNonQuery();
                        Console.WriteLine($"{rowsAffected} row(s) inserted into the database.");
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine("Error inserting data into the database: " + ex.Message);
                }
            }
        }
        public void UpdateData(string dataModel, string dataPO)
        {
            int dataCounter = Global.model.Vision.Counter;
            string dataPath = Global.model.Vision.PathImg;
            long dataSize = Global.model.Vision.Size;
            string updateQuery = @"UPDATE DATA 
                           SET 
                               DATA_COUNTER = @DataCounter, 
                               DATA_PATH = @DataPath, 
                               DATA_SIZE = @DataSize
                           WHERE DATA_MODEL = @DataModel AND DATA_PO = @DataPO";

            using (SqlConnection connection = new SqlConnection(connectionString))
            {
                try
                {
                    connection.Open();

                    using (SqlCommand updateCommand = new SqlCommand(updateQuery, connection))
                    {
                        updateCommand.Parameters.AddWithValue("@DataModel", dataModel);
                        updateCommand.Parameters.AddWithValue("@DataPO", dataPO);

                        updateCommand.Parameters.AddWithValue("@DataCounter", (object)dataCounter ?? DBNull.Value);
                        updateCommand.Parameters.AddWithValue("@DataPath", (object)dataPath ?? DBNull.Value);
                        updateCommand.Parameters.AddWithValue("@DataSize", (object)dataSize ?? DBNull.Value);

                        int rowsAffected = updateCommand.ExecuteNonQuery();

                        if (rowsAffected > 0)
                        {
                            Console.WriteLine($"{rowsAffected} row(s) updated in the database.");
                        }
                        else
                        {
                            Console.WriteLine("No matching record found. Update failed.");
                        }
                    }
                }
                catch (Exception ex)
                {
                   // Console.WriteLine("Error updating data in the database: " + ex.Message);
                }
            }
            
            }
        public string SelectData(string dataModel, string dataPO)
        {
            string query = @"SELECT DATA_COUNTER, DATA_PATH, DATA_SIZE 
                     FROM DATA 
                     WHERE DATA_MODEL = @DataModel AND DATA_PO = @DataPO";

            using (SqlConnection connection = new SqlConnection(connectionString))
            {
                try
                {
                    connection.Open();

                    using (SqlCommand command = new SqlCommand(query, connection))
                    {
                        command.Parameters.AddWithValue("@DataModel", dataModel);
                        command.Parameters.AddWithValue("@DataPO", dataPO);

                        using (SqlDataReader reader = command.ExecuteReader())
                        {
                            if (reader.Read())
                            {
                                int dataCounter = Convert.ToInt32(reader["DATA_COUNTER"]);// as int;
                                string dataPath = Convert.ToString(reader["DATA_PATH"]);// as string;
                                double dataSize = Convert.ToDouble(reader["DATA_SIZE"]);//as long;

                                return $"Counter: {dataCounter}, Path: {dataPath}, Size: {dataSize}";
                            }
                            else
                            {
                                return "No data found for the given conditions.";
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    return "Error retrieving data: " + ex.Message;
                }
            }
        }

    }
}
