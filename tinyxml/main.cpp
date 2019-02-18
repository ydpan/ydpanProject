
#include <stdio.h>
#include "tinyxml.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
bool XmlTest(const char* testString, const char* expected, const char* found, bool noEcho = false);
bool XmlTest(const char* testString, int expected, int found, bool noEcho = false);

static int gPass = 0;
static int gFail = 0;

bool XmlTest(const char* testString, const char* expected, const char* found, bool noEcho)
{
	bool pass = !strcmp(expected, found);
	if (pass)
		printf("[pass]");
	else
		printf("[fail]");

	if (noEcho)
		printf(" %s\n", testString);
	else
		printf(" %s [%s][%s]\n", testString, expected, found);

	if (pass)
		++gPass;
	else
		++gFail;
	return pass;
}


bool XmlTest(const char* testString, int expected, int found, bool noEcho)
{
	bool pass = (expected == found);
	if (pass)
		printf("[pass]");
	else
		printf("[fail]");

	if (noEcho)
		printf(" %s\n", testString);
	else
		printf(" %s [%d][%d]\n", testString, expected, found);

	if (pass)
		++gPass;
	else
		++gFail;
	return pass;
}


void NullLineEndings(char* p)
{
	while (p && *p) {
		if (*p == '\n' || *p == '\r') {
			*p = 0;
			return;
		}
		++p;
	}
}

int main()
{
	{
		ifstream in("E:\\cameraFile\\Leaper_GigE_Vision.xml");

		//获取文件的大小
		in.seekg(0, in.end);
		int length = (int)in.tellg();
		in.seekg(0, in.beg);

		char* temp = new char[length];

		if (in.is_open()) {
			cout << "Now reading..." << endl;
			in.read(temp, length);
		}
		cout << endl;
		in.close();
		TiXmlDocument doc;// ("E:\\cameraFile\\Leaper_GigE_Vision.xml");
		doc.Parse(temp);


		printf("** Demo doc read from disk: ** \n\n");
		printf("** Printing via doc.Print **\n");
		doc.Print(stdout);

		{
			printf("** Printing via TiXmlPrinter **\n");
			TiXmlPrinter printer;
			doc.Accept(&printer);
			fprintf(stdout, "%s", printer.CStr());
		}
		TiXmlNode* node = 0;
		TiXmlElement* todoElement = 0;
		TiXmlElement* itemElement = 0;
		node = doc.FirstChild("RegisterDescription");
		TiXmlNode* p = node->FirstChild();
		todoElement = node->ToElement();
		node = todoElement->FirstChildElement();	// This skips the "PDA" comment.

		int count = 0;
		TiXmlElement*	element;

		node = doc.RootElement();
		const char *pp = node->Value();
		node = node->FirstChild();
		node = node->NextSibling();
		
		node = node->FirstChild();
		

		//////////////////////////////////////////////////////
		printf("\n** Iterators. **\n");

		// Walk all the top level nodes of the document.
		count = 0;
		for (node = doc.FirstChild();
			node;
			node = node->NextSibling())
		{
			count++;
		}
		
		count = 0;
		for (node = doc.LastChild();
			node;
			node = node->PreviousSibling())
		{
			count++;
		}

		count = 0;
		for (node = doc.IterateChildren(0);
			node;
			node = doc.IterateChildren(node))
		{
			count++;
		}
		
		//  bob group
		count = 0;
		for (element = todoElement->FirstChildElement();
			element;
			element = element->NextSiblingElement())
		{
			count++;
		}

		// Walk all the elements in a node by value.
		count = 0;
		for (node = todoElement->FirstChild("Item");
			node;
			node = node->NextSibling("Item"))
		{
			count++;
		}
		
		count = 0;
		for (node = todoElement->LastChild("Item");
			node;
			node = node->PreviousSibling("Item"))
		{
			count++;
		}
		
		const TiXmlNode* lpMapNode = NULL;
		lpMapNode = doc.RootElement()->IterateChildren(lpMapNode);
		if (lpMapNode)
		{
			string ss = lpMapNode->Value();
			const TiXmlNode* lpItemNode = NULL;
			while (lpItemNode = lpMapNode->IterateChildren("Group",lpItemNode))
			{
				string str = lpItemNode->ToElement()->Attribute("");
				cout << str << endl;
			}
		}
	}

	printf("\nPass %d, Fail %d\n", gPass, gFail);
	return gFail;
}
