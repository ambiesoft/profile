# profile
.Net reader/writer for .ini

## Basic Usage
```
string inipath = Application.ExecutablePath + ".ini";
Profile.WriteInt("sec", "aaa", 12345, inipath);
Profile.GetInt("sec", "aaa", 0, out i, inipath);
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


