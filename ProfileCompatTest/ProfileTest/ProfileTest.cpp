

#include "stdafx.h"

using namespace System;
using namespace System::Collections;
using namespace Ambiesoft;

void errorhappen111()
{
	Console::WriteLine("EEEEEEEEEEEEEEEEEEEEEEE11111111111111111");
}

bool checksamestring(LPCTSTR sec, LPCTSTR key, LPCTSTR file)
{
	String^ a1="a1";
	String^ a2="a2";

	TCHAR buff[256];
	GetPrivateProfileString(sec, key, _T(""), buff, sizeof(buff), file);
	a1 = gcnew String(buff);

	
	Profile::GetString(gcnew String(sec), gcnew String(key), _T(""), a2, gcnew String(file));
	
	return a1==a2;
}
bool checksameint(LPCTSTR sec, LPCTSTR key, LPCTSTR file)
{
	int a1=1;
	int a2=2;
	a1 = GetPrivateProfileInt(sec, key, 0, file);
	Profile::GetInt(gcnew String(sec), gcnew String(key), 0, a2, gcnew String(file));
	return a1==a2;
}

bool checksamestringandint(LPCTSTR sec, LPCTSTR key, LPCTSTR file)
{
	return checksamestring(sec,key,file) && checksameint(sec,key,file)  ;
}

void temptest()
{
	Profile::ReadAll("C:\\T\\test.ini");
	Profile::ReadAll("Test.otita");
}

int main(array<System::String ^> ^args)
{
	temptest();

	TCHAR szFile[512];
	GetCurrentDirectory(512, szFile);
	lstrcat(szFile, _T("\\Test.wini"));
	
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


	TCHAR szBuff[32];
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



	
	GetCurrentDirectory(512, szFile);
	lstrcat(szFile, _T("\\Test.wini5"));
	WritePrivateProfileStringW(L"SEC", L"space", L"   ", szFile);
	return 0;
}

void unusedfunc()
{
	array<String^>^ aa;
	HashIni^ hi;
	Profile::GetStringArray("aaa","bbb", aa, hi);
}