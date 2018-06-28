#pragma once

/*
任何人都拥有平等的获取知识的权利

CppEasyJson 是开放源代码的软件，任何人都可以下载、使用、修改和重新发布，不必担心任何版权问题

请在重新发布的时候保留以上信息
*/

#include <string>
#include <vector>
#include "encode.hpp"

static const char  leftAnglebrackets = '<';
static const char  rightAnglebrackets = '>';
static const char  double_quotation_marks = '\"';
static const char  equal = '=';
static const char  slash = '/';
static const char  question_mark ='?';
static const char  exclamation_mark='!';
static const char  left_squarebrackets='[';
static const char  right_squarebrackets=']';

typedef struct _tag_XmlAttrItem
{
	std::string attrName;
	std::string attrValue;
}XmlAttrItem,*PXmlAttrItem;

typedef std::vector<XmlAttrItem> XmlAtrr;
class XmlNode;
typedef std::vector<XmlNode> XmlNodes;
class XmlNode
{
public:
	XmlNode()
	{

	}
	~XmlNode()
	{

	}
	void SetName(const char * strName)
	{
		if(strName)
		name = strName;
	}

	void SetData(const char * strData)
	{
		if(strData)
			data = strData;
	}

	void SetAttr(const char * attrName,const char * attrValue)
	{
		if(attrName && attrValue)
		{
			XmlAttrItem item;
			item.attrName=attrName;
			item.attrValue=attrValue;
			attr.push_back(item);
		}
	}
	void AddSub(XmlNode & subNode)
	{
		subnode.push_back(subNode);
	}
	std::string ToString()
	{
		std::string strXml;
		strXml+=leftAnglebrackets;
		strXml+=name;
		if(attr.size()>0)
		{
			for(int i=0;i<attr.size();i++)
			{
				strXml+=" ";
				strXml+=attr.at(i).attrName;
				strXml+=equal;
				strXml+=double_quotation_marks;
				strXml+=attr.at(i).attrValue;
				strXml+=double_quotation_marks;
			}
		}
		strXml+=rightAnglebrackets;
		strXml+="\n";
		if(subnode.size()>0)
		{
			for(int i=0;i<subnode.size();i++)
			{
				strXml+=subnode.at(i).ToString();
			}
		}
		if(data.length()>0)
		{			
			strXml+=data;
			strXml+="\n";
		}
		strXml+=leftAnglebrackets;
		strXml+=slash;
		strXml+=name;
		strXml+=rightAnglebrackets;
		strXml+="\n";
		return strXml;
	}
	std::string name;
	std::string data;
	XmlAtrr attr;
	XmlNodes subnode;
};

