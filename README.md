# profile
.Net reader/writer for .ini

## Basic Usage
```
string inipath = Application.ExecutablePath + ".ini";
Profile.WriteInt("sec", "aaa", 12345, inipath);
Profile.GetInt("sec", "aaa", 0, out i, inipath);
```
