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

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.IO;
using System.Diagnostics;

[assembly: CLSCompliant(true)]
namespace Ambiesoft
{
    public abstract partial class Profile
    {
        static String wpTrim(String sin)
        {
            if (String.IsNullOrEmpty(sin))
                return sin;

            String sout = sin.Trim();
            if (sout.Length == 0)
                return sout;

            if (sout[0] == '\'' && sout.Length > 1)
            {
                if (sout.EndsWith("\'"))
                {
                    sout = sout.Substring(1, sout.Length - 2);
                }
            }
            else if (sout[0] == '\"' && sout.Length > 1)
            {
                if (sout.EndsWith("\""))
                {
                    sout = sout.Substring(1, sout.Length - 2);
                }
            }
            return sout;
        }




        static public bool GetInt(String app, String key, int def, out int ret, String inipath)
        {
            HashIni al = ReadAll(inipath);
            return GetInt(app, key, def, out ret, al);
        }
        static public bool WriteInt(String app, String key, int val, String inipath)
        {
            String sval = val.ToString();
            return WriteString(app, key, sval, inipath);
        }


        static public bool GetString(String app, String key, String def, out String ret, String inipath)
        {
            HashIni hi = ReadAll(inipath);
            return GetString(app, key, def, out ret, hi);
        }
        static public bool WriteString(String app, String key, String val, String inipath)
        {
            Mutex mutex = null;
            try
            {
                mutex = createmutex(inipath);
                waitmutex(mutex);

                HashIni hi = ReadAll(inipath);

                if (hi == null)
                    return false;

                if (!WriteString(app, key, val, hi))
                    return false;

                return WriteAll(hi, inipath);
            }
            finally
            {
                if (mutex != null)
                {
                    mutex.ReleaseMutex();
                }
            }

        }




        static public bool GetStringArray(String app, String key, out String[] arRet, String inipath)
        {
            HashIni hi = ReadAll(inipath);
            return GetStringArray(app, key, out arRet, hi);
        }
        static public bool WriteStringArray(String app, String key, String[] arent, String inipath)
        {
            Mutex mutex = null;
            try
            {
                mutex = createmutex(inipath);
                waitmutex(mutex);
                HashIni hi = ReadAll(inipath);
                if (hi == null)
                    return false;

                if (!WriteStringArray(app, key, arent, hi))
                    return false;

                return WriteAll(hi, inipath);
            }
            finally
            {
                if (mutex != null)
                {
                    mutex.ReleaseMutex();
                }
            }
        }



        static public bool GetBinary(String app, String key, out Byte[] arRet, String inipath)
        {
            HashIni hi = ReadAll(inipath);
            return GetBinary(app, key, out arRet, hi);
        }
        static public bool WriteBinary(String app, String key, Byte[] arent, String inipath)
        {
            Mutex mutex = null;
            try
            {
                mutex = createmutex(inipath);
                waitmutex(mutex);
                HashIni hi = ReadAll(inipath);
                if (hi == null)
                    return false;


                if (!WriteBinary(app, key, arent, hi))
                    return false;

                return WriteAll(hi, inipath);
            }
            finally
            {
                if (mutex != null)
                {
                    mutex.ReleaseMutex();
                }
            }
        }



              
        static public bool GetBool(String app, String key, bool def, out bool bret, String inipath)
        {
            HashIni hi = ReadAll(inipath);
            return GetBool(app, key, def, out bret, hi);
        }
        static public bool WriteBool(String app, String key, bool val, String inipath)
        {
            return WriteInt(app, key, val ? 1 : 0, inipath);
        }





        static public bool GetSection(String app, out string[] arr, String inipath)
        {
            HashIni hi = ReadAll(inipath);
            return GetSection(app, out arr, hi);
        }






        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////




















        static public bool GetInt(String app, String key, int def, out int ret, HashIni hi)
        {
            ret = def;

            String sret;
            if (!GetString(app, key, null, out sret, hi))
                return false;

            if (sret == null)
                return false;


            try
            {
                ret = int.Parse(sret,
                    System.Globalization.NumberStyles.Any
                    );
            }
            catch (Exception)
            {
                return false;
            }

            return true;
        }
        static public bool WriteInt(String app, String key, int val, HashIni hi)
        {
            String sval = val.ToString();
            return WriteString(app, key, sval, hi);
        }








        static public bool GetString(String app, String key, String def, out String ret, HashIni hi)
        {
            ret = def;
            if (hi == null)
                return false;

            if (app == null)
                return false;

            if (key == null)
                return false;

            Hashtable al = hi.Hash;
            Hashtable sec = (Hashtable)al[app];
            if (sec == null)
                return false;

            ArrayList arent = (ArrayList)sec[key];
            if (arent == null || arent.Count == 0)
                return false;

            String val = (String)arent[0];
            if (val == null)
                return false;

            ret = val;
            return true;
        }
        static public bool WriteString(String app, String key, String val, HashIni hi)
        {
            if (hi == null)
                return false;

            if (app == null)
                return false;

            if (null == key)
                return false;

            Hashtable al = hi.Hash;
            Hashtable sec = (Hashtable)al[app];
            if (null == sec)
            {
                sec = new Hashtable();
                al[app] = sec;
            }

            ArrayList arent = (ArrayList)sec[key];
            if (null == arent)
            {
                arent = new ArrayList();
                sec[key] = arent;
            }

            if (arent.Count == 0)
            {
                arent.Add(val);
            }
            else
            {
                arent[0] = val;
            }


            return true;
        }




