/*
const char* asciiString =
"   _____  _______________ _      _________ ____\n"
"  /  _/ |/ /_  __/ __/ _ | | /| / / __/ _ )_  /\n"
"  / //    / / / / _// , _/ |/ |/ / _// _  |/ /_\n"
"/___/_/|_/ /_/ /___/_/|_||__/|__/___/____//___/\n\n\n\n";
*/

#include "Includes_CSS.h"
#include "Includes.h"


IBaseClientDLL* g_pClient = NULL;
CInput* g_pInput = NULL;
IVEngineClient* g_pEngine = NULL;
IClientEntityList* g_pEntList = NULL;
IEngineTrace* g_pEnginetrace = NULL;
IMatSystemSurface* g_pMatSystemSurface = NULL;
vgui::ISurface* g_pSurface = NULL;
IVModelInfoClient* g_pModelinfo = NULL;
vgui::IPanel* g_pPanel = NULL;
CGlobalVarsBase* g_pGlobalVars = NULL;
ICvar* g_pCvar = NULL;
vgui::ILocalize* g_pVGuiLocalize = NULL;
IPhysicsSurfaceProps* g_pPhysics = NULL;
IGameMovement* g_pGameMovement = NULL;
IVDebugOverlay* g_pDebugOverlay = NULL;
IVModelRender* g_pModelRender = NULL;
IVRenderView* g_pRenderView = NULL;

IMaterialSystem* g_pIMaterialSystem = NULL;

IGameEventManager2* g_pGameEventManager2 = NULL;


Base::Utils::CVMTHookManager* g_pPaintTraverseVMT = NULL;
Base::Utils::CVMTHookManager* g_pCreateMoveVMT = NULL;
Base::Utils::CVMTHookManager* g_pEngineVMT = NULL;
Base::Utils::CVMTHookManager* g_pD3D9vmt = NULL;
Base::Utils::CVMTHookManager* g_pInputVmt = NULL;
Base::Utils::CVMTHookManager* g_pDrawModelVMT = NULL;
Base::Utils::CVMTHookManager* g_pIMaterialSystemVMT = NULL;

ISteamClient* g_pSteamClient = NULL;
ISteamFriends* g_pSteamFriends = NULL;
ISteamUtils* g_pSteamUtils = NULL;
ISteamUser* g_pSteamUser = NULL;
ISteamUserStats* g_pSteamStats = NULL;
ISteamMatchmaking* g_pMatchMaking = NULL;
ISteamMatchmakingServers* g_pMatchMakingSvr =  NULL;
ISteamApps* g_pSteamApps = NULL;
CreateInterfaceFn g_SteamClientFactory = NULL;

cVisuals g_Visuals;
cNetVars g_NetVars;
cDraw g_Draw;
cAimbot g_Aimbot;

VOID HitESP ( VOID );

class CGameEvents : public IGameEventListener2 {
public:
	void Init( void );
	void FireGameEvent( IGameEvent *event );
};
CGameEvents g_GameEvents;
/*
DWORD poffsets::dw_cHudradar() // far esp poiner 
{
GetPattern_PTR(cRadar,dw_cHudradar,Modules[0],CLIENT_SIZE,"\xCC\x8B\x0D\x00\x00\x00\x00\x85\xC9\x74\x08\x8B\x01\x8B\x10\x6A\x03\xFF\xD2\xC3","xxx????xxxxxxxxxxxxx",0x3);
}
findmaterial index: 70
*/

bool WorldToScreen ( const Vector &vOrigin, Vector &vScreen )
{
	return ( g_pDebugOverlay->ScreenPosition( vOrigin, vScreen ) != 1 );
}

void AngleVectors ( const QAngle &angles, Vector *forward, Vector *right, Vector *up )
{
	Assert( s_bMathlibInitialized );

	float sr, sp, sy, cr, cp, cy;

#ifdef _X360
	fltx4 radians, scale, sine, cosine;
	radians = LoadUnaligned3SIMD( angles.Base() );
	scale = ReplicateX4( M_PI_F / 180.f );
	radians = MulSIMD( radians, scale );
	SinCos3SIMD( sine, cosine, radians );
	sp = SubFloat( sine, 0 );	sy = SubFloat( sine, 1 );	sr = SubFloat( sine, 2 );
	cp = SubFloat( cosine, 0 );	cy = SubFloat( cosine, 1 );	cr = SubFloat( cosine, 2 );
#else
	SinCos( DEG2RAD( angles[YAW] ), &sy, &cy );
	SinCos( DEG2RAD( angles[PITCH] ), &sp, &cp );
	SinCos( DEG2RAD( angles[ROLL] ), &sr, &cr );
#endif

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1*sr*sp*cy+-1*cr*-sy);
		right->y = (-1*sr*sp*sy+-1*cr*cy);
		right->z = -1*sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy+-sr*-sy);
		up->y = (cr*sp*sy+-sr*cy);
		up->z = cr*cp;
	}
}

