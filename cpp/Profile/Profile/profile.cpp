#include "profile.h"

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

HashIni* DLL_EXPORT Profile::ReadAll(const char* file)
{
    HashIni* hi = HashIni::CreateEmptyInstanceForSpecialUse();
    HashIni::HashType& al = hi->Hash();

    try
    {
        std::wifstream fs;
        fs.open(file);
        static_assert(sizeof(wchar_t) == 2, "error.");//Linux‚Å‚Í‚Â‚©‚¤cvtˆá‚¤‚©‚ç’¼‚µ‚Ä‚­‚ê
        fs.imbue(std::locale(std::locale(""), new std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::consume_header>()));
        if(!fs)
            return hi;
    }
}

} // namespace

