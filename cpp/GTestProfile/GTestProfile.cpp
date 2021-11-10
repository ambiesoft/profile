// GTestProfile.cpp : Defines the entry point for the console application.
//
#include <string>
#include <deque>
#include <list>
#include "../Profile/include/ambiesoft.profile.h"

// run git submodule update -i
#include <gtest/gtest.h>

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32)) && (!defined(__CYGWIN__) && !defined(__GNUC__))
#define HAS_WCHAR_PROFILE
#endif

using namespace Ambiesoft;
using namespace std;
TEST(ProfileTest, Int)
{
	string iniFile("inifile.ini");
	{
		EXPECT_TRUE(Profile::WriteInt("test1", "key1", 1, iniFile));

		int val = 0;
		EXPECT_TRUE(Profile::GetInt("test1", "key1", 0, val, iniFile));
		EXPECT_EQ(val, 1);
	}

	{
		EXPECT_TRUE(Profile::WriteInt("mysection", "mykey", 12345, iniFile));
		EXPECT_TRUE(Profile::WriteInt("mysection", "mykey", 54321, iniFile));

		EXPECT_TRUE(Profile::WriteInt("mysection", "mykey2", 21, iniFile));
		EXPECT_TRUE(Profile::WriteInt("mysection2", "mykey21", 212, iniFile));

		int intval;
		Profile::GetInt("mysection", "mykey", -1, intval, iniFile);
		EXPECT_EQ(intval , 54321);
		Profile::GetInt("mysection", "mykey2", -1, intval, iniFile);
		EXPECT_EQ(intval , 21);
		Profile::GetInt("mysection2", "mykey21", -1, intval, iniFile);
		EXPECT_EQ(intval , 212);
	}
}

TEST(ProfileTest, Bool)
{
	string iniFile("inifile.ini");
	{
		bool b;
		Profile::WriteBool("MBS", "MKB", true, iniFile);
		Profile::GetBool("MBS", "MKB", false, b, iniFile);
		EXPECT_TRUE(b);

		Profile::WriteBool("MBS", "MKB", false, iniFile);
		Profile::GetBool("MBS", "MKB", true, b, iniFile);
		EXPECT_FALSE(b);
	}
}
#pragma warning(disable:4566)
TEST(ProfileTest, String)
{
	string iniFile("inifile.ini");
	{

		EXPECT_TRUE(Profile::WriteString("mysection2", "mykey22", u8"あああ", iniFile));

		string sval;
		Profile::GetString("mysection2", "mykey22", string(), sval, iniFile);
		EXPECT_EQ(sval, u8"あああ");
	}

	{
		{
			const char* pU8 = u8"ユーティーエフ１６斷言、宣稱、主張、聲明어서션(assertion) 또는";
			string s;
			Profile::WriteString("UTF8APP", "UTF8KEY", pU8, iniFile);
			Profile::GetString("UTF8APP", "UTF8KEY", "", s, iniFile);
			EXPECT_EQ(s , pU8);
		}

		{
			string ss;
			Profile::WriteString("quote", "quote", "\"", iniFile);
			Profile::GetString("quote", "quote", "", ss, iniFile);
			EXPECT_EQ(ss , "\"");
		}
	}
}