void initLOG ( void )
{
	using namespace Base::Utils;

	Base::Utils::ShowConsoleCursor( FALSE );

	Base::Utils::SetColor( GREEN );

	Base::Utils::printfCenter( "   _____  _______________ _      _________ ____" ); printf( "\n" );
	Base::Utils::printfCenter( "  /  _/ |/ /_  __/ __/ _ | | /| / / __/ _ )_  /" ); printf( "\n" );
	Base::Utils::printfCenter( "  / //    / / / / _// , _/ |/ |/ / _// _  |/ /_" ); printf( "\n" );
	Base::Utils::printfCenter( "/___/_/|_/ /_/ /___/_/|_||__/|__/___/____//___/" );
	printf( "\n\n" );
	Base::Utils::SetColor( YELLOW );
	Base::Utils::printfCenter( "[ hidden-boards.us ]" );printf( "\n" );printf( "\n" );

	Base::Utils::SetColor( RED );

	printf( "            " );
	std::cout<<"\xc9";Base::Utils::dup('\xcd', AsciiBoxLen);std::cout<<'\xbb'<<std::endl;
	printf( "            " );
	std::cout<<"\xba";Base::Utils::dup(' ', AsciiBoxLen);std::cout<<'\xba'<<std::endl;
	printf( "            " );
	std::cout<<"\xba";Base::Utils::dup(' ', AsciiBoxLen);std::cout<<'\xba'<<std::endl;
	printf( "            " );
	std::cout<<"\xba";Base::Utils::dup(' ', AsciiBoxLen);std::cout<<'\xba'<<std::endl;
	printf( "            " );
	std::cout<<"\xba";Base::Utils::dup(' ', AsciiBoxLen);std::cout<<'\xba'<<std::endl;
	printf( "            " );
	std::cout<<"\xba";Base::Utils::dup(' ', AsciiBoxLen);std::cout<<'\xba'<<std::endl;
	printf( "            " );
	std::cout<<"\xba";Base::Utils::dup(' ', AsciiBoxLen);std::cout<<'\xba'<<std::endl;
	printf( "            " );
	std::cout<<"\xba";Base::Utils::dup(' ', AsciiBoxLen);std::cout<<'\xba'<<std::endl;
	printf( "            " );
	std::cout<<"\xba";Base::Utils::dup(' ', AsciiBoxLen);std::cout<<'\xba'<<std::endl;
	printf( "            " );
	std::cout<<"\xba";Base::Utils::dup(' ', AsciiBoxLen);std::cout<<'\xba'<<std::endl;
	printf( "            " );
	std::cout<<"\xba";Base::Utils::dup(' ', AsciiBoxLen);std::cout<<'\xba'<<std::endl;
	printf( "            " );
	std::cout<<"\xba";Base::Utils::dup(' ', AsciiBoxLen);std::cout<<'\xba'<<std::endl;
	printf( "            " );
	std::cout<<"\xba";Base::Utils::dup(' ', AsciiBoxLen);std::cout<<'\xba'<<std::endl;
	printf( "            " );
	std::cout<<"\xba";Base::Utils::dup(' ', AsciiBoxLen);std::cout<<'\xba'<<std::endl;
	printf( "            " );
	std::cout<<"\xc8";Base::Utils::dup('\xcd', AsciiBoxLen);std::cout<<'\xbc'<<std::endl;
}

void LOG ( const char* pszFunction, bool Succeeded )
{
	static int lineCounter = 8;

	Base::Utils::SetColor( WHITE );
	Base::Utils::SetPos( 14, lineCounter );
	int x = 0;
	const char* barRotate = "|\\-/";
	printf ( pszFunction );
	//printf ( "... " );

	static int calls = 0;
	for ( ; ; )
	{
		if ( x > 3 )
			x = 0;

		printf( "%c\b", barRotate[x++] );
		Base::Utils::sleep( 50 );

		calls++;

		if ( calls >= _GetRandom( 5, 10 ) )
			break;
	}

	Base::Utils::SetColor( Succeeded ? GREEN : RED );
	printf( Succeeded ? "OK!\n" : "ERROR!\n" );

	lineCounter++;
	calls = 0;
}

PCHAR szModuleList[ ] = 
{
	"client.dll",
	"engine.dll",
	"vguimatsurface.dll",
	"vgui2.dll",
	"vstdlib.dll",
	"MaterialSystem.dll"
};

bool bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for(;*szMask;++szMask,++pData,++bMask)
		if(*szMask=='x' && *pData!=*bMask )
			return false;
	return (*szMask) == NULL;
}
DWORD dwFindPattern(DWORD dwAddress,DWORD dwLen,BYTE *bMask,char * szMask)
{
	for(DWORD i=0; i < dwLen; i++)
	{
		if( bDataCompare( (BYTE*)( dwAddress+i ),bMask,szMask) )
			return (DWORD)(dwAddress+i);
	}
	return 0;
}

std::string g_lastVersionString = "";
struct EasyInterface_s
{
	const DWORD BruteForceInterfaceList( PCHAR pszModule )
	{
		HMODULE hModule = GetModuleHandle( pszModule );
		FARPROC CIFn = GetProcAddress( hModule, "CreateInterface" );

		// This module doesn't have the CreateInterface export
		if ( CIFn == NULL )
			return NULL;

		DWORD dwJmpBase = dwFindPattern( ( DWORD )CIFn, 0x100, ( PBYTE )"\xE9", "x" ); // Find JMP
		DWORD dwJmpOffset = *( DWORD* )( dwJmpBase + 0x1 ); // Dereference the relative JMP offset
		DWORD dwRealBase = ( dwJmpBase + 0x5 ) - dwJmpOffset - 0x100; // Assume the virtual base is 0x100 because we're lazy and then calculate the address
		DWORD dwMovBase = dwFindPattern( dwRealBase, 0x100, ( PBYTE )"\x8B\x35", "xx" ); // Find the address that moves the global interface list
		DWORD dwFinal = **( DWORD** )( dwMovBase + 0x2 ); // Now we have our global interface list

		return dwFinal;
	}

	template< typename T >
	T* QuickGetPointer( PCHAR pszLabel, PCHAR pszModule = NULL )
	{
		g_lastVersionString = "";

		// Passed a module so we'll only look here
		if ( pszModule )
		{
			EasyInterface_s* pBase = ( EasyInterface_s* )BruteForceInterfaceList( pszModule ); // Get base pointer

			// Loopdy loop
			do
			{
				if ( strstr( pBase->pszName, pszLabel ) ) // Check if the current interface name is a match
				{
					if ( isdigit( pBase->pszName[ strlen( pszLabel ) ] ) ) // Make sure we're getting the proper interface (VClientxxx instead of VClientEntityListxxx, etc.)
					{
						g_lastVersionString = pBase->pszName;
						g_lastVersionString += " ";

						if ( !strstr( pBase->pszName, "GAMEEVENTSMANAGER001" ) )
						return ( T* )pBase->GetPointer( ); // Return the pointer
					}
				}
			} while ( pBase = pBase->pNext );
		} else
		{
			// Didn't pass a module so let's search them all
			UINT TotalSize = sizeof( szModuleList ) / sizeof( PCHAR );

			for ( UINT i = 0; i < TotalSize; i ++ )
			{
				pszModule = szModuleList[ i ];
				EasyInterface_s* pBase = ( EasyInterface_s* )BruteForceInterfaceList( pszModule ); // Get base pointer

				// Loopdy loop
				do
				{
					if ( strstr( pBase->pszName, pszLabel ) ) // Check if the current interface name is a match
					{
						//MessageBox( 0, pszLabel,  pBase->pszName, 0 );
						if ( isdigit( pBase->pszName[ strlen( pszLabel ) ] ) ) // Make sure we're getting the proper interface (VClientxxx instead of VClientEntityListxxx, etc.)
						{
							g_lastVersionString = pBase->pszName;
							g_lastVersionString += " ";
							if ( !strstr( pBase->pszName, "GAMEEVENTSMANAGER001" ) )
							return ( T* )pBase->GetPointer( ); // Return the pointer
						}
					}
				} while ( pBase = pBase->pNext );
			}
		}

		return NULL; // Couldn't find our pointer
	}

