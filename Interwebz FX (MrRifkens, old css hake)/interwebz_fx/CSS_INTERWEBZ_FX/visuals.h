#ifndef _VISUALS_H
#define _VISUALS_H
class CBaseEntity;

class cVisuals
{
	
public:
	bool WorldToScreen ( const Vector &vOrigin, Vector &vScreen );
	void FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a );
	void DrawBox(int x, int y, int w, int h, int lw, Color c);
	void doBoundingBox(CBaseEntity* pBaseEntity,int r,int g,int b);
	void doESP(CBaseEntity* pBaseEntity);
	void doFarESP(CBaseEntity* pEntity,int index);
	Color GetTeamColor(size_t TeamNumber, bool Visible);
	void drawESP();
	void doXhair( BOOL Dynamic, INT x, INT y, INT Alpha );
private:

};

#endif