#include <iostream>
#include <string>
#include<sstream>
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
#include<Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

using namespace std;
using namespace cgicc;


void showSQLError(unsigned int handleType, const SQLHANDLE& handle)
{
	SQLCHAR SQLState[1024];
	SQLCHAR message[1024];
	if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
		// Returns the current values of multiple fields of a diagnostic record that contains error, warning, and status information
		cout << "|.................SQL driver message......................|" << message << "\nSQL state: " << SQLState << "." << endl;
}

void CheckSQL(char * SQLQuery, int noOfCol)//function to connect to db and run query accordingly
{
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;
	int Col = noOfCol-1;

	do {
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLEnvHandle))
			// Allocates the environment
			break;

		if (SQL_SUCCESS != SQLSetEnvAttr(SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
			// Sets attributes that govern aspects of environments
			break;

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SQLEnvHandle, &SQLConnectionHandle))
			// Allocates the connection
			break;

		if (SQL_SUCCESS != SQLSetConnectAttr(SQLConnectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
			// Sets attributes that govern aspects of connections
			break;

		SQLCHAR retConString[1024]; // Conection string
		switch (SQLDriverConnect(SQLConnectionHandle, NULL,
			(SQLCHAR*)"DRIVER={SQL Server}; SERVER=DESKTOP-7IBUVRI\\SQLEXPRESS; DATABASE=PublisherSubscriberDB;Trusted=true;",
			SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT)) {
			// Establishes connections to a driver and a data source
		case SQL_SUCCESS:
			break;
		case SQL_SUCCESS_WITH_INFO:
			break;
		case SQL_NO_DATA_FOUND:
			showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			retCode = -1;
			break;
		case SQL_INVALID_HANDLE:
			showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			retCode = -1;
			break;
		case SQL_ERROR:
			showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			retCode = -1;
			break;
		default:
			break;
		}

		if (retCode == -1)
			break;

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle))
			// Allocates the statement
			break;

		if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery, SQL_NTS)) {
			// Executes a preparable statement
			showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
			break;
		}
		else {
			if (noOfCol == 4)
			{
				cout << "Sno" << "&nbsp&nbsp&nbsp&nbsp" << "Name" << "&nbsp&nbsp&nbsp&nbsp" 
					<< "IpAddress" << "&nbsp&nbsp&nbsp&nbsp" << "Topic" << "<br>";
				while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
					int Sno, Ip;
					char name[20], topic[20];
					SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &Sno, sizeof(Sno), NULL);
					SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &name, sizeof(name), NULL);
					SQLGetData(SQLStatementHandle, 3, SQL_C_DEFAULT, &Ip, sizeof(Ip), NULL);
					SQLGetData(SQLStatementHandle, 4, SQL_C_DEFAULT, &topic, sizeof(topic), NULL);
					cout << Sno << "&nbsp&nbsp&nbsp&nbsp&nbsp" << name 
						<< "&nbsp&nbsp&nbsp&nbsp&nbsp" << Ip << "&nbsp&nbsp&nbsp&nbsp&nbsp" << topic << "<br>";
				}
			}
			else if (noOfCol == 2)
			{
				cout << "Sno" << "&nbsp&nbsp&nbsp&nbsp" << "Topic" << "&nbsp&nbsp&nbsp&nbsp" << "<br>";
				while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
					int Sno;
					char topic[20];
					SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &Sno, sizeof(Sno), NULL);
					SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &topic, sizeof(topic), NULL);
					cout << Sno << "&nbsp&nbsp&nbsp&nbsp&nbsp" << topic << "&nbsp&nbsp&nbsp&nbsp&nbsp" << "<br>";
				}
			}
		}
	} while (FALSE);

	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
	// Free the resources and disconnects
}