	// Member variables
	InstantiateInterfaceFn GetPointer;
	const char* pszName;
	EasyInterface_s* pNext;
	static EasyInterface_s* pBaseList;
} EasyInterface_t;


class __CTraceFilterSkipTwoEntities
{
public:
	__CTraceFilterSkipTwoEntities( void *pPassEnt1, void *pPassEnt2 )
	{
		m_pPassEnt1 = pPassEnt1;
		m_pPassEnt2 = pPassEnt2;
	}

	virtual bool ShouldHitEntity( void *pHandleEntity, int contentsMask )
	{
		return !( pHandleEntity == m_pPassEnt1 || pHandleEntity == m_pPassEnt2 );
	}

	virtual int GetTraceType() const
	{
		return 0;
	}

	void *m_pPassEnt1;
	void *m_pPassEnt2;
};
// CTraceFilterSkipTwoEntities traceFilter( kirois, homofgt );
// TraceRay( ..., ( ITraceFilter *)&traceFilter );

int CenterX, CenterY;

typedef struct hit_s
{
	int entityId;
	int dmgDone;
	DWORD timeStart;
} hit_t;
std::vector < hit_s > g_vHits;

HFont drawFont;
VOID __stdcall hooked_PaintTraverse ( vgui::VPANEL vguiPanel, bool forceRepaint, bool allowForce )
{
	static DWORD realPaintTraverse = g_pPaintTraverseVMT->dwGetMethodAddress( 40 );

	__asm
	{
		PUSH allowForce;
		PUSH forceRepaint;
		PUSH vguiPanel;
		MOV ECX, g_pPanel;
		CALL realPaintTraverse;
	}

	const char* pszPanelName = g_pPanel->GetName( vguiPanel );
	bool isValidPanel = false;

	if ( pszPanelName && pszPanelName [ 0 ] == 'M' && pszPanelName [ 3 ] == 'S' &&
		pszPanelName [ 9 ] == 'T' && pszPanelName [ 12 ] == 'P' )
		isValidPanel = true;

	if ( !isValidPanel )
		return;

	if ( g_pEngine == NULL )
		return;
	if ( g_pEngine->IsConnected( ) == false || g_pEngine->IsInGame( ) == false || g_pEngine->Con_IsVisible() == true )
		return;

	REPEAT_ONCE( g_GameEvents.Init(); g_Draw.initFont(16,"Segoe Script"); )
	
	// DRAWING
	int ScreenX, ScreenY;
	g_pEngine->GetScreenSize ( ScreenX, ScreenY );
	CenterX = ScreenX * 0.5;
	CenterY = ScreenY * 0.5;
	
	// DRAWING
	g_Visuals.drawESP();
	HitESP();
	g_Draw.drawString(true,250,10,255,0,0,/*InterweBz FX 1.0*/_X( "\xaa\x83\xcb\xaf\xc5\x4b\x98\x51\x99\xcd\xf9\x92\x97\x0d\xd3\x23", 0xe3edbfca, 0xb73cfd13 ));
	//g_Visuals.doXhair(TRUE,CenterX,CenterY,255);
}





void Speedhack( void )
{
	//static ConVar* psv_cheats = g_pCvar->FindVar( "sv_cheats" );
	static ConVar* psv_timescale = g_pCvar->FindVar( "host_timescale" );
	//REPEAT_ONCE( HideVar( pCvar, "my_sv_cheats" ); /*HideCvar( "host_timescale" );*/ )

	BOOL bSpeedKeyPressed = FALSE;
	GetAsyncKeyState( 0x56 ) < 0 ? bSpeedKeyPressed = TRUE : bSpeedKeyPressed = FALSE;

	static DWORD dwPatchOff = ( DWORD )( GetModuleHandle( "engine" ) ) + 0x594110;
	if ( *( PBYTE )( dwPatchOff ) != 0x1 )
		memset( ( void* )( dwPatchOff ), 0x1, 1 );

	//WriteProcessMemory (hProcess, (LPVOID)(engine + 0x590AE8), &sv_cheats, sizeof(sv_cheats), NULL);
	if ( bSpeedKeyPressed )
	{
		//psv_cheats->SetValue( 1 );
		psv_timescale->SetValue( 10 );
	}

	else
	{
		//psv_cheats->SetValue( 0 );
		psv_timescale->SetValue( 1 );
	}

}


C_BaseCombatWeapon* GetBaseCombatActiveWeapon ( CBaseEntity* pBaseEntity )
{
	__asm
	{
		MOV  EAX, pBaseEntity	; eax = pBaseEntity
		MOV  EDI, EAX	; edi = eax
		MOV  EAX, DWORD PTR DS :[EDI]	; eax = pointer to edi
		MOV  ECX, EDI	; ecx = edi
		CALL DWORD PTR DS :[EAX+0x35C]	; call GetActiveWeapon
	}
}

float sseSqrt( float x )
{
	float root = 0.0f;

	__asm
	{
		sqrtss xmm0, x
		movss root, xmm0
	}

	return root;
}

