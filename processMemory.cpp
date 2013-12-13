// Constructor
// Tries to find the window, the pid and get the base memory of the module
#include "processMemory.h"

processMemory::processMemory()
{
	// you should never use this..
	exit(1);
}

processMemory::processMemory(LPCWSTR windowName, WCHAR* processName)
{
	this->deviceFunctionAddresses_ = NULL;
	this->dllHandle_ = NULL;
	this->windowName_ = windowName;
	this->processName_ = processName;
	this->d3d9_ = TEXT("d3d9");
	HWND hWnd = FindWindow( NULL , windowName ); 
	if ( hWnd == NULL )
	{
		printf("Unable to find the window.. panic!\n");
		exit(1);
	}
	this->hWnd_ = hWnd;

	DWORD pid;
	if (!GetWindowThreadProcessId(this->hWnd_, &pid))
	{
		printf("Could not get the PID\n");
		exit(1);
	}
	this->pid_ = pid;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, this->pid_);
	if ( hProcess == NULL )
	{
		printf("Could not get the handle for the process\n");
		exit(1);
	}
	this->hProcess_ = hProcess;

	BOOL privs = this->SetDebugPrivileges();
	if (!privs)
	{
		printf("Could not set privileges =( \n");
		exit(1);
	}
	
	if (!SetBaseAddress())
	{
		printf("Could not figure out the base address for the module\n");
		exit(1);
	}
	
}

HWND processMemory::GethWnd()
{
	return this->hWnd_;
}

DWORD processMemory::GetPid()
{
	return this->pid_;
}

HANDLE processMemory::GethProcess()
{
	return this->hProcess_;
}

DWORD processMemory::GetBaseAddress()
{
	return this->baseAddress_;
}

BOOL processMemory::SetDebugPrivileges()
{
   BOOL               bRET = FALSE;
   TOKEN_PRIVILEGES   tp;
   HANDLE             hToken;

   if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
   {
      if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
      {
         if (hToken != INVALID_HANDLE_VALUE)
         {
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            tp.PrivilegeCount = 1;
            if (AdjustTokenPrivileges(hToken, FALSE, &tp, 0, 0, 0))
               bRET = TRUE;
            CloseHandle(hToken);
         }
      }
   }
   return bRET;
}

BOOL processMemory::SetBaseAddress() 
{ 
  HANDLE hModuleSnap = INVALID_HANDLE_VALUE; 
  MODULEENTRY32 me32; 
 
//  Take a snapshot of all modules in the specified process. 
  hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, this->pid_ ); 
  if( hModuleSnap == INVALID_HANDLE_VALUE ) 
  { 
    printf("CreateToolhelp32Snapshot (of modules)"); 
    return( FALSE ); 
  } 
 
//  Set the size of the structure before using it. 
  me32.dwSize = sizeof( MODULEENTRY32 ); 
 
//  Retrieve information about the first module, 
//  and exit if unsuccessful 
  if( !Module32First( hModuleSnap, &me32 ) ) 
  { 
    printf("Module32First");  // Show cause of failure 
    CloseHandle( hModuleSnap );     // Must clean up the snapshot object! 
    return( FALSE ); 
  } 
 
//  Now walk the module list of the process, 
//  and display information about each module 
  do 
  {
	  if ( !wcscmp( me32.szModule, this->processName_) ) {
		_tprintf( TEXT("\n\n     MODULE NAME:     %s"),             me32.szModule ); 
		_tprintf( TEXT("\n     executable     = %s"),             me32.szExePath ); 
		_tprintf( TEXT("\n     process ID     = 0x%08X"),         me32.th32ProcessID ); 
		_tprintf( TEXT("\n     ref count (g)  =     0x%04X"),     me32.GlblcntUsage ); 
		_tprintf( TEXT("\n     ref count (p)  =     0x%04X"),     me32.ProccntUsage ); 
		_tprintf( TEXT("\n     base address   = 0x%08X"), (DWORD) me32.modBaseAddr ); 
		_tprintf( TEXT("\n     base size      = %d\n"),             me32.modBaseSize );
		this->baseAddress_ = (DWORD) me32.modBaseAddr;
		CloseHandle( hModuleSnap ); 
		return( TRUE );
	  }
 
  } while( Module32Next( hModuleSnap, &me32 ) ); 

//  Do not forget to clean up the snapshot object. 
  CloseHandle( hModuleSnap ); 
  return( FALSE ); 
} 

