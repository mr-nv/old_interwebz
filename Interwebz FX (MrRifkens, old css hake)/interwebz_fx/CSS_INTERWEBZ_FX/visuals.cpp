#include "Includes.h"

bool cVisuals::WorldToScreen( const Vector &vOrigin, Vector &vScreen )
{
		return ( g_pDebugOverlay->ScreenPosition( vOrigin, vScreen ) != 1 );
}

void cVisuals::FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a )
{
	g_pSurface->DrawSetColor( r, g, b, a );
	g_pSurface->DrawFilledRect( x, y, x + w, y + h );
}

void cVisuals::DrawBox(int x, int y, int w, int h, int lw, Color c)
{
	FillRGBA( x, y, w, lw, c.r(), c.g(), c.b(), c.a() ); // top
	FillRGBA( x, y + lw, lw, h - lw, c.r(), c.g(), c.b(), c.a() ); // left
	FillRGBA( x + w - lw, y + lw, lw, h - lw, c.r(), c.g(), c.b(), c.a() ); // right
	FillRGBA( x + lw, y + h - lw, w - lw * 2, lw, c.r(), c.g(), c.b(), c.a() ); // bottom
}

void DrawOutlinedRect(int x0,int y0,int x1,int y1, int R, int G, int B, int Health)
{
	int BoxWidth = x1 - x0;
	int BoxHeight = y1 - y0;

	if ( BoxWidth < 10 ) BoxWidth = 10;
	if ( BoxHeight < 15 ) BoxHeight = 15;


	g_pSurface->DrawFilledRect( x0, y0, x0 + (BoxWidth/5), y0+1 ); //left top
	g_pSurface->DrawFilledRect( x0, y0, x0+1, y0 + (BoxHeight/6) ); //left top

	g_pSurface->DrawFilledRect( x1 - (BoxWidth/5) + 1, y0, x1, y0+1 ); //right top
	g_pSurface->DrawFilledRect( x1, y0, x1+1, y0 + (BoxHeight/6) ); //right top

	g_pSurface->DrawFilledRect( x0, y1, x0 + (BoxWidth/5), y1+1 ); //left bottom
	g_pSurface->DrawFilledRect( x0, y1 - (BoxHeight/6)+1, x0+1, y1+1 ); //left bottom

	g_pSurface->DrawFilledRect( x1-(BoxWidth/5)+1, y1, x1, y1+1 ); //right bottom
	g_pSurface->DrawFilledRect( x1, y1 - (BoxHeight/6)+1, x1+1, y1+1 ); //right bottom


	int HealthWidth = ( ( x1 - x0 ) - 2 );
	int HealthHeight = ( y1 - y0 ) - 2;
	//DrawGUIBoxHorizontal( x0, y0 - (HealthHeight + 5), HealthWidth, HealthHeight, R, G, B, Health );
}

Color cVisuals::GetTeamColor ( size_t TeamNumber, bool Visible )
{
	Color ReturnColor ( 255, 255, 255, 255 );

	switch ( TeamNumber )
	{
	case 3:
		{
			ReturnColor = Color( 66, 115, 247, 255 );
			break;
		}

	case 2:
		{
			ReturnColor = Color( 247, 66, 115, 255 );
			break;
		}
	}

	CBaseEntity* pLocal = ( CBaseEntity* )( g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer( ) ) );

	if ( !pLocal )
		return Color ( 0, 0, 0, 255 );

	if ( Visible && pLocal && TeamNumber != g_NetVars.GetBasePlayerTeam( ( pLocal  ) ) )
		ReturnColor = Color( 66, 247, 115, 255 );

	return ReturnColor;
}

DWORD FindStringAddress ( DWORD dwStart, DWORD dwSize, CHAR* szString )
{
	for ( DWORD dw = dwStart; dw <= ( dwStart + dwSize ); dw++ )
	{
		CHAR* szCurString = *( CHAR** )( dw );

		if ( !IsBadStringPtr( szCurString, strlen ( szString ) ) )
			if ( !strcmp( szCurString, szString ) )
				return dw - 0x1;
	}

	return 0;
}

