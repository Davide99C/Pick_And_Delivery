
#include <stdio.h>
#include <mysql/mysql.h>
//#include <mysql/my_byteorder.h>
//#include <mysql/my_global.h>


MYSQL *con;

void insertDB(MYSQL *con) {
    if (mysql_query(con, "INSERT INTO Stanze VALUES(sala relax,5.23,21.88,0.02)")) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
}

int reader_db() {
  con = mysql_init(NULL);

  if (con == NULL) {
      fprintf(stderr, "mysql_init() failed\n");
      exit(EXIT_FAILURE);
  }
  if (!(mysql_real_connect(con, "localhost", "root", "", "StanzeDiag", 3306, NULL, 0))) {
      //finish_with_error(con);
      fprintf(stderr, "mysql_real_connect() failed\n");
      exit(EXIT_FAILURE);
  }
  if (mysql_query(con, "SELECT * FROM Stanze")) {
      //finish_with_error(con);
      fprintf(stderr, "mysql_query() failed\n");
      exit(EXIT_FAILURE);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  if (result == NULL) {
      //finish_with_error(con);
      fprintf(stderr, "mysql_store_result() failed\n");
      exit(EXIT_FAILURE);
  }

  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(result))) {
      for(int i = 0; i < num_fields; i++) {
          printf("%s ", row[i] ? row[i] : "NULL");
      }
          printf("\n");
  }
  mysql_free_result(result);
  mysql_close(con);
  return 1;
}

//////////////////////////////////////////////////
/*
#include <string>
#include <iostream>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <mysql_error.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/resultset_metadata.h>

using namespace std;

const string user = "root";
const string passwd = "";
const string database = "StanzeDiag";
const string dbhost = "tcp://127.0.0.1:3306";

int main()
{
	sql::Driver* driver;
	sql::Connection* connection;
	sql::Statement* stmt;
	sql::ResultSet* res;
	sql::ResultSetMetaData* res_md;
	
    string query = "SELECT * FROM Stanze ORDER BY nome";

	try {
		// stabiliamo la connesione e selez. il db
		driver = get_driver_instance();
		connection = driver->connect(dbhost, user, passwd);
		connection->setSchema(database);
		// creiamo uno statement
		stmt = connection->createStatement();
		// eseguiamo la query
		res = stmt->executeQuery(query);
		// otteniamo i metadati
		res_md = res->getMetaData();

		// contiamo le righe ottenute
		cout << ">> Presi "
			<< res->rowsCount()
			<< " record <<"
			<< endl << endl;

		// stampiamo i nomi dei campi
		for(int i = 1; i <= res_md->getColumnCount(); i++)
			cout << res_md->getColumnLabel(i) << "\t\t";
		cout << endl << endl;

		// fetch dei dati
		while(res->next()) {
			for(int i = 1; i <= res_md->getColumnCount(); i++)
				cout << res->getString(i) << "\t\t";
			cout << endl;
		}

		// liberiamo le risorse
		delete res;
		delete stmt;
		connection->close();
		delete connection;
	}
	catch(sql::SQLException& e)
	{
		cout << e.what() << endl;
	}
}
*/