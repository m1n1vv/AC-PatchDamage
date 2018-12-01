/*==========================================================================

	Copyright (c) 2017 Yakovlev Ivan.

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the
use of this software.

Permission is granted to anyone to use this software for
any purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1.	The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software in
	a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

2.	Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

3.	This notice may not be removed or altered from any source distribution.

=========================================================================*/

#if  defined AC_PATCH_DAMAGE
	#endinput
#endif
#define AC_PATCH_DAMAGE

#if !defined AC_PATCH_DAMAGE_TIMER
	#define AC_PATCH_DAMAGE_TIMER   			1000	//Время таймера
#endif

#if !defined AC_PATCH_DAMAGE_REGISTR
	#define AC_PATCH_DAMAGE_REGISTR				5		//Промежуток на проверку нанесения урона
#endif

//#define AC_PATCH_DAMAGE_USE_PLAYERID					//TakeDamage регистрируется на убийцу

//#define AC_PATCH_DAMAGE_DEVELOPER_MODE				//Активируется для тестирования. Отключается функция Kick и включаются подсказки в чат

static
	afk[MAX_PLAYERS char],
	afk__tick[MAX_PLAYERS],
	afk__check[MAX_PLAYERS][2],
	afk__info[MAX_PLAYERS char],

	ac_pd__targetid[MAX_PLAYERS],
	ac_pd__take_check[MAX_PLAYERS char],
	ac_pd__give_check[MAX_PLAYERS char],
	ac_pd__warn[MAX_PLAYERS char];

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if (hittype == BULLET_HIT_TYPE_PLAYER)
	{
		if (afk{hitid} == 0)
		{
			if ((22 <= weaponid <= 34))
			{
				#if defined AC_PATCH_DAMAGE_DEVELOPER_MODE
				static
					message,
					str[30];
				#endif

				if (ac_pd__targetid[playerid] != hitid)
				{
					#if defined AC_PATCH_DAMAGE_USE_PLAYERID
					ac_pd__take_check{playerid} =
					#else
					ac_pd__take_check{hitid} =
					#endif
					ac_pd__warn{playerid} = 0;
					ac_pd__targetid[playerid] = hitid;
				}

				#if defined AC_PATCH_DAMAGE_USE_PLAYERID
				ac_pd__take_check{playerid}++;
				#else
				ac_pd__take_check{hitid}++;
				#endif
				ac_pd__give_check{playerid}++;
				ac_pd__warn{playerid}++;

				if (ac_pd__warn{playerid} == AC_PATCH_DAMAGE_REGISTR)
				{
					#if defined AC_PATCH_DAMAGE_DEVELOPER_MODE
						message = 0;
					#endif
					#if defined AC_PATCH_DAMAGE_USE_PLAYERID
					if (ac_pd__take_check{playerid} == AC_PATCH_DAMAGE_REGISTR)
					#else
					if (ac_pd__take_check{hitid} == AC_PATCH_DAMAGE_REGISTR)
					#endif
					{
						#if defined AC_PATCH_DAMAGE_DEVELOPER_MODE
							message = 1;
							format(str, sizeof str, "ID(%i) cheats {00FF00}ON", hitid);
							SendClientMessageToAll(-1, str);
						#else
							Kick(hitid);
						#endif
					}
					if (ac_pd__give_check{playerid} == AC_PATCH_DAMAGE_REGISTR)
					{
						#if defined AC_PATCH_DAMAGE_DEVELOPER_MODE
							message = 1;
							format(str, sizeof str, "ID(%i) cheats {00FF00}ON", playerid);
							SendClientMessageToAll(-1, str);
						#else
							Kick(playerid);
						#endif
					}
					#if defined AC_PATCH_DAMAGE_DEVELOPER_MODE
						if (message == 0)
						{
							SendClientMessageToAll(-1, !"cheats {FF0000}OFF");
						}
					#endif
					#if defined AC_PATCH_DAMAGE_USE_PLAYERID
					ac_pd__take_check{playerid} =
					#else
					ac_pd__take_check{hitid} =
					#endif
					ac_pd__give_check{playerid} =
					ac_pd__warn{playerid} = 0;
				}
			}

			return 1;
		}
	}