        // stringarray ------------------------------->
        static public bool GetStringArray(String app, String key, out String[] arRet, HashIni hi)
        {
            arRet = new String[0];
            if (null == hi)
                return false;

            if (null == app)
                return false;

            if (null == key)
                return false;

            Hashtable al = hi.Hash;
            Hashtable sec = (Hashtable)al[app];
            if (null == sec)
                return false;

            if (null == sec[key])
            {
                arRet = new String[0];
            }
            else
            {
                // arRet = reinterpret_cast<array<String^>^>(((ArrayList^)sec[key])->ToArray(String::typeid));
                arRet = ((ArrayList)sec[key]).ToArray(typeof(String)) as string[];
            }
            return true;
        }
        static public bool WriteStringArray(String app, String key, String[] arent, HashIni hi)
        {
            if (hi == null)
                return false;

            if (app == null)
                return false;

            if (key == null)
                return false;

            Hashtable al = hi.Hash;
            Hashtable sec = (Hashtable)al[app];
            if (sec == null)
            {
                sec = new Hashtable();
                al[app] = sec;
            }

            sec[key] = new ArrayList(arent);

            return true;
        }
        // <------------------- stringarray








        // binary----------------->
        static bool GetByte1(char c, out byte b)
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
            b = (byte)'?';
            return false;
        }

        static bool GetByte(char c1, char c2, out byte b)
        {
            b = 0;
            byte b1, b2;
            if (!GetByte1(c1, out b1))
                return false;
            if (!GetByte1(c2, out b2))
                return false;

            b = (byte)((b1 << 4) | (byte)b2);
            return true;
        }

        static public bool GetBinary(String app, String key, out byte[] arRet, HashIni hi)
        {
            arRet = new byte[0];
            String sret;
            if (!GetString(app, key, null, out sret, hi))
                return false;

            if (String.IsNullOrEmpty(sret))
                return false;

            if ((sret.Length % 2) != 0)
                return false;

            if (sret.Length == 2)
            {
                if (sret[0] == '0' && sret[1] == '0')
                {
                    arRet = new byte[0]; // array<System::Byte>(0);
                    return true;
                }
                return false;
            }

            int length = (sret.Length / 2) - 1;
            byte[] ret = new byte[length];
            int i = 0;
            int bi = 0;
            byte sum = 0;
            byte bu;
            int length2 = length * 2;
            for (i = 0; i < length2; i += 2, ++bi)
            {
                if (!GetByte(sret[i], sret[i + 1], out bu))
                    return false;

                ret[bi] = bu;
                sum += ret[bi];
            }

            if (!GetByte(sret[i], sret[i + 1], out bu))
                return false;

            if (sum != bu)
                return false;

            arRet = ret;

            return true;
        }

        static char GetString1(byte b)
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

        static void GetString(byte b, char[] sc2)
        {
            sc2[0] = GetString1((byte)(b >> 4));
            sc2[1] = GetString1((byte)(b & 0x0F));
        }

        static public bool WriteBinary(String app, String key, byte[] arent, HashIni hi)
        {
            if (arent == null)
            {
                return WriteString(app, key, null, hi);
            }

            byte sum = 0;
            System.Text.StringBuilder sb = new StringBuilder((arent.Length * 2) + 2);
            char[] sc2 = new char[2]; // gcnew array<System::Char>(2);
            foreach (byte b in arent)
            {
                GetString(b, sc2);
                sb.Append(sc2);
                sum += b;
            }
            GetString(sum, sc2);
            sb.Append(sc2);

            return WriteString(app, key, sb.ToString(), hi);
        }
        // <--------------------- binary





































        // bool ------------------------> 
        static public bool GetBool(String app, String key, bool def, out bool ret, HashIni hi)
        {
            ret = def;

            string sret;
            if (!GetString(app, key, null, out sret, hi))
                return false;

            if (sret == null)
                return false;

            if( string.Compare(sret,"yes" ,true)==0 ||
                string.Compare(sret,"on"  ,true)==0 ||
                string.Compare(sret, true.ToString() ,true)==0 )
            {
                ret = true;
                return true;
            }

            int iret=0;
            try
            {
                iret = int.Parse(sret,
                    System.Globalization.NumberStyles.Any
                    );
            }
            catch (Exception)
            {
                return false;
            }

            ret = iret==1;
            return true;
        }
        static public bool WriteBool(String app, String key, bool val, HashIni hi)
        {
            // for compatibility, use int to write
            return WriteInt(app, key, val ? 1 : 0, hi);
        }
        // <------------------------------bool



        // section --------------------------->
        static public bool GetSection(String app, out string[] arr, HashIni hi)
        {
            List<String> rets = new List<string>();
            arr = rets.ToArray();
            if (hi == null)
                return false;

            if (app == null)
                return false;

            Hashtable al = hi.Hash;
            Hashtable sec = (Hashtable)al[app];
            if (sec == null)
                return false;

            foreach(object o in sec.Keys)
                rets.Add(o.ToString());

            arr = rets.ToArray();
            return true;
        }
        // <--------------------------- section
    }
}