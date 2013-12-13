#include "common.h"
#include "processMemory.h"
#include "offsets.h"

typedef union BUFFERDWORD
{
	DWORD val;
	char buffer[4];
}BUFFERDWORD;

typedef union BUFFERULONGLONG
{
	unsigned __int64 val;
	char buffer[8];
}BUFFERULONGLONG;

typedef struct COORDS
{
	float x;
	float y;
	float z;
}COORDS;

typedef union BUFFCOORDS
{
	COORDS loc;
	char buffer[12];
}BUFFCOORDS;

typedef union BUFFFLOAT
{
	float val;
	char buffer[4];
}BUFFFLOAT;

void main(void) {
	
	processMemory* wowMemory = new processMemory( TEXT("World of Warcraft") , TEXT("Wow.exe") );
	wowMemory->GetEndScene();

	
	SIZE_T NumberOfBytesRead = 0;
	BUFFERDWORD CurrentManager_Pre;
	CurrentManager_Pre.val = 0;

	BUFFERDWORD CurrentManager;
	CurrentManager.val = 0;

	BUFFERULONGLONG PlayerGUID;
	PlayerGUID.val = 0;
	
	wowMemory->Read( wowMemory->GetBaseAddress() + ClientConnection , CurrentManager_Pre.buffer , sizeof(DWORD) );
	wowMemory->Read( CurrentManager_Pre.val + ObjectMgrOffset , CurrentManager.buffer , sizeof(DWORD) );
	wowMemory->Read( CurrentManager.val + LocalGuidOffset , PlayerGUID.buffer , sizeof(unsigned __int64) );
	printf("Base Address: 0x%08X\n", wowMemory->GetBaseAddress());
	printf("Client Connection offset: 0x%08X\n",ClientConnection);
	printf("ClientConnection Address: 0x%08X\n", wowMemory->GetBaseAddress() + ClientConnection);
	printf("CurrentManager_Pre Address: 0x%08X\n",CurrentManager_Pre.val);
	printf("CurrentManager Location: 0x%08X\n", CurrentManager_Pre.val + ObjectMgrOffset);
	printf("CurrentManager Address: 0x%08X\n",CurrentManager.val);
	printf("Player GUID: \t 0x%016I64X\n",PlayerGUID.val);
	
	// Get the first object
	BUFFERDWORD wowObj,currentObj;
	wowObj.val = 0;
	currentObj.val = 0;
	BUFFERDWORD type;
	type.val = 0;
	BUFFERULONGLONG objGUID;
	objGUID.val = 0;
	BUFFCOORDS playerLocation;
	BUFFFLOAT rotation;
	BUFFERDWORD desc;
	desc.val = 0;
	BUFFERDWORD health;
	health.val = 0;
	BUFFERULONGLONG target;
	target.val = 0;
	BUFFERULONGLONG toTarget;
	toTarget.val = 0;
	

	//char allData[64];			// DELETE ME



	wowMemory->Read( CurrentManager.val + FirstObjectOffset , wowObj.buffer , sizeof(DWORD) );
	wowMemory->Read( wowObj.val + TypeOffset , type.buffer , sizeof(DWORD) );

	//printf("Address of obj: 0x%08X\n",wowObj.val);
	//printf("Type of obj: 0x%08X\n",type.val);

	unsigned int i = 0;
	std::ofstream myfile;
	myfile.open ("GUID.txt");

	while (1)
	{
		Sleep(1000);
		while ( type.val > 0 && type.val <= 7 )
		{
			if (type.val == 4 )
			{
				wowMemory->Read( wowObj.val + GuidOffset, objGUID.buffer , sizeof(unsigned __int64) );
				//wowMemory->Read( wowObj.val , allData , sizeof(allData) );
				/*int u;
				for( u = 0; u < sizeof(allData); u++)
					printf("%X",allData[u]);
				printf("\n");*/

				//printf("Obj GUID: \t 0x%016I64X  type -> %d\n",objGUID.val,type.val);
				myfile << std::hex << objGUID.val << std::endl;
				i++;
				if ( objGUID.val == PlayerGUID.val )
				{
					//printf("Found the player object!\n");
					wowMemory->Read( wowObj.val + Location , playerLocation.buffer , sizeof(playerLocation.buffer) );
					wowMemory->Read( wowObj.val + Rotation , rotation.buffer , sizeof(rotation.buffer) );
					wowMemory->Read( wowObj.val + DescFieldsOffset , desc.buffer , sizeof(desc.buffer) );
					wowMemory->Read( desc.val + HealthOffset , health.buffer , sizeof(health.buffer) );
					wowMemory->Read( desc.val + TargetOffset , target.buffer , sizeof(target.buffer) );
					system("cls");
					printf("Coords X=%f -- Y=%f -- Z=%f -- R=%f\n",playerLocation.loc.x , playerLocation.loc.y , playerLocation.loc.z , rotation.val*57.295 );
					printf("Health = %d  -  Target = 0x%016I64X\n", health.val , target.val );
					//wowMemory->Write( desc.val + TargetOffset ,  toTarget.buffer , sizeof(unsigned __int64) );
					//wowMemory->Read( desc.val + TargetOffset , target.buffer , sizeof(target.buffer) );
					//printf("Health = %d  -  Target = 0x%016I64X\n", health.val , target.val );
					//delete wowMemory;
					myfile.close();
					//std::cin.get();
					//exit(0);
				}
			}
			if ( type.val == 3)
			{
				wowMemory->Read( wowObj.val + GuidOffset, objGUID.buffer , sizeof(unsigned __int64) );
				toTarget.val = objGUID.val;
				//printf("Retargeting -> 0x%016I64X\n", objGUID.val );
			}
			objGUID.val = 0;
			type.val = 0;
			//std::cin.get();
			wowMemory->Read( wowObj.val + NextObjectOffset , wowObj.buffer , sizeof(DWORD) );
			wowMemory->Read( wowObj.val + TypeOffset , type.buffer , sizeof(DWORD) ); 
			//printf("Address of obj: 0x%08X\n",wowObj.val);
			//printf("Type of obj: 0x%08X\n",type.val);
		}
		wowMemory->Read( CurrentManager.val + FirstObjectOffset , wowObj.buffer , sizeof(DWORD) );
		wowMemory->Read( wowObj.val + TypeOffset , type.buffer , sizeof(DWORD) );
		//printf("Scanned %d units\n",i);
	}

	delete wowMemory;
	myfile.close();
	std::cin.get();
 }