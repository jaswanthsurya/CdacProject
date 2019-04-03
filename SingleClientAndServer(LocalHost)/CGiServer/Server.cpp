#include <iostream>
#include <string>
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


int main(int argc, char **argv)
{
	string SqlQuery;
	int noOfCol;
	int flag = 0;
	try {
		Cgicc cgi;

		// Send HTTP header
		std::cout << HTTPHTMLHeader() << endl;

		// Set up the HTML document
		std::cout << html() << head(title("Manager")) << endl;
		//std::cout << body() << endl;
		//NAV BAR
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
			"        <a class=\"nav-link\" href=\"../manager.html\">Home <span class=\"sr-only\">(current)</span></a>\n"
			"      </li>\n"
			"      <li class=\"nav-item\">\n"
			"        <a class=\"nav-link\" href=\"../detailsfinal.html\">View Details</a>\n"
			"      </li>\n"
			"    </ul>\n"
			"  </div>\n"
			"</nav>\n"
			"</div>  ";
		//added till here
		// Print out the submitted element
		form_iterator name = cgi.getElement("name");
		//added developers section

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



		// Close the HTML document
		std::cout << body() << html();
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
	Status = CreateProcess("C:\\Users\\jassusai\\source\\repos\\CdacProject\\SingleClientAndServer(LocalHost)\\Release\\TCPManagerMultiThreading.exe",//null if the application name is not given using variable but using command line arguments
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
}