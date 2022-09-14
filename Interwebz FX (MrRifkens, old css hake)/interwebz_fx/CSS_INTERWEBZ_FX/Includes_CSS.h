#ifndef CSS_INCLUDES_H_INCLUDED
#define CSS_INCLUDES_H_INCLUDED

	/* CSS_SDK */ #ifndef CLIENT_DLL
	/* CSS_SDK */ #define CLIENT_DLL
	/* CSS_SDK */ #endif


	/* CSS_SDK */ #include "SDK/game/client/cbase.h"
	/* CSS_SDK */ #include "SDK/public/cdll_int.h"
	/* CSS_SDK */ #include "SDK/game/client/input.h"
	/* CSS_SDK */ #include "SDK/public/vgui/ISurface.h"
	/* CSS_SDK */ #include "SDK/public/vgui_controls/Frame.h"
	/* CSS_SDK */ #include "SDK/public/vgui_controls/Panel.h"
	/* CSS_SDK */ #include "SDK/public/VGuiMatSurface/IMatSystemSurface.h"
	/* CSS_SDK */ #include "SDK/public/tier1/iconvar.h"
	/* CSS_SDK */ #include "SDK/public/tier1/convar.h"
	/* CSS_SDK */ #include "SDK/game/shared/usercmd.h"
	/* CSS_SDK */ #include "SDK/game/shared/in_buttons.h"
	/* CSS_SDK */ #include "SDK/public/engine/ivmodelrender.h"
	/* CSS_SDK */ #include "SDK/public/view_shared.h"
	/* CSS_SDK */ #include "SDK/public/inetchannelinfo.h"
	/* CSS_SDK */ #include "SDK/public/ienginevgui.h"
	/* CSS_SDK */ #include "SDK/public/vgui/IPanel.h"
	/* CSS_SDK */ #include "SDK/public/vgui/IClientPanel.h"
	/* CSS_SDK */ #include "SDK/public/vgui/ISurface.h"
	/* CSS_SDK */ #include "sdk/public/vgui/IVGui.h"
	/* CSS_SDK */ #include "sdk/public/vgui/ISystem.h"
	/* CSS_SDK */ #include "SDK/public/vgui_controls/Panel.h"
	/* CSS_SDK */ #include "SDK/public/vgui_controls/Controls.h"
	/* CSS_SDK */ #include "SDK/public/vgui/ILocalize.h"
	/* CSS_SDK */ #include "SDK/public/materialsystem/imesh.h"
	/* CSS_SDK */ #include "SDK/public/materialsystem/imaterial.h"
	/* CSS_SDK */ #include "SDK/public/materialsystem/imaterialsystem.h"
	/* CSS_SDK */ #include "SDK/public/engine/ICollideable.h"
	/* CSS_SDK */ #include "SDK/public/igameevents.h"
	/* CSS_SDK */ #include "SDK/public/inputsystem/ButtonCode.h"
	/* CSS_SDK */ #include "SDK/Game/Shared/igamemovement.h"
	/* CSS_SDK */ #include "SDK/public/engine/ivdebugoverlay.h"

	/* STEAMAPI_SDK */ #include "SDK\public\steam\steam_api.h"

	typedef struct MapPlayer_s
	{
		int             index;          // player's index
		int             userid;         // user ID on server
		int             icon;           // players texture icon ID
		Color			color;          // players team color
		char			name[MAX_PLAYER_NAME_LENGTH];
		int             team;           // N,T,CT
		int             health;         // 0..100, 7 bit
		Vector		position;       // current x,y pos
		QAngle		angle;          // view origin 0..360
		Vector2D	trail[30]; // save 1 footstep each second for 1 minute
	} MapPlayer_t;

#endif // CSS_INCLUDES_H_INCLUDED
