#ifndef __MAIN_H__
#define __MAIN_H__

#include <map>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <codecvt>

#include <boost/algorithm/string/trim.hpp>

namespace Ambiesoft {
	// https://stackoverflow.com/a/2072890
	static bool ends_with(std::wstring const & value, std::wstring const & ending)
	{
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}

	static std::wstring wpTrim(const std::wstring& sin)
	{
		if (sin.empty())
			return sin;

		std::wstring sout = boost::algorithm::trim_copy(sin);
		if (sout.empty())
			return sout;

		if (sout[0] == L'\'')
		{
			if (ends_with(sout, L"\'"))
			{
				sout = sout.substr(1, sout.size() - 2);// Substring(1, sout.Length - 2);
			}
		}
		else if (sout[0] == L'\"')
		{
			if (ends_with(sout, L"\""))
			{
				sout = sout.substr(1, sout.size() - 2);// Substring(1, sout.Length - 2);
			}
		}
		return sout;
	}
    class HashIni
    {
	public:
	    typedef std::map<std::wstring, std::vector<std::wstring> > HashSection;
		typedef std::map<std::wstring, std::shared_ptr<HashSection> > HashAll;


	private:
        HashAll hash_;
        HashIni()
        {
        }
    public:

        HashAll& Hash()
        {
            return hash_;
        }

        static HashIni* CreateEmptyInstanceForSpecialUse()
        {
            return new HashIni();
        }
    };

    struct Profile
    {
		static std::pair<std::wstring, std::wstring> splitLine(const std::wstring& line)
		{
			size_t pos = line.find(L'=', 0);
			if (pos == std::wstring::npos)
				return std::pair<std::wstring, std::wstring>(line, std::wstring());

			std::wstring key = line.substr(0, pos);
			std::wstring val = line.substr(pos + 1);
			return std::pair<std::wstring, std::wstring>(key, val);
		}

        static std::wstring getSectionName(const std::wstring& line)
		{
			std::wstring ret;
			if (line.empty() || line[0] != L'[')
				return ret;

			for (size_t i = 1; i <line.size(); ++i)
			{
				wchar_t c = line[i];
				if (c == L']')
					break;

				ret += c;
			}
			return ret;
		}
        
                static HashIni* ReadAll(const std::string& file, bool throwexception = false)
		{
			HashIni* hi = HashIni::CreateEmptyInstanceForSpecialUse();
			HashIni::HashAll& al = hi->Hash();

			try
			{
				// TODO: implement mutex
				//mutex = createmutex(inipath);
				//waitmutex(mutex);

                                std::wifstream fs;
                                fs.open(file);
                                if(!fs)
                                    throw std::exception();

				static_assert(sizeof(wchar_t) == 2, "error.");//Linux is no ready
				fs.imbue(std::locale(std::locale(""), new std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::consume_header>()));
				if (fs)
				{
					std::wstring line;
					std::shared_ptr<HashIni::HashSection> cursec = nullptr;

					while (getline(fs, line))
					{
						boost::algorithm::trim_left(line);
						if (line.empty() || line[0] == L'#')
							continue;

						if (line[0] == L'[')
						{
							std::wstring secname = getSectionName(line);
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

							std::pair<std::wstring, std::wstring> vals = splitLine(line);
							if (!vals.first.empty())
								vals.first = wpTrim(vals.first);

							if (!vals.second.empty())
								vals.second = wpTrim(vals.second);

							std::vector<std::wstring>& arent = (*cursec)[vals.first];
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
			return hi;
		}
                static bool WriteAll(HashIni* hi, const std::string& inipath)
		{
			return WriteAll(hi, inipath, false);
		}
                static bool WriteAll(HashIni* hi, const std::string& inipath, bool throwexception){
			if (hi == nullptr)
				return false;

			HashIni::HashAll& al = hi->Hash();

			// Mutex mutex = null;
			try
			{
				//mutex = createmutex(inipath);
				//waitmutex(mutex);

				std::wofstream ofs;
				ofs.open(inipath);
                                if(!ofs)
                                    throw std::exception();

				ofs.imbue({ {}, new std::codecvt_utf8<wchar_t, 0x10FFFF, std::consume_header> });


				{

					//ArrayList arKeys = new ArrayList(al.Keys);
					//arKeys.Sort();
					for (HashIni::HashAll::iterator it = al.begin(); it != al.end(); ++it)
					{
						std::wstring secname = it->first;
						std::shared_ptr<HashIni::HashSection> sec = it->second;

						ofs << L"[";
						ofs << secname;
						ofs << L"]";
						ofs << std::endl;

						if (sec == nullptr)
							continue;

						for (HashIni::HashSection::iterator its = sec->begin(); its != sec->end(); ++its)
						{
							std::wstring keyname = its->first;
							std::vector<std::wstring>& arent = its->second;
							for (std::wstring& val : arent)
							{
								if (!val.empty())
								{
									ofs << keyname;
									ofs << L"=";
									ofs << val;
									ofs << std::endl;
								}
							}
						}
						ofs << std::endl;
					}
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

		static bool GetString(const std::wstring& app, const std::wstring& key, const std::wstring& def, std::wstring& ret, HashIni* hi)
		{
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

			std::vector<std::wstring>& arent = (*sec)[key];
			
			if (arent.empty())
				return false;

			ret  = arent[0];
			return true;
		}
		static bool WriteString(const wchar_t* app, const wchar_t* key, const wchar_t* val, HashIni* hi)
		{
			if (hi == nullptr)
				return false;

			if (app == nullptr)
				return false;

			if (key == nullptr)
				return false;

			std::shared_ptr<HashIni::HashSection> sec = hi->Hash()[app];
			if (!sec)
			{
				sec = std::make_shared<HashIni::HashSection>();
				hi->Hash()[app] = sec;
			}

			std::vector<std::wstring>& arent = (*sec)[key];

			if (arent.empty())
			{
				arent.push_back(val);
			}
			else
			{
				arent[0] = val;
			}
			return true;
		}

		static bool GetInt(const std::wstring& app, const std::wstring& key, int def, int& ret, HashIni* hi)
		{
			ret = def;

			std::wstring sret;
			if (!GetString(app, key, std::wstring(), sret, hi))
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
		static bool WriteInt(const wchar_t* app, const wchar_t* key, int val, HashIni* hi)
		{
			std::wstring sval = std::to_wstring(val);
			return WriteString(app, key, sval.c_str(), hi);
		}
    };


}



#endif // __MAIN_H__
