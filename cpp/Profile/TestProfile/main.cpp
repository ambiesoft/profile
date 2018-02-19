
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
    string filename = "TestProfile.ini";
    HashIni* pIni = Profile::ReadAll(filename, false);

	VERIFY(Profile::WriteInt(L"mysection", L"mykey", 12345, pIni));
	VERIFY(Profile::WriteInt(L"mysection", L"mykey", 54321, pIni));

	VERIFY(Profile::WriteInt(L"mysection", L"mykey2", 21, pIni));

	VERIFY(Profile::WriteInt(L"mysection2", L"mykey21", 212, pIni));
	VERIFY(Profile::WriteString(L"mysection2", L"mykey22", L"あああ", pIni));

    VERIFY(Profile::WriteAll(pIni, filename));
	
	delete pIni;
}
void testread()
{
    string filename = "TestProfile.ini";
    HashIni* pIni = Profile::ReadAll(filename, false);

	int intval;
	wstring sval;
	Profile::GetInt(L"mysection", L"mykey", -1, intval, pIni);
	assert(intval == 54321);
	Profile::GetInt(L"mysection", L"mykey2", -1, intval, pIni);
	assert(intval == 21);
	Profile::GetInt(L"mysection2", L"mykey21", -1, intval, pIni);
	assert(intval == 212);

	Profile::GetString(L"mysection2", L"mykey22", wstring(), sval, pIni);
	assert(sval == L"あああ");

	delete pIni;
}
int main()
{
	testwrite();
	testread();
    return 0;
}
