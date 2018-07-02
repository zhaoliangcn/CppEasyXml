// CppEasyXml.cpp : Defines the entry point for the console application.
//

/*
任何人都拥有平等的获取知识的权利

CppEasyXml 是开放源代码的软件，任何人都可以下载、使用、修改和重新发布，不必担心任何版权问题

请在重新发布的时候保留以上信息
*/

#include "stdafx.h"
#include "CppEasyXml.h"


CppEasyXml::CppEasyXml(void)
{
}

CppEasyXml::~CppEasyXml(void)
{
}
std::string CppEasyXml::toString(const char * encode)
{
	//std::string temp=xmlheadline;
	//if(stricmp(encode,"UTF-8")==0)
	//{
	//	size_t pos1=temp.find("encoding=\"");
	//	if(pos1!=std::string::npos)
	//	{
	//		pos1+=strlen("encoding=\"");
	//		size_t pos2=temp.find("\"",pos1+1);
	//		if(pos2!=std::string::npos)
	//		{
	//			temp.replace(pos1,pos2-pos1,encode);
	//		}
	//	}		
	//}
	std::string temp;
	for (int i = 0;i < xmlheadlines.size();i++)
	{
		temp += xmlheadlines.at(i).toStrng(encode);
		temp += "\n";
	}
	return temp+ xmlRoot.ToString();
	//return temp +"\n"+ xmlRoot.ToString();
}
XmlNode CppEasyXml::GetRoot()
{
	return xmlRoot;
}
bool CppEasyXml::parseString(std::string & s)
{
	xmlheadline = GetHeaderLine(s.c_str());
	lex.parseData(s.c_str(),s.length(),encoding, xmlRoot, xmlheadlines);
	return true;
}
std::string  CppEasyXml::GetHeaderLine(const char * s)
{
	std::string str;
	char  * headline =NULL;
	int length=0;
	char * p =(char *)strstr(s,"<?xml");
	char * q =NULL;
	if(p)
	{
		q=strstr(p+strlen("<?xml"),"?>");
		if(q)
		{
			q=q+strlen("?>");
			length=q-p;
			if(length>0)
			{
				headline=(char *)malloc(length+1);
				if(headline)
				{
					memset(headline,0,length+1);
					strncpy_s(headline,length+1,p,length);
					str=headline;
				}
			}			
		}
	}
	return str;
}
bool CppEasyXml::parseFile(const char * FilePathName)
{
	encoding = GetEncodeType(FilePathName);	
	std::string content ;
	BOOL bRet = FALSE;
	DWORD dwSize =0;
	char * Buffer=NULL;
	HANDLE hFile=CreateFileA(FilePathName,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		dwSize=GetFileSize(hFile,NULL);
		Buffer = (char *)malloc(dwSize);
		if(Buffer)
		{
			DWORD dwReadCount=0;
			bRet = ReadFile(hFile,Buffer,dwSize,&dwReadCount,NULL);
			if(encoding == "ISO-10646-UCS-2")
			{
				content = W2UTF((wchar_t *)Buffer);
			}
			else if(encoding =="UTF-8")
			{				
				content = Buffer;
			}	
			else if (encoding == "ISO-8859-1")
			{
				content = AToU(Buffer);
			}
			else
			{
				content = Buffer;
			}
			free(Buffer);
		}
		CloseHandle(hFile);
	}
	return parseString(content);
}
bool CppEasyXml::saveToFile(const char * FilePathName,const char * encode)
{
	BOOL bRet = FALSE;
	HANDLE hFile=CreateFileA(FilePathName,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		std::string xxstr=toString();		
		DWORD dwWriteCount=0;
		bRet = WriteFile(hFile,xxstr.c_str(),xxstr.length(),&dwWriteCount,NULL);		
		CloseHandle(hFile);
	}
	return bRet;
}
std::string CppEasyXml::GetEncodeType(const char *FilePathName)
{
	std::string encode;
	BOOL bRet = FALSE;
	HANDLE hFile=CreateFileA(FilePathName,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		char Buffer[1024]={0};
		std::string pBuffer;
		DWORD dwWriteCount=0;
		bRet = ReadFile(hFile,Buffer,1024,&dwWriteCount,NULL);
		if(IsTextUnicode(Buffer,1024,NULL))
		{
			pBuffer = W2UTF((wchar_t *)Buffer);
		}
		else
		{
			pBuffer = Buffer;
		}
		char * p =(char *)strstr(pBuffer.c_str(),"encoding=\"");		
		if(p)
		{
			encode=p+strlen("encoding=\"");
			size_t pos =encode.find("\"");
			if(pos!=std::string::npos)
			{
				encode=encode.substr(0,pos);
			}
		}
		else
		{
			p = (char *)strstr(pBuffer.c_str(), "encoding=\'");
			if (p)
			{
				encode = p + strlen("encoding=\'");
				size_t pos = encode.find("\'");
				if (pos != std::string::npos)
				{
					encode = encode.substr(0, pos);
				}
			}
		}
		CloseHandle(hFile);
	}
	return encode;
}