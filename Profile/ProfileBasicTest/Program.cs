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
            Profile.GetBinary("aaa","bbb", out mybyte,inipath);

            
            
            
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


            b =Profile.WriteBool("option", "sin", true, inipath);
            Debug.Assert(b);

            Profile.GetBool("option", "sin", false, out b, inipath);
            Debug.Assert(b);
        }
    }
}