#define square( x ) ( x * x )
void vectorAngles( PFLOAT forward, PFLOAT angles )
{
	if( forward[1] == 0.0f && forward[0] == 0.0f )
	{
		angles[0] = ( forward[2] > 0.0f ) ? 270.0f : 90.0f;
		angles[1] = 0.0f;
	}
	else
	{
		float len2d = sseSqrt( square( forward[0] ) + square( forward[1] ) );

		angles[0] = RAD2DEG( atan2f( -forward[2], len2d ) );
		angles[1] = RAD2DEG( atan2f( forward[1], forward[0] ) );

		if( angles[0] < 0.0f ) angles[0] += 360.0f;
		if( angles[1] < 0.0f ) angles[1] += 360.0f;
	}

	angles[2] = 0.0f;
}

namespace Base
{
    namespace pnospread
    {
        void RandomSeed( unsigned int seed )
        {
            typedef int ( __cdecl* RandomSeed_t )( unsigned int seed );
            static RandomSeed_t pRandomSeed = NULL;

            if ( !pRandomSeed )
                pRandomSeed = ( RandomSeed_t )( ( DWORD )( GetProcAddress( GetModuleHandle( "vstdlib.dll" ), "RandomSeed" ) ) );
            if ( pRandomSeed )
                pRandomSeed( seed );
        }

        float RandomFloat( float min, float max )
        {
            typedef float ( __cdecl* RandomFloat_t )( float min, float max );
            static RandomFloat_t pRandomFloat = NULL;

            if ( !pRandomFloat )
                pRandomFloat = ( RandomFloat_t )( ( DWORD )( GetProcAddress( GetModuleHandle( "vstdlib.dll" ), "RandomFloat" ) ) );
            if ( pRandomFloat )
                return pRandomFloat( min, max );
        }

        void GetSpreadVars( CBaseEntity* LocalBaseEntity, int Seed, Vector& outSpread )
        {
            #define SPREAD_OFFSET 0x5DC

            float _spreadX, _spreadY;

            if ( !LocalBaseEntity )
                return;

            C_BaseCombatWeapon* m_pWeapon = GetBaseCombatActiveWeapon( LocalBaseEntity );

            if ( !m_pWeapon )
                return;

            __asm // UpdateAccuracyPenalty
            {
                MOV ESI, m_pWeapon;
                MOV EAX, DWORD PTR DS:[ ESI ];
                MOV EDX, DWORD PTR DS:[ EAX + 0x5EC ];
                MOV ECX, ESI;
                CALL EDX;
            }

            __asm // GetSpreadCone -> Y
            {
                MOV ESI, m_pWeapon;
                MOV EAX, DWORD PTR DS:[ ESI ];
                MOV EDX, DWORD PTR DS:[ EAX + 0x5E8 ];
                MOV ECX, ESI;
                CALL EDX;
                FSTP _spreadY;
            }

            __asm // GetSpread -> X
            {
                MOV ESI, m_pWeapon;
                MOV EAX, DWORD PTR DS:[ ESI ];
                MOV EDX, DWORD PTR DS:[ EAX + 0x5E4 ];
                MOV ECX, ESI;
                CALL EDX;
                FSTP _spreadX;
            }

            pnospread::RandomSeed( (Seed & 0xFF) + 1 );

            float flRandom1 = pnospread::RandomFloat( 0.0f, M_PI_F + M_PI_F );
            float flRandom2 = pnospread::RandomFloat( 0.0f, _spreadX );
            float flRandom3 = pnospread::RandomFloat( 0.0f, M_PI_F + M_PI_F );
            float flRandom4 = pnospread::RandomFloat( 0.0f, _spreadY );

            float cosresult = flRandom2 * cosf( flRandom1 ) + flRandom4 * cosf( flRandom3 );
            float sinresult = flRandom2 * sinf( flRandom1 ) + flRandom4 * sinf( flRandom3 );

            outSpread.x = cosresult;
            outSpread.y = sinresult;
            outSpread.z = 0.0f;
        }

        void ApplySpreadAngles( CUserCmd* pUserCmd, CBaseEntity* LocalBaseEntity, QAngle& outAngles )
        {
            Vector vForward, vRight, vUp, vSpread;

            GetSpreadVars( LocalBaseEntity, pUserCmd->random_seed, vSpread );

            AngleVectors( pUserCmd->viewangles, &vForward, &vRight, &vUp );
            Vector aimSpot = vForward + ( vRight * vSpread[ 0 ] ) + ( vUp * vSpread[ 1 ] );
            //aimSpot.NormalizeInPlace();
///*
/*
            if  ( !aimSpot[0] == 0 && !aimSpot[1] == 0 )
            {
                outAngles[0] = -atan2f( aimSpot[2], _mm_cvtss_f32( _mm_rcp_ss( _mm_rsqrt_ss( _mm_set_ss( aimSpot[0] * aimSpot[0] + aimSpot[1] * aimSpot[1] ) ) ) ) ) * 57.2957f;
                outAngles[1] = atan2f( aimSpot[1], aimSpot[0] ) * 57.2957f;
                outAngles[2] = 0;
            }
       // */
       // //*/
            vectorAngles( aimSpot.Base(), outAngles.Base() );

            //clampAngle( outAngles.Base() );
            outAngles = pUserCmd->viewangles - outAngles;

            //if ( /*(pUserCmd->buttons & IN_ATTACK) &&*/ !g_AntiAim )
            //{
            //    pUserCmd->viewangles += outAngles;
           // }

        }
    }
}


FORCEINLINE float VectorNormalizeHLSDK (float v[3])
{
	float   length, ilength;

	length = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	length = sqrt (length);         // FIXME

	if (length)
	{
		ilength = 1/length;
		v[0] *= ilength;
		v[1] *= ilength;
		v[2] *= ilength;
	}

	return length;
}

QAngle GetPunchAngle( CBaseEntity* pBaseEntity )
{
	//return *(QAngle*)( (DWORD)this + 0xD24 + 0x6C );
	static DWORD dwOffs = NULL;
	static DWORD dwOffs2 = NULL;
	if( dwOffs == NULL ) {
		dwOffs = g_NetVars.dwGetNetVarOffset( "DT_BasePlayer>localdata>m_Local" );
	}
	if( dwOffs2 == NULL ) {
		dwOffs2 = g_NetVars.dwGetNetVarOffset( "DT_BasePlayer>localdata>m_Local>m_vecPunchAngle" );
	}
	return *(QAngle*)( (DWORD)pBaseEntity + dwOffs + dwOffs2 );
}

