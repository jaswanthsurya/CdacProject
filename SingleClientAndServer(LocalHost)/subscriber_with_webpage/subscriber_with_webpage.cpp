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
			cout << html() << head(title("POSTS")) << endl;
			cout << body() << endl;
			/*cout << "<head>\n"
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
				"</html>";*/

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
				"                <link href=\"/css/subscriber.css\" rel=\"stylesheet\">\n"
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
				"        <a class=\"nav-link\" href=\"../subscriber.html\">Home <span class=\"sr-only\">(current)</span></a>\n"
				"      </li>\n"
				"    </ul>\n"
				"  </div>\n"
				"</nav>\n"
				"</div>  ";


			//Subscriber home page here

			cout << "<div class=\"container mt-4\">\n"
				"            <!-- Grid row -->\n"
				"            <div class=\"row\">\n"
				"                <!-- Grid column -->\n"
				"                <div class=\"col-md-6 mb-4\">\n"
				"                    <div class=\"card\">\n"
				"                        <div class=\"card-body\">\n"
				"                            <h3 class=\"text-center default-text py-3\"><i class=\"fa fa-refresh\"></i> Update</h3>\n"
				"                            <!--Body-->\n"
				"                            <form method=\"post\" action=\"subscriber_with_webpage1.cgi\">\n"
				"                            <div class=\"text-center\">\n"
				"                                <input type=\"submit\" class=\"btn btn-default waves-effect waves-light\" value=\"Refresh\"></input>\n"
				"                            </div>\n"
				"                        </form>\n"
				"                        </div>\n"
				"                    </div>\n"
				"                </div>\n"
				"                <!-- Grid column -->\n"
				"                <!-- Grid column -->\n"
				"                <div class=\"col-md-6 mb-4\">\n"
				"                    <div class=\"card indigo form-white\">\n"
				"                        <div class=\"card-body\">\n"
				"                            <h3 class=\"text-center white-text py-3\"><i class=\"fa fa-history\"></i> Posts Till Now</h3>\n"
				"                            <!--Body-->\n"
				"                            <form method=\"post\" action=\"subscriber_with_DetailsOfPreviousMessages.cgi\">\n"
				"                            <div class=\"text-center\">\n"
				"                                <input type=\"submit\" class=\"btn btn-default waves-effect waves-light\" value=\"View Posts\"></input>\n"
				"                            </div>\n"
				"                        </div>\n"
				"                    </div>\n"
				"                </div>\n"
				"                <!-- Grid column -->\n"
				"            </div>\n"
				"            <!-- Grid row -->\n"
				"            <!-- Grid row -->\n"
				"        </div>\n"
				"";

			//ending home page here
			


			//Developers section

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
			//ending dev
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
