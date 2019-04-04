#include<iostream>
#include<WinSock2.h>
#include<windows.h>
#include<cstring>
#include<string.h>
#include<sstream>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include<vector>
#include<algorithm>

#define MaxThreads 10
#define getip 1
#define getTopic 2

#pragma warning(disable:4996) 
using namespace std;

vector<int> vect;
vector<int> repVect;
char *PubTopic;
char *PubName;
SOCKET sAcceptSocket1;
int CurrentIpAddressOfClient;
int CurrentIpAddressOfClientSubscriber;
int PastIpAddressOfClientSubscriber;

CRITICAL_SECTION cs;
void SendSubscribed(char*SenderBuffer, int iSendBuffer);
DWORD WINAPI ThreadFun(LPVOID str)
{
	EnterCriticalSection(&cs);
	int iRecv;
	char RecvBuffer[512];
	int iRecvBuffer = strlen(RecvBuffer) + 1;
	//receive data
	iRecv = recv(sAcceptSocket1, RecvBuffer, iRecvBuffer, 0);
	if (iRecv == SOCKET_ERROR)
	{
		cerr << "|.............receiving failed due to error...............|" << WSAGetLastError() << endl;
	}
	else
	{
		cout << "|...............receiving data succedded..................|" << endl;
		cout << "|......................received..........................::" << RecvBuffer << endl;
	}
	SendSubscribed(RecvBuffer, iRecvBuffer);
	LeaveCriticalSection(&cs);
	return 0;
}

int findVector(int CurrentIpAddress)
{
	vector<int>::iterator it;
	for (it = vect.begin(); it != vect.end(); it++)
	{
		if (*it == CurrentIpAddress)
			return 1;
	}
	return -1;
}

int findRep(int CurrentIpAddress)
{
	vector<int>::iterator it;
	for (it = repVect.begin(); it != repVect.end(); it++)
	{
		if (*it == CurrentIpAddress)
			return -1;
	}
	return 1;
}

int FindNumberOfElements()
{
	int count = 0;
	vector<int>::iterator it;
	for (it = vect.begin(); it != vect.end(); it++)
		count++;
	return count;
}

void clearVect()
{
	vect.clear();
	repVect.clear();
}

void showSQLError(unsigned int handleType, const SQLHANDLE& handle)
{
	SQLCHAR SQLState[1024];
	SQLCHAR message[1024];
	if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
		// Returns the current values of multiple fields of a diagnostic record that contains error, warning, and status information
		cout << "|.................SQL driver message......................|" << message << "\nSQL state: " << SQLState << "." << endl;
}

void CheckSQL(char * SQLQuery,int type)//function to connect to db and run query accordingly
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
		else {
			int num;
			char topic[20];
			char Name[30];
			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
				if (type == 1)
				{
					SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &num, sizeof(num), NULL);
					vect.push_back(num);
				}
				if (type == 2)
				{
					SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &Name, sizeof(Name), NULL);
					SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &topic, sizeof(topic), NULL);
					PubTopic = new char[strlen(topic) + 1];
					PubName = new char[strlen(Name) + 1];
					int i;
					for (i = 0; i < strlen(topic); i++)
					{
						PubTopic[i] = topic[i];
					}
					PubTopic[i] = '\0';
					i = 0;
					for (i = 0; i < strlen(Name); i++)
					{
						PubName[i] = Name[i];
					}
					PubName[i] = '\0';
				}
			}
		}
	} while (FALSE);

	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
	// Frees the resources and disconnects
}

