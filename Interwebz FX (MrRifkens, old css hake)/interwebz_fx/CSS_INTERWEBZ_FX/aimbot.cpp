#include "Includes.h"

float _sqrtf(float _X)
{
	Assert( s_bMathlibInitialized );
	return sqrtf(_X); 
}


float (*pfSqrt)(float x)  = _sqrtf;

void VectorTransform (const float *in1, const matrix3x4_t& in2, float *out)
{
	Assert( s_bMathlibInitialized );
	Assert( in1 != out );
	out[0] = DotProduct(in1, in2[0]) + in2[0][3];
	out[1] = DotProduct(in1, in2[1]) + in2[1][3];
	out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}


void AngleVectors (const QAngle &angles, Vector *forward)
{
	Assert( s_bMathlibInitialized );
	Assert( forward );

	float	sp, sy, cp, cy;

	SinCos( DEG2RAD( angles[YAW] ), &sy, &cy );
	SinCos( DEG2RAD( angles[PITCH] ), &sp, &cp );

	forward->x = cp*cy;
	forward->y = cp*sy;
	forward->z = -sp;
}

void VectorAngles( const Vector& forward, QAngle &angles )
{
	Assert( s_bMathlibInitialized );
	float	tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = FastSqrt (forward[0]*forward[0] + forward[1]*forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}


bool cAimbot::isValidPlayer(C_BaseEntity* pBaseEntity)
{
	if(pBaseEntity && pBaseEntity->IsPlayer() && !pBaseEntity->IsDormant()
		&& g_NetVars.GetBasePlayerLifeState(pBaseEntity) == LIFE_ALIVE
		&& g_NetVars.GetBasePlayerHealth(pBaseEntity) > 0 )
	{
		return true;
	}
	return false;
}

bool cAimbot::isVisible(C_BaseEntity* pBaseEntity)
{
	Ray_t ray;
	trace_t tr;

	CBaseEntity* pLocal = g_pEntList->GetClientEntity ( g_pEngine->GetLocalPlayer( ) )->GetBaseEntity( );

	if ( !pLocal )
		return FALSE;
	
	Vector vEyePos = g_NetVars.GetEyePosition ( pLocal );

	Vector vRemoteEyePos = g_NetVars.GetEyePosition ( pBaseEntity );
	
	ray.Init ( vEyePos, vRemoteEyePos );

	g_pEnginetrace->TraceRay ( ray, MASK_SHOT, NULL, &tr );

	return ( tr.m_pEnt == pBaseEntity || tr.fraction == 1.0f );
}

bool cAimbot::getHitbox(Vector &vec, int hitbox, int index)
{
	matrix3x4_t matrix [MAXSTUDIOBONES];
	Vector min, max;
	IClientEntity* pClientEntity;
	pClientEntity = g_pEntList->GetClientEntity(index);

	if(!pClientEntity->SetupBones(matrix,MAXSTUDIOBONES,BONE_USED_BY_HITBOX,0.0f))
		return false;
	
	studiohdr_t* pStudioHdr = g_pModelinfo->GetStudiomodel(pClientEntity->GetModel());

	if ( !pStudioHdr )
		return FALSE;

	mstudiobbox_t *box = pStudioHdr->pHitbox(hitbox,0);
	VectorTransform(box->bbmin,matrix[box->bone], min);
	VectorTransform(box->bbmax,matrix[box->bone], max);
	vec = (min + max) / 2;

}

void cAimbot::getBestTargetIndex(C_BaseEntity* pLocal,Vector& vLocalEye)
{
	Vector vPlayer;
	float flTargetDst, flDst = INT_MAX;
	for(int ax = g_pGlobalVars->maxClients; --ax >=1; )
	{
		C_BaseEntity* pBaseEntity = (C_BaseEntity*) g_pEntList->GetClientEntity(ax);

		if(isValidPlayer(pBaseEntity))
		{
			Vector vTarget;
			getHitbox(vTarget,12,ax);

			if(vTarget.IsZero())
				continue;

			if(g_NetVars.GetBasePlayerTeam(pBaseEntity) != g_NetVars.GetBasePlayerTeam(pLocal) && isVisible(pBaseEntity) )
			{
				flTargetDst = ( vPlayer - g_NetVars.GetEyePosition(pLocal)).Length();

				if(flTargetDst < flDst)
				{
					flDst = flTargetDst;
					bestTraget = ax;
				}
			}
		}
	}
}

void cAimbot::doAim(C_BaseEntity* pLocal,Vector& vLocalEye,CUserCmd* cmd,QAngle qPunch)
{
	getBestTargetIndex(pLocal, vLocalEye);
	QAngle qAimAngles;
	int tIndex = 0;
	tIndex = bestTraget;
	

	C_BaseEntity* pBaseEntity = ( C_BaseEntity* ) g_pEntList->GetClientEntity(tIndex);

	if(isValidPlayer(pBaseEntity))
	{
		Vector vTargetDest;
		getHitbox(vTargetDest,12,tIndex);

		Vector vAimspot = vTargetDest - vLocalEye;
		VectorAngles(vAimspot,qAimAngles);

		if(g_NetVars.GetBasePlayerTeam(pBaseEntity) != g_NetVars.GetBasePlayerTeam(pLocal) && isVisible(pBaseEntity))
		{
			g_pEngine->SetViewAngles(qAimAngles);
			cmd->viewangles = qAimAngles;
			cmd->buttons |= IN_ATTACK;
		}
	}
}