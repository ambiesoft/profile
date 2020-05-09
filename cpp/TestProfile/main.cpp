﻿//MIT License
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
#include <string>
#include <vector>
#include <memory>
#include <cstring>




#ifdef _DEBUG
#define VERIFY(s) assert(s)
#else
#define VERIFY(s) s;
#endif

#include <ambiesoft.profile.h>


using namespace std;

using namespace Ambiesoft;

template<class C>
void testwrite(std::basic_string<C, std::char_traits<C>, std::allocator<C>> filename)
{
    Profile::CHashIni ini = Profile::ReadAll(filename, false);

    VERIFY(Profile::WriteInt("mysection", "mykey", 12345, ini));
    VERIFY(Profile::WriteInt("mysection", "mykey", 54321, ini));

    VERIFY(Profile::WriteInt("mysection", "mykey2", 21, ini));
    VERIFY(Profile::WriteInt("mysection2", "mykey21", 212, ini));

#if _MSC_VER > 1800
    VERIFY(Profile::WriteString("mysection2", "mykey22", u8"あああ", ini));
#endif

    vector<string> v;
    v.push_back("aaa");
    v.push_back("bbb");
    VERIFY(Profile::WriteStringArray("sa", "sak", v, ini));

    VERIFY(Profile::WriteAll(ini, filename));
}
void testread(const string& filename)
{
    Profile::CHashIni ini = Profile::ReadAll(filename, false);

    int intval;
    string sval;
    Profile::GetInt("mysection", "mykey", -1, intval, ini);
    assert(intval == 54321);
    Profile::GetInt("mysection", "mykey2", -1, intval, ini);
    assert(intval == 21);
    Profile::GetInt("mysection2", "mykey21", -1, intval, ini);
    assert(intval == 212);

#if defined(_MSC_VER) && _MSC_VER > 1800
    //Profile::GetString("mysection2", "mykey22", string(), sval, ini);
    //assert(sval == u8"あああ");
#endif

    vector<string> v;
    Profile::GetStringArray("sa", "sak", v, ini);
    assert(v.size() == 2);
    assert(v[0] == "aaa");
    assert(v[1] == "bbb");
}

template<class C>
void testdirect(const std::basic_string<C, std::char_traits<C>, std::allocator<C>>& inifile)
{
    Profile::WriteInt("MySection", "MyKey", 666666, inifile);

    int intval;
    Profile::GetInt("MySection", "MyKey", 0, intval, inifile);
    assert(intval == 666666);


    vector<unsigned char> v;
    v.push_back(11);
    v.push_back(22);
    v.push_back(33);
    Profile::WriteBinary("MyBinarySection", "MyBinaryKey", v, inifile);

    vector<unsigned char> vout;
    Profile::GetBinary("MyBinarySection", "MyBinaryKey", vout, inifile);

    assert(v == vout);


    // bool
    bool b;
    Profile::WriteBool("MBS", "MKB", true, inifile);
    Profile::GetBool("MBS", "MKB", false, b, inifile);
    assert(b == true);

    Profile::WriteBool("MBS", "MKB", false, inifile);
    Profile::GetBool("MBS", "MKB", true, b, inifile);
    assert(!b);


    // vector<string>
    vector<string> vs;
    vs.push_back("abc");
    vs.push_back("xyz");
    vs.push_back("tre");
    vs.push_back("faaaaaaae" );
    Profile::WriteStringArray("vs", "vsk", vs, inifile);
    vector<string> vsout;
    Profile::GetStringArray("vs", "vsk", vsout, inifile);
    assert(vs == vsout);


    srand(static_cast<unsigned int>(time(nullptr)));

    // binay
    struct TestS
    {
        int a;
        int b;
        char c;

        int random[100];

        TestS()
        {
            a = 0;
            b = 0;
            c = 0;
        }
        void set()
        {
            a = 111;
            b = 1234555;
            c = 33;

            for(size_t i=0 ; i < (sizeof(random)/sizeof(random[0])); ++i)
            {
                random[i] = rand();
            }
        }
    };

    TestS ts;
    ts.set();
    Profile::WriteBinary("ba", "bskey", (unsigned char*)&ts, sizeof(ts), inifile);

    TestS tsout;
    Profile::GetBinary("ba", "bskey", (unsigned char*)&tsout, inifile);

    assert(memcmp(&ts, &tsout, sizeof(ts)) == 0);

//	const wchar_t* pU = L"ユーティーエフ１６斷言、宣稱、主張、聲明어서션(assertion) 또는";
//	Profile::WriteString("UTF16APP", "UTF16KEY", pU, inifile);
//	std::wstring ws;
//	Profile::GetString("UTF16APP", "UTF16KEY", L"", ws, inifile);
//	assert(ws == pU);

    string ss;
    Profile::WriteString("quote", "quote", "\"", inifile);
    Profile::GetString("quote", "quote", "", ss, inifile);
    assert(ss == "\"");
}

template<class C>
bool file_exists(std::basic_string<C, std::char_traits<C>, std::allocator<C>> file)
{
    ifstream my_file(file);
    return my_file.good();
}
int main()
{
    {
        string filename = "TestProfile.ini";
        if (file_exists(filename))
            testread(filename);

        testwrite(filename);
        testread(filename);

        testdirect(filename);
    }

    return 0;
}
