// subscriber_with_DetailsOfPreviousMessages.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include<WinSock2.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include<sstream>

#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

#pragma warning(disable:4996) 
using namespace std;
using namespace cgicc;
void showSQLError(unsigned int handleType, const SQLHANDLE& handle)

{

	SQLCHAR SQLState[1024];

	SQLCHAR message[1024];

	if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))

		// Returns the current values of multiple fields of a diagnostic record that contains error, warning, and status information

		cout << "SQL driver message: " << message << "\nSQL state: " << SQLState << "." << endl;

}

void checkSQL(char* SQLQuery)

{

	SQLHANDLE SQLEnvHandle = NULL;

	SQLHANDLE SQLConnectionHandle = NULL;

	SQLHANDLE SQLStatementHandle = NULL;

	SQLRETURN retCode = 0;

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

			(SQLCHAR*)"DRIVER={SQL Server}; SERVER=LAPTOP-1KKBO5U4; DATABASE=subcriber_table;Trusted=true;",

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

			char name[256];
			char time[256];
			//adding table head
			//cout << "<table class=\"table\">\n"
			//	"  <thead class=\"thead-dark\">\n"
			//	"    <tr>\n"
			//	"      <th scope=\"col\">MESSAGES</th>\n"
			//	"      <th scope=\"col\">TIME</th>\n"
			//	"    </tr>\n"
			//	"  </thead>\n";
			////table head done
			//cout << "  <tbody>\n";



			/*
			"<div class=\"card text-center border-secondary\" style=\"background-color: rgba(255, 99, 71, 0.5)\"> \n"
			"  <div class=\"card-header\">\n"
			"   Here's your Post:\n"
			"  </div>\n"
			"  <div class=\"card-body\">\n"
			"    <h5 class=\"card-title\">Special title treatment</h5>\n"
			"  </div>\n"
			"  <div class=\"card-footer text-muted\">\n"
			"    2 days ago\n"
			"  </div>\n"
			"</div>"
			*/

			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {

				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &name, sizeof(name), NULL);
				SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &time, sizeof(time), NULL);

				/*cout << "<tr>\n";
				cout << "<td>";*/
				cout << "<div class=\"card text-center border-secondary\" style=\"background-color: rgba(255, 99, 71, 0.5)\"> \n"
					"  <div class=\"card-header\">\n"
					"   Here's your Post:\n"
					"  </div>\n";
				cout << "  <div class=\"card-body\">\n";
				cout << "    <h5 class=\"card-title\">";
				cout << name;
				cout<<"</h5>\n"
					"  </div>\n";
				cout << "  <div class=\"card-footer text-muted\">\n";
				cout << time;
					cout<<"  </div>\n"
					"</div>";

				
				/*cout << "</td> \n";
				cout << "<td>";*/
				/*cout << time;*/
				/*cout << "</td> \n";
				cout << "</tr>";*/
			}


			/*cout << "  </tbody>\n"
				"</table>\n";
				*/



			

		}

	} while (FALSE);
	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);

	SQLDisconnect(SQLConnectionHandle);

	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);

	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);

	// Frees the resources and disconnects

}

int main(int argc, char **argv)
{
	//connect with web page
	int i;
	try {
		Cgicc cgi;

		// Send HTTP header
		cout << HTTPHTMLHeader() << endl;

		// Set up the HTML document
		cout << html() << head(title("cgicc example")) << endl;
		// adding bootstrap links and then nav bar starts 
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
			"                <link href=\"/css/subscriber.css\" rel=\"stylesheet\">\n"
			"  <!-- Bootstrap core JavaScript -->\n"
			"  <script src=\"/vendor/jquery/jquery.min.js\"></script>\n"
			"  <script src=\"/vendor/bootstrap/js/bootstrap.bundle.min.js\"></script>"
			"  <script src=\"/vendor/bootstrap/js/details.js\"></script>"
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
			"        <a class=\"nav-link\" href=\"../subscriber.html\">Home <span class=\"sr-only\">(current)</span></a>\n"
			"      </li>\n"
			"    </ul>\n"
			"  </div>\n"
			"</nav>\n"
			"</div>  ";
		//nav bar ends here

		

		
		char SQLQuery2[] = "select * from subsriber order by date_time desc";

			checkSQL(SQLQuery2);
	}
	catch (exception& e) {
		// handle any errors - omitted for brevity
	}
	// Close the HTML document
	cout << "<section class=\"testimonials text-center bg-light\">\n"
		"    <div class=\"container\">\n"
		"      <h2 class=\"mb-5\">Developers</h2>\n"
		"      <div class=\"row\">\n"
		"        <div class=\"col-lg-4\">\n"
		"          <div class=\"testimonial-item mx-auto mb-5 mb-lg-0\">\n"
		"            <img class=\"img-fluid rounded-circle mb-3\" src=\"/img/pp.jpg\" alt=\"\">\n"
		"            <h5>Subodh Thota</h5>\n"
		"          </div>\n"
		"        </div>\n"
		"        <div class=\"col-lg-4\">\n"
		"          <div class=\"testimonial-item mx-auto mb-5 mb-lg-0\">\n"
		"            <img class=\"img-fluid rounded-circle mb-3\" src=\"/img/surya.jpg\" alt=\"\">\n"
		"            <h5>Jaswanth Surya</h5>\n"
		"          </div>\n"
		"        </div>\n"
		"        <div class=\"col-lg-4\">\n"
		"          <div class=\"testimonial-item mx-auto mb-5 mb-lg-0\">\n"
		"            <img class=\"img-fluid rounded-circle mb-3\" src=\"/img/haritha.jpg\" alt=\"\">\n"
		"            <h5>Haritha</h5>\n"
		"          </div>\n"
		"        </div>\n"
		"      </div>\n"
		"    </div>\n"
		"  </section>\n"
		"\n";

	cout << body() << html();
}
