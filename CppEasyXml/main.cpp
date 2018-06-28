#include "CppEasyXml.h"
#include <windows.h>


int Test1()
{
	XmlNode xmlnode;
	xmlnode.SetName("ROOT");
	xmlnode.SetAttr("DATE_CREATED","2018-06-27 09:17:38");
	xmlnode.SetAttr("CREATED_BY","Microsoft Log Parser V2.2");

		XmlNode sub;
		sub.SetName("ROW");

			XmlNode xmlsubNode;
			xmlsubNode.SetName("TimeGenerated");
			xmlsubNode.SetData("2017-12-28 13:28:08");


			XmlNode xmlsubNode1;
			xmlsubNode1.SetName("SourceName");
			xmlsubNode1.SetData("Service Control Manager");

			XmlNode xmlsubNode2;
			xmlsubNode2.SetName("EventID");
			xmlsubNode2.SetData("7036");

			XmlNode xmlsubNode3;
			xmlsubNode3.SetName("Message");
			xmlsubNode3.SetData("Windows Error Reporting Service 服务处于 正在运行 状态。");
		  
			sub.AddSub(xmlsubNode);
			sub.AddSub(xmlsubNode1);
			sub.AddSub(xmlsubNode2);
			sub.AddSub(xmlsubNode3);

		xmlnode.AddSub(sub);

	std::string xstr=xmlnode.ToString();
	std::string myxml="<?xml version=\"1.0\" encoding=\"ISO-10646-UCS-2\" standalone=\"yes\" ?>";
	myxml+="<!DOCTYPE ROOT[\
 <!ATTLIST ROOT DATE_CREATED CDATA #REQUIRED>\
 <!ATTLIST ROOT CREATED_BY CDATA #REQUIRED>\
 <!ELEMENT TimeGenerated (#PCDATA)>\
 <!ELEMENT SourceName (#PCDATA)>\
 <!ELEMENT EventID (#PCDATA)>\
 <!ELEMENT Message (#PCDATA)>\
 <!ELEMENT ROW (TimeGenerated, SourceName, EventID, Message)>\
 <!ELEMENT ROOT (ROW*)>\
]>";
	myxml+=xstr;


	//printf("%s\r\n",xstr.c_str());
	//std::string xmlencoding;
	//XmlLex lex;
	//XmlNode node = lex.parseData(xstr.c_str(),xstr.length(),xmlencoding);
	//std::string xxstr=node.ToString();
	//printf("%s\r\n",xxstr.c_str());

	CppEasyXml xml;
	xml.parseString(myxml);
	std::string xxstr=xml.toString();
	printf("%s\r\n",xxstr.c_str());
	return 0;
}
int TestFile()
{
	
	CppEasyXml xml;
	std::string encode = xml.GetEncodeType("../Test/Events.xml");
	printf("%s\r\n",encode.c_str());
	xml.parseFile("../Test/Events.xml");
	std::string xxstr=UToA(xml.toString());
	printf("%s\r\n",xxstr.c_str());

	return 0;
}
int TestFileParse(const char * FilePathName)
{

	CppEasyXml xml;
	std::string encode = xml.GetEncodeType(FilePathName);
	printf("%s\r\n", encode.c_str());
	xml.parseFile(FilePathName);
	std::string xxstr = UToA(xml.toString());
	printf("%s\r\n", xxstr.c_str());

	return 0;
}
int wmain(int argc, wchar_t* argv[])
{
	TestFileParse("../Test/UISkin1.xml");
	TestFile();
	//Test1();
	return 0;
}