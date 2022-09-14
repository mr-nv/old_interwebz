#ifndef _AIMBOT_H
#define _AIMBOT_H

#include "Includes.h"

class cAimbot
{
public:
	bool getHitbox(Vector &vec, int hitbox, int index);
	void getBestTargetIndex(C_BaseEntity* pLocal,Vector& vLocalEye);
	void doAim(C_BaseEntity* pLocal,Vector& vLocalEye,CUserCmd* cmd,QAngle qPunch);
	bool isValidPlayer(C_BaseEntity* pBaseEntity);
	bool isVisible(C_BaseEntity* pBaseEntity);
	
	int bestTraget;
private:
};

#endif