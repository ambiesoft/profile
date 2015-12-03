using System;
using System.Collections;
using System.Text;
using System.IO;
using System.Threading;


namespace Ambiesoft
{
    partial class Profile
    {
        static public HashIni ReadAll(String inipath)
        {
            return ReadAll(inipath, false);
        }
        static public HashIni ReadAll(String inipath, bool throwexception)
        {
            Mutex mutex = null;
            HashIni hi = HashIni.CreateEmptyInstanceForSpecialUse();
            Hashtable al = hi.Hash;
            try
            {
                mutex = createmutex(inipath);
                waitmutex(mutex);
                using (System.IO.StreamReader sr = new System.IO.StreamReader(inipath, System.Text.Encoding.UTF8))
                {

                    String line = null;
                    Hashtable cursec = null;

                    while ((line = sr.ReadLine()) != null)
                    {
                        line = line.TrimStart();
                        if (line.Length == 0 || line[0] == '#')
                            continue;

                        if (line[0] == '[')
                        {
                            String secname = line.Trim(new char[] { '[', ']' });
                            cursec = (Hashtable)al[secname];
                            if (cursec == null)
                            {
                                cursec = new Hashtable();
                                al[secname] = cursec;
                            }
                            continue;
                        }
                        else
                        {
                            if (cursec == null)
                                continue;

                            String[] vals = line.Split(new char[] { '=' }, 2);


                            if (vals[0] != null)
                                vals[0] = wpTrim(vals[0]);

                            if (vals.Length < 2)
                            {
                                vals = new String[2] { vals[0], String.Empty };
                            }
                            else
                            {
                                if (vals[1] != null)
                                    vals[1] = wpTrim(vals[1]);
                            }

                            ArrayList arent = (ArrayList)cursec[vals[0]];
                            if (arent == null)
                            {
                                arent = new ArrayList();
                                cursec[vals[0]] = arent;
                            }

                            arent.Add(vals[1]);

                        }


                    }
                }
            }
            catch (Exception e3)
            {
                if (throwexception)
                    throw e3;

                return hi;
            }
            finally
            {
                if (mutex != null)
                {
                    mutex.ReleaseMutex();
                }
            }
            return hi;
        }

        static public bool WriteAll(HashIni hi, String inipath)
        {
            return WriteAll(hi, inipath, false);
        }


        static public bool WriteAll(HashIni hi, String inipath, bool throwexception)
        {
            if (hi == null)
                return false;

            Hashtable al = hi.Hash;
            if (al == null)
                return false;


            Mutex mutex = null;
            try
            {
                mutex = createmutex(inipath);
                waitmutex(mutex);

                using (System.IO.StreamWriter sw = new System.IO.StreamWriter(inipath, false, new System.Text.UTF8Encoding(false)))
                {

                    ArrayList arKeys = new ArrayList(al.Keys);
                    arKeys.Sort();

                    foreach (String secname in arKeys)
                    {
                        sw.Write("[");
                        sw.Write(secname);
                        sw.Write("]");
                        sw.WriteLine();

                        Hashtable sec = (Hashtable)al[secname];
                        if (sec == null)
                            continue;

                        ArrayList arKeyKeys = new ArrayList(sec.Keys);
                        arKeyKeys.Sort();
                        foreach (String keyname in arKeyKeys)
                        {
                            ArrayList arent = (ArrayList)sec[keyname];
                            foreach (String val in arent)
                            {
                                if (val != null)
                                {
                                    sw.Write(keyname);
                                    sw.Write("=");
                                    sw.Write(val);
                                    sw.WriteLine();
                                }
                            }
                        }
                        sw.WriteLine();
                    }
                }
            }
            catch (Exception ex)
            {
                if (throwexception)
                    throw ex;

                return false;
            }
            finally
            {
                if (mutex != null)
                {
                    mutex.ReleaseMutex();
                }
            }
            return true;
        }
    }
}
