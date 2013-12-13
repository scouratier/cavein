#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <winuser.h>
#include <string>
#include <iostream>
#include <tlhelp32.h> 
#include <tchar.h> 
#include <fstream>
#include <sstream>  // Required for stringstreams
#include <d3d9.h>
#include <d3dx9.h>

using namespace std;
using std::string;

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")