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

#include "stdafx.h"

using namespace System;
using namespace System::Collections;
using namespace Ambiesoft;

void errorhappen111()
{
	Console::WriteLine("EEEEEEEEEEEEEEEEEEEEEEE11111111111111111");
}

bool checksamefile(LPCTSTR pF1, LPCTSTR pF2)
{
	bool ret =false;
	BYTE* pB1=NULL, *pB2=NULL;
	HANDLE h1 = CreateFile(pF1,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	HANDLE h2 = CreateFile(pF2,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if(h1==INVALID_HANDLE_VALUE || h2==INVALID_HANDLE_VALUE)
		goto error;

	DWORD size1 = GetFileSize(h1,NULL);
	DWORD size2 = GetFileSize(h2,NULL);
	if(size1 != size2)
		goto error;

	pB1 = (BYTE*)malloc(size1);
	pB2 = (BYTE*)malloc(size2);

	DWORD dwRead1, dwRead2;
	ReadFile(h1, pB1, size1, &dwRead1, NULL);
	ReadFile(h2, pB2, size2, &dwRead2, NULL);

	if(memcmp(pB1, pB2, size1) != 0)
		goto error;

	ret = true;
error:
	CloseHandle(h1);
	CloseHandle(h2);
	free(pB1);
	free(pB2);

	return ret;
}

bool checksamestring(LPCTSTR sec, LPCTSTR key, LPCTSTR file)
{
	String^ a1="a1";
	String^ a2="a2";

	TCHAR buff[256];
	GetPrivateProfileString(sec, key, _T(""), buff, sizeof(buff), file);
	a1 = gcnew String(buff);

	
	Profile::GetString(gcnew String(sec), gcnew String(key), _T(""), a2, gcnew String(file));
	
	if (a1 == a2)
		return true;

	Console::Error->WriteLine(String::Format("string failed:sec={0}, key={1}, file={2} ({3} != {4}",
		gcnew String(sec), gcnew String(key), gcnew String(file), a1, a2));
	return false;
}
bool checksameint(LPCTSTR sec, LPCTSTR key, LPCTSTR file)
{
	int a1=1;
	int a2=2;
	a1 = GetPrivateProfileInt(sec, key, 0, file);
	Profile::GetInt(gcnew String(sec), gcnew String(key), 0, a2, gcnew String(file));
	if (a1 == a2)
		return true;

	Console::Error->WriteLine(String::Format("int failed:sec={0}, key={1}, file={2} (win32 != Profile)({3} != {4}",
		gcnew String(sec),gcnew String(key),gcnew String(file) , a1, a2));
	return false;
}

bool checksamestringandint(LPCTSTR sec, LPCTSTR key, LPCTSTR file)
{
	if (!checksamestring(sec, key, file))
		return false;
	if (!checksameint(sec, key, file))
		return false;

	return true;
}

void temptest()
{
	Profile::ReadAll("C:\\T\\test.ini");
	Profile::ReadAll("Test.otita");
}

int main(array<System::String ^> ^args)
{
	temptest();
	
	TCHAR szBuff[32];
	TCHAR szCurrentDir[512];
	GetCurrentDirectory(512, szCurrentDir);
	lstrcat(szCurrentDir, _T("\\"));

	TCHAR szFile[512];
	lstrcpy(szFile, szCurrentDir);
	lstrcat(szFile, _T("Test.wini"));


	// API SPACE test -> get is trimmed
	{
		WritePrivateProfileString(_T("SECTION_W"), _T("SPACE"), _T("THEVALUE	"), szFile);
		GetPrivateProfileString(_T("SECTION_W"), _T("SPACE"), _T(""), szBuff, 32, szFile);
	}
	WritePrivateProfileString(_T("SECTION_W"), _T("THEKEY"), _T("THEVALUE"), szFile);

	Profile::WriteString("SECTION_W", "THEKEY", "THEVALUE", "Test.wini");
	Profile::WriteString("SECTION_W", "THEKEY", "THEVALUE", "Test.wini");
	String^ theValue;
	Profile::GetString("SECTION_W", "THEKEY", "", theValue, "Test.wini");
	if(theValue != "THEVALUE")
	{
		errorhappen111();
	}

	Profile::WriteInt("SECTION_W", "THEINT", 333, "Test.wini");
	int the333;
	Profile::GetInt("SECTION_W", "THEINT", 2423, the333, "Test.wini");
	if(the333!=333)
	{
		errorhappen111();
	}

	int a;
	Profile::GetInt("SECTION_A", "AAA", 0, a, "Test.wini");


	String^ s;
	Profile::GetString("SECTION_A", "AAA", String::Empty, s, "Test.wini");
	HashIni^ hi = Profile::ReadAll("Test.wini");
	Profile::WriteString("SECTION_Y", "YYY", "tttt", hi);
	Profile::WriteAll(hi, "Test.wini.new");


	System::Collections::Generic::List<String^> ar;
	ar.Add("111");
	ar.Add("222");
	ar.Add("333");
	Profile::WriteStringArray("SEC_AA", "AR", ar.ToArray(), hi);
	Profile::WriteAll(hi, "Test.wini.new2");

	HashIni^ hi2 = Profile::ReadAll("Test.wini.new2");
	array<String^>^ getar;
	Profile::GetStringArray("SEC_AA", "AR", getar, hi2);

	Profile::WriteInt("SEC_AA", "AR", 5000, "Test.wini.new2");

	array<String^>^ getar2;
	Profile::GetStringArray("SEC_AA", "AR", getar2, hi2);









	GetCurrentDirectory(512, szFile);
	lstrcat(szFile, _T("\\Test.wini3"));

	WritePrivateProfileString(_T("AppName"), _T("KeyName"), _T("VVVVV"), szFile);
	String^ vvv;
	Profile::GetString(_T("AppName"), _T("KeyName"), _T(""), vvv, _T("Test.wini"));

	{
		BYTE byt[128];
		byt[0]=0;
		byt[1]=1;
		byt[2]=2;
		byt[127]=0xee;
		WritePrivateProfileStruct(_T("AppName"), _T("Binary"), byt, sizeof(byt), szFile);

		array<System::Byte>^ arRet;
		Profile::GetBinary(_T("AppName"), _T("Binary"), arRet, gcnew String(szFile));

		if(!arRet || arRet->Length!=128)
		{
			errorhappen111();
		}
		try
		{
			for(int i=0 ; i<128 ; ++i)
			{
				if(byt[i]!= arRet[i])
				{
					errorhappen111();

				}
			}
		}
		catch(Exception^)
		{
			errorhappen111();
		}



		array<System::Byte>^ arW=gcnew array<System::Byte>(128);
		arW[0]=0;
		arW[1]=2;
		arW[2]=5;
		arW[127]=0xee;
		Profile::WriteBinary(_T("AppName"), _T("Binary"), arW, gcnew String(szFile));
		BYTE bytr[128];
		if(!GetPrivateProfileStruct(_T("AppName"), _T("Binary"), bytr, sizeof(bytr), szFile))
			Console::WriteLine("FDSAF");
		try
		{
			for(int i=0 ; i<128 ; ++i)
			{
				if(bytr[i]!= arW[i])
				{
					errorhappen111();

				}
			}
		}
		catch(Exception^)
		{
			errorhappen111();
		}
	}

	{
		BYTE byt[3];
		byt[0]=0;
		byt[1]=1;
		byt[2]=2;
		WritePrivateProfileStruct(_T("AppName"), _T("Binary3"), byt, sizeof(byt), szFile);
	}

	{
		BYTE byt[1];
		byt[0]=5;
		WritePrivateProfileStruct(_T("AppName"), _T("Binary4"), byt, sizeof(byt), szFile);
	}


	Profile::WriteBinary(_T("AppName"), _T("Binary"), gcnew array<System::Byte>(0), gcnew String(szFile));
	Profile::WriteBinary(_T("AppName"), _T("Binary"), nullptr, gcnew String(szFile));

	WritePrivateProfileString(NULL,NULL,_T("AAA"),szFile);
	Profile:: WriteString(nullptr,nullptr,nullptr, (gcnew String(szFile))+"xxx");
	Profile:: WriteString("AAA",nullptr,nullptr, (gcnew String(szFile))+"xxx");

	GetPrivateProfileString(NULL,NULL,NULL,NULL,0,NULL);
	Profile::GetString(nullptr,nullptr,nullptr,s,(gcnew String(szFile))+"xxx");
	Profile::GetString("AAA",nullptr,nullptr,s,(gcnew String(szFile))+"xxx");



	GetCurrentDirectory(512, szFile);
	lstrcat(szFile, _T("\\Test.wini4"));
	int po;
	//WritePrivateProfileString(L"SEC", L"PlayOnly", L" 5", szFile);
	GetPrivateProfileString(_T("SEC"), _T("PlayOnly"), _T(""), szBuff, sizeof(szBuff), szFile);
	int poo=GetPrivateProfileInt(L"SEC", L"PlayOnly", 0, szFile);
	Profile::GetInt("SEC", "PlayOnly", 0, po, gcnew String(szFile));
	if(po!=poo)
	{
		errorhappen111();
	}

	if(!checksamestring(_T("SEC"), _T("aaa"), szFile))
		errorhappen111();
	if(!checksameint(_T("SEC"), _T("aaa"), szFile))
		errorhappen111();

	if(!checksamestring(_T("SEC"), _T("bbb"), szFile))
		errorhappen111();
	if(!checksameint(_T("SEC"), _T("bbb"), szFile))
		errorhappen111();

	if(!checksamestring(_T("SEC"), _T("ccc"), szFile))
		errorhappen111();
	if(!checksameint(_T("SEC"), _T("ccc"), szFile))
		errorhappen111();

	if(!checksamestringandint(_T("SEC"), _T("d"), szFile))
		errorhappen111();
	if(!checksamestringandint(_T("SEC"), _T("e"), szFile))
		errorhappen111();
	if(!checksamestringandint(_T("SEC"), _T("f"), szFile))
		errorhappen111();
	if(!checksamestringandint(_T("SEC"), _T("g"), szFile))
		errorhappen111();
	if(!checksamestringandint(_T("SEC"), _T("h"), szFile))
		errorhappen111();



	
	//GetCurrentDirectory(512, szFile);
	//lstrcat(szFile, _T("\\Test.wini5"));
	//WritePrivateProfileStringW(L"SEC", L"space", L"   ", szFile);



	// multiline
	{
		TCHAR szFile2[MAX_PATH];

		GetCurrentDirectory(512, szFile);
		lstrcat(szFile, _T("\\newfile1"));

		GetCurrentDirectory(512, szFile2);
		lstrcat(szFile2, _T("\\newfile2"));

		LPCTSTR pMultiLine=_T("AAA\r\nBBB\r\nCCC");
		WritePrivateProfileStringW(L"SEC", L"MM", pMultiLine, szFile);
		Profile::WriteString(L"SEC", L"MM", gcnew String(pMultiLine), gcnew String(szFile2));

		if(!checksamefile(szFile, szFile2))
			errorhappen111();

		TCHAR szT[36];
		GetPrivateProfileStringW(L"SEC", L"MM", L"",szT,36, szFile);

		if(!checksamestringandint(_T("SEC"), _T("MM"), szFile))
			errorhappen111();



	}
		
	
	// section format test (Fixed)
	{
		TCHAR szFileSecTest[MAX_PATH] ;
		lstrcpy(szFileSecTest, szCurrentDir);
		lstrcat(szFileSecTest, L"Test.SectionFormat.wini");
		if (!checksamestringandint(_T("SEC"), _T("a"), szFileSecTest))
			errorhappen111();
	}

	// dont change existing
	// API add new string at last
	// API tries to keep the file as it is
	{
		TCHAR szFileChangeExistingTest[MAX_PATH];
		lstrcpy(szFileChangeExistingTest, szCurrentDir);
		lstrcat(szFileChangeExistingTest, L"still.wini");
		System::IO::StreamWriter sw(gcnew String(szFileChangeExistingTest));
		sw.WriteLine("STILL");
		sw.WriteLine("[SEC]");
		sw.WriteLine("APPXX");
		sw.WriteLine("");
		sw.WriteLine("BPP");
		sw.WriteLine("");
		sw.WriteLine("");
		sw.WriteLine("APP=ssss");
		sw.Close();

		WritePrivateProfileString(L"SEC", L"APP", L"AAA", szFileChangeExistingTest);
	}
	return 0;
}

void unusedfunc()
{
	array<String^>^ aa;
	HashIni^ hi;
	Profile::GetStringArray("aaa","bbb", aa, hi);
}