bool ignoreMouseMovement = false;
VOID __stdcall hooked_CreateMove ( int sequence_number, float input_sample_frametime, bool active )
{
    static DWORD realCreateMove = g_pCreateMoveVMT->dwGetMethodAddress( 21 );

    __asm
    {
        PUSH active;
        PUSH input_sample_frametime;
        PUSH sequence_number;
        CALL realCreateMove;
    }

    CUserCmd* cmd = g_pInput->GetUserCmd( sequence_number );

    C_BaseEntity* pLocal = ( C_BaseEntity* )( g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );

    if ( !cmd || !pLocal )
        return;

    if ( FBitSet( cmd->buttons, IN_JUMP ) )
    {
        if ( !FBitSet( g_NetVars.GetBasePlayerFlags( pLocal ), FL_ONGROUND ) )
            cmd->buttons &= ~IN_JUMP;
    }

    Speedhack( );

    if ( g_NetVars.GetBasePlayerLifeState( pLocal ) != LIFE_ALIVE )
        return;

    ignoreMouseMovement = false;

	DWORD dwPunchOffset = g_NetVars.dwGetNetVarOffset( "DT_BasePlayer>localdata>m_Local" );
	
	QAngle qPunch = GetPunchAngle( pLocal );

    float flModifier = VectorNormalizeHLSDK( qPunch.Base() );
    flModifier -= ( flModifier * 0.5f + 10.0f ) * g_pGlobalVars->interval_per_tick;
    if( flModifier < 0.0f )
        flModifier = 0.0f;
    qPunch *= flModifier;
	

    QAngle QSpreadAngles;
    Base::pnospread::ApplySpreadAngles( cmd, pLocal, QSpreadAngles );

    if ( cmd->buttons & IN_ATTACK )
    {
		cmd->viewangles.x = (cmd->viewangles.x) - (qPunch.x * 2) + QSpreadAngles.x;
        cmd->viewangles.y = (cmd->viewangles.y) - (qPunch.y * 2) + QSpreadAngles.y;
    }


	Vector vLocalEye = g_NetVars.GetEyePosition( pLocal );
	g_Aimbot.doAim(pLocal,vLocalEye,cmd,qPunch);

    CInput::CVerifiedUserCmd* ver = *( ( CInput::CVerifiedUserCmd** )( ( DWORD ) g_pInput + 0xB8 ) );
    ver [ sequence_number % MULTIPLAYER_BACKUP ].m_cmd = *cmd;
    ver [ sequence_number % MULTIPLAYER_BACKUP ].m_crc = cmd->GetChecksum( );
    g_pInput->m_pVerifiedCommands = ver;
}

CUserCmd* __stdcall hooked_GetUserCmd( int sequence_number )
{
	DWORD GetUserCmd = *( DWORD* )( ( DWORD ) g_pInput + 0xB4 ) + ( sequence_number % 90 << 6 );
	CUserCmd *pCmd = ( CUserCmd* ) GetUserCmd;

	pCmd->random_seed = 165;
	pCmd->command_number = 2087;

	return pCmd;
}

VOID __stdcall hooked_BeginFrame( VOID )
{
	CViewSetup* pView = NULL;

	__asm
	{
		PUSH EBX;
		LEA EBX, [ESI+0x8];
		MOV pView, EBX;
		POP EBX;
	}

	QAngle viewangles;
	g_pEngine->GetViewAngles( viewangles );
	QAngle* pViewRenderAngles = &pView->angles;

	QAngle qDifference = QAngle(viewangles.x - pViewRenderAngles->x, viewangles.y - pViewRenderAngles->y, viewangles.z - pViewRenderAngles->z);
	if ( (qDifference.x || qDifference.y) /*&& (g_isRageMode || g_isAimKeyPressed)*/ )
	{
		pViewRenderAngles->x += qDifference.x;
		pViewRenderAngles->y += qDifference.y;
	}

	static DWORD realBeginFrame = g_pEngineVMT->dwGetMethodAddress( 54 );
	__asm CALL realBeginFrame;
}
IMaterial* pMat;
float flColor[4],flColor2[4];
void __stdcall hooked_DrawModel(const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld )
{

	g_pDrawModelVMT->UnHook();
	if(pInfo.pModel)
	{
		const char* pszModelName = g_pModelinfo->GetModelName(pInfo.pModel);

		if( (strstr(pszModelName, "models/player") && g_pEngine->IsInGame()))
		{
			g_pModelinfo->GetModelMaterials(pInfo.pModel,1,&pMat);
			C_BaseEntity* pModelEntity = (C_BaseEntity*)g_pEntList->GetClientEntity(pInfo.entity_index);

			if(pModelEntity)
			{
				if(g_NetVars.GetBasePlayerTeam(pModelEntity) == 2)
				{
					flColor[0] = 255;
					flColor[1] = 255;
					flColor[2] = 0;
					flColor[3] = 255;

					flColor2[0] = 255;
					flColor2[1] = 0;
					flColor2[2] = 0;
					flColor2[3] = 255;
				}else if(g_NetVars.GetBasePlayerTeam(pModelEntity) == 3)
				{

					flColor[0] = 0;
					flColor[1] = 255;
					flColor[2] = 0;
					flColor[3] = 255;

					flColor2[0] = 0;
					flColor2[1] = 0;
					flColor2[2] = 255;
					flColor2[3] = 255;
				}
			}
			g_pRenderView->SetColorModulation(flColor2);
			pMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ,true);
			g_pModelRender->ForcedMaterialOverride(pMat);
			g_pModelRender->DrawModelExecute(state,pInfo,pCustomBoneToWorld);
			pMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ,false);
			g_pModelRender->ForcedMaterialOverride(pMat);
			g_pRenderView->SetColorModulation(flColor);
		}
	}
	g_pModelRender->DrawModelExecute(state,pInfo,pCustomBoneToWorld);
	g_pModelRender->ForcedMaterialOverride(NULL);
	g_pDrawModelVMT->ReHook();

}

