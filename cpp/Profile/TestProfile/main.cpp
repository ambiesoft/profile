
#include <cassert>
#ifdef _DEBUG
#define VERIFY(s) assert(s)
#else
#define VERIFY(s) s;
#endif

#include "../Profile/profile.h"


using namespace std;

using namespace Ambiesoft;

void testwrite(string filename)
{
    HashIniHandle ini = Profile::ReadAll(filename, false);

	VERIFY(Profile::WriteInt("mysection", "mykey", 12345, ini));
	VERIFY(Profile::WriteInt("mysection", "mykey", 54321, ini));

	VERIFY(Profile::WriteInt("mysection", "mykey2", 21, ini));

	VERIFY(Profile::WriteInt("mysection2", "mykey21", 212, ini));
	VERIFY(Profile::WriteString("mysection2", "mykey22", u8"あああ", ini));

    VERIFY(Profile::WriteAll(ini, filename));
	
	Profile::FreeHandle(ini);
}
void testread(string filename)
{
    HashIniHandle ini = Profile::ReadAll(filename, false);

	int intval;
	string sval;
	Profile::GetInt("mysection", "mykey", -1, intval, ini);
	assert(intval == 54321);
	Profile::GetInt("mysection", "mykey2", -1, intval, ini);
	assert(intval == 21);
	Profile::GetInt("mysection2", "mykey21", -1, intval, ini);
	assert(intval == 212);

	Profile::GetString("mysection2", "mykey22", string(), sval, ini);
	assert(sval == u8"あああ");

	Profile::FreeHandle(ini);
}
bool file_exists(string file)
{
	ifstream my_file(file);
	return my_file.good();
}
int main()
{
	string filename = "TestProfile.ini";
	if (file_exists(filename))
		testread(filename);

	testwrite(filename);
	testread(filename);
    return 0;
}
