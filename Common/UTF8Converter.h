#pragma once

#include <string>

namespace Utility
{
	class UTF8Converter
	{
	public:
		static std::string ConvertToUTF8(const std::string & str );
		static std::string ConvertToANSI(const std::string & str );

	private:
		static const int MAX_FIELD_LEN;
	};

}	// end-of namespace