DWORD CodeSecBegin ( const char* szModuleName )
{
	HMODULE hModule = GetModuleHandle( szModuleName );

	if ( !hModule )
		return 0x0;

	PIMAGE_DOS_HEADER pDosHeader = PIMAGE_DOS_HEADER( hModule );
	if ( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE )
		return 0x0;

	PIMAGE_NT_HEADERS peHeader = PIMAGE_NT_HEADERS( ( ULONG )( hModule ) + pDosHeader->e_lfanew );
	if ( peHeader->Signature != IMAGE_NT_SIGNATURE )
		return 0x0;

	PIMAGE_OPTIONAL_HEADER pOptionalHeader = &peHeader->OptionalHeader;

	DWORD dwEngineBase = ( ULONG )( hModule ) + pOptionalHeader->BaseOfCode;
	DWORD dwEngineSize = pOptionalHeader->SizeOfCode;

	return dwEngineBase;
}

DWORD CodeSecSize ( const char* szModuleName )
{
	HMODULE hModule = GetModuleHandle( szModuleName );

	if ( !hModule )
		return 0x0;

	PIMAGE_DOS_HEADER pDosHeader = PIMAGE_DOS_HEADER( hModule );
	if ( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE )
		return 0x0;

	PIMAGE_NT_HEADERS peHeader = PIMAGE_NT_HEADERS( ( ULONG )( hModule ) + pDosHeader->e_lfanew );
	if ( peHeader->Signature != IMAGE_NT_SIGNATURE )
		return 0x0;

	PIMAGE_OPTIONAL_HEADER pOptionalHeader = &peHeader->OptionalHeader;

	DWORD dwEngineBase = ( ULONG )( hModule ) + pOptionalHeader->BaseOfCode;
	DWORD dwEngineSize = pOptionalHeader->SizeOfCode;

	return dwEngineSize;
}

MapPlayer_t* GetRadarInfoByIndex ( size_t iIndex )
{
	/*
	$ ==>    > . 68 E0D8E160    PUSH client.60E1D8E0                     ; /Arg1 = 60E1D8E0 ASCII "CCSMapOverview"
	$+5      > . B9 D045FC60    MOV ECX,client.60FC45D0                  ; |
	$+A      > . E8 007EF3FF    CALL client.60983A00                     ; \client.60983A00
	$+F      > . 8B0D 6071FE60  MOV ECX,DWORD PTR DS:[60FE7160]
*/
    /*
    $ ==>    >  68 DC139262     PUSH client.629213DC                     ; ASCII "CCSMapOverview"
$+5      >  B9 10CCAB62     MOV ECX,client.62ABCC10
$+A      >  E8 C09CF3FF     CALL client.624B2DF0
$+F      >  8B0D 10ECAD62   MOV ECX,DWORD PTR DS:[62ADEC10]

*/
    //return MakePtr( MapPlayer_t*, dwOffset, ( index + (index * 4) ) << 6 );

    //dwOffset = ( DWORD )( GetModuleHandle( "client" ) ) + 0x1e9130; // Address 62579130 Base 62390000
    //dwOffset += 0x2;
    //dwOffset = *( DWORD* )( dwOffset );

	//MessageBox( 0, "1", "info", 0 );
    static DWORD dwOffset = 0x0;
	if ( !dwOffset )
	{
		//dwOffset = FindStringAddress( CodeSecBegin( "client.dll"), CodeSecSize( "client.dll" ), "CCSMapOverview" );
		//dwOffset += 0xF;
		//dwOffset += 0x2;
		//dwOffset = *( DWORD* )( dwOffset );

		//dwOffset = GetPattern_PTR( )
		dwOffset = Base::Utils::PatternSearch( "client", (PBYTE)"\xCC\x8B\x0D\x00\x00\x00\x00\x85\xC9\x74\x08\x8B\x01\x8B\x10\x6A\x03\xFF\xD2\xC3", "xxx????xxxxxxxxxxxxx", 0x0, 0x0 );
		dwOffset += 0x3;
		dwOffset = *( DWORD* )( dwOffset );
	}
	//MessageBox( 0, "2", "info", 0 );
	__asm
	{
		MOV ECX, dwOffset;
		MOV ECX, DWORD PTR DS: [ ECX ];
		MOV EAX, iIndex;
		SUB EAX, 1;
		LEA EAX, DWORD PTR DS: [ EAX + EAX * 4 ];
		SHL EAX, 6;
		LEA EAX, DWORD PTR DS: [ EAX + ECX + 0x28 ];
	}

	//MessageBox( 0, "3", "info", 0 );

}

