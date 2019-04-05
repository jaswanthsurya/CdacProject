#include<WinSock2.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include<sstream>

#pragma warning(disable:4996) 
using namespace std;
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

			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {

				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &name, sizeof(name), NULL);

				cout << name << endl;

				// Retrieves data for a single column in the result set



			}

		}

	} while (FALSE);
	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);

	SQLDisconnect(SQLConnectionHandle);

	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);

	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);

	// Frees the resources and disconnects

}

int main()
{
	cout << "|.....................TCP SUBSCRIBER.........................|" << endl;
	WSADATA Winsockdata;//structure variable used to initialise winsock library
	int iWsaStartup;
	int iWsaCleanup;//variables to hold return types of startup and cleanup functions


	SOCKET TCPClientSocket;//variable to hold the socket
	int iCloseSocket;
	int iConnect;

	struct sockaddr_in TCPServerAdd;
	struct sockaddr_in TCPClientAdd;//structures for storing client and server details like ip and port number
	int iTCPClientAdd = sizeof(TCPClientAdd);


	/*int iSend;
	char SenderBuffer[512] = "hello from client!";
	int iSenderBuffer = strlen(SenderBuffer);//sending buffer parameters*/

	int iRecv;
	char RecvBuffer[512];
	int iRecvBuffer = strlen(RecvBuffer) + 1;//receiving buffer parameters
	//ZeroMemory(RecvBuffer, 512);

	//initialise wsastartup
	iWsaStartup = WSAStartup(MAKEWORD(2, 2), &Winsockdata);
	if (iWsaStartup != 0)
	{
		cerr << "|...................Wsa Startup failed.......................|" << endl;
	}
	else
		cout << "|.................Wsa Startup successfull....................|" << endl;


	//filling the server details
	TCPServerAdd.sin_family = AF_INET;
	TCPServerAdd.sin_addr.s_addr = inet_addr("192.168.56.2");
	TCPServerAdd.sin_port = htons(8100);

	//socket creation
	TCPClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPClientSocket == INVALID_SOCKET)
	{
		cerr << "|............socket creation failed due to error.............|" << WSAGetLastError() << endl;
	}
	else
		cout << "|...............socket creation successfull..................|" << endl;

	//connect
	iConnect = connect(TCPClientSocket, (SOCKADDR *)&TCPServerAdd, sizeof(TCPServerAdd));
	if (iConnect == SOCKET_ERROR)
	{
		cerr << "|...........connect to server failed due to error............|" << WSAGetLastError() << endl;
	}
	else
		cout << "|.................connection successfull.....................|" << endl;

	//receive data
	iRecv = recv(TCPClientSocket, RecvBuffer, iRecvBuffer, 0);
	if (iRecv == SOCKET_ERROR)
	{
		cerr << "|.............receiving failed due to error..................|" << WSAGetLastError() << endl;
	}
	else
	{
		cout << "|..............receiving data succedded received.............|" << RecvBuffer << endl;
	}

	//closing socket
	iCloseSocket = closesocket(TCPClientSocket);
	if (iCloseSocket == SOCKET_ERROR)
	{
		cerr << "|............closing socket failed due to error..............|" << WSAGetLastError() << endl;
	}
	else
		cout << "|.....................socket closed..........................|" << endl;

	//WSAcleanup
	iWsaCleanup = WSACleanup();
	if (iWsaCleanup == SOCKET_ERROR)
	{
		cerr << "|.............WSA cleanup failed due to error................|" << WSAGetLastError() << endl;
	}
	else
		cout << "|...................wsacleanup successful....................|" << endl;
	if (strlen(RecvBuffer) >= 1)

	{

		char SQLQuery1[2000] = "INSERT INTO subsriber(message,date_time) VALUES('";

		strcat(SQLQuery1, RecvBuffer);

		char c[] = "',";
		strcat(SQLQuery1, c);
		strcat(SQLQuery1, "CURRENT_TIMESTAMP");
		strcat(SQLQuery1, ")");

		checkSQL(SQLQuery1);

		//char SQLQuery2[] = "select * from subsriber";

		//checkSQL(SQLQuery2);

	}
	system("PAUSE");
	return 0;
}
