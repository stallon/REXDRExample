#pragma once

#include <string>

class Helper
{
public:
	static const int MAX_FIELD_LEN;

	static std::string ConvertToUTF8(const std::string & str );
	static std::string ConvertToANSI(const std::string & str );
};