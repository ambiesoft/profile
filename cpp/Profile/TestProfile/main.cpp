//MIT License
//
//Copyright (c) 2018 Ambiesoft
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.


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
void testdirect(const string& inifile)
{
	Profile::WriteInt("MySection", "MyKey", 666666, inifile);
	
	int intval;
	Profile::GetInt("MySection", "MyKey", 0, intval, inifile);
	assert(intval == 666666);
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

	testdirect(filename);
	return 0;
}
