# AC-PatchDamage
> Версия: 0.1

Античит проверяет игроков, по которым был нанесен урон. Дальше будет больше.

Установка
---------
```pawn
#include <AC-PatchDamage>
```
В конце мода создаем **stock** с названием **PlayerKick** с уведомление о причине кика:
```pawn
stock PlayerKick(playerid)
{
	static const
		str[] = "Игрок [%i]%s был кикнут за использование читов";

	new
		string[sizeof str + 4 + MAX_PLAYER_NAME + 1 - (2*2)];

	format(string, sizeof string, str, playerid, pInfo[playerid][pName]);
	SendClientMessageToAll(0xFF6347FF, string);
	
	Kick(playerid);

	return 1;
}
```

Директивы
---------
|Директива|Описание|
|---|---|
|AC_PATCH_DAMAGE_TIMER|Время таймера|
|AC_PATCH_DAMAGE_REGISTR|Промежуток на проверку нанесения урона|
|AC_PATCH_DAMAGE_DEVELOPER_MODE|Активируется для тестирования. Отключается функция **Kick** и включаются подсказки в чат|