void cVisuals::doBoundingBox(C_BaseEntity* pBaseEntity,int r,int g,int b)
{
	Vector vPlayer, vScreen;

            //Color DrawColor = Color( r, g, b, 255 );
            vPlayer = pBaseEntity->GetAbsOrigin( );


            vPlayer.z += 32;

            Vector vAimOrg;
            Vector vAimOrgScreen;

            bool bDucking = g_NetVars.GetBasePlayerFlags( pBaseEntity ) & FL_DUCKING;

            Vector vPlayerFoot = pBaseEntity->GetAbsOrigin();
            vPlayerFoot.z -= 10;

            Vector vPlayerHead = vPlayerFoot + Vector( 0, 0, bDucking ? 66 : 80 );

            Vector vPlayerFootScreen, vPlayerHeadScreen;

            if ( WorldToScreen( vPlayer, vScreen ) &&
                WorldToScreen( vPlayerFoot, vPlayerFootScreen ) &&
                 WorldToScreen( vPlayerHead, vPlayerHeadScreen ) )
            {
				float width2 = ( vPlayerFootScreen.y - vPlayerHeadScreen.y ) / 4;

				//vPlayerFootScreen.y += 8.0f; // test

				g_pSurface->DrawSetColor( GetTeamColor( g_NetVars.GetBasePlayerTeam( pBaseEntity ), false ) );

				DrawOutlinedRect( vPlayerHeadScreen.x - width2, vPlayerHeadScreen.y, vPlayerFootScreen.x + width2, vPlayerFootScreen.y,
                     255, 255, 255, 100 );
			}
}

void cVisuals::doESP(CBaseEntity* pBaseEntity)
{
	Vector vPlayer,vscreen,vPlayerFootScreen;
	int r,g,b;
	player_info_t pi;
	
	BOOL bIsValidEntity = pBaseEntity && g_NetVars.GetBasePlayerLifeState ( pBaseEntity ) == LIFE_ALIVE &&
		/*GetBasePlayerHealth ( pBaseEntity ) > 0 &&*/ !pBaseEntity->IsDormant( ) &&
		g_pEngine->GetPlayerInfo ( pBaseEntity->index, &pi );



	if(bIsValidEntity)
	{
		vPlayer = pBaseEntity->GetAbsOrigin();
		vPlayer.z += 32;

		Vector vPlayerFoot = pBaseEntity->GetAbsOrigin();
		vPlayerFoot.z -= 10;

		int teamNum = g_NetVars.GetBasePlayerTeam(pBaseEntity);

		if(teamNum == 2) { r = 247, g = 66, b = 115; }
		if(teamNum == 3) { r = 66, g = 115, b = 247; }

		if( WorldToScreen(vPlayer,vscreen) && WorldToScreen(vPlayerFoot,vPlayerFootScreen))
		{


			g_Draw.drawString(true,vPlayerFootScreen.x,vPlayerFootScreen.y,r,g,b,pi.name);
		}

	}



}

void cVisuals::doFarESP(CBaseEntity* pBaseEntity,int index)
{
		MapPlayer_t* pRadarPlayer = GetRadarInfoByIndex ( index );
		
		player_info_t pi;
		BOOL bIsValidEntity = pBaseEntity && g_NetVars.GetBasePlayerLifeState ( pBaseEntity ) == LIFE_ALIVE &&
			/*GetBasePlayerHealth ( pBaseEntity ) > 0 &&*/ !pBaseEntity->IsDormant( ) &&
			g_pEngine->GetPlayerInfo ( pBaseEntity->index, &pi );

		BOOL bIsValidRadarPlayer = pRadarPlayer != NULL &&
												  pRadarPlayer->team != 0 &&
			strlen( pRadarPlayer->name ) != 0 &&
			pRadarPlayer->health > 0 &&
			pRadarPlayer->userid &&
			!pRadarPlayer->position.IsZero();
		
	if ( bIsValidEntity )
	{
		doBoundingBox(pBaseEntity,0, 0,0  );
	}
	else if ( bIsValidRadarPlayer )
	{
		int teamNum = pRadarPlayer->team;


		Vector vPlayer, vScreen;

		//Color DrawColor = Color( r, g, b, 255 );
		vPlayer = pRadarPlayer->position;
		
		vPlayer.z += 32;

		Vector vAimOrg;
		Vector vAimOrgScreen;

		//bool bDucking = g_NetVars.GetBasePlayerFlags( pBaseEntity ) & FL_DUCKING;

		Vector vPlayerFoot = pRadarPlayer->position;
		vPlayerFoot.z -= 10;

		Vector vPlayerHead = vPlayerFoot + Vector( 0, 0, 80 );

		Vector vPlayerFootScreen, vPlayerHeadScreen;

		if ( WorldToScreen( vPlayer, vScreen ) &&
			WorldToScreen( vPlayerFoot, vPlayerFootScreen ) &&
			WorldToScreen( vPlayerHead, vPlayerHeadScreen ) )
		{
			float width2 = ( vPlayerFootScreen.y - vPlayerHeadScreen.y ) / 4;

			//vPlayerFootScreen.y += 8.0f; // test

			g_pSurface->DrawSetColor( GetTeamColor( pRadarPlayer->team, false ) );

			DrawOutlinedRect( vPlayerHeadScreen.x - width2, vPlayerHeadScreen.y, vPlayerFootScreen.x + width2, vPlayerFootScreen.y,
				255, 255, 255, 100 );
		}
	}
}

