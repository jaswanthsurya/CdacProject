// subscriber_with_DetailsOfPreviousMessages.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include<WinSock2.h>
#include <iostream>
#include <string>
#include <windows.h>
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
			"</head>\n"
			"</style>\n"
			"<form method=\"post\" action=\"subscriber_with_webpage1.cgi\">\n"

			"</form>\n"
			"</body>\n"
			"</html>";
		cout << "<table class=\"table\">\n"
			"  <thead class=\"thead-dark\">\n"
			"    <tr>\n"
			"      <th scope=\"col\">MESSAGES</th>\n"
			"      <th scope=\"col\">TIME</th>\n"
			"    </tr>\n"
			"  </thead>\n";
		//table head done
		cout << "  <tbody>\n";
		string line;
		ifstream myfile("data.txt");
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				cout << "<tr>\n";
				cout << "<td>";
				cout << line;
				cout << "</td> \n";
				cout << "</tr>";
			}
			myfile.close();
			cout << "  </tbody>\n"
				"</table>\n";
		}
		else cout << "Unable to open file"; 		// Close the HTML document
		cout << body() << html();
	}
	catch (exception& e) {
		// handle any errors - omitted for brevity
	}
	
}
