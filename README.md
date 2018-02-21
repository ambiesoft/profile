# profile
.Net and C++ reader/writer for .ini

## Basic Usage
### C#
Add reference to *Ambiesoft.Profile.dll*.

code example:
```
string inipath = Application.ExecutablePath + ".ini";
Profile.WriteInt("sec", "aaa", 12345, inipath);
Profile.GetInt("sec", "aaa", 0, out i, inipath);
```

### C++
Include *Profile.h*.

code example:
```
#include "dir/Profile.h"
using namespace Ambiesoft;

string filename = "TestProfile.ini";

Profile::WriteInt("MySection", "MyKey", 666666, inifile);

int intval;
Profile::GetInt("MySection", "MyKey", 0, intval, inifile);
assert(intval == 666666);
```

## Character Code
Ini File will be loaded and saved with UTF8. 

## Benefit
- Easy to use
- Backward and forward compatibility
- Easy to edit
 

## Available type for loading and saving
- string (single line)
- int
- byte[]
- string array
- bool
 
## License
Public Domain


