#include<iostream>
#include<WinSock2.h>
#include<windows.h>
#include<cstring>


#pragma warning(disable:4996) 
using namespace std;



void GetPublished(char* RecvBuffer,int iRecvBuffer)
{


	SOCKET TCPServerSocket;//variable to hold the socket
	int iCloseSocket;

	struct sockaddr_in TCPServerAdd;
	struct sockaddr_in TCPClientAdd;//structures for storing client and server details like ip and port number
	int iTCPClientAdd = sizeof(TCPClientAdd);

	int iBind;
	int iListen;
	int iRecv;

	SOCKET sAcceptSocket;


	//filling the server details
	TCPServerAdd.sin_family = AF_INET;
	TCPServerAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	TCPServerAdd.sin_port = htons(8000);

	//socket creation
	TCPServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPServerSocket == INVALID_SOCKET)
	{
		cerr << "socket creation failed due to error :" << WSAGetLastError() << endl;
	}
	else
		cout << "socket creation successfull" << endl;

	//binding socket
	iBind = bind(TCPServerSocket, (SOCKADDR*)&TCPServerAdd, sizeof(TCPServerAdd));
	if (iBind == SOCKET_ERROR)
	{
		cerr << "binding filed due to error :" << WSAGetLastError() << endl;
	}
	else
		cout << "binding successfull" << endl;

	//listen function
	iListen = listen(TCPServerSocket, SOMAXCONN);
	if (iListen == SOCKET_ERROR)
	{
		cerr << "listen function failed due to error: " << WSAGetLastError() << endl;
	}
	else
		cout << "listen function successfull" << endl;

	while (1)
	{
		//accept
		sAcceptSocket = accept(TCPServerSocket, (SOCKADDR*)&TCPClientAdd, &iTCPClientAdd);
		if (sAcceptSocket == INVALID_SOCKET)
			cerr << "accept failed due to error: " << WSAGetLastError() << endl;
		else if (TCPClientAdd.sin_addr.s_addr == 1916315840)
		{
			cout << "accept successfull" << endl;
			break;
		}
	} 

	

	//receive data
	iRecv = recv(sAcceptSocket, RecvBuffer, iRecvBuffer, 0);
	if (iRecv == SOCKET_ERROR)
	{
		cerr << "receiving failed due to error" << WSAGetLastError() << endl;
	}
	else
	{
		cout << "receiving data succedded" << endl;
		cout << "received :" << RecvBuffer << endl;
	}

	//closing socket
	iCloseSocket = closesocket(TCPServerSocket);
	if (iCloseSocket == SOCKET_ERROR)
	{
		cerr << "closing socket failed due to error" << WSAGetLastError() << endl;
	}
	else
		cout << "socket closed" << endl;
}

void SendSubscribed(char* SenderBuffer,int iSenderBuffer)
{
	


	SOCKET TCPServerSocket;//variable to hold the socket
	int iCloseSocket;

	struct sockaddr_in TCPServerAdd;
	struct sockaddr_in TCPClientAdd;//structures for storing client and server details like ip and port number
	int iTCPClientAdd = sizeof(TCPClientAdd);

	int iBind;
	int iListen;

	SOCKET sAcceptSocket;

	
	

	//filling the server details
	TCPServerAdd.sin_family = AF_INET;
	TCPServerAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	TCPServerAdd.sin_port = htons(8100);

	//socket creation
	TCPServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPServerSocket == INVALID_SOCKET)
	{
		cerr << "socket creation failed due to error :" << WSAGetLastError() << endl;
	}
	else
		cout << "socket creation successfull" << endl;

	//binding socket
	iBind = bind(TCPServerSocket, (SOCKADDR*)&TCPServerAdd, sizeof(TCPServerAdd));
	if (iBind == SOCKET_ERROR)
	{
		cerr << "binding filed due to error :" << WSAGetLastError() << endl;
	}
	else
		cout << "binding successfull" << endl;

	//listen function
	iListen = listen(TCPServerSocket, SOMAXCONN);
	if (iListen == SOCKET_ERROR)
	{
		cerr << "listen function failed due to error: " << WSAGetLastError() << endl;
	}
	else
		cout << "listen function successfull" << endl;

	while (1)
	{
		//accept
		sAcceptSocket = accept(TCPServerSocket, (SOCKADDR*)&TCPClientAdd, &iTCPClientAdd);
		if (sAcceptSocket == INVALID_SOCKET)
			cerr << "accept failed due to error: " << WSAGetLastError() << endl;
		else if (TCPClientAdd.sin_addr.s_addr == 1027123392)
		{
			cout << "accept successfull" << endl;
			break;
		}
	}

	int iSend;
	//sending data
	iSend = send(sAcceptSocket, SenderBuffer, iSenderBuffer, 0);
	if (iSend == SOCKET_ERROR)
	{
		cerr << "sending failed due to error: " << WSAGetLastError() << endl;
	}
	else
	{
		cout << "sending data sucessful" << endl;
		cout << "sent :" << SenderBuffer << endl;
	}

	//closing socket
	iCloseSocket = closesocket(TCPServerSocket);
	if (iCloseSocket == SOCKET_ERROR)
	{
		cerr << "closing socket failed due to error" << WSAGetLastError() << endl;
	}
	else
		cout << "socket closed" << endl;

}

int main()
{
	char RecvBuffer[512];
	int iRecvBuffer = strlen(RecvBuffer) + 1;//receiving buffer parameters
	
	char SenderBuffer[512];
	int iSenderBuffer = strlen(SenderBuffer);//sending buffer parameters
	ZeroMemory(RecvBuffer, 512);

	WSADATA Winsockdata;//structure variable used to initialise winsock library
	int iWsaStartup;
	int iWsaCleanup;//variables to hold return types of startup and cleanup functions

	cout << ".....................TCP MANAGER........................." << endl;
	//initialise wsastartup
	iWsaStartup = WSAStartup(MAKEWORD(2, 2), &Winsockdata);
	if (iWsaStartup != 0)
	{
		cerr << "Wsa Startup failed" << endl;
	}
	else
		cout << "Wsa Startup successfull" << endl;
	
	
	
	
	GetPublished(RecvBuffer,iRecvBuffer);


	cout << endl << endl;
	strcpy( SenderBuffer,RecvBuffer);
	
	SendSubscribed(SenderBuffer,iSenderBuffer);
	
	//WSAcleanup
	iWsaCleanup = WSACleanup();
	if (iWsaCleanup == SOCKET_ERROR)
	{
		cerr << "WSA cleanup failed due to error :" << WSAGetLastError() << endl;
	}
	else
		cout << "wsacleanup successful" << endl;
	system("PAUSE");
	return 0;
}