void CGameEvents::Init( void )
{
	//g_pGameEventManager2->AddListener( this, "round_start", false );
	//g_pGameEventManager2->AddListener( this, "game_newmap", false );
	g_pGameEventManager2->AddListener( this, "player_death", false );
	g_pGameEventManager2->AddListener( this, "player_hurt", false );
}

void CGameEvents::FireGameEvent( IGameEvent *event )
{
	if ( !g_pEngine || !event )
		return;

	const char *pszEventName = event->GetName();
	if( !strcmp( pszEventName, "player_death" ) )
	{
		int iVictim = g_pEngine->GetPlayerForUserID( event->GetInt( "userid" ) );
		int iKiller = g_pEngine->GetPlayerForUserID( event->GetInt( "attacker" ) );

		bool playSound = (iKiller != iVictim) && iKiller == g_pEngine->GetLocalPlayer();

		if ( playSound )
        {
			//MessageBeep( 0xFFFFFFFF );
            PlaySound( "C:\\interwebz\\hitsound.wav", NULL, SND_ASYNC );
        }

	}
    else if( !strcmp( pszEventName, "player_hurt" ) )
	{
		int iVictim = g_pEngine->GetPlayerForUserID( event->GetInt( "userid" ) );
		int iAttacker = g_pEngine->GetPlayerForUserID( event->GetInt( "attacker" ) );

        bool playSound = (iAttacker != iVictim) && iAttacker == g_pEngine->GetLocalPlayer();

        if ( playSound )
        {
			hit_s dummyHit;
			dummyHit.entityId = iVictim;
			dummyHit.timeStart = GetTickCount();
			g_vHits.push_back( dummyHit );
			//dummyHit.dmgDone = iDmgDone;

			//MessageBeep( 0xFFFFFFFF );
			PlaySound( "C:\\interwebz\\hitsound.wav", NULL, SND_ASYNC );
        }

	}
}

VOID HitESP ( VOID )
{
	INT Alpha = 0;

	std::vector < hit_s >::iterator iter = g_vHits.begin();

	for ( int iHit = 0; iHit < (int)g_vHits.size(); iHit++ )
	{
		//CBaseEntity* pBaseEntity = ( CBaseEntity* )( g_pEntList->GetClientEntity( g_vHits[iHit].entityId ) );

		if ( (( g_vHits[iHit].timeStart + 3000 ) < GetTickCount()) /*|| !pBaseEntity*/ )
		{
			g_vHits.erase( iter );
			continue;
		}

		//if ( ( g_vHits[iHit].timeStart + 1000 ) < GetTickCount() )
		//{
		Alpha = 0;

		Alpha += (((int)g_vHits[iHit].timeStart + 3000) - (int)GetTickCount()) / 12;
		//Alpha -= (g_vHits[iHit].timeStart + 3000 - 


		//Alpha = (int)( 255 * ( (int)g_vHits[iHit].timeStart + 2000 - (int)GetTickCount() ) / 2000 );
		//}

		g_Visuals.doXhair( FALSE, CenterX, CenterY, Alpha );
		//Vector screenSpace;
		//if ( WorldToScreen ( pBaseEntity->GetAbsOrigin(), screenSpace ) )
		//{
			//g_Font.print( true, screenSpace.x, (screenSpace.y - 40) + Alpha, 255, 255, 255, "%i", g_vHits[iHit].dmgDone );
			//g_Font.print( true, )
			//  g_Utils.DrawString ( TRUE, X, Y, 255, 255, 255, "x: %i y: %i", ( INT ) flScreenCoords.x, ( INT ) flScreenCoords.y);
		//}
		iter++;
	}
}

#pragma comment( lib, "winmm" )

std::vector<IMaterial*> vWorldMaterials,vSkyMaterials,vHands;

void ClearMapTextures()
{
	vWorldMaterials.clear();
	vSkyMaterials.clear();

}

void setSolid()
{
	for (int ax = 0; ax < vWorldMaterials.size();ax++)
		vWorldMaterials[ax]->AlphaModulate(1.0);
}

void setTrans()
{
	for(int i = 0;i<vWorldMaterials.size();i++)
	{
		vWorldMaterials[i]->AlphaModulate(0.5f);
	}

	for(int i = 0; i < vSkyMaterials.size(); i++ )
	{
		vSkyMaterials[ i ]->ColorModulate( 0, 0, 0 );
	}
}
bool bIngame = false;

DWORD g_dwOrgFindMaterial;
IMaterial* __stdcall hooked_FindMaterial(char const* pMaterialName, const char *pTextureGroupName, bool complain, const char *pComplainPrefix)
{
	g_pIMaterialSystemVMT->UnHook();
	printf("FindMaterial hooked");
	IMaterial* pMatReturn = g_pIMaterialSystem->FindMaterial(pMaterialName,pTextureGroupName,complain,pComplainPrefix);		
	IMaterial* noHands = g_pIMaterialSystem->FindMaterial( "models\\weapons\\v_models\\hands\\v_hands", "Model textures",0,0 );
	IMaterial* noSmoke = g_pIMaterialSystem->FindMaterial("particle\\particle_smokegrenade",0,0,0);
	IMaterial* noSmoke2 = g_pIMaterialSystem->FindMaterial("particle\\particle_smokegrenade1",0,0,0 );
	IMaterial* noSmokeStack = g_pIMaterialSystem->FindMaterial( "SmokeStack", "ClientEffect textures");
	setTrans();

	if(pMaterialName && pTextureGroupName)
	{
		if(g_pEngine->IsInGame())
		{
			bIngame = true; 
		}
		else
		{
			if(strstr(pTextureGroupName,"World textures"))
			{
				if (bIngame)
				{
					ClearMapTextures();
					bIngame = false;
				}
				vWorldMaterials.push_back( pMatReturn );
			}
			if (strstr(pMaterialName,"wood")
				||strstr(pMaterialName,"WOOD")
				||strstr(pMaterialName,"door")
				||strstr(pMaterialName,"DOOR")
				||strstr(pMaterialName,"gate")
				||strstr(pMaterialName,"GATE")
				||strstr(pMaterialName,"box")
				||strstr(pMaterialName,"BOX") )
				vWorldMaterials.push_back( pMatReturn );
			if(strstr(pMaterialName, "skybox"))
				vSkyMaterials.push_back(pMatReturn);
		/*if(noHands)
			noHands->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW,true);
		if(noSmoke)
			noSmoke->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW,true);
		if(noSmoke2)
			noSmoke2->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW,true);
		if(noSmokeStack)
			noSmokeStack->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW,true);
			*/
		}
	}

	g_pIMaterialSystemVMT->ReHook();
	return pMatReturn;
}

