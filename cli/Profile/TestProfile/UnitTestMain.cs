using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using Ambiesoft;

namespace TestProfile
{
    [TestClass]
    public class UnitTestMain
    {
        // https://stackoverflow.com/a/283917
        public static string AssemblyDirectory
        {
            get
            {
                string codeBase = Assembly.GetExecutingAssembly().CodeBase;
                UriBuilder uri = new UriBuilder(codeBase);
                string path = Uri.UnescapeDataString(uri.Path);
                return Path.GetDirectoryName(path);
            }
        }

        [TestMethod]
        public void TestMethodMain()
        {
            string inipath = Path.Combine(AssemblyDirectory, "TestProfile.ini");
            int i;
            string ss;

            Assert.IsTrue(Profile.WriteString("quote", "quote", "\"", inipath));
            Assert.IsTrue(Profile.GetString("quote", "quote", "", out ss, inipath));
            Assert.AreEqual(ss, "\"");

            Assert.IsFalse(Profile.GetString("nonexistetnt", "nonexistetnt", "", out ss, inipath));
            Assert.AreEqual(ss, "");

            Assert.IsTrue(Profile.WriteInt("aaa", "aaa", 12345, inipath));
            Assert.IsTrue(Profile.GetInt("aaa", "aaa", 0, out i, inipath));
            Assert.AreEqual(12345, i);

            byte[] writeBytes = new byte[] { 0xaa, 0xbb };
            Assert.IsTrue(Profile.WriteBinary("aaa", "bbb",writeBytes ,inipath));
            byte[] myBytes;
            Assert.IsTrue(Profile.GetBinary("aaa", "bbb", out myBytes, inipath));
            CollectionAssert.AreEqual(writeBytes, myBytes);

            int tick = System.Environment.TickCount;
            Assert.IsTrue(Profile.WriteInt("option", "tick", tick, inipath));
            int val;
            Assert.IsTrue(Profile.GetInt("option", "tick", 0, out val, inipath));
            Assert.AreEqual(tick, val);

            int tick2 = System.Environment.TickCount;
            Assert.IsTrue(Profile.WriteInt("option", "tick2", tick2, inipath));
            Assert.IsTrue(Profile.GetInt("option", "tick2", 0, out val, inipath));
            Assert.AreEqual(tick2, val);

            Assert.IsTrue(Profile.GetInt("option", "tick", 0, out val, inipath));
            Assert.AreEqual(tick, val);

            bool b;
            Profile.GetBool("option", "singi", false, out b, inipath);
            Assert.IsTrue(!b);

            Profile.GetBool("option", "singi", true, out b, inipath);
            Assert.IsTrue(b);


            Assert.IsTrue(Profile.WriteBool("option", "sin", true, inipath));

            Assert.IsTrue(Profile.GetBool("option", "sin", false, out b, inipath));
            Assert.IsTrue(b);


            // test array
            {
                List<string> ls = new List<string>();
                ls.Add("fewjo  jof  fwefjw3j ffwe         2  ");
                ls.Add("few  fewefjo  jof  fwefjw3j ffwe");
                ls.Add("fewj4324323o  jof3  fwefjw3j ffwe");
                Profile.WriteStringArray("option", "array", ls.ToArray(), inipath);

                string[] outls;
                Profile.GetStringArray("option", "array", out outls, inipath);
                Assert.AreEqual(ls.Count, outls.Length);
                for (i = 0; i < ls.Count; ++i)
                    Assert.AreEqual(ls[i].Trim(), outls[i].Trim());
            }


            // test Test.SectionFormat.wini
            {
                string sfPath=Path.Combine(AssemblyDirectory, "..\\..", "Test.SectionFormat.wini");
                Assert.IsTrue(File.Exists(sfPath));
                string s;
                Assert.IsTrue(Profile.GetString("SEC", "a", "", out s, sfPath));
                Assert.AreEqual(s , "1");
            }

            // Dont change existing file
            {
                string theFile = "Text.Change";
                System.IO.StreamWriter sw = new System.IO.StreamWriter(theFile);
                sw.WriteLine("STILL");
                sw.Close();

                Assert.IsTrue(Profile.WriteString("option", "a", "x", theFile));
            }

            // Test Section
            {
                string theFile = "Test.Section.wini";
                Assert.IsTrue(Profile.WriteInt("mysec", "sec1", 1, theFile));
                Assert.IsTrue(Profile.WriteInt("mysec", "sec2", 2, theFile));
                Assert.IsTrue(Profile.WriteInt("mysec", "sec3", 3, theFile));

                string[] arr;
                Assert.IsTrue(Profile.GetSection("mysec", out arr, theFile));
                Assert.AreEqual(arr.Length, 3);
                int sum = 0;
                foreach (string sec in arr)
                {
                    Assert.IsTrue(Profile.GetInt("mysec", sec, 100, out val, theFile));
                    sum += val;
                }
                Assert.AreEqual(sum, (1 + 2 + 3));
            }
         }

        [TestMethod]
        public void TestMethodAsText()
        {
            string iniString = "[aaa]\r\nkkk=mystring\r\n\r\n";
            {
                HashIni ini = HashIni.CreateEmptyInstanceForSpecialUse();

                Profile.WriteString("aaa", "kkk", "mystring", ini);
                string all = Profile.ReadAllAsString(ini);
                Assert.AreEqual(all, iniString);
            }

            {
                HashIni ini = Profile.WriteAllAsString(iniString);
                string all = Profile.ReadAllAsString(ini);
                Assert.AreEqual(all, iniString);
            }

            {
                HashIni ini = Profile.WriteAllAsString(iniString);
                string k;
                Profile.GetString("aaa", "kkk", "", out k, ini);
                Assert.AreEqual(k, "mystring");
            }
        }

        [TestMethod]
        public void TestFloat()
        {
            string inipath = Path.Combine(AssemblyDirectory, "TestProfile.ini");
            float f = 1.234f;
            Assert.IsTrue(Profile.WriteFloat("fff", "ttt", f, inipath));

            f = 0;
            Assert.IsTrue(Profile.GetFloat("fff", "ttt", 0, out f, inipath));
            Assert.AreEqual(f, 1.234f);
        }

        [TestMethod]
        public void TestDouble()
        {
            string inipath = Path.Combine(AssemblyDirectory, "TestProfile.ini");
            double d = 1222.234;
            Assert.IsTrue(Profile.WriteDouble("fff", "ttt", d, inipath));

            d = 0;
            Assert.IsTrue(Profile.GetDouble("fff", "ttt", 0, out d, inipath));
            Assert.AreEqual(d, 1222.234);
        }
    }
}
