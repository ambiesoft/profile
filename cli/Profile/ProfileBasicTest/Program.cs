using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Diagnostics;

namespace ProfileCheck
{
    using Ambiesoft;
    static class Program
    {
        static void Main()
        {
            string inipath = Application.ExecutablePath + ".ini";
            int i;

            Profile.WriteInt("aaa", "aaa", 12345, inipath);
            Profile.GetInt("aaa", "aaa", 0, out i, inipath);

            Profile.WriteBinary("aaa", "bbb", new byte[] { 0xaa, 0xbb }, inipath);
            byte[] mybyte;
            Profile.GetBinary("aaa", "bbb", out mybyte, inipath);




            int tick = System.Environment.TickCount;
            Profile.WriteInt("option", "tick", tick, inipath);
            int val;
            Profile.GetInt("option", "tick", 0, out val, inipath);
            Debug.Assert(tick == val);

            int tick2 = System.Environment.TickCount;
            Profile.WriteInt("option", "tick2", tick2, inipath);
            Profile.GetInt("option", "tick2", 0, out val, inipath);
            Debug.Assert(tick2 == val);

            Profile.GetInt("option", "tick", 0, out val, inipath);
            Debug.Assert(tick == val);



            bool b;
            Profile.GetBool("option", "singi", false, out b, inipath);
            Debug.Assert(!b);

            Profile.GetBool("option", "singi", true, out b, inipath);
            Debug.Assert(b);


            b = Profile.WriteBool("option", "sin", true, inipath);
            Debug.Assert(b);

            Profile.GetBool("option", "sin", false, out b, inipath);
            Debug.Assert(b);


            // test array
            {
                List<string> ls = new List<string>();
                ls.Add("fewjo  jof  fwefjw3j ffwe         2  ");
                ls.Add("few  fewefjo  jof  fwefjw3j ffwe");
                ls.Add("fewj4324323o  jof3  fwefjw3j ffwe");
                Profile.WriteStringArray("option", "array", ls.ToArray(), inipath);

                string[] outls;
                Profile.GetStringArray("option", "array", out outls, inipath);
                Debug.Assert(ls.Count == outls.Length);
                for ( i = 0; i < ls.Count; ++i)
                    Debug.Assert(ls[i].Trim() == outls[i].Trim());
            }


            // test Test.SectionFormat.wini
            {
                string s;
                Profile.GetString("SEC", "a", "", out s, "Test.SectionFormat.wini");
                Debug.Assert(s == "1");
            }

            // Dont change existing file
            {
                string theFile = "Text.Change";
                System.IO.StreamWriter sw = new System.IO.StreamWriter(theFile);
                sw.WriteLine("STILL");
                sw.Close();

                Profile.WriteString("option", "a", "x", theFile);
            }
        }
    }
}