SIZE_T processMemory::Read( DWORD address , CHAR* buffer , SIZE_T toRead )
{
	SIZE_T NumberOfBytesRead = 0;

	BOOL sucess = ReadProcessMemory( this->hProcess_ , (LPVOID) address , buffer, toRead, &NumberOfBytesRead);
	if (sucess)
	{
		//printf("Read %d bytes\n", NumberOfBytesRead);
		//printf("\n");
	}
	else
	{
		DWORD error;
		error = GetLastError();
		printf("Error %d\n", error);
	}
	return NumberOfBytesRead;
}

SIZE_T processMemory::Write( DWORD address , CHAR* buffer , SIZE_T toWrite )
{
	SIZE_T NumberOfBytesWritten = 0;

	BOOL sucess = WriteProcessMemory( this->hProcess_ , (LPVOID) address , buffer, toWrite, &NumberOfBytesWritten);
	if (sucess)
	{
		
	}
	else
	{
		DWORD error;
		error = GetLastError();
		printf("Error %d\n", error);
	}
	return NumberOfBytesWritten;
}

BOOL processMemory::GetEndScene()
{

	if(!this->deviceFunctionAddresses_){
		HMODULE		hDll = LoadLibraryA("d3d9.dll");
		if (hDll == NULL)
		{
			printf("Could not load module handle\n");
			std::cin.get();
		}
		LPDIRECT3D9       pD3D       = NULL;
		LPDIRECT3DDEVICE9 pd3dDevice = NULL;

		HWND hWnd = CreateWindowA("BUTTON", "Temporary Window", WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, this->dllHandle_, NULL);
		if (hWnd == NULL)
		{
			printf("Could not create window\n");
			std::cin.get();
		}
		pD3D = Direct3DCreate9( D3D_SDK_VERSION );
		if(pD3D == NULL )
		{
			printf("Could not create D3D\n");
			std::cin.get();
		}
		D3DDISPLAYMODE d3ddm;

		if( FAILED( pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
		{
			printf("Could not create D3D\n");
			std::cin.get();
		}
		D3DCAPS9 d3dCaps;
		if( FAILED(pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps ) ) )
		{
			// TO DO: Respond to failure of GetDeviceCaps
			return 0;
		}

		DWORD dwBehaviorFlags = 0;

		if( d3dCaps.VertexProcessingCaps != 0 )
			dwBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else
			dwBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		//
		// Everything checks out - create a simple device.
		//

		D3DPRESENT_PARAMETERS d3dpp;
		memset(&d3dpp, 0, sizeof(d3dpp));

		d3dpp.BackBufferFormat       = d3ddm.Format;
		d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
		d3dpp.Windowed               = TRUE;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

		if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, dwBehaviorFlags, &d3dpp, &pd3dDevice ) ) )
		{
			printf("Could not create D3D Device\n");
			std::cin.get();
		}
		// retrieve a pointer to the VTable
		uintptr_t* pInterfaceVTable = (uintptr_t*)*(uintptr_t*)pd3dDevice;
		this->deviceFunctionAddresses_ = new uintptr_t*[interfaceMethodCount];

		printf("d3d9.dll base address: 0x%x\n", hDll);

		// Retrieve the addresses of each of the methods (note first 3 IUnknown methods)
		// See d3d9.h IDirect3D9Device to see the list of methods, the order they appear there
		// is the order they appear in the VTable, 1st one is index 0 and so on.
		for (int i=0; i<interfaceMethodCount; i++) {
			this->deviceFunctionAddresses_[i] = (uintptr_t*)pInterfaceVTable[i];
						
			// Log the address offset
			printf("Method [%i] offset: 0x%x\n", i, pInterfaceVTable[i] - (uintptr_t)hDll);
		}

	}

	/*LPDIRECT3D9(__stdcall*pDirect3DCreate9)(UINT) = (LPDIRECT3D9(__stdcall*)(UINT))GetProcAddress( hDll, "Direct3DCreate9");

	LPDIRECT3D9 pD3D = pDirect3DCreate9(D3D_SDK_VERSION);

		
	D3DDISPLAYMODE d3ddm;
	HRESULT hRes = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm );
	D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = d3ddm.Format;

	IDirect3DDevice9 * ppReturnedDeviceInterface;	// interface IDirect3DDevice9 (pointer to array of pointers)

	HWND hWnd=FindWindowA(NULL, "Your window");

	hRes = pD3D->CreateDevice( 
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,	// the device we suppose any app would be using.
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&d3dpp, &ppReturnedDeviceInterface);

	unsigned long* pInterface = (unsigned long*)*((unsigned long*)ppReturnedDeviceInterface);*/

	return true;
}