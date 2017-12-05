#include <fstream>
#include <codecvt>

#include <boost/algorithm/string/trim.hpp>

#include "profile.h"

using namespace std;
namespace Ambiesoft
{
// a sample exported function
void DLL_EXPORT SomeFunction(const LPCSTR sometext)
{
    MessageBoxA(0, sometext, "DLL Message", MB_OK | MB_ICONINFORMATION);
}

void DLL_EXPORT Profile::ssss()
{

}

HashIni* Profile::ReadAll(const char* file, bool throwexception)
{
	HashIni* hi = HashIni::CreateEmptyInstanceForSpecialUse();
	HashIni::HashType& al = hi->Hash();

	try
	{
		// TODO: implement mutex
		//mutex = createmutex(inipath);
		//waitmutex(mutex);

		std::wifstream fs;
		fs.open(file);
		static_assert(sizeof(wchar_t) == 2, "error.");//Linux is no ready
		fs.imbue(std::locale(std::locale(""), new std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::consume_header>()));
		if (fs)
		{
			wstring line;
			
			while (getline(fs,line))
			{
				boost::algorithm::trim_left(line);
				if (line.empty() || line[0] == L'#')
					continue;

				// TODO: from here, check section format([aaa])
			}
		}
		return hi;
	}
	catch (std::exception& e)
	{ 
		if (throwexception)
			throw e;
	}
	return hi;
}

} // namespace

