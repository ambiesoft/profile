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
			return static_cast<HashIni*>(new HashIni());
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
						for (std::string& val : arent)
						{
							if (!val.empty())
							{
								ofs << keyname;
								ofs << "=";
								ofs << val;
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

            if(val != nullptr)
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
           return WriteString(app,key,val.c_str(),hih);
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
            catch(std::exception&)
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
            std::string sval = std::to_string(val);
            return WriteString(app, key, sval, hih);
        }
        static bool WriteInt(const std::string& app,
                             const std::string& key,
                             int val,
                             const std::string& inifile)
        {
            std::string sval = std::to_string(val);
            return WriteString(app, key, sval, inifile);
        }
    };


}



#endif // __MAIN_H__
