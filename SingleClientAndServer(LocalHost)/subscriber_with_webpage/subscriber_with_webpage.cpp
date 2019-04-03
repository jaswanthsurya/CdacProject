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
			cout << body() << endl;
			cout << "<head>\n"
				"<style>\n"
				"body{\n"
				"  background-color:lightblue;\n"
				"}\n"
				".submit {\n"
				"  background-color: blue;\n"
				"  border: none;\n"
				"  color:white;\n"
				"  padding: 40px 100px;\n"
				"  text-align: center;\n"
				"  text-decoration: none;\n"
				"  display: inline-block;\n"
				"  font-size: 32px;\n"
				"  margin: 200px 300px;\n"
				"  cursor: pointer;\n"
				"}\n"
				"</head>\n"
				"</style>\n"
				"<form method=\"post\" action=\"cgi-bin/subscriber_with_webpage1.cgi\">\n"
				"<input id=\"two\" type=\"submit\" class=\"submit\"  value=\"Received successfully\"/>\n"
				"</form>\n"
				"</body>\n"
				"</html>";
			
			// Close the HTML document
			cout << body() << html();
		}
		catch (exception& e) {
			// handle any errors - omitted for brevity
		}
		STARTUPINFO StartUpInfo, StartUpInfo1;
		PROCESS_INFORMATION ProcessInfo, ProcessInfo1;
		ZeroMemory(&StartUpInfo, sizeof(StartUpInfo));
		ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));
		StartUpInfo.cb = sizeof(StartUpInfo);
		BOOL Status = 0;
		Status = CreateProcess("C:\\Users\\haritha\\source\\repos\\CdacProject\\SingleClientAndServer(LocalHost)\\x64\\Debug\\TCPSubscriber.exe",//null if the application name is not given using variable but using command line arguments
			NULL,//application name given as command line argument 
			NULL,//process not inheritable if null
			NULL,//thread not inheritable if null
			FALSE,//handle inheritance not possible
			0,//flags
			NULL,//use parents environment variables
			NULL,//use parents directory
			&StartUpInfo,//startup information
			&ProcessInfo);//process information
		if (Status == 0)//status 0 indicates that the creation of process failed
		{
			cout << "creation of process failed due to error(" << ")" << GetLastError() << endl;
		}
		WaitForSingleObject(ProcessInfo.hProcess, INFINITE);//wait until child process exits
	return 0;
}