void GetPublished()
{
	SOCKET TCPServerSocket;//variable to hold the socket
	int iCloseSocket;

	struct sockaddr_in TCPServerAdd;
	struct sockaddr_in TCPClientAdd;//structures for storing client and server details like ip and port number
	int iTCPClientAdd = sizeof(TCPClientAdd);

	int iBind;
	int iListen;
	int ThreadCount = 0;

	HANDLE CreateThreadHandle[3];//handle to thread


	//filling the server details
	TCPServerAdd.sin_family = AF_INET;
	TCPServerAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	TCPServerAdd.sin_port = htons(8000);

	//socket creation
	TCPServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPServerSocket == INVALID_SOCKET)
	{
		cerr << "|...........socket creation failed due to error...........|" << WSAGetLastError() << endl;
	}
	else
		cout << "|..............socket creation successfull................|" << endl;

	int opt = 1;
	int iSockOpt;
	//setsock opt for reusing the port and address
	iSockOpt = setsockopt(TCPServerSocket, SOL_SOCKET, SO_REUSEADDR, (CHAR*)&opt, sizeof(opt));
	if (iSockOpt == SOCKET_ERROR)
		cerr << "|..............sockopt failed due to error.................|" << WSAGetLastError() << endl;
	else
		cout << "|.................sockopt successfull.....................|" << endl;
	//binding socket
	iBind = bind(TCPServerSocket, (SOCKADDR*)&TCPServerAdd, sizeof(TCPServerAdd));
	if (iBind == SOCKET_ERROR)
	{
		cerr << "|..............binding failed due to error.................|" << WSAGetLastError() << endl;
	}
	else
		cout << "|.................binding successfull.....................|" << endl;

	//listen function
	iListen = listen(TCPServerSocket, SOMAXCONN);
	if (iListen == SOCKET_ERROR)
	{
		cerr << "|...........listen function failed due to error...........|" << WSAGetLastError() << endl;
	}
	else
		cout << "|..............listen function successfull................|" << endl;

	//function to get the ip address of publisher
	char SQLQuery[] = "SELECT  PublisherIP FROM PublisherDetails";
	cout << "|............getting ip addresses of publishers...........|" << endl;
	CheckSQL(SQLQuery,getip);
	vect.push_back(0);
	while (1)
	{
		//accept
		sAcceptSocket1 = accept(TCPServerSocket, (SOCKADDR*)&TCPClientAdd, &iTCPClientAdd);
		if (sAcceptSocket1 == INVALID_SOCKET)
			cerr << "|..............accept failed due to error.................|" << WSAGetLastError() << endl;
		CurrentIpAddressOfClient = TCPClientAdd.sin_addr.s_addr;
		if (findVector(CurrentIpAddressOfClient))
		{
			char SQLQuery1[100]= "select PublisherName,topic from PublisherDetails where PublisherIp=";
			stringstream strs;
			strs << CurrentIpAddressOfClient;
			string temp_str = strs.str();
			char * CurrentIpAddressOfClientstr = (char *)temp_str.c_str();
			strcat(SQLQuery1, CurrentIpAddressOfClientstr);
			CheckSQL(SQLQuery1, getTopic);
			cout << "|..................accept successfull.....................|" << endl << endl;
			cout << "|............The ip address connected is " << inet_ntoa(TCPClientAdd.sin_addr) << ".....|" << endl;
			//create a thread to handle receiving multiple publishers
			CreateThreadHandle[ThreadCount] = CreateThread(NULL,//security attributes thread is not inherited if null
				0,//stack size for thread if 0 it uses size of executable
				ThreadFun,//call back function
				NULL,//parameter to function
				0,//creation flag
				NULL);//thread id
			if (CreateThreadHandle[ThreadCount] == NULL)
			{
				cout << "creation of thread failed due to error (" << GetLastError() << ")" << endl;
			}
			ThreadCount++;
			if (ThreadCount == MaxThreads)
				break;
		}
	}
	WaitForMultipleObjects(MaxThreads, CreateThreadHandle, TRUE, INFINITE);
	//closing socket
	iCloseSocket = closesocket(TCPServerSocket);
	if (iCloseSocket == SOCKET_ERROR)
	{
		cerr << "|...........closing socket failed due to error............|" << WSAGetLastError() << endl;
	}
	else
		cout << "|....................socket closed........................|" << endl;
	ThreadCount--;
	while (ThreadCount!=-1)
	{
		CloseHandle(CreateThreadHandle[ThreadCount]);
		ThreadCount--;
	}
}

