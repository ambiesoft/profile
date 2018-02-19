
#include <cassert>
#ifdef _DEBUG
#define VERIFY(s) assert(s)
#else
#define VERIFY(s) s;
#endif

#include "../Profile/profile.h"
// #include "C:/T/Profile/Profile/profile.h"

using namespace std;

using namespace Ambiesoft;

void testwrite()
{
	wstring filename = L"TestProfile.ini";
	HashIni* pIni = Profile::ReadAll(filename.c_str(), false);

	VERIFY(Profile::WriteInt(L"mysection", L"mykey", 12345, pIni));
	VERIFY(Profile::WriteInt(L"mysection", L"mykey", 54321, pIni));

	VERIFY(Profile::WriteInt(L"mysection", L"mykey2", 21, pIni));

	VERIFY(Profile::WriteInt(L"mysection2", L"mykey22", 21, pIni));
	VERIFY(Profile::WriteString(L"mysection2", L"mykey22", L"あああ", pIni));

	VERIFY(Profile::WriteAll(pIni, filename.c_str()));
	
	delete pIni;
}
void testread()
{
	wstring filename = L"TestProfile.ini";
	HashIni* pIni = Profile::ReadAll(filename.c_str(), false);

	Profile::ReadInt()
}
int main()
{
    Profile::ssss();

	testwrite();
	testread();
    return 0;
}
