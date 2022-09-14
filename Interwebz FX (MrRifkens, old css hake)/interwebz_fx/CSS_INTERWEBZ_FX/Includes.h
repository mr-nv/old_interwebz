#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <psapi.h>
#include <time.h>
#include <process.h>
#include <vector>
#include <map>

#include "Includes_CSS.h"
#include "Macros.h"
#include "Utils.h"
#include "visuals.h"
#include "netvars.h"
#include "draw.h"
#include "aimbot.h"

#define _X( x, y, z ) Base::Utils::XORDecrypt( x, y, z ).c_str()

extern vgui::ISurface* g_pSurface;
extern IVDebugOverlay* g_pDebugOverlay;
extern IBaseClientDLL* g_pClient;
extern IVEngineClient* g_pEngine;
extern IClientEntityList* g_pEntList;
extern CGlobalVarsBase* g_pGlobalVars;
extern IVModelInfoClient* g_pModelinfo;
extern CGlobalVarsBase* g_pGlobalVars;
extern IEngineTrace* g_pEnginetrace;

extern cVisuals g_Visuals;
extern cNetVars g_NetVars;
extern cDraw g_Draw;
extern cAimbot g_Aimbot;