void SendSubscribed(char* SenderBuffer, int iSenderBuffer)
{
	SOCKET TCPServerSocket;//variable to hold the socket
	int iCloseSocket;

	struct sockaddr_in TCPServerAdd;
	struct sockaddr_in TCPClientAdd;//structures for storing client and server details like ip and port number
	int iTCPClientAdd = sizeof(TCPClientAdd);

	int iBind;
	int iListen;
	int iTemp;

	SOCKET sAcceptSocket;

	//filling the server details
	TCPServerAdd.sin_family = AF_INET;
	TCPServerAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	TCPServerAdd.sin_port = htons(8100);

	//socket creation
	TCPServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPServerSocket == INVALID_SOCKET)
	{
		cerr << "|..........socket creation failed due to error............|" << WSAGetLastError() << endl;
	}
	else
		cout << "|.............socket creation successfull.................|" << endl;

	//binding socket
	iBind = bind(TCPServerSocket, (SOCKADDR*)&TCPServerAdd, sizeof(TCPServerAdd));
	if (iBind == SOCKET_ERROR)
	{
		cerr << "|..............binding filed due to error.................|" << WSAGetLastError() << endl;
	}
	else
		cout << "|................binding successfull......................|" << endl;

	//listen function
	iListen = listen(TCPServerSocket, SOMAXCONN);
	if (iListen == SOCKET_ERROR)
	{
		cerr << "|.........listen function failed due to error.............|" << WSAGetLastError() << endl;
	}
	else
		cout << "|.............listen function successfull.................|" << endl;

	//code to convert the ip address of the client into a query string
	char SQLQuery1[512] = "select SubscriberIP from SubscriberDetails where topic=(select topic from PublisherDetails where PublisherIP=";
	stringstream strs;
	strs << CurrentIpAddressOfClient;
	string temp_str = strs.str();
	char * CurrentIpAddressOfClientstr = (char *)temp_str.c_str();
	strcat(SQLQuery1, CurrentIpAddressOfClientstr);
	char c[] = ")";
	strcat(SQLQuery1, c);

	clearVect();
	CheckSQL(SQLQuery1,1);
	vect.push_back(0);
	int noOfEle;
	noOfEle = FindNumberOfElements();
	PastIpAddressOfClientSubscriber = 0;
	while (1)
	{
		//accept
		sAcceptSocket = accept(TCPServerSocket, (SOCKADDR*)&TCPClientAdd, &iTCPClientAdd);
		if (sAcceptSocket == INVALID_SOCKET)
			cerr << "|.............accept failed due to error..................|" << WSAGetLastError() << endl;
		CurrentIpAddressOfClientSubscriber = TCPClientAdd.sin_addr.s_addr;
		if (findVector(CurrentIpAddressOfClientSubscriber))
		{
			if (findRep(CurrentIpAddressOfClientSubscriber) == 1)
			{
				cout << "|.................accept successfull......................|" << endl << endl;
				cout << "|............The ip address connected is " << inet_ntoa(TCPClientAdd.sin_addr) << ".....|" << endl;
				noOfEle--;
				//send the buffer content to the client
				int iSend;
				//sending data
				char temp[512] = "Here is an update about ";
				strcat(temp, PubTopic);
				strcat(temp, " by ");
				strcat(temp, PubName);
				strcat(temp, ":<br>");
				strcat(temp, SenderBuffer);
				iTemp = strlen(temp)+1;
				iSend = send(sAcceptSocket, temp, iTemp, 0);
				if (iSend == SOCKET_ERROR)
				{
					cerr << "|.............sending failed due to error.................|" << WSAGetLastError() << endl;
				}
				else
				{
					cout << "|...............sending data sucessfull...................|" << endl;
					cout << "|.......................sent.............................::" << SenderBuffer << endl;
				}
				repVect.push_back(CurrentIpAddressOfClientSubscriber);
				if (noOfEle == 1)
					break;
			}
		}
	}
	delete(PubTopic);
	delete(PubName);

	//closing socket
	iCloseSocket = closesocket(TCPServerSocket);
	if (iCloseSocket == SOCKET_ERROR)
	{
		cerr << "|...........closing socket failed due to error............|" << WSAGetLastError() << endl;
	}
	else
		cout << "|....................socket closed........................|" << endl;
}

int main()
{
	WSADATA Winsockdata;//structure variable used to initialise winsock library
	int iWsaStartup;
	int iWsaCleanup;//variables to hold return types of startup and cleanup functions

	InitializeCriticalSectionAndSpinCount(&cs, 1000);
	cout << "|.....................TCP MANAGER.........................|" << endl;
	//initialise wsastartup
	iWsaStartup = WSAStartup(MAKEWORD(2, 2), &Winsockdata);
	if (iWsaStartup != 0)
	{
		cerr << "|..................Wsa Startup failed.....................|" << endl;
	}
	else
		cout << "|................Wsa Startup successfull..................|" << endl;
	//*******************************************************************************************************
	GetPublished();


	cout << endl << endl;

	clearVect();
	//WSAcleanup
	iWsaCleanup = WSACleanup();
	if (iWsaCleanup == SOCKET_ERROR)
	{
		cerr << "|..............WSA cleanup failed due to error.............|" << WSAGetLastError() << endl;
	}
	else
		cout << "|.................wsacleanup successful...................|" << endl;
	DeleteCriticalSection(&cs);
	system("PAUSE");
	return 0;
}