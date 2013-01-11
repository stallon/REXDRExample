#pragma once

#include <string>

class Helper
{
public:
	static std::string ConvertToUTF8(const std::string & str );
	static std::string ConvertToANSI(const std::string & str );

private:
	static const int MAX_FIELD_LEN;
};