TEST(ProfileTest, StringArray)
{
	string iniFile("inifile.ini");
	{ 
		{
			vector<string> v;
			v.push_back("aaa");
			v.push_back("bbb");
			EXPECT_TRUE(Profile::WriteStringArray("sa", "sak", v, iniFile));
		}

		{
			vector<string> v;
			Profile::GetStringArray("sa", "sak", v, iniFile);
			EXPECT_EQ(v.size() , 2);
			EXPECT_EQ(v[0] , "aaa");
			EXPECT_EQ(v[1] , "bbb");
		}

	}

	{
		vector<string> vs;
		vs.push_back(u8"あああああああ");
		vs.push_back(u8"いいいいいいいい");
		vs.push_back(u8"うううううううううう");
		vs.push_back("faaaaaaae");
		Profile::WriteStringArray("vs", "vsk", vs, iniFile);
		vector<string> vsout;
		Profile::GetStringArray("vs", "vsk", vsout, iniFile);
		EXPECT_EQ(vs, vsout);
	}
	{
		vector<string> vs;
		vs.push_back(u8"あああああああ");
		vs.push_back(u8"いいいいいいいい");
		vs.push_back(u8"うううううううううう");
		vs.push_back("faaaaaaae");
		Profile::WriteStringArray("vs", "vsk", vs, iniFile);
		deque<string> vsout;
		Profile::GetStringArray("vs", "vsk", vsout, iniFile);
		EXPECT_EQ(vs.size(), vsout.size());
		for (size_t i = 0; i < vs.size(); ++i)
			EXPECT_EQ(vs[i], vsout[i]);
	}
	{
		vector<string> vs;
		vs.push_back(u8"あああああああ");
		vs.push_back(u8"いいいいいいいい");
		vs.push_back(u8"うううううううううう");
		vs.push_back("faaaaaaae");
		Profile::WriteStringArray("vs", "vsk", vs, iniFile);
		list<string> vsout;
		Profile::GetStringArray("vs", "vsk", vsout, iniFile);
		EXPECT_EQ(vs.size(), vsout.size());
		vector<string>::iterator itv = vs.begin();
		list<string>::iterator itl = vsout.begin();
		for (; itv != vs.end() && itl != vsout.end(); ++itv, ++itl)
			EXPECT_EQ(*itv, *itl);
	}
	{
		list<string> vs;
		vs.push_back(u8"あああああああ");
		vs.push_back(u8"いいいいいいいい");
		vs.push_back(u8"うううううううううう");
		vs.push_back("faaaaaaae");
		Profile::WriteStringArray("vs", "vsk", vs, iniFile);
		list<string> vsout;
		Profile::GetStringArray("vs", "vsk", vsout, iniFile);
		EXPECT_EQ(vs.size(), vsout.size());
		list<string>::iterator itv = vs.begin();
		list<string>::iterator itl = vsout.begin();
		for (; itv != vs.end() && itl != vsout.end(); ++itv, ++itl)
			EXPECT_EQ(*itv, *itl);
	}
}

TEST(ProfileTest, Binary)
{
	string iniFile("inifile.ini");
	srand(static_cast<unsigned int>(time(nullptr)));

	{
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

				for (size_t i = 0; i < (sizeof(random) / sizeof(random[0])); ++i)
				{
					random[i] = rand();
				}
			}
		};

		TestS ts;
		ts.set();
		Profile::WriteBinary("ba", "bskey", (unsigned char*)&ts, sizeof(ts), iniFile);

		TestS tsout;
		Profile::GetBinary("ba", "bskey", (unsigned char*)&tsout, iniFile);

		EXPECT_EQ(memcmp(&ts, &tsout, sizeof(ts)), 0);
	}

	{
		vector<unsigned char> v;
		v.push_back(11);
		v.push_back(22);
		v.push_back(33);
		Profile::WriteBinary("MyBinarySection", "MyBinaryKey", v, iniFile);

		vector<unsigned char> vout;
		Profile::GetBinary("MyBinarySection", "MyBinaryKey", vout, iniFile);

		EXPECT_EQ(v , vout);
	}
}


TEST(ProfileTest, Exception)
{
    try
    {
        Profile::ReadAll<char>("\";;;;;;;;:::", true);
        FAIL() << "Expected exception";
    }
    catch(file_not_found_error&)
    {

    }
    catch(file_not_opened_error&)
    {

    }
	catch (std::exception&)
	{
		FAIL() << "Unexpected Exception";
	}
    catch(...)
    {
        FAIL() << "Unexpected Exception";
    }
}

TEST(ProfileTest, IncludeEqual)
{
	string iniFile("inifileEqual.ini");
	EXPECT_TRUE(Profile::WriteString("SECTION_EQUAL", "KEY_EQUAL", "use-gl=desktop", iniFile));
	string val;
	EXPECT_TRUE(Profile::GetString("SECTION_EQUAL", "KEY_EQUAL", "", val, iniFile));
	EXPECT_STREQ("use-gl=desktop", val.c_str());
}