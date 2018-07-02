#pragma once

/*
任何人都拥有平等的获取知识的权利

CppEasyXml 是开放源代码的软件，任何人都可以下载、使用、修改和重新发布，不必担心任何版权问题

请在重新发布的时候保留以上信息
*/

#include <string>
#include <vector>
#include "encode.hpp"

static const char  leftAnglebrackets = '<';
static const char  rightAnglebrackets = '>';
static const char  double_quotation_marks = '\"';
static const char  single_quotation_marks = '\'';
static const char  equal = '=';
static const char  slash = '/';
static const char  question_mark ='?';
static const char  exclamation_mark='!';
static const char  left_squarebrackets='[';
static const char  right_squarebrackets=']';
static const char  backslash = '\\';

typedef struct _tag_XmlAttrItem
{
	std::string attrName;
	std::string attrValue;
}XmlAttrItem,*PXmlAttrItem;

typedef std::vector<XmlAttrItem> XmlAtrr;
class XmlNode;
typedef std::vector<XmlNode> XmlNodes;
class XmlHeadLine
{
public:
	XmlHeadLine()
	{

	}
	~XmlHeadLine()
	{

	}
	std::string toStrng(const char * encode)
	{
		std::string temp;
		temp += leftAnglebrackets;
		temp += question_mark;
		temp += name;
		for (int i = 0;i < attr.size();i++)
		{
			temp += " ";
			temp += attr.at(i).attrName;
			temp += "=";
			temp += "\"";
			if (stricmp(attr.at(i).attrName.c_str(), "encoding") == 0)
			{
				temp += encode;
			}
			else
			{
				temp += attr.at(i).attrValue;
			}			
			temp += "\"";
		}
		temp += question_mark;
		temp += rightAnglebrackets;
		return temp;
	}
	std::string name;
	XmlAtrr attr;
};
typedef std::vector<XmlHeadLine> XmlHeadLines;
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
	std::string GetName()
	{
		return name;
	}
	void SetData(const char * strData)
	{
		if(strData)
			data = strData;
	}
	std::string GetData()
	{
		return data;
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
	bool GetAttr(const char * attrName, std::string & attrValue)
	{
		bool bret = false;
		if (attrName)
		{
			for (int i = 0;i < attr.size();i++)
			{
				if (stricmp(attr.at(i).attrName.c_str(), attrName) == 0)
				{
					attrValue = attr.at(i).attrValue;
					bret = true;
					break;
				}
			}
		}
		return bret;
	}
	void AddSub(XmlNode & subNode)
	{
		subnode.push_back(subNode);
	}
	bool GetSub(int index, XmlNode & node)
	{
		bool bret = false;
		if (index >= 0 && index < subnode.size())
		{
			node =  subnode.at(index);
			bret = true;
		}
		return bret;
	}
	int GetFirstSub(const char * subName,XmlNode & node)
	{
		int index = -1;
		if (subName)
		{
			for (int i = 0;i < subnode.size();i++)
			{
				if (stricmp(subnode.at(i).GetName().c_str(), subName) == 0)
				{
					index = i;
					node = subnode.at(i);					
					break;
				}
			}
		}
		return index;
	}
	// index is current subnode 
	bool GetNextSub(const char * subName, int &index, XmlNode & node)
	{
		bool bret = false;
		if (subName)
		{
			if (index >= 0 && index < subnode.size())
			{
				for (int i = index+1;i < subnode.size();i++)
				{
					if (stricmp(subnode.at(i).GetName().c_str(), subName) == 0)
					{
						index = i;
						node = subnode.at(i);
						bret = true;
						break;
					}
				}
			}			
		}
		return bret;
	}
	XmlNodes GetSubArray(const char * subName)
	{
		XmlNodes nodes;
		if (subName)
		{
			for (int i = 0;i < subnode.size();i++)
			{
				if (stricmp(subnode.at(i).GetName().c_str(), subName) == 0)
				{
					nodes.push_back(subnode.at(i));
				}
			}
		}		
		return nodes;
	}
	std::string ToString()
	{
		std::string strXml;
		std::string strSub;
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
		if(subnode.size()>0)
		{
			for(int i=0;i<subnode.size();i++)
			{
				strSub+=subnode.at(i).ToString();
			}
		}
		if(data.length()>0)
		{	
			strXml += rightAnglebrackets;
			strXml += "\n";
			strXml += strSub;
			strXml+=data;
			strXml+="\n";
			strXml += leftAnglebrackets;
			strXml += slash;
			strXml += name;
			strXml += rightAnglebrackets;
		}
		else
		{
			if (strSub.length() > 0)
			{
				strXml += rightAnglebrackets;
				strXml += "\n";
				strXml += strSub;
				strXml += leftAnglebrackets;
				strXml += slash;
				strXml += name;
				strXml += rightAnglebrackets;
			}
			else
			{
				strXml += slash;
				strXml += rightAnglebrackets;
			}
			
		}
		
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
	XmlHeadLine parseHeadLine(std::string & encoding)
	{
		XmlHeadLine xmlheadline;
		std::string tk= GetNextToken();
		if (!tk.empty() && tk.at(0) == question_mark)
		{			
			tk = GetNextToken();
			xmlheadline.name = tk;
			while (tk.at(0) != question_mark)
			{
				XmlAttrItem xmlattritem;
				tk = GetNextToken();
				xmlattritem.attrName = tk;
				if (tk == "encoding")
				{
					tk = GetNextToken();
					if (tk.at(0) == equal)
					{
						tk = GetNextToken();
						encoding = tk;
						xmlattritem.attrValue = tk;
					}
				}
				else
				{
					tk = GetNextToken();
					if (tk.at(0) == equal)
					{
						tk = GetNextToken();
						xmlattritem.attrValue = tk;
					}
				}
				xmlheadline.attr.push_back(xmlattritem);
				tk = PeekNextToken();
			}
			tk = GetNextToken();
			if (!tk.empty() && tk.at(0) == question_mark )
			{
				tk = PeekNextToken();
				if (!tk.empty() && tk.at(0) == rightAnglebrackets)
				{
					tk = GetNextToken();
				}
			}			
		}	
		return xmlheadline;
	}
	void parseComment()
	{
		std::string tk= GetNextToken();
		//tk= GetNextToken();
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
	void parseDTD()
	{
		parseComment();
	}
	bool parseData(const char * xStr,size_t xLen,std::string &encoding, XmlNode & root, XmlHeadLines& xmlheadlines)
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
		std::string token=GetNextToken();
		if(token.at(0)==leftAnglebrackets)
		{			
			root = parseNode(encoding, xmlheadlines);
		}		
		return true;
	}
	XmlNode parseNode(std::string &encoding ,XmlHeadLines& xmlheadlines)
	{
		XmlNode node;
		std::string temp = PeekNextToken();	
		while(!temp.empty())
		{
			if(temp.at(0)==question_mark)
			{		
				//尝试解析Headline	
				XmlHeadLine xmlheadline = parseHeadLine(encoding);
				xmlheadlines.push_back(xmlheadline);
				temp=GetNextToken();
				temp = PeekNextToken();	
				continue;
			}
			else if(temp.at(0)==exclamation_mark)
			{		
				//尝试解析并跳过注释
				temp = GetNextToken();
				temp = PeekNextToken();
				if (temp == "DOCTYPE")
				{
					parseDTD();
				}
				else
				{
					parseComment();
				}				
				temp = GetNextToken();
				temp = PeekNextToken();
				continue;
			}
			else if(temp.at(0)!=exclamation_mark && temp.at(0)!=leftAnglebrackets)
			{			
				node.name = GetNextToken();
				std::string tk=GetNextToken();
				bool gotdata = false;
				while(!tk.empty() && tk.at(0)!=rightAnglebrackets && tk.at(0)!= slash)
				{
					std::string attrName = tk;
					tk= GetNextToken();
					if(!tk.empty() && tk.at(0)==equal)
					{
						tk= GetNextToken();
						std::string attrValue =tk;
						node.SetAttr(attrName.c_str(),attrValue.c_str());
						tk = GetNextToken();
					}			
				}
				if (!tk.empty() && tk.at(0) == slash)
				{
					std::string temp = PeekNextToken();
					if (!temp.empty() && temp.at(0) == rightAnglebrackets)
					{
						temp = GetNextToken();
						break;
					}
				}
				if (!tk.empty() && tk.at(0) == rightAnglebrackets)
				{
					if (node.name == "input"||
						node.name == "br"||
						node.name == "hr"||
						node.name == "img"||
						node.name == "option"||
						node.name == "meta"||
						node.name == "link")
					{
						break;
					}					
				}
			recheck:
				if (stricmp(node.name.c_str(), "script")==0)
				{
					tk = GetNextToken_NodeStart(true);
				}
				else
				{
					tk = GetNextToken_NodeStart(false);
				}
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
						XmlNode subnode =  parseNode(encoding, xmlheadlines);
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
	std::string GetNextToken_NodeStart(bool bscript)
	{		
		std::string tk;
		bool gotstring = false;
		char * buffer = NULL;
		size_t cursor = currentpos;
		unsigned char c = *(xmlStr + cursor);
		while (isspace(c))
		{
			cursor++;
			currentpos++;
			c = *(xmlStr + cursor);
		}
		if (c == double_quotation_marks && !gotstring)
		{
			gotstring = true;
			cursor++;
			currentpos++;			
		}
		if (c == single_quotation_marks && !gotstring)
		{
			gotstring = true;
			cursor++;
			currentpos++;
		}
		if (c == slash)
		{
			if ((*(xmlStr + cursor + 1)) == slash)
			{
				cursor++;
				cursor++;
				c = *(xmlStr + cursor);
				while (c != '\n')
				{
					cursor++;
					c = *(xmlStr + cursor);
				}
			}
		}
		while (cursor < xmlLen)
		{
			c = *(xmlStr + cursor);		
				
			if (c == leftAnglebrackets 
				)
			{
				if (cursor == currentpos)
				{
					currentpos++;
					if (!gotstring)
						tk = c;
					break;
				}
				else
				{
					if(bscript )
					{
						if ((*(xmlStr + cursor + 1)) == slash)
						{
							int len = cursor - currentpos;
							buffer = (char *)malloc(len + 1);
							if (buffer)
							{
								memset(buffer, 0, len + 1);
								memcpy(buffer, xmlStr + currentpos, len);
								tk = buffer;
								free(buffer);
								currentpos = cursor;
								if (gotstring)
								{
									currentpos++;
								}
								break;
							}
						}
						
					}	
					else
					{
						int len = cursor - currentpos;
						buffer = (char *)malloc(len + 1);
						if (buffer)
						{
							memset(buffer, 0, len + 1);
							memcpy(buffer, xmlStr + currentpos, len);
							tk = buffer;
							free(buffer);
							currentpos = cursor;
							if (gotstring)
							{
								currentpos++;
							}
							break;
						}
					}
				}
			}
			cursor++;
		}
		if (cursor >= xmlLen)
		{
			currentpos = cursor;
		}
		return tk;
	}
	std::string GetNextToken()
	{
		bool gotstring = false;
		bool single_quotation_string = false;
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
			else if(c!= double_quotation_marks && gotstring && !single_quotation_string)
			{
				if (c == backslash)
				{
					if ((*(xmlStr + cursor + 1)) == double_quotation_marks)
					{
						cursor++;
						cursor++;
						continue;
					}
				}
				cursor++;
				continue;
			}
			if (c == single_quotation_marks && !gotstring)
			{
				gotstring = true;
				single_quotation_string = true;
				cursor++;
				currentpos++;
				continue;
			}
			else if (c != single_quotation_marks && gotstring && single_quotation_string)
			{
				if (c == backslash)
				{
					if ((*(xmlStr + cursor + 1)) == single_quotation_marks)
					{
						cursor++;
						cursor++;
						continue;
					}
				}
				cursor++;
				continue;
			}
			
			if(c == leftAnglebrackets ||
				c == rightAnglebrackets ||
				c == double_quotation_marks ||
				c == single_quotation_marks ||
				c == equal ||
				c == slash ||
				c == question_mark||
				c == exclamation_mark ||
				c == left_squarebrackets ||
				c == right_squarebrackets
				)
			{
				if (c == slash)
				{
					if (((*(xmlStr + cursor - 1)) != leftAnglebrackets) &&
						(((*(xmlStr + cursor + 1)) != rightAnglebrackets)))
					{
						cursor++;
						continue;
					}
				}				
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
			else if(isspace(c))
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

	bool parseString(std::string & s);//string's encode must be UTF-8
	bool parseFile(const char * FilePathName);
	bool saveToFile(const char * FilePathName,const char * encode="UTF-8");
	std::string GetEncodeType(const char *FilePathName);
	std::string GetHeaderLine(const char * s);
	std::string toString(const char * encode="UTF-8");
	XmlNode GetRoot();

	XmlHeadLines xmlheadlines;
	std::string xmlheadline;
	std::string encoding;
	XmlNode xmlRoot;
	XmlLex lex;

};
