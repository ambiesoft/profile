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

#ifndef __MAIN_H__
#define __MAIN_H__

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#ifdef _MSC_VER
#if _MSC_VER <= 1900
#pragma warning( disable : 4503 )
#endif
#endif
#endif



#include <map>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
// #include <codecvt>
#include <algorithm>
#include <cctype>
#include <locale>


// #include <boost/algorithm/string/trim.hpp>

namespace Ambiesoft {
	typedef void* HashIniHandle;
	// typedef unsigned char byte;

	static inline bool myisspace(char c)
	{
		return c == ' ' || c == '\t' || c == '\n' || c == '\n';
	}
	// https://stackoverflow.com/a/217605
	// trim from start (in place)
	static inline void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !myisspace(ch);
		}));
	}

	// trim from end (in place)
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !myisspace(ch);
        }).base(), s.end());
    }
    static inline void rtrimr(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !(ch=='\r');
        }).base(), s.end());
    }

	// trim from both ends (in place)
	static inline void trim(std::string &s) {
		ltrim(s);
		rtrim(s);
	}

	// trim from start (copying)
	static inline std::string ltrim_copy(std::string s) {
		ltrim(s);
		return s;
	}

	// trim from end (copying)
	static inline std::string rtrim_copy(std::string s) {
		rtrim(s);
		return s;
	}

	// trim from both ends (copying)
	static inline std::string trim_copy(std::string s) {
		trim(s);
		return s;
	}

	// https://stackoverflow.com/a/2072890
	static bool ends_with(std::string const & value, std::string const & ending)
	{
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}

	static std::string wpTrim(const std::string& sin)
	{
		if (sin.empty())
			return sin;

		std::string sout = trim_copy(sin); // boost::algorithm::trim_copy(sin);
		if (sout.empty())
			return sout;

		if (sout[0] == '\'')
		{
			if (ends_with(sout, "\'"))
			{
				sout = sout.substr(1, sout.size() - 2);// Substring(1, sout.Length - 2);
			}
		}
		else if (sout[0] == '\"')
		{
			if (ends_with(sout, "\""))
			{
				sout = sout.substr(1, sout.size() - 2);// Substring(1, sout.Length - 2);
			}
		}
		return sout;
	}

	class Profile;
	class HashIni
	{
	public:
		typedef std::map<std::string, std::vector<std::string> > HashSection;
		typedef std::map<std::string, std::shared_ptr<HashSection> > HashAll;


	private:
		HashAll hash_;
		HashIni()
		{}
		~HashIni()
		{}
		HashAll & Hash()
		{
			return hash_;
		}

	public:
		static HashIniHandle CreateEmptyInstanceForSpecialUse()
		{
			return static_cast<HashIniHandle>(new HashIni());
		}

		friend Profile;
	};




	class Profile
	{
		class HashIniHandleWrapper
		{
			HashIniHandle h_;
		public:
			HashIniHandleWrapper(HashIniHandle h) : h_(h)
			{}
			~HashIniHandleWrapper()
			{
				Profile::FreeHandle(h_);
			}
			operator HashIniHandle()
			{
				return h_;
			}
		};

	public:
		static std::pair<std::string, std::string> splitLine(const std::string& line)
		{
			size_t pos = line.find('=', 0);
			if (pos == std::string::npos)
				return std::pair<std::string, std::string>(line, std::string());

			std::string key = line.substr(0, pos);
			std::string val = line.substr(pos + 1);
			return std::pair<std::string, std::string>(key, val);
		}

		static std::string getSectionName(const std::string& line)
		{
			std::string ret;
			if (line.empty() || line[0] != '[')
				return ret;

			for (size_t i = 1; i < line.size(); ++i)
			{
				char c = line[i];
				if (c == ']')
					break;

				ret += c;
			}
			return ret;
		}

		static HashIniHandle ReadAll(const std::string& file, bool throwexception = false)
		{
			HashIni* hi = static_cast<HashIni*>(HashIni::CreateEmptyInstanceForSpecialUse());
			HashIni::HashAll& al = hi->Hash();

			try
			{
				// TODO: implement mutex
				//mutex = createmutex(inipath);
				//waitmutex(mutex);

				std::ifstream ifs;
				ifs.open(file);
				if (!ifs)
					throw std::exception();

				// static_assert(sizeof(char) == 2, "error.");//Linux is no ready
				// fs.imbue(std::locale(std::locale(""), new std::codecvt_utf8_utf16<char, 0x10ffff, std::consume_header>()));
				// fs.imbue(std::locale(std::locale(""), new std::codecvt_utf8_utf16<char, 0x10ffff, std::consume_header>()));
				ifs.imbue(std::locale(""));

				// skip UTF-8 BOM
				{
					char a, b, c;
					a = ifs.get();
					b = ifs.get();
					c = ifs.get();
					if (a != (char)0xEF || b != (char)0xBB || c != (char)0xBF)
					{
						ifs.seekg(0);
					}
				}

				if (ifs)
				{
					std::string line;
					std::shared_ptr<HashIni::HashSection> cursec = nullptr;

					while (getline(ifs, line))
					{
						ltrim(line); // boost::algorithm::trim_left(line);
                        rtrimr(line);

						if (line.empty() || line[0] == '#')
							continue;

						if (line[0] == '[')
						{
							std::string secname = getSectionName(line);
							cursec = al[secname];
							if (!cursec)
							{
								cursec = std::make_shared<HashIni::HashSection>();
								al[secname] = cursec;
							}
							continue;
						}
						else  // not a section line
						{
							if (!cursec)
								continue;

							std::pair<std::string, std::string> vals = splitLine(line);
							if (!vals.first.empty())
								vals.first = wpTrim(vals.first);

							if (!vals.second.empty())
								vals.second = wpTrim(vals.second);

							std::vector<std::string>& arent = (*cursec)[vals.first];
							//if (arent == null)
							//{
							//	arent = new ArrayList();
							//	cursec[vals[0]] = arent;
							//}

							arent.push_back(vals.second);
						}

					}
				}
				return hi;
			}
			catch (std::exception& e)
			{
				if (throwexception)
					throw e;
			}
			return (HashIniHandle)hi;
		}
		static bool WriteAll(HashIniHandle hih, const std::string& inipath)
		{
			return WriteAll(hih, inipath, false);
		}
		static bool WriteAll(HashIniHandle hih, const std::string& inipath, bool throwexception)
		{
			HashIni* hi = static_cast<HashIni*>(hih);
			if (hi == nullptr)
				return false;

			HashIni::HashAll& al = hi->Hash();

			// Mutex mutex = null;
			try
			{
				//mutex = createmutex(inipath);
				//waitmutex(mutex);

				std::ofstream ofs;
				ofs.open(inipath);
				if (!ofs)
					throw std::exception();

				// ofs.imbue({ {}, new std::codecvt_utf8<char, 0x10FFFF, std::consume_header> });
				ofs.imbue(std::locale(""));



				//ArrayList arKeys = new ArrayList(al.Keys);
				//arKeys.Sort();
				for (HashIni::HashAll::iterator it = al.begin(); it != al.end(); ++it)
				{
					std::string secname = it->first;
					std::shared_ptr<HashIni::HashSection> sec = it->second;

					ofs << "[";
					ofs << secname;
					ofs << "]";
					ofs << std::endl;

					if (sec == nullptr)
						continue;

					for (HashIni::HashSection::iterator its = sec->begin(); its != sec->end(); ++its)
					{
						std::string keyname = its->first;
						std::vector<std::string>& arent = its->second;
						for (std::vector<std::string>::const_iterator it = arent.begin() ; it != arent.end(); ++it)
						{
							if (!it->empty())
							{
								ofs << keyname;
								ofs << "=";
								ofs << *it;
								ofs << std::endl;
							}
						}
					}
					ofs << std::endl;
				}

			}
			catch (std::exception& e)
			{
				if (throwexception)
					throw e;

				return false;
			}
			//finally
			//{
			//	if (mutex != null)
			//	{
			//		mutex.ReleaseMutex();
			//	}
			//}
			return true;
		}
		static void FreeHandle(HashIniHandle hih)
		{
			HashIni* hi = static_cast<HashIni*>(hih);
			delete hi;
		}

		// String starts ----------------
		static bool GetString(const std::string& app,
			const std::string& key,
			const std::string& def,
			std::string& ret,
			HashIniHandle hih)
		{
			HashIni* hi = static_cast<HashIni*>(hih);
			ret = def;
			if (hi == nullptr)
				return false;

			if (app.empty())
				return false;

			if (key.empty())
				return false;

			std::shared_ptr<HashIni::HashSection> sec = hi->Hash()[app];
			if (sec == nullptr)
				return false;

			std::vector<std::string>& arent = (*sec)[key];

			if (arent.empty())
				return false;

			ret = arent[0];
			return true;
		}
		static bool GetString(const std::string& app,
			const std::string& key,
			const std::string& def,
			std::string& ret,
			std::string& inifile)
		{
			HashIniHandleWrapper ini(ReadAll(inifile));
			return GetString(app, key, def, ret, ini);
		}
		static bool WriteString(const std::string& app,
			const std::string& key,
			const char* val,
			HashIniHandle hih)
		{
			HashIni* hi = static_cast<HashIni*>(hih);

			if (hi == nullptr)
				return false;

			if (app.empty())
				return false;

			if (key.empty())
				return false;

			std::shared_ptr<HashIni::HashSection> sec = hi->Hash()[app];
			if (!sec)
			{
				sec = std::make_shared<HashIni::HashSection>();
				hi->Hash()[app] = sec;
			}

			std::vector<std::string>& arent = (*sec)[key];

			if (val != nullptr)
			{
				if (arent.empty())
				{
					arent.push_back(val);
				}
				else
				{
					arent[0] = val;
				}
			}
			else
			{
				// val is null, remove all values
				arent.clear();
			}
			return true;
		}
		static bool WriteString(const std::string& app,
			const std::string& key,
			const std::string& val,
			HashIniHandle hih)
		{
			return WriteString(app, key, val.c_str(), hih);
		}
		static bool WriteString(const std::string& app,
			const std::string& key,
			const std::string& val,
			const std::string& inifile)
		{
			// Mutex mutex = null;
			try
			{
				// mutex = createmutex(inipath);
				// waitmutex(mutex);

				HashIniHandleWrapper ini(ReadAll(inifile));

				if (!WriteString(app, key, val, ini))
					return false;

				return WriteAll(ini, inifile);
			}
			catch (std::exception&)
			{
				return false;
			}
		}


		// Int starts ----------------
		static bool GetInt(const std::string& app,
			const std::string& key,
			int def,
			int& ret,
			HashIniHandle hih)
		{
			HashIni* hi = static_cast<HashIni*>(hih);
			ret = def;

			std::string sret;
			if (!GetString(app, key, std::string(), sret, hi))
				return false;

			if (sret.empty())
				return false;

			try
			{
				ret = std::stoi(sret);
			}
			catch (std::exception&)
			{
				return false;
			}

			return true;
		}
		static bool GetInt(const std::string& app,
			const std::string& key,
			int def,
			int& ret,
			const std::string& inifile)
		{
			HashIniHandleWrapper ini(ReadAll(inifile));
			return GetInt(app, key, def, ret, ini);
		}
		static bool WriteInt(const std::string& app,
			const std::string& key,
			int val,
			HashIniHandle hih)
		{
			std::string sval = std::to_string(static_cast<long long>(val));
			return WriteString(app, key, sval, hih);
		}
		static bool WriteInt(const std::string& app,
			const std::string& key,
			int val,
			const std::string& inifile)
		{
			std::string sval = std::to_string(static_cast<long long>(val));
			return WriteString(app, key, sval, inifile);
		}

		static bool GetByte1(char c, unsigned char& b)
		{
			switch (c)
			{
			case '0': b = 0; return true;
			case '1': b = 1; return true;
			case '2': b = 2; return true;
			case '3': b = 3; return true;
			case '4': b = 4; return true;
			case '5': b = 5; return true;
			case '6': b = 6; return true;
			case '7': b = 7; return true;
			case '8': b = 8; return true;
			case '9': b = 9; return true;
			case 'a': b = 10; return true;
			case 'A': b = 10; return true;
			case 'b': b = 11; return true;
			case 'B': b = 11; return true;
			case 'c': b = 12; return true;
			case 'C': b = 12; return true;
			case 'd': b = 13; return true;
			case 'D': b = 13; return true;
			case 'e': b = 14; return true;
			case 'E': b = 14; return true;
			case 'f': b = 15; return true;
			case 'F': b = 15; return true;
			}
			b = (unsigned char)'?';
			return false;
		}
		static bool GetByte(char c1, char c2, unsigned char& b)
		{
			b = 0;
			unsigned char b1, b2;
			if (!GetByte1(c1, b1))
				return false;
			if (!GetByte1(c2, b2))
				return false;

			b = (unsigned char)((b1 << 4) | (unsigned char)b2);
			return true;
		}


		// Binary starts -------------------
		static bool GetBinary(
			const std::string& app,
			const std::string& key,
			std::vector<unsigned char>& vRet,
			HashIniHandle hih)
		{
			return GetBinary(app, key, &vRet, hih, false);
		}
		static bool GetBinary(
			const std::string& app,
			const std::string& key,
			unsigned char* pV,
			HashIniHandle hih)
		{
			return GetBinary(app, key, pV, hih, true);
		}
	private:
		static bool GetBinary(
			const std::string& app,
			const std::string& key,
			void* pVoid,
			HashIniHandle hih,
			bool isNavive)
		{
			std::string sret;
			if (!GetString(app, key, std::string(), sret, hih))
				return false;

			if (sret.empty())
				return false;

			if ((sret.size() % 2) != 0)
				return false;

			if (sret.size() == 2)
			{
				if (sret[0] == '0' && sret[1] == '0')
				{
					return true;
				}
				return false;
			}

			size_t length = (sret.size() / 2) - 1;

			unsigned char* pV = nullptr;
			std::vector<unsigned char>* pVector = nullptr;
			if (isNavive)
				pV = static_cast<unsigned char*>(pVoid);
			else
				pVector = static_cast<std::vector<unsigned char>*>(pVoid);

			// byte[] ret = new byte[length];
			if(pVector)
				pVector->resize(length);

			size_t i = 0;
			size_t bi = 0;
			unsigned char sum = 0;
			unsigned char bu;
			size_t length2 = length * 2;

			if (isNavive)
			{
				for (i = 0; i < length2; i += 2, ++bi)
				{
					if (!GetByte(sret[i], sret[i + 1], bu))
						return false;

					pV[bi] = bu;
					sum += pV[bi];
				}
			}
			else
			{
				for (i = 0; i < length2; i += 2, ++bi)
				{
					if (!GetByte(sret[i], sret[i + 1], bu))
						return false;

					(*pVector)[bi] = bu;
					sum += (*pVector)[bi];
				}
			}

			if (!GetByte(sret[i], sret[i + 1], bu))
				return false;

			if (sum != bu)
				return false;

			return true;
		}
	public:
		static bool GetBinary(
			const std::string& app,
			const std::string& key,
			unsigned char* pV,
			const std::string& inipath)
		{
			HashIniHandleWrapper hih = ReadAll(inipath);
			return GetBinary(app, key, pV, hih);
		}
		static bool GetBinary(
			const std::string& app,
			const std::string& key,
			std::vector<unsigned char>& vRet,
			const std::string& inipath)
		{
			HashIniHandleWrapper hih = ReadAll(inipath);
			return GetBinary(app, key, vRet, hih);
		}

		static char GetString1(unsigned char b)
		{
			switch (b)
			{
			case 0: return '0';
			case 1: return '1';
			case 2: return '2';
			case 3: return '3';
			case 4: return '4';
			case 5: return '5';
			case 6: return '6';
			case 7: return '7';
			case 8: return '8';
			case 9: return '9';
			case 10: return 'A';
			case 11: return 'B';
			case 12: return 'C';
			case 13: return 'D';
			case 14: return 'E';
			case 15: return 'F';
			}
			return '?';
		}
		static void GetString(unsigned char b, char* sc2)
		{
			sc2[0] = GetString1((unsigned char)(b >> 4));
			sc2[1] = GetString1((unsigned char)(b & 0x0F));
		}
		static bool WriteBinary(
			const std::string& app,
			const std::string& key,
			const std::vector<unsigned char>& v,
			HashIniHandle hih)
		{
			return WriteBinary(app, key, v.data(), v.size(), hih);
		}
		static bool WriteBinary(
			const std::string& app,
			const std::string& key,
			const unsigned char* pV,
			size_t size,
			HashIniHandle hih)
		{
			unsigned char sum = 0;

			std::string sb; // = new StringBuilder((arent.Length * 2) + 2);
			char sc2[2];
			for (size_t i=0 ; i < size ; ++i)
			{
				unsigned char b = pV[i];
				GetString(b, sc2);
				sb.append(sc2, sc2 + 2);
				sum += b;
			}
			GetString(sum, sc2);
			sb.append(sc2, sc2 + 2);

			return WriteString(app, key, sb, hih);
		}
		static bool WriteBinary(
			const std::string& app,
			const std::string& key,
			const std::vector<unsigned char>& v,
			const std::string& inipath)
		{
			return WriteBinary(app, key, v.data(), v.size(), inipath);
		}
		static bool WriteBinary(
			const std::string& app,
			const std::string& key,
			const unsigned char* pV,
			size_t size,
			const std::string& inipath)
		{
			// Mutex mutex = null;
			//try
			{
				//mutex = createmutex(inipath);
				//waitmutex(mutex);
				HashIniHandle hih = ReadAll(inipath);
				if (hih == nullptr)
					return false;

				if (!WriteBinary(app, key, pV, size, hih))
					return false;

				return WriteAll(hih, inipath);
			}
			//finally
			//{
			//	if (mutex != null)
			//	{
			//		mutex.ReleaseMutex();
			//	}
			//}
		}


		// Bool starts----------
		static bool iequals(const std::string& a, const std::string& b)
		{
			size_t sz = a.size();
			if (b.size() != sz)
				return false;
			for (size_t i = 0; i < sz; ++i)
				if (std::tolower(a[i]) != std::tolower(b[i]))
					return false;
			return true;
		}
		static bool GetBool(
			const std::string& app,
			const std::string& key,
			bool def,
			bool& ret,
			const std::string& inipath)
		{
			HashIniHandleWrapper hih = ReadAll(inipath);
			return GetBool(app, key, def, ret, hih);
		}
		static bool GetBool(
			const std::string& app,
			const std::string& key,
			bool def,
			bool& ret,
			HashIniHandle hih)
		{
			ret = def;

			std::string sret;
			if (!GetString(app, key, std::string(), sret, hih))
				return false;

			if (sret.empty())
				return false;


			if (iequals(sret, "yes") ||
				iequals(sret, "on") ||
				iequals(sret, "true"))
			{
				ret = true;
				return true;
			}

			int iret = std::stoi(sret);
			ret = iret == 1;
			return true;
		}

		static bool WriteBool(
			const std::string& app,
			const std::string& key,
			const bool val,
			HashIniHandle hih)
		{
			// for compatibility, use int to write
			return WriteInt(app, key, val ? 1 : 0, hih);
		}
		static bool WriteBool(
			const std::string& app,
			const std::string& key,
			const bool val,
			const std::string& inipath)
		{
			return WriteInt(app, key, val ? 1 : 0, inipath);
		}


		// vector<string> starts ------------
		static bool GetStringArray(const std::string& app,
			const std::string& key,
			std::vector<std::string>& vs,
			HashIniHandle hih)
		{
			HashIni* hi = static_cast<HashIni*>(hih);

			if (hi == nullptr)
				return false;

			if (app.empty())
				return false;

			if (key.empty())
				return false;

			std::shared_ptr<HashIni::HashSection> sec = hi->Hash()[app];
			if (sec == nullptr)
				return false;

			vs = (*sec)[key];

			return true;
		}
		static bool GetStringArray(const std::string& app,
			const std::string& key,
			std::vector<std::string>& vs,
			const std::string& inipath)
		{
			HashIniHandleWrapper hih(ReadAll(inipath));
			return GetStringArray(app, key, vs, hih);
		}
		static bool WriteStringArray(
			const std::string& app,
			const std::string& key,
			const std::vector<std::string>& vs,
			const std::string& inifile)
		{
			HashIniHandleWrapper ini(ReadAll(inifile));

			if (!WriteStringArray(app, key, vs, ini))
				return false;

			return WriteAll(ini, inifile);
		}
		static bool WriteStringArray(
			const std::string& app,
			const std::string& key,
			const std::vector<std::string>& vs,
			HashIniHandle hih)
		{
			HashIni* hi = static_cast<HashIni*>(hih);

			if (hi == nullptr)
				return false;

			if (app.empty())
				return false;

			if (key.empty())
				return false;

			std::shared_ptr<HashIni::HashSection> sec = hi->Hash()[app];
			if (!sec)
			{
				sec = std::make_shared<HashIni::HashSection>();
				hi->Hash()[app] = sec;
			}

			(*sec)[key] = vs;
			return true;
		}
	};


}



#endif // __MAIN_H__