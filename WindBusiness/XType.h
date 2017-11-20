#ifndef XTYPE_H_
#define XTYPE_H_
#include <string>
#include <fstream>

#ifdef UNICODE
#define xstring		std::wstring
#define xfstring	std::wfstream
#define xchar		wchar_t
#define X(p)		L ## p
#else
#define xstring		std::string
#define xfstring	std::fstream
#define xchar		char
#define X(p)		p
#endif


#endif