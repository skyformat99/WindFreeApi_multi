#ifndef XSTRING_H_
#define XSTRING_H_

namespace XMyTools
{
	class XString
	{
	public:
		XString();
		~XString();

		int strlen(const char * szText);
		double atof(char *src);
	};

}// XMyTools end

#endif

