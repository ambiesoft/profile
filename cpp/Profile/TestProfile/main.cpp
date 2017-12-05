#include <iostream>
#include "../Profile/profile.h"
// #include "C:/T/Profile/Profile/profile.h"

using namespace std;

using namespace Ambiesoft;

int main()
{
    // SomeFunction("aaa");


    Profile::ssss();

	HashIni* pIni = Profile::ReadAll("TestProfile.ini", false);

    cout << "Hello world!" << endl;
    return 0;
}
