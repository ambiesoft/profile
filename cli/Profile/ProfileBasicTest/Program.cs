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
            string ss;

            Profile.WriteString("quote", "quote", "\"", inipath);
            Profile.GetString("quote", "quote", "", out ss, inipath);
            
            Profile.GetString("aaa", "aaa", "", out ss, inipath);
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