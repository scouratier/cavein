const DWORD ClientConnection  = 0xEABE18;
const DWORD ObjectMgrOffset   = 0x462C;
const DWORD LocalGuidOffset   = 0xE0;
const DWORD FirstObjectOffset = 0xCC;
const DWORD NextObjectOffset  = 0x34;
const DWORD GuidOffset        = 0x28;
const DWORD DescFieldsOffset  = 0x4;
const DWORD TypeOffset        = 0xC;
const DWORD NPCName1		  = 0x9AC;
const DWORD NPCName2		  = 0x6C;
const DWORD Location		  = 0x830;
const DWORD Rotation		  = 0x840;
const DWORD CTM_Base		  = 0xCF99D0;
const DWORD CTM_X			  = 0x8C;
const DWORD CTM_Y			  = 0x90;
const DWORD CTM_Z			  = 0x94;
const DWORD CTM_Action		  = 0x1C;
const DWORD HealthOffset	  = 0x84;
const DWORD TargetOffset	  = 0x58;

/*
//ClntObjMgrInitializeStd
#define CurMgrPointer                   0xEABE18    		        
//Same place as above
#define CurrMgrOffset                   0x462C                   
//ClntObjMgrEnumVisibleObjects
#define FirstObjectOffset               0xCC				        
//ClntObjMgrEnumVisibleObjects
#define NextObjectOffset                0x34 				    
//ClntObjMgrGetActivePlayer
#define localPlayerGUIDOffset           0xE0                     
//ClntObjMgrEnumVisibleObjects
#define GameObjGUIDOffset               0x28				       
#define GameObjTypeOffset               0xC				      
#define MouseOverGUID                   0xD4D410
#define PlayerNameOffset                0xEABE58
#define RealmName                       0xEABFEE
#define BuildNumber                     0xB8020C
//Framescript_ExecuteBuffer
#define Framescript_ExecuteBuffer       0x4ffa0		
//FrameScript__GetLocalizedText
#define GetLocalizedText                0x40c69a
// UpdateDisplayInfo
#define UpdateDisplayInfo               0x42636b
#define HelmOffset                      19164
#define DisplayIDPlayer                 15756
// ClntObjMgrEnumVisibleObjects
#define ClntObjMgrEnum                  0x393175
// Reverse UpdatePlayerAFK  (0x8AF5CD)
#define GameTimeStamp                   0xB91298 or 0xBA9D90
#define LastHardwareAction              0xb9e044
// ClickToMove - CGUnit_C__InitializeTrackingState
#define ClickToMoveOfs                  0x417b06*/