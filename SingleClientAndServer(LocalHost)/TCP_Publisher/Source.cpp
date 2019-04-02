#include <iostream>
#include <vector>
#include <string>
#include<WinSock2.h>
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

using namespace std;
using namespace cgicc;
#pragma warning(disable:4996) 
int
main(int argc,
	char **argv)
{
	form_iterator name;
	Cgicc cgi;
	try {
		

		// Send HTTP header
		cout << HTTPHTMLHeader() << endl;

		// Set up the HTML document
		cout << html() << head(title("cgicc example")) << endl;
		cout << body() << endl;

		// Print out the submitted element
		name = cgi.getElement("name");
		if (name != cgi.getElements().end()) {
			cout << "<b>Sending this message to the server :</b> " << **name << endl;
		}

		// Close the HTML document
		cout << body() << html();
	}
	catch (exception& e) {
		// handle any errors - omitted for brevity
	}
	//system("pause");
	
	std::cout << ".....................TCP PUBLISHER........................." << endl;
	if (name != cgi.getElements().end())
		std::cout << "<br>Your message is " << name->getValue() << endl;
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
	char SenderBuffer[512] = "hello from publisher!";
	int iSenderBuffer = strlen(SenderBuffer);//sending buffer parameters

	int iRecv;
	char RecvBuffer[512];
	int iRecvBuffer = strlen(RecvBuffer) + 1;//receiving buffer parameters
	ZeroMemory(RecvBuffer, 512);

	//initialise wsastartup
	iWsaStartup = WSAStartup(MAKEWORD(2, 2), &Winsockdata);
	if (iWsaStartup != 0)
	{
		std::cerr << "Wsa Startup failed" << endl;
	}
	else
		std::cout << "Wsa Startup successfull" << endl;

	//filling the server details
	TCPServerAdd.sin_family = AF_INET;
	TCPServerAdd.sin_addr.s_addr = inet_addr("192.168.56.64");
	TCPServerAdd.sin_port = htons(8000);

	//socket creation
	TCPClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPClientSocket == INVALID_SOCKET)
	{
		cerr << "socket creation failed due to error :" << WSAGetLastError() << endl;
	}
	else
		std::cout << "socket creation successfull" << endl;

	//connect
	iConnect = connect(TCPClientSocket, (SOCKADDR *)&TCPServerAdd, sizeof(TCPServerAdd));
	if (iConnect == SOCKET_ERROR)
	{
		cerr << "connect to server failed due to error" << WSAGetLastError() << endl;
	}
	else
		std::cout << "connection successfull" << endl;

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

	//copying the input data from form to sender buffer
	int i; 
	string sendthis = name->getValue();
	//sendthis = "hey";
	for (i = 0;i < sendthis.length(); i++) {
		SenderBuffer[i] = sendthis[i];
	}
	SenderBuffer[i] = '\0';
	iSend = send(TCPClientSocket, SenderBuffer, iSenderBuffer, 0);
	if (iSend == SOCKET_ERROR)
	{
		cerr << "sending failed due to error: " << WSAGetLastError() << endl;
	}
	else
	{
		std::cout << "sending data sucessful" << endl;
		std::cout << "sent :" << SenderBuffer << endl;
	}

	//closing socket
	iCloseSocket = closesocket(TCPClientSocket);
	if (iCloseSocket == SOCKET_ERROR)
	{
		cerr << "closing socket failed due to error" << WSAGetLastError() << endl;
	}
	else
		std::cout << "socket closed" << endl;

	//WSAcleanup
	iWsaCleanup = WSACleanup();
	if (iWsaCleanup == SOCKET_ERROR)
	{
		cerr << "WSA cleanup failed due to error :" << WSAGetLastError() << endl;
	}
	else
		std::cout << "wsacleanup successful" << endl;
	system("PAUSE");
	return 0;
}
