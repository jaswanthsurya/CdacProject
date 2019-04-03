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
	string subscriberName;
	string subscriberIp;
	string topic;
	try {
		Cgicc cgi;
		// Send HTTP header
		std::cout << HTTPHTMLHeader() << endl;

		// Set up the HTML document
		std::cout << html() << head(title("cgicc example")) << endl;
		std::cout << body() << endl;

		// Print out the submitted element
		form_iterator name = cgi.getElement("subscriber_name");
		if (name != cgi.getElements().end()) {
			subscriberName = **name;
		}
		name = cgi.getElement("subscriber_ip");
		if (name != cgi.getElements().end()) {
			subscriberIp = **name;
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
	//convert subscriber name in string to char
	int len = subscriberName.length();
	int i = 0;
	char *subscriberName1;
	subscriberName1 = new char[len + 1];
	for (i = 0; i < len; i++)
	{
		subscriberName1[i] = subscriberName[i];
	}
	subscriberName1[i] = '\0';

	//convert subscriberip in string to char
	i = 0;
	char* subscriberIp1 = new char[subscriberIp.length() + 1];
	for (i = 0; i < subscriberIp.length(); i++)
	{
		subscriberIp1[i] = subscriberIp[i];
	}
	subscriberIp1[i] = '\0';
	unsigned long subscriberIpNum = inet_addr(subscriberIp1);
	delete(subscriberIp1);
	//convert number to character array
	stringstream srs;
	srs << subscriberIpNum;
	string temp = srs.str();
	char *subscriberIp2 = (char *)temp.c_str();

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
	char SqlQuery[512] = "insert into SubscriberDetails values('";
	strcat_s(SqlQuery, subscriberName1);
	strcat_s(SqlQuery, "',");
	strcat_s(SqlQuery, subscriberIp2);
	strcat_s(SqlQuery, ",'");
	strcat_s(SqlQuery, topic1);
	strcat_s(SqlQuery, "')");
	//call to insert function
	CheckSQL(SqlQuery);
	//deallocate the associated memory
	delete(subscriberName1);
	delete(subscriberIp2);
	delete(topic1);
	return 0;
}