#include <iostream>
#include<WinSock2.h>
#include <string>
#include<cstring>
#include<string.h>
#include<sstream>
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
#include<Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

#pragma warning(disable:4996)
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

void CheckSQL(char * SQLQuery)//function to connect to db and run query accordingly
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
	} while (FALSE);

	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
	// Free the resources and disconnects
}

int main(int argc, char **argv)
{
	string publisherName;
	string publisherIp;
	string topic;
	try {
		Cgicc cgi;

		// Send HTTP header
		std::cout << HTTPHTMLHeader() << endl;

		// Set up the HTML document
		std::cout << html() << head(title("Manager")) << endl;
		std::cout << body() << endl;
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
			"        <a class=\"nav-link\" href=\"../manager.html\">Home <span class=\"sr-only\">(current)</span></a>\n"
			"      </li>\n"
			"      <li class=\"nav-item\">\n"
			"        <a class=\"nav-link\" href=\"../detailsfinal.html\">View Details</a>\n"
			"      </li>\n"
			"    </ul>\n"
			"  </div>\n"
			"</nav>\n"
			"</div>  ";
		//added till here
		// Print out the submitted element

		//added form element
		cout << "<div class=\"container register\">\n"
			"            <div class=\"row\">\n"
			"                <div class=\"col-md-12\">\n"
			"                    <ul class=\"nav nav-tabs nav-justified\" id=\"myTab\" role=\"tablist\">\n"
			"                        <li class=\"nav-item\">\n"
			"                            <a class=\"nav-link active\" id=\"home-tab\" data-toggle=\"tab\" href=\"#home\" role=\"tab\" aria-controls=\"home\" aria-selected=\"true\">Add Publisher</a>\n"
			"                        </li>\n"
			"                        <li class=\"nav-item\">\n"
			"                            <a class=\"nav-link\" id=\"profile-tab\" data-toggle=\"tab\" href=\"#profile\" role=\"tab\" aria-controls=\"profile\" aria-selected=\"false\">Add Subscriber</a>\n"
			"                        </li>\n"
			"                        <li class=\"nav-item\">\n"
			"                            <a class=\"nav-link\" id=\"profile-tab\" data-toggle=\"tab\" href=\"#server\" role=\"tab\" aria-controls=\"profile\" aria-selected=\"false\">Run Server</a>\n"
			"                        </li>\n"
			"                    </ul>\n"
			"                    <div class=\"tab-content\" id=\"myTabContent\">\n"
			"                        <div class=\"tab-pane fade show active text-align form-new\" id=\"home\" role=\"tabpanel\" aria-labelledby=\"home-tab\">\n"
			"                            <h3 class=\"register-heading\">Publisher Form</h3>\n"
			"                            <div class=\"row register-form\">\n"
			"                                <div class=\"col-md-12\">\n"
			"                                    <form method=\"post\" action=\"insert_publisher.cgi\">\n"
			"                                        <div class=\"form-group\">\n"
			"                                            <input type=\"text\" name=\"publisher_name\" class=\"form-control\" placeholder=\"Publisher name *\" value=\"\" required=\"\"/>\n"
			"                                        </div>\n"
			"                                        <div class=\"form-group\">\n"
			"                                            <input type=\"text\" name=\"publisher_ip\" class=\"form-control\" placeholder=\"Publisher ip *\" value=\"\" required=\"\"/>\n"
			"                                        </div>\n"
			"                                        <div class=\"form-group\">\n"
			"                                            <input type=\"text\" name=\"topic\" class=\"form-control\" placeholder=\"TOPIC *\" value=\"\" required=\"\"/>\n"
			"                                        </div>\n"
			"                                        <div class=\"form-group\">\n"
			"                                            <input type=\"submit\" name=\"publisher_button\" class=\"btnContactSubmit\" value=\"submit\" />\n"
			"                                        </div>\n"
			"                                    </form>\n"
			"                                </div>\n"
			"                            </div>\n"
			"                        </div>\n"
			"                        <div class=\"tab-pane fade show text-align form-new\" id=\"profile\" role=\"tabpanel\" aria-labelledby=\"profile-tab\">\n"
			"                            <h3  class=\"register-heading\">Subscriber Form</h3>\n"
			"                            <div class=\"row register-form\">\n"
			"                                <div class=\"col-md-12\">\n"
			"                                    <form method=\"post\" action=\"insert_subscriber.cgi\">\n"
			"                                        <div class=\"form-group\">\n"
			"                                            <input type=\"text\" name=\"subscriber_name\" class=\"form-control\" placeholder=\"Subscriber name *\" value=\"\" required=\"\" />\n"
			"                                        </div>\n"
			"                                        <div class=\"form-group\">\n"
			"                                            <input type=\"text\" name=\"subscriber_ip\" class=\"form-control\" placeholder=\"Subscriber ip*\" value=\"\" required=\"\"/>\n"
			"                                        </div>\n"
			"                                        <div class=\"form-group\">\n"
			"                                            <input type=\"text\" name=\"topic\" class=\"form-control\" placeholder=\"TOPIC *\" value=\"\" required=\"\"/>\n"
			"                                        </div>\n"
			"                                        <div class=\"form-group\">\n"
			"                                            <input type=\"submit\" name=\"subscriber_button\" class=\"btnContactSubmit\" value=\"submit\" />\n"
			"                                        </div>\n"
			"                                    </form>\n"
			"                                </div>\n"
			"                            </div>\n"
			"                        </div>\n"
			"\n"
			"\n"
			"                        <div class=\"tab-pane fade show text-align form-new\" id=\"server\" role=\"tabpanel\" aria-labelledby=\"profile-tab\">\n"
			"                            <h3  class=\"register-heading\">Server</h3>\n"
			"                            <div class=\"row register-form\">\n"
			"                                <div class=\"col-md-12\">\n"
			"                                    <form method=\"post\" action=\"server.cgi\">\n"
			"                                        <div class=\"form-group\">\n"
			"                                            <input type=\"submit\" name=\"subscriber_button\" class=\"btnContactSubmit\" value=\"Click to run server       \" />\n"
			"                                        </div>\n"
			"                                    </form>\n"
			"                                </div>\n"
			"                            </div>\n"
			"                        </div>\n"
			"\n"
			"\n"
			"                    </div>\n"
			"                </div>\n"
			"            </div>\n"
			"        </div>\n"
			"";
		//added developers section

		//DEVELOPERS SECTION

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


		// Print out the submitted element
		form_iterator name = cgi.getElement("publisher_name");
		if (name != cgi.getElements().end()) {
			publisherName = **name;
		}
		name = cgi.getElement("publisher_ip");
		if (name != cgi.getElements().end()) {
			publisherIp = **name;
		}
		name = cgi.getElement("topic");
		if (name != cgi.getElements().end()) {
			topic = **name;
		}
		// Close the HTML document
		std::cout << body() << html();
	}
	catch (exception& e) {
		// handle any errors - omitted for brevity
	}
	//convert publisher name in string to char
	int len = publisherName.length();
	int i = 0;
	char *publisherName1;
	publisherName1 = new char[len + 1];
	for (i = 0; i < len; i++)
	{
		publisherName1[i] = publisherName[i];
	}
	publisherName1[i] = '\0';

	//convert publisherip in string to char
	i = 0;
	char* publisherIp1 = new char[publisherIp.length()+1];
	for (i = 0; i < publisherIp.length(); i++)
	{
		publisherIp1[i] = publisherIp[i];
	}
	publisherIp1[i] = '\0';
	unsigned long publisherIpNum=inet_addr(publisherIp1);
	delete(publisherIp1);
	//convert number to character array
	stringstream srs;
	srs << publisherIpNum;
	string temp = srs.str();
	char *publisherIp2 = (char *)temp.c_str();
	
	//convert topic in string to char
	i = 0;
	len = topic.length();
	char *topic1 = new char[len + 1];
	for (i = 0; i < len; i++)
	{
		topic1[i] = topic[i];
	}
	topic1[i] = '\0';

	//generate query
	char SqlQuery[512] = "insert into PublisherDetails values('";
	strcat_s(SqlQuery, publisherName1);
	strcat_s(SqlQuery, "',");
	strcat_s(SqlQuery, publisherIp2);
	strcat_s(SqlQuery, ",'");
	strcat_s(SqlQuery, topic1);
	strcat_s(SqlQuery, "')");
	//call to insert function
	CheckSQL(SqlQuery);
	//deallocate the associated memory
	delete(publisherName1);
	delete(publisherIp2);
	delete(topic1);
	return 0;
}