#pragma once
#include <string>

class Helper
{
public:
	static const int MAX_FIELD_LEN;

	static std::string convertToUTF8 (const std::string & str );
	static std::string convertToANSI (const std::string & str );
}