#if defined ac_pd__OnPlayerWeaponShot
	ac_pd__OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif
	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	#if defined AC_PATCH_DAMAGE_USE_PLAYERID
	if (issuerid != INVALID_PLAYER_ID)
	{
		ac_pd__take_check{issuerid} += 20;
	}
	#else
	ac_pd__take_check{playerid} += 20;
	#endif

#if defined ac_pd__OnPlayerTakeDamage
	ac_pd__OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif
	return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float: amount, weaponid, bodypart)
{
    ac_pd__give_check{playerid} += 20;

#if defined ac_pd__OnPlayerGiveDamage
	ac_pd__OnPlayerGiveDamage(playerid, damagedid, Float: amount, weaponid, bodypart);
#endif
    return 1;
}

public OnGameModeInit()
{
	SetTimer(!"CheckAFK", AC_PATCH_DAMAGE_TIMER, true);

#if defined ac_pd__OnGameModeInit
	ac_pd__OnGameModeInit();
#endif
	return 1;
}

public OnPlayerUpdate(playerid)
{
	if ((GetTickCount() - afk__tick[playerid]) > AC_PATCH_DAMAGE_TIMER)
	{
		afk__tick[playerid] = GetTickCount();
		afk__info{playerid} = !afk__info{playerid};
	}
#if defined ac_pd__OnPlayerUpdate
	ac_pd__OnPlayerUpdate(playerid);
#endif
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	afk{playerid} =
	afk__tick[playerid] =
	afk__check[playerid][0] =
	afk__check[playerid][1] =
	afk__info{playerid} = 0;
    
	ac_pd__targetid[playerid] =
	ac_pd__take_check{playerid} =
	ac_pd__give_check{playerid} =
	ac_pd__warn{playerid} = 0;

#if defined ac_pd__OnPlayerDisconnect
	ac_pd__OnPlayerDisconnect(playerid, reason);
#endif
	return 1;
}

forward CheckAFK();
public CheckAFK()
{
	foreach (Player, i)
	{
		afk__check[i][1] = afk__check[i][0];
		afk__check[i][0] = afk__info{i};
		if (afk__check[i][1] == afk__check[i][0])
		{
			afk{i} = 1;
		}
		else
		{
			afk{i} = 0;
		}
	}
    
	return 1;
}

#if defined _ALS_OnPlayerWeaponShot
	#undef OnPlayerWeaponShot
#else
	#define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot ac_pd__OnPlayerWeaponShot
#if  defined ac_pd__OnPlayerWeaponShot
forward ac_pd__OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

#if defined _ALS_OnPlayerTakeDamage
	#undef OnPlayerTakeDamage
#else
	#define _ALS_OnPlayerTakeDamage
#endif
#define OnPlayerTakeDamage ac_pd__OnPlayerTakeDamage
#if  defined ac_pd__OnPlayerTakeDamage
forward ac_pd__OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

#if defined _ALS_OnPlayerGiveDamage
	#undef OnPlayerGiveDamage
#else
	#define _ALS_OnPlayerGiveDamage
#endif
#define OnPlayerGiveDamage ac_pd__OnPlayerGiveDamage
#if  defined ac_pd__OnPlayerGiveDamage
forward ac_pd__OnPlayerGiveDamage(playerid, damagedid, Float: amount, weaponid, bodypart);
#endif

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit ac_pd__OnGameModeInit
#if  defined ac_pd__OnGameModeInit
forward ac_pd__OnGameModeInit();
#endif

#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate ac_pd__OnPlayerUpdate
#if  defined ac_pd__OnPlayerUpdate
forward ac_pd__OnPlayerUpdate(playerid);
#endif

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect ac_pd__OnPlayerDisconnect
#if  defined ac_pd__OnPlayerDisconnect
forward ac_pd__OnPlayerDisconnect(playerid, reason);
#endif
