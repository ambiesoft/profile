#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>

#include <map>
#include <string>
#include <memory>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef PROFILE_EXPORTS
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

namespace Ambiesoft {
    class HashIni
    {
	public:
	    typedef std::map<std::wstring, std::wstring> HashSection;
		typedef std::map<std::wstring, std::shared_ptr<HashSection> > HashAll;


	private:
        HashAll hash_;
        HashIni()
        {
        }

    public:

        HashAll& Hash()
        {
            return hash_;
        }

        static HashIni* CreateEmptyInstanceForSpecialUse()
        {
            return new HashIni();
        }
    };

    struct DLL_EXPORT Profile
    {
        static void ssss();
        static wstring getSectionName(const wstring& line);
        static HashIni* ReadAll(const char* file, bool throwexception = false);
    };
}

#ifdef __cplusplus
extern "C"
{
#endif

namespace Ambiesoft {
void DLL_EXPORT SomeFunction(const LPCSTR sometext);

}


#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