class XmlLex
{
public:
	XmlLex()
	{
		currentpos =0;
	}
	~XmlLex()
	{

	}
	void parseHeadLine(std::string & encoding)
	{
		std::string tk= GetNextToken();
		tk= GetNextToken();
		while(tk.at(0)!=question_mark)
		{
			tk= GetNextToken();
			if(tk=="encoding")
			{
				tk=GetNextToken();
				if(tk.at(0)==equal)
				{
					tk=GetNextToken();
					encoding = tk;
				}
			}
		}
		tk= GetNextToken(); 
	}
	void parseComment()
	{
		std::string tk= GetNextToken();
		tk= GetNextToken();
		while(tk.at(0)!=rightAnglebrackets)
		{
			tk= GetNextToken();	
			if(tk.at(0)==leftAnglebrackets)
			{
				std::string temp = PeekNextToken();
				if(temp.at(0)==exclamation_mark)
				{			
					parseComment();
				}
			}
		}
	}
	XmlNode parseData(const char * xStr,size_t xLen,std::string &encoding)
	{
		//while(!token.empty())
		//{
		//	printf("%s\r\n",token.c_str());			
		//	token=GetNextToken();
		//}

		xmlStr=xStr;
		xmlLen=xLen;
		//首先跳过BOM
		unsigned char c= *(xmlStr+currentpos);
		while(!isprint(c))
		{
			currentpos++;
			c= *(xmlStr+currentpos);
		}
		XmlNode node;
		std::string token=GetNextToken();
		if(token.at(0)==leftAnglebrackets)
		{			
			node = parseNode(encoding);
		}		
		return node;
	}
	XmlNode parseNode(std::string &encoding)
	{
		XmlNode node;
		std::string temp = PeekNextToken();	
		while(!temp.empty())
		{
			if(temp.at(0)==question_mark)
			{		
				//尝试解析Headline	
				parseHeadLine(encoding);
				temp=GetNextToken();
				temp = PeekNextToken();	
				continue;
			}
			else if(temp.at(0)==exclamation_mark)
			{		
				//尝试解析并跳过注释
				parseComment();
				temp=GetNextToken();
				temp = PeekNextToken();	
				continue;
			}
			else if(temp.at(0)!=exclamation_mark && temp.at(0)!=leftAnglebrackets)
			{			
				node.name = GetNextToken();
				std::string tk=GetNextToken();
				while(!tk.empty() && tk.at(0)!=rightAnglebrackets)
				{
					std::string attrName = tk;
					tk= GetNextToken();
					if(tk.at(0)==equal)
					{
						tk= GetNextToken();
						std::string attrValue =tk;
						node.SetAttr(attrName.c_str(),attrValue.c_str());
					}			
				}
		recheck:
				tk= GetNextToken(true);
				rtrim(tk);
				if(tk.empty())
				{
					if(currentpos>=xmlLen)
					{
						break;
					}
					goto recheck;
				}
				if(tk.at(0)==leftAnglebrackets)
				{
					std::string temp = PeekNextToken();
					if(!temp.empty() && temp.at(0)==slash)
					{
						tk= GetNextToken();
						std::string tkname= GetNextToken();
						tk= GetNextToken();
						if(tk.at(0)==rightAnglebrackets)
						{
							if(tkname==node.name)
							{
								break;
							}
						}
					}
					else
					{
						XmlNode subnode =  parseNode(encoding);
						node.AddSub(subnode);
						goto recheck;
					}
				}
				else
				{
					node.data = tk;
					goto recheck;
				}			
			}
		}
		return node;
		
	}
	std::string PeekNextToken()
	{
		size_t privpos=currentpos;
		std::string tk = GetNextToken();
		currentpos=privpos;
		return tk;
	}
	std::string GetNextToken(bool containspace=false)
	{
		bool gotstring = false;
		std::string tk;
		char * buffer =NULL;
		size_t cursor=currentpos;
		unsigned char c= *(xmlStr+cursor);
		while(isspace(c))
		{				
			cursor++;
			currentpos++;
			c= *(xmlStr+cursor);
		}
		while(cursor<xmlLen)
		{
			c= *(xmlStr+cursor);
			if(c== double_quotation_marks && !gotstring)
			{
				gotstring = true;
				cursor ++;
				currentpos++;
				continue;
			}
			else if(c!= double_quotation_marks && gotstring)
			{
				cursor++;
				continue;
			}
			if(c == leftAnglebrackets ||
				c== rightAnglebrackets ||
				c== double_quotation_marks ||
				c == equal ||
				c == slash ||
				c == question_mark||
				c ==exclamation_mark ||
				c ==left_squarebrackets ||
				c ==right_squarebrackets
				)
			{
				if(cursor==currentpos)
				{
					currentpos++;
					if(!gotstring)
						tk=c;
					break;
				}
				else
				{
					int len=cursor-currentpos;
					buffer = (char *)malloc(len+1);
					if(buffer)
					{
						memset(buffer,0,len+1);
						memcpy(buffer,xmlStr+currentpos,len);
						tk=buffer;
						free(buffer);
						currentpos = cursor ;	
						if(gotstring)
						{
							currentpos++;
						}
						return tk;
					}
				}
			}
			else if(!containspace && isspace(c))
			{
				int len=cursor-currentpos;
				buffer = (char *)malloc(len+1);
				if(buffer)
				{
					memset(buffer,0,len+1);
					memcpy(buffer,xmlStr+currentpos,len);
					tk=buffer;
					free(buffer);
					currentpos = cursor ;
					return tk;
				}
			}			
			else
			{
				cursor++;
			}
		}
		return tk;
	}
	size_t currentpos;
	const char * xmlStr;
	size_t xmlLen;
};
class CppEasyXml
{
public:
	CppEasyXml(void);
	~CppEasyXml(void);

	bool parseString(std::string & s);
	bool parseFile(const char * FilePathName);
	bool saveToFile(const char * FilePathName,const char * encode="UTF-8");
	std::string GetEncodeType(const char *FilePathName);
	std::string GetHeaderLine(const char * s);
	std::string toString(const char * encode="UTF-8");

	std::string xmlheadline;
	std::string encoding;
	XmlNode xmlRoot;
	XmlLex lex;

};
