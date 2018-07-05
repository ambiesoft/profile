// GTestProfile.cpp : Defines the entry point for the console application.
//
#include "../Profile/include/ambiesoft.profile.h"
#include <gtest/gtest.h>

using namespace Ambiesoft;

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

TEST(ProfileTest, Exception)
{
    try
    {
        Profile::ReadAll("\";;;;;;;;:::", true);
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
