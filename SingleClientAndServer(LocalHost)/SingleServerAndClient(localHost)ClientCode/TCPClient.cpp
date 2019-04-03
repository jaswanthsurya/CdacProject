#include<iostream>
#include<WinSock2.h>

#pragma warning(disable:4996) 
using namespace std;

int main()
{
	cout << ".....................TCP CLIENT........................." << endl;
	WSADATA Winsockdata;//structure variable used to initialise winsock library
	int iWsaStartup;
	int iWsaCleanup;//variables to hold return types of startup and cleanup functions


	SOCKET TCPClientSocket;//variable to hold the socket
	int iCloseSocket;
	int iConnect;

	struct sockaddr_in TCPServerAdd;
	struct sockaddr_in TCPClientAdd;//structures for storing client and server details like ip and port number
	int iTCPClientAdd = sizeof(TCPClientAdd);


	int iSend;
	char SenderBuffer[512] = "hello from client!";
	int iSenderBuffer = strlen(SenderBuffer);//sending buffer parameters

	int iRecv;
	char RecvBuffer[512];
	int iRecvBuffer = strlen(RecvBuffer) + 1;//receiving buffer parameters
	ZeroMemory(RecvBuffer, 512);

	//initialise wsastartup
	iWsaStartup = WSAStartup(MAKEWORD(2, 2), &Winsockdata);
	if (iWsaStartup != 0)
	{
		cerr << "Wsa Startup failed" << endl;
	}
	else
		cout << "Wsa Startup successfull" << endl;

	//filling the server details
	TCPServerAdd.sin_family = AF_INET;
	TCPServerAdd.sin_addr.s_addr = inet_addr("192.168.56.2");
	TCPServerAdd.sin_port = htons(8100);

	//socket creation
	TCPClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPClientSocket == INVALID_SOCKET)
	{
		cerr << "socket creation failed due to error :" << WSAGetLastError() << endl;
	}
	else
		cout << "socket creation successfull" << endl;

	//connect
	iConnect = connect(TCPClientSocket, (SOCKADDR *)&TCPServerAdd, sizeof(TCPServerAdd));
	if (iConnect == SOCKET_ERROR)
	{
		cerr << "connect to server failed due to error" << WSAGetLastError() << endl;
	}
	else
		cout << "connection successfull" << endl;

	//receive data
	/*iRecv = recv(TCPClientSocket, RecvBuffer, iRecvBuffer, 0);
	if (iRecv == SOCKET_ERROR)
	{
		cerr << "receiving failed due to error" << WSAGetLastError() << endl;
	}
	else
	{
		cout << "receiving data succedded" << endl;
		cout << "received :" << RecvBuffer << endl;
	}*/

	//sending data
	iSend = send(TCPClientSocket, SenderBuffer, iSenderBuffer, 0);
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
	iCloseSocket = closesocket(TCPClientSocket);
	if (iCloseSocket == SOCKET_ERROR)
	{
		cerr << "closing socket failed due to error" << WSAGetLastError() << endl;
	}
	else
		cout << "socket closed" << endl;

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