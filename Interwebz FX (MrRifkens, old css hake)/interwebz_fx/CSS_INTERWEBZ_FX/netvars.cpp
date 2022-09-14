#include "Includes.h"
DWORD cNetVars::dwGetNetVarOffset( const char *pszPath )
{
	/*typedef ClientClass *( __thiscall *GetAllClasses_t )( void *_this );
	static GetAllClasses_t pGetAllClasses = NULL;
	if( pGetAllClasses == NULL )
		pGetAllClasses = (GetAllClasses_t)( ( *(DWORD**)g_pBaseClientDLL )[ 8 ] );*/

	std::string sPath( pszPath );
	std::vector< std::string > vItems;
	std::stringstream stringStream( sPath );
	std::string sItem;
	while( std::getline( stringStream, sItem, '>' ) ) {
		vItems.push_back( sItem );
	}

	//ClientClass *pClass = pGetAllClasses( g_pBaseClientDLL );
	ClientClass *pClass = g_pClient->GetAllClasses();

	while( pClass ) {
		if( !strcmp( pClass->m_pRecvTable->GetName(), vItems[ 0 ].c_str() ) ) {
			break;
		}
		pClass = pClass->m_pNext;
	}

	if( pClass == NULL )
		return NULL;

	RecvTable *pTable = pClass->m_pRecvTable;
	for( size_t i = 1; i < vItems.size(); i++ ) {
		for( int j = 0; j < pTable->m_nProps; j++ ) {
			RecvProp *pProp = pTable->GetProp( j );
			if( !strcmp( pProp->GetName(), vItems[ i ].c_str() ) ) {
				if( pProp->GetType() == 6 && i < ( vItems.size() - 1 ) ) { // DPT_DataTable
					pTable = pProp->GetDataTable();
					break;
				}
				return (DWORD)pProp->GetOffset();
			}
		}
	}

	return NULL;
}

int cNetVars::GetBasePlayerFlags( C_BaseEntity* pEntity )
{
	static DWORD dwOffs = NULL;
	if ( dwOffs == NULL )
	{
		dwOffs = dwGetNetVarOffset( "DT_BasePlayer>m_fFlags" );
	}

	return *(int*)( (DWORD)pEntity + dwOffs );
}

char cNetVars::GetBasePlayerLifeState( C_BaseEntity* pEntity )
{
	static DWORD dwOffs = NULL;
	if ( dwOffs == NULL )
	{
		dwOffs = dwGetNetVarOffset( "DT_BasePlayer>m_lifeState" );
	}

	return *(char*)( (DWORD)pEntity + dwOffs );
}

int cNetVars::GetBasePlayerTeam(C_BaseEntity* pEntity)
{
	static DWORD dwOffs = NULL;
	if ( dwOffs == NULL )
	{
		dwOffs = dwGetNetVarOffset( "DT_BaseEntity>m_iTeamNum" );
	}

	return *(int*)( (DWORD)pEntity + dwOffs );
}

int cNetVars::GetBasePlayerHealth(C_BaseEntity* pEntity)
{
	static DWORD dwOffs = NULL;
	if ( dwOffs == NULL )
	{
		dwOffs = dwGetNetVarOffset( "DT_BaseEntity>m_iHealth" );
	}

	return *(int*)( (DWORD)pEntity + dwOffs );
}

Vector cNetVars::GetEyePosition(C_BaseEntity* pEntity)
{
	static DWORD dwOffs = NULL;
	if ( dwOffs == NULL )
	{
		dwOffs = dwGetNetVarOffset( "DT_BasePlayer>localdata>m_vecViewOffset[0]" );
	}
	return *(Vector*)( (DWORD)pEntity + dwOffs );
}