int main(int argc,char **argv)
{
	string SqlQuery;
	int noOfCol;
	int flag = 0;
	try {
		Cgicc cgi;

		// Send HTTP header
		std::cout << HTTPHTMLHeader() << endl;

		// Set up the HTML document
		std::cout << html() << head(title("Manager")) << endl;
		//std::cout << body() << endl;
		//NAV BAR
		cout << "                <meta charset=\"UTF-8\">\n"
			"                <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
			"                <link href=\"/vendor/bootstrap/css/bootstrap.min.css\" rel=\"stylesheet\">\n"
			"          <!-- Custom fonts for this template -->\n"
			"                <link href=\"/vendor/fontawesome-free/css/all.min.css\" rel=\"stylesheet\">\n"
			"                <link href=\"/vendor/simple-line-icons/css/simple-line-icons.css\" rel=\"stylesheet\" type=\"text/css\">\n"
			"                <link href=\"https://fonts.googleapis.com/css?family=Lato:300,400,700,300italic,400italic,700italic\" rel=\"stylesheet\" type=\"text/css\">\n"
			"                <link href=\"/css/MainStyle.css\" rel=\"stylesheet\"><link href=\"css/MainStyle.css\" rel=\"stylesheet\">\n"
			"                <link href=\"/css/menubar.css\" rel=\"stylesheet\">\n"
			"                <link href=\"/css/details.css\" rel=\"stylesheet\">\n"
			 "  <!-- Bootstrap core JavaScript -->\n"
			"  <script src=\"/vendor/jquery/jquery.min.js\"></script>\n"
			"  <script src=\"/vendor/bootstrap/js/bootstrap.bundle.min.js\"></script>"
			"  <script src=\"/vendor/bootstrap/js/bootstrap.details.js\"></script>"
			"                \n"
			"    </head>\n"
			"    <body>\n"
			"         <div class=\"container-fluid\">\n"
			"    \n"
			"    <nav class=\"navbar navbar-expand-lg navbar-light bg-light\">\n"
			"  \n"
			"  <button class=\"navbar-toggler\" type=\"button\" data-toggle=\"collapse\" data-target=\"#navbarTogglerDemo02\" aria-controls=\"navbarTogglerDemo02\" aria-expanded=\"false\" aria-label=\"Toggle navigation\">\n"
			"    <span class=\"navbar-toggler-icon\"></span>\n"
			"  </button>\n"
			"\n"
			"  <div class=\"collapse navbar-collapse\" id=\"navbarTogglerDemo02\">\n"
			"    <ul class=\"navbar-nav mr-auto mt-2 mt-lg-0\">\n"
			"      <li class=\"nav-item active\">\n"
			"        <a class=\"nav-link\" href=\"manager.html\">Home <span class=\"sr-only\">(current)</span></a>\n"
			"      </li>\n"
			"      <li class=\"nav-item\">\n"
			"        <a class=\"nav-link\" href=\"detailsfinal.html\">View Details</a>\n"
			"      </li>\n"
			"    </ul>\n"
			"  </div>\n"
			"</nav>\n"
			"</div>  ";
		//added till here
		// Print out the submitted element
		form_iterator name = cgi.getElement("name");
		if (name != cgi.getElements().end()) {
			if (**name == "1")
			{
				SqlQuery = "SELECT  * FROM PublisherDetails";
				noOfCol = 4;
			}
			else if (**name == "2")
			{
				SqlQuery = "SELECT  * FROM SubscriberDetails";
				noOfCol = 4;
			}
			else if (**name == "3")
			{
				SqlQuery = "SELECT  * FROM TopicDetails";
				noOfCol = 2;
			}
			else
			{
				std::cout << "invalid input proceed again" << endl;
				flag = 1;
			}
		}

		// Close the HTML document
		std::cout << body() << html();
	}
	catch (exception& e) {
		// handle any errors - omitted for brevity
	}
	if (flag == 0)
	{
		stringstream strs;
		strs << SqlQuery;
		SqlQuery = strs.str();
		char * SqlQuery1 = (char *)SqlQuery.c_str();
		CheckSQL(SqlQuery1,noOfCol);
	}
}