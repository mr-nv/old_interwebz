#ifndef _NETVARS_H
#define _NETVARS_H

class C_BaseEntity;

class cNetVars
{
public:
	DWORD dwGetNetVarOffset( const char *pszPath );
	int GetBasePlayerFlags( C_BaseEntity* pEntity );
	char GetBasePlayerLifeState( C_BaseEntity* pEntity );
	int GetBasePlayerTeam(C_BaseEntity* pEntity);
	int GetBasePlayerHealth(C_BaseEntity* pEntity);
	Vector GetEyePosition(C_BaseEntity* pEntity);
	
private:
};

#endif