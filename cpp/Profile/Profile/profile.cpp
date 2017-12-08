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

std::pair<wstring,wstring> splitLine(const wstring& line)
{
    size_t pos = line.find(L'=',0);
    if(pos==std::nopos)
        hihihu
    wstring key=line.substr(0,pos);
}
static wstring Profile::getSectionName(const wstring& line)
{
    wstring ret;
    if(line.empty() || line[0]!=L'[')
        return ret;

    for(int i=1;i <line.size(); ++i)
    {
        wchar_t c = line[i];
        if(c==L']')
            break;

        ret.append(c);
    }
    return ret;
}
HashIni* Profile::ReadAll(const char* file, bool throwexception)
{
	HashIni* hi = HashIni::CreateEmptyInstanceForSpecialUse();
	HashIni::HashAll& al = hi->Hash();

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
			shared_ptr<HashSection> cursec = nullptr;

			while (getline(fs,line))
			{
				boost::algorithm::trim_left(line);
				if (line.empty() || line[0] == L'#')
					continue;

				if(line[0]==L'[')
                {
                    wcstring secname = GetSectionName(line);
                    cursec = al[secname];
                    if(!cursec)
                    {
                        cursec=make_shared(new HashIni::HashSection());
                        al[secname]=cursec;
                    }
                    continue;
                }
                else  // not a section line
                {
                    if(!cursec)
                        continue;

                    std::pair<wstring,wstring> vals = splitLine(line);

                }

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

