using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;


namespace Ambiesoft
{
    public class HashIni
    {
        private Hashtable hash_;
        private HashIni()
        {
            hash_ = new Hashtable();
        }

        internal Hashtable Hash
        {
            get { return hash_; }
        }

        public static HashIni CreateEmptyInstanceForSpecialUse()
        {
            return new HashIni();
        }
    }
}
