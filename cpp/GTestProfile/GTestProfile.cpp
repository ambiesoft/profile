// GTestProfile.cpp : Defines the entry point for the console application.
//
#include <string>
#include "../Profile/include/ambiesoft.profile.h"

// run git submodule update -i
#include <gtest/gtest.h>

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32)) && (!defined(__CYGWIN__) && !defined(__GNUC__))
#define HAS_WCHAR_PROFILE
#endif

using namespace Ambiesoft;
using namespace std;
TEST(ProfileTest, Write)
{
    EXPECT_TRUE(Profile::WriteInt("test1", "key1", 1, "inifile.ini"));
}

TEST(ProfileTest, Read)
{
	EXPECT_TRUE(Profile::WriteInt("test1", "key1", 1, "inifile.ini"));

	int val = 0;
	EXPECT_TRUE(Profile::GetInt("test1", "key1", 0, val, "inifile.ini"));
	EXPECT_EQ(val, 1);
}

TEST(ProfileTest, WCharTest)
{
#ifdef HAS_WCHAR_PROFILE
	// S-W (string write, wstiring read)
	{
		EXPECT_TRUE(Profile::WriteInt("test1", "key1", 1, "inifile.ini"));

		int val = 0;
		EXPECT_TRUE(Profile::GetInt("test1", "key1", 0, val, L"inifile.ini"));
		EXPECT_EQ(val, 1);
	}


	// W-S (string write, wstiring read)
	{

        EXPECT_TRUE(Profile::WriteInt("test1", "key1", 11233, L"inifile.ini"));

		int val = 0;
		EXPECT_TRUE(Profile::GetInt("test1", "key1", 0, val, "inifile.ini"));
		EXPECT_EQ(val, 11233);
	}


	// W-W (string write, wstiring read)
	{
		EXPECT_TRUE(Profile::WriteInt("test1", "key1", 54321, L"inifile.ini"));

		int val = 0;
		EXPECT_TRUE(Profile::GetInt("test1", "key1", 0, val, L"inifile.ini"));
		EXPECT_EQ(val, 54321);
	}
#endif
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
