#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>

#include <map>
#include <string>

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
		typedef std::map<std::wstring, std::wstring> HashType;

	private:
        HashType hash_;
        HashIni()
        {
        }

    public:

        HashType& Hash()
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
