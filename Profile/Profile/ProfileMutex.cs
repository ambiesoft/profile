using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.IO;


namespace Ambiesoft
{
    partial class Profile
    {
        static private Mutex createmutex(string filename)
        {
            FileInfo fi = new FileInfo(filename);
            string mutexname = @"Global\profile-" + fi.FullName.ToLower().Replace('\\', '/');
            if (mutexname.Length > 260)
                mutexname = mutexname.Substring(0, 260);

            return new Mutex(false, mutexname);
        }
        static private void waitmutex(Mutex mutex)
        {
            try
            {
                mutex.WaitOne();
            }
            catch (Exception)
            {
                // abandaned causes exception
            }
        }
    }
}
