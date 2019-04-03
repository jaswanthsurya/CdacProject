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
		cout << html() << head(title("Redirected")) << endl; 
		//cout << body() << endl;
		cout << " <link href=\"/vendor/bootstrap/css/bootstrap.min.css\" rel=\"stylesheet\">    ";
		cout << "<link href = \"/vendor/fontawesome-free/css/all.min.css\" rel = \"stylesheet\"> ";
		cout << " <link href = \"/vendor/simple-line-icons/css/simple-line-icons.css\" rel = \"stylesheet\" type = \"text/css\"> ";
		cout << "<link href = \"https://fonts.googleapis.com/css?family=Lato:300,400,700,300italic,400italic,700italic\" rel = \"stylesheet\" type = \"text/css\"> ";
		cout << "<link href = \"/css/landing-page.min.css\" rel = \"stylesheet\"> ";
		cout << "<link href = \"/css/sticky_alerts.css\" rel = \"stylesheet\"> ";
		cout << "</head><body>" << endl;
		cout << "<header class=\"masthead text-white text-center\">\n"
			"    <div class=\"overlay\"></div>\n"
			"    <div class=\"container\">\n"
			"      <div class=\"row\">\n"
			"        <div class=\"col-xl-9 mx-auto\">\n";
			name = cgi.getElement("name");
			cout << "<b><i>WAIT TILL YOU GET A CONFIRMATION NOTIFICATION BEFORE PUBLISHING A NEW ARTICLE </i></b><br>";
			if (name != cgi.getElements().end()) {
				cout << "<b>Sending this message to the server :</b> " << **name << endl;
			}
	//FORM SECTION
		cout<<	"          <h1 class=\"mb-5\">Go ahead and publish something!</h1>\n"
			"        </div>\n"
			"        <div class=\"col-md-10 col-lg-8 col-xl-7 mx-auto\">\n"
			"          <form method=POST action=\"/cgi-bin/example.cgi\">\n"
			"            <div class=\"form-row\">\n"
			"              <div class=\"col-12 col-md-9 mb-2 mb-md-0\">\n"
			"                <input type=\"text\" name=\"name\"class=\"form-control form-control-lg\" placeholder=\"Enter your message here...\">\n"
			"              </div>\n"
			"              <div class=\"col-12 col-md-3\">\n"
			"                <button type=\"submit\" class=\"btn btn-block btn-lg btn-primary\">Submit!</button>\n"
			"              </div>\n"
			"            </div>\n"
			"          </form>\n"
			"        </div>\n"
			"      </div>\n"
			"    </div>\n"
			"  </header>";
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
		/*name = cgi.getElement("name");
		if (name != cgi.getElements().end()) {
			cout << "<b>Sending this message to the server :</b> " << **name << endl;
		}
			*/   	
		cout << "";
		// Close the HTML document
		
	}
	catch (exception& e) {
		// handle any errors - omitted for brevity
	}
	//system("pause");
	
	//std::cout << ".....................TCP PUBLISHER........................." << endl;
	//if (name != cgi.getElements().end())
		//std::cout << "<br>Your message is " << name->getValue() << endl;
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
	else {
		cout << "<div class = \"alert alert-info alert-dismissible\" id=\"message\">\n"
			"\t\t\t<a href=\"#\" class=\"close\"  data-dismiss=\"alert\" aria-label=\"close\">&times;</a>\n"
			"\t\t\t<strong>Info:</strong> WSA START UP SUCCESS.\n"
			"\t\t\t</div>";
		//std::cout << "Wsa Startup successfull" << endl;
	}
	//filling the server details
	TCPServerAdd.sin_family = AF_INET;
	TCPServerAdd.sin_addr.s_addr = inet_addr("192.168.56.2");
	TCPServerAdd.sin_port = htons(8000);

	//socket creation
	TCPClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPClientSocket == INVALID_SOCKET)
	{
		cout << "<div class=\"alert alert-danger alert-dismissible\" id=\"message\">\n"
			"        <a href=\"#\" class=\"btn btn-xs btn-danger pull-right\">TRY AGAIN</a>\n"
			"        <strong>FAILED:</strong>socket creation failed !\n"
			"    </div>";
		//cerr << "socket creation failed due to error :" << WSAGetLastError() << endl;
	}
	else
	{
		cout << "<div class = \"alert alert-info alert-dismissible\" id=\"message\">\n"
			"\t\t\t<a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\">&times;</a>\n"
			"\t\t\t<strong>Info:</strong> SOCKET CREATION SUCCESS.\n"
			"\t\t\t</div>";

		//std::cout << "socket creation successfull" << endl;
	}
	//connect
	iConnect = connect(TCPClientSocket, (SOCKADDR *)&TCPServerAdd, sizeof(TCPServerAdd));
	if (iConnect == SOCKET_ERROR)
	{
		cout << "<div class=\"alert alert-danger alert-dismissible\" id=\"message\">\n"
			"        <a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\">&times;</a>\n"
			"        <strong>FAILED:</strong>connect to server failed !\n"
			"    </div>";
		//cerr << "connect to server failed due to error" << WSAGetLastError() << endl;
	}
	else {
		cout << "<div class = \"alert alert-info alert-dismissible\" id=\"message\">\n"
			"\t\t\t<a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\">&times;</a>\n"
			"\t\t\t<strong>Info:</strong> CONNECTION SUCCESFUL.\n"
			"\t\t\t</div>";

		//std::cout << "connection successfull" << endl;
	}


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
	//cout << sendthis;
	//sendthis = "hey";
	for (i = 0;i < sendthis.length(); i++) {
		SenderBuffer[i] = sendthis[i];
	}
	SenderBuffer[i] = '\0';
	cout << SenderBuffer << endl;
	iSenderBuffer = strlen(SenderBuffer) + 1;
	iSend = send(TCPClientSocket, SenderBuffer, iSenderBuffer, 0);
	if (iSend == SOCKET_ERROR)
	{
		cout << "<div class=\"alert alert-danger alert-dismissible\" id=\"message\">\n"
			"        <a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\">&times;</a>\n"
			"        <strong>FAILED:</strong>sending failed !\n"
			"    </div>";
		//cerr << "sending failed due to error: " << WSAGetLastError() << endl;
	}
	else
	{   
		cout << "<div class = \"alert alert-info alert-dismissible\" id=\"message\">\n"
			"\t\t\t<a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\">&times;</a>\n"
			"\t\t\t<strong>Info:</strong>SUCCESSFULLY SENT.\n"
			"\t\t\t</div>";
		/*std::cout << "sending data sucessful" << endl;
		std::cout << "sent :" << SenderBuffer << endl;*/
	}

	//closing socket
	iCloseSocket = closesocket(TCPClientSocket);
	if (iCloseSocket == SOCKET_ERROR)
	{
		
		//cerr << "closing socket failed due to error" << WSAGetLastError() << endl;
	}
	else {
		/*cout << "<div class = \"alert alert-info alert-dismissible\" id=\"message\">\n"
			"\t\t\t<a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\">&times;</a>\n"
			"\t\t\t<strong>Info:</strong> SOCKET CLOSED.\n"
			"\t\t\t</div>";*/
		//std::cout << "socket closed" << endl;
	}
	//WSAcleanup
	iWsaCleanup = WSACleanup();
	if (iWsaCleanup == SOCKET_ERROR)
	{
		cout << "<div class=\"alert alert-danger alert-dismissible\" id=\"message\">\n"
			"        <a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\">&times;</a>"
			"        <strong>FAILED:</strong>WSA CLEANUP failed !\n"
			"    </div>";
		//cerr << "WSA cleanup failed due to error :" << WSAGetLastError() << endl;
	}
	else
	{
		/*cout << "<div class = \"alert alert-info alert-dismissible\" id=\"message\">\n"
			"\t\t\t<a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\">&times;</a>DONE</a>\n"
			"\t\t\t<strong>Info:</strong> WSA CLEAN UP SUCCESS.\n"
			"\t\t\t</div>";*/
			//std::cout << "wsacleanup successful" << endl;
	}
		

	cout<<"  <!-- Bootstrap core JavaScript -->\n"
		"  <script src=\"/vendor/jquery/jquery.min.js\"></script>\n"
		"  <script src=\"/vendor/bootstrap/js/bootstrap.bundle.min.js\"></script>";
	cout << body() << html();

	//system("PAUSE");
	return 0;
}