VOID CSS_InitDllThread ( void* dummyptr )
{
    while ( FindWindowA ( "Valve001", NULL ) == NULL )
		Sleep ( 100 );

	while ( GetModuleHandleA ( "engine.dll" ) == NULL || GetModuleHandleA ( "client.dll" ) == NULL )
		Sleep ( 100 );

    Sleep( 2000 );

	//DWORD dwEngineBase = CodeSecBegin( "client" );
	// 0x%08X
	char szDebugString[1024];
	//sprintf( szDebugString, "dwEngineBase: 0x%08X ", dwEngineBase);
	//LOG( szDebugString, dwEngineBase != 0x0 );
	//DWORD dwEngineSize = CodeSecSize( "client" );
	//sprintf( szDebugString, "dwEngineSize: 0x%08X ", dwEngineSize);
	//LOG( szDebugString, dwEngineSize != 0x0 );

	//DWORD dwVClientStrPtr = dwFindPatternByHash ( dwEngineBase, dwEngineSize, 0x83AB6D25, 0xFFFF ); // VClient
	//sprintf( szDebugString, "dwVClientStrPtr: 0x%08X ", dwVClientStrPtr);
	//LOG( szDebugString, dwVClientStrPtr != 0x0 );

	g_pEngine = EasyInterface_t.QuickGetPointer< IVEngineClient >( "VEngineClient" ); // Get interface with nothing but the identifier
	sprintf( szDebugString, "g_pEngine: " );
	strcat( szDebugString, g_lastVersionString.c_str() );

	LOG( szDebugString, (DWORD)g_pEngine != 0x0 );

	g_pClient = EasyInterface_t.QuickGetPointer< IBaseClientDLL >( "VClient", "client" ); // Get interface using a specified module
	sprintf( szDebugString, "g_pClient: " );
	strcat( szDebugString, g_lastVersionString.c_str() );
	LOG( szDebugString, (DWORD)g_pClient != 0x0 );

	PDWORD* pdwClientVMT = ( PDWORD* )( g_pClient );
	DWORD* pdwClientVMT2 =  ( DWORD* )*( DWORD* ) g_pClient;

	g_pInput = ( CInput* )*( DWORD* )*( DWORD* )( pdwClientVMT2[21] + 0x28 );
	sprintf( szDebugString, "g_pInput: 0x%08X ", (DWORD)g_pInput );
	//strcat( szDebugString, g_lastVersionString.c_str() );
	LOG( szDebugString, (DWORD)g_pInput != 0x0 );

	g_pModelinfo = EasyInterface_t.QuickGetPointer< IVModelInfoClient >( "VModelInfoClient" );
	sprintf( szDebugString, "g_pModelinfo: ", (DWORD)g_pModelinfo );
	strcat( szDebugString, g_lastVersionString.c_str() );
	LOG( szDebugString, (DWORD)g_pModelinfo != 0x0 );

    g_pEnginetrace = EasyInterface_t.QuickGetPointer< IEngineTrace >( "EngineTraceClient" );
	sprintf( szDebugString, "g_pEnginetrace: ", (DWORD)g_pEnginetrace );
	strcat( szDebugString, g_lastVersionString.c_str() );
	LOG( szDebugString, (DWORD)g_pEnginetrace != 0x0 );

    g_pSurface = EasyInterface_t.QuickGetPointer< vgui::ISurface >( "VGUI_Surface" );
	sprintf( szDebugString, "g_pSurface: ", (DWORD)g_pSurface );
	strcat( szDebugString, g_lastVersionString.c_str() );
	LOG( szDebugString, (DWORD)g_pSurface != 0x0 );

	//g_pMatSystemSurface = EasyInterface_t.QuickGetPointer< IMatSystemSurface >( "VMatSystemSurface" );
	//sprintf( szDebugString, "g_pMatSystemSurface: 0x%08X ", (DWORD)g_pMatSystemSurface );
	//LOG( szDebugString, (DWORD)g_pMatSystemSurface != 0x0 );

	g_pCvar = EasyInterface_t.QuickGetPointer< ICvar >( "VEngineCvar" );
	sprintf( szDebugString, "g_pCvar: ", (DWORD)g_pCvar );
	strcat( szDebugString, g_lastVersionString.c_str() );
	LOG( szDebugString, (DWORD)g_pCvar != 0x0 );

	g_pEntList = EasyInterface_t.QuickGetPointer< IClientEntityList >( "VClientEntityList" );
	sprintf( szDebugString, "g_pEntList: ", (DWORD)g_pEntList );
	strcat( szDebugString, g_lastVersionString.c_str() );
	LOG( szDebugString, (DWORD)g_pEntList != 0x0 );

	g_pPanel = EasyInterface_t.QuickGetPointer< vgui::IPanel >( "VGUI_Panel" );
	sprintf( szDebugString, "g_pPanel: ", (DWORD)g_pPanel );
	strcat( szDebugString, g_lastVersionString.c_str() );
	LOG( szDebugString, (DWORD)g_pPanel != 0x0 );

	g_pModelRender = EasyInterface_t.QuickGetPointer< IVModelRender>( "VEngineModel");
	if(g_pModelRender == 0)
	{
		printf("ModelRender error");
	}

	g_pRenderView = EasyInterface_t.QuickGetPointer< IVRenderView> ( "VEngineRenderView");
	if(g_pRenderView == 0)
	{
		printf("Renderview error");
	}

	g_pIMaterialSystem = EasyInterface_t.QuickGetPointer< IMaterialSystem > ("VMaterialSystem");
	sprintf( szDebugString, "g_pIMaterialSystem: ", (DWORD)g_pIMaterialSystem );
	strcat( szDebugString, g_lastVersionString.c_str() );

	LOG( szDebugString, (DWORD)g_pIMaterialSystem != 0x0 );
    //g_pGameEventManager2 = (IGameEventManager2*)appSystemFactory( "GAMEEVENTSMANAGER002", NULL );
    g_pDebugOverlay = EasyInterface_t.QuickGetPointer< IVDebugOverlay >( "VDebugOverlay" );
	sprintf( szDebugString, "g_pDebugOverlay: ", (DWORD)g_pDebugOverlay );
	strcat( szDebugString, g_lastVersionString.c_str() );
	LOG( szDebugString, (DWORD)g_pDebugOverlay != 0x0 );
    //g_pPhysics = (IPhysicsSurfaceProps*)appSystemFactory(VPHYSICS_SURFACEPROPS_INTERFACE_VERSION,NULL);
    //g_pGameMovement = ( IGameMovement* )clientFactory( INTERFACENAME_GAMEMOVEMENT, NULL );

	g_pGameEventManager2 = EasyInterface_t.QuickGetPointer< IGameEventManager2 >( "GAMEEVENTSMANAGER" );
	sprintf( szDebugString, "g_pGameEventManager2: ", (DWORD)g_pGameEventManager2 );
	strcat( szDebugString, g_lastVersionString.c_str() );
	LOG( szDebugString, (DWORD)g_pGameEventManager2 != 0x0 );
	//if ( !g_pGameEventManager2 )
	//	printf( "NULLLLLLLLLL");

    PDWORD* pdwPanelVMT = ( PDWORD* )g_pPanel;
	PDWORD* pdwModelRenderVMT = (PDWORD*)g_pModelRender;
	PDWORD* pdwMaterialSystemVMT = (PDWORD*)g_pIMaterialSystem;

    //g_pVGuiLocalize = (vgui::ILocalize*)appSystemFactory( VGUI_LOCALIZE_INTERFACE_VERSION, NULL );
    //IVModelRender* g_pModelRender = ( IVModelRender* ) appSystemFactory( "VEngineModel012", NULL );

    g_pPaintTraverseVMT = new Base::Utils::CVMTHookManager ( pdwPanelVMT );
    g_pPaintTraverseVMT->dwHookMethod ( ( DWORD ) hooked_PaintTraverse, 40 );
    g_pCreateMoveVMT = new Base::Utils::CVMTHookManager( pdwClientVMT );
    g_pCreateMoveVMT->dwHookMethod( (DWORD)hooked_CreateMove, 21 );
    //g_pCreateMoveVMT->dwHookMethod( (DWORD)hooked_HudUpdate, 11 );
    PDWORD* pdwEngineVMT = ( PDWORD* )g_pEngine;
    g_pEngineVMT = new Base::Utils::CVMTHookManager( pdwEngineVMT );

	g_pDrawModelVMT = new Base::Utils::CVMTHookManager(pdwModelRenderVMT);
	g_pDrawModelVMT->dwHookMethod( (DWORD)hooked_DrawModel, 19);

	//g_pIMaterialSystemVMT = new Base::Utils::CVMTHookManager(pdwMaterialSystemVMT);
	//g_pIMaterialSystemVMT->dwHookMethod( (DWORD)hooked_FindMaterial,70);
	

    //g_pEngineVMT->dwHookMethod( ( DWORD )hookedSetViewAngles, 20 );
    g_pEngineVMT->dwHookMethod((DWORD)hooked_BeginFrame,54);
    //g_pEngineVMT->dwHookMethod((DWORD)hooked_EndFrame,55);

    DWORD** ppdwInputVmt = (DWORD**)( g_pInput );
    g_pInputVmt = new Base::Utils::CVMTHookManager( ppdwInputVmt );
    //Base::LogToFile( Base::LOC_DESKTOP, Base::LOG_TRUNCATE, "GetUserCmd: 0x%08X", g_pInputVmt->dwGetMethodAddress( 8 ) );
    g_pInputVmt->dwHookMethod( (DWORD)hooked_GetUserCmd, 8 );
    //g_pInputVmt->dwHookMethod( (DWORD)hooked_KeyEvent, 11 );
    //g_pInputVmt->dwHookMethod( (DWORD)hooked_AccumulateMouse, 17 );
    //DWORD dwGlobalsOffset = Base::Utils::PatternSearch( "engine.dll",
   //         (PBYTE) "\x68\x00\x00\x00\x00\x50\x50\x8B\x02\xFF\xD0\x85\xC0",
   //                 "x????xxxxxxxx" );
   // dwGlobalsOffset += 0x1;
   // g_pGlobalVars = *( CGlobalVarsBase** )dwGlobalsOffset;

	PVOID pEngineClient = ( PVOID )( EasyInterface_t.QuickGetPointer< IVEngineClient >( "EngineClientReplay" ) );
	typedef CGlobalVars* ( __thiscall* OriginalFn )( PVOID );
	g_pGlobalVars = getvfunc< OriginalFn >( pEngineClient, 13 )( pEngineClient );

	sprintf( szDebugString, "g_pGlobalVars: 0x%08X ", (DWORD)g_pGlobalVars );
	LOG( szDebugString, (DWORD)g_pGlobalVars != 0x0 );
	

	Base::Utils::SetPos( 0, 25 );
	Base::Utils::SetColor( BLACK );
}

BOOL WINAPI DllMain ( HINSTANCE hinstDLL,  // handle to DLL module
					  DWORD fdwReason,     // reason for calling function
					  LPVOID lpReserved )  // reserved
{
	// Perform actions based on the reason for calling.
	switch ( fdwReason ) 
	{ 
	case DLL_PROCESS_ATTACH:
		// Initialize once for each new process.
		// Return FALSE to fail DLL load.

		if ( !Base::Utils::AttachDebugConsole() )
			MessageBox( 0, "Error while attaching debug console!", "Info", MB_ICONINFORMATION );
		else
			initLOG();
		LOG( "CSS_InitDllThread()... ", _beginthread( CSS_InitDllThread, 0, NULL ) != 1L );

		break;

	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;

	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;

	case DLL_PROCESS_DETACH:
		// Perform any necessary cleanup.
		break;
	}
	return TRUE; // Successful DLL_PROCESS_ATTACH.
}