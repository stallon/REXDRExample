#include <windows.h>
#include "UTF8Converter.h"

namespace Utility
{
	const int UTF8Converter::MAX_FIELD_LEN = 1024;

	std::string UTF8Converter::ConvertToUTF8 (const std::string& str )
	{
		if (str.length() > MAX_FIELD_LEN )
		{
			*const_cast<std::string*>(&str) = str.substr(0, MAX_FIELD_LEN );
		}

		wchar_t wret [ MAX_FIELD_LEN + 1] = {0, };
		MultiByteToWideChar(CP_ACP , 0, str. c_str (), static_cast<int >( str.length()+1), wret , sizeof( wret ));

		std::wstring ws( wret );
		char ret [ MAX_FIELD_LEN + 1] = {0, };
		int len = WideCharToMultiByte( CP_UTF8 , 0, ws.c_str(), static_cast<int >( ws.length()+1), ret , sizeof( ret ), NULL , NULL);
		return std::string( ret );
	}

	std::string UTF8Converter::ConvertToANSI (const std::string& str )
	{
		if (str.length() > MAX_FIELD_LEN )
		{
			*const_cast<std::string*>(&str) = str.substr(0, MAX_FIELD_LEN );
		}

		wchar_t wret [ MAX_FIELD_LEN + 1] = {0, };
		MultiByteToWideChar (CP_UTF8 , 0, str.c_str (), static_cast<int>( str.length()+1), wret , sizeof( wret ));

		std ::wstring ws( wret );
		char ret [ MAX_FIELD_LEN + 1] = {0, };
		int len = WideCharToMultiByte( CP_ACP , 0, ws.c_str(), static_cast<int >( ws.length()+1), ret , sizeof( ret ), NULL , NULL);
		return std::string( ret );
	}

}	// end-of namespace