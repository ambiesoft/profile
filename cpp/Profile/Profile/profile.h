#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

namespace Ambiesoft {
    class DLL_EXPORT HashIni
    {
        typedef HashType std::map<std::wstring, std::wstring>;
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
        static void DLL_EXPORT ssss();

        static HashIni* DLL_EXPORT ReadAll(const char* file);
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
