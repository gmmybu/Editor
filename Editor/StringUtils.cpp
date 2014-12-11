#include "stdafx.h"
#include "StringUtils.h"
#include <algorithm>
#include <Windows.h>

int StringUtils::compare(std::string l, std::string r)
{
	std::transform(l.begin(), l.end(), l.begin(), tolower);
	std::transform(r.begin(), r.end(), r.begin(), tolower);
	return l.compare(r);
}

std::string StringUtils::utf8ToGbk(std::string src)
{
	std::string res;
	if(src.empty())
		return res;

	// utf8 -> unicode
	int count = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, NULL, 0);
	wchar_t *unicode = new wchar_t[count+1];
	memset(unicode, 0, 2*(count+1));
	MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, unicode, count);

	// unicode -> gbk
	count = WideCharToMultiByte(CP_ACP, 0, unicode, -1, NULL, 0, NULL, NULL);
	char *gbk = new char[count+1];
	memset(gbk, 0, count+1);
	WideCharToMultiByte(CP_ACP, 0, unicode, -1, gbk, count, NULL, NULL);
	res = gbk;

	delete[] unicode;
	delete[] gbk;

	return res;
}

std::string StringUtils::gbkToUtf8(std::string src)
{
	return std::string();
}

std::string StringUtils::externalPath(std::string prefix, std::string path)
{
	TCHAR pathBuffer[MAX_PATH];
	::GetFullPathName((prefix + "//" + path).c_str(), MAX_PATH, pathBuffer, NULL);
	return std::string(pathBuffer);
}

std::string StringUtils::internalPath(std::string prefix, std::string path)
{
	TCHAR pathBuffer[MAX_PATH];
	::GetFullPathName(prefix.c_str(), MAX_PATH, pathBuffer, NULL);
	return path.substr(strlen(pathBuffer));
}

std::string StringUtils::filename(std::string path)  
{  
    std::string filename;  
  
    int index = 0;  
    for(int i=path.length()-1; i>=0; i--)  
    {  
        if(path[i] == '.')  
        {  
            index = i;  
            break;  
        }  
    }  
    for(int i=0; i<index; i++)  
    {  
		filename.push_back(path[i]);  
    }  
  
    return filename;  
}  
  
std::string StringUtils::extension(std::string path)  
{  
    std::string extension;  
  
    int index = 0;  
    for(int i=path.length()-1; i>=0; i--)  
    {  
        if(path[i] == '.')  
        {  
            index = i+1;  
            break;  
        }  
    }  
    for(; index<path.length(); index++)  
    {  
		extension.push_back(path[index]);  
    }  
  
	std::transform(extension.begin(), extension.end(), extension.begin(), tolower);
    return extension;  
}
