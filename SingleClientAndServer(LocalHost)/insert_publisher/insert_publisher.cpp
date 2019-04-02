#include <iostream>
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
		std::cout << html() << head(title("cgicc example")) << endl;
		std::cout << body() << endl;

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
	len = publisherIp.length();
	char *publisherIp1 = new char[len + 1];
	for (i = 0; i < len; i++)
	{
		publisherIp1[i] = publisherIp[i];
	}
	publisherIp1[i] = '\0';

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
	strcat_s(SqlQuery, publisherIp1);
	strcat_s(SqlQuery, ",'");
	strcat_s(SqlQuery, topic1);
	strcat_s(SqlQuery, "')");
	//call to insert function
	CheckSQL(SqlQuery);
	//deallocate the associated memory
	delete(publisherName1);
	delete(publisherIp1);
	delete(topic1);
	return 0;
}