void cVisuals::drawESP()
{
	Color drawCLR;
	for ( unsigned int index = g_pGlobalVars->maxClients; index > 0; --index )
	{
		if ( index == g_pEngine->GetLocalPlayer( ) )
			continue;

		CBaseEntity* pBaseEntity = ( CBaseEntity* )( g_pEntList->GetClientEntity( index ) );
		//if(farEsp)
			doFarESP(pBaseEntity,index);
		//else 
			doESP(pBaseEntity);
	}
}
void cVisuals::doXhair( BOOL Dynamic, INT x, INT y, INT Alpha )
{
	INT g_iCrosshairColor [ 3 ] = { 121, 255, 255 };

		INT iAbsPitchPunch = 1;
		INT iLineWidth = 2;


		if ( Dynamic /*&& (!g_isRageMode && !g_isAimKeyPressed )*/ )
		{
			//CBaseEntity* pLocal = ( CBaseEntity* )( g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer( ) ) );
			//	if ( !pLocal )
			//	return;
			//QAngle punchAngle = GetPunchAngle( pLocal );
			//iAbsPitchPunch += ( INT ) abs ( punchAngle.Length() ) * 4;
			//iAbsPitchPunch *= 4;
		}


		// MITTE DOT
		FillRGBA(x, y, 1, 1, g_iCrosshairColor[0], g_iCrosshairColor[1], g_iCrosshairColor[2], Alpha);

		// OBEN
		FillRGBA(x, y-8-iAbsPitchPunch, 1, 3, g_iCrosshairColor[0], g_iCrosshairColor[1], g_iCrosshairColor[2], Alpha);
		FillRGBA(x-3, y-5-iAbsPitchPunch, 7, 1, g_iCrosshairColor[0], g_iCrosshairColor[1], g_iCrosshairColor[2], Alpha);

		// RECHTS
		FillRGBA(x+6+iAbsPitchPunch, y, 3, 1, g_iCrosshairColor[0], g_iCrosshairColor[1], g_iCrosshairColor[2], Alpha);
		FillRGBA(x+5+iAbsPitchPunch, y-3, 1, 7, g_iCrosshairColor[0], g_iCrosshairColor[1], g_iCrosshairColor[2], Alpha);

		// UNTEN
		FillRGBA(x, y+6+iAbsPitchPunch, 1, 3, g_iCrosshairColor[0], g_iCrosshairColor[1], g_iCrosshairColor[2], Alpha);
		FillRGBA(x-3, y+5+iAbsPitchPunch, 7, 1, g_iCrosshairColor[0], g_iCrosshairColor[1], g_iCrosshairColor[2], Alpha);

		// LINKS
		FillRGBA(x-8-iAbsPitchPunch, y, 3, 1, g_iCrosshairColor[0], g_iCrosshairColor[1], g_iCrosshairColor[2], Alpha);
		FillRGBA(x-5-iAbsPitchPunch, y-3, 1, 7, g_iCrosshairColor[0], g_iCrosshairColor[1], g_iCrosshairColor[2], Alpha);
}

