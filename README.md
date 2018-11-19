# AC-PatchDamage
> Версия: 0.2

Античит проверяет игроков, которые наносят и по которым был нанесен урон с огнестрельного оружия [22-34].

Защищает от патча GiveTakeDamage:
* OnPlayerGiveDamage
* OnPlayerTakeDamage

Установка
---------
```pawn
#include <AC-PatchDamage>
```
В конце мода создаем **stock** с названием **KickMessage** с уведомление о причине кика:
```pawn
stock KickMessage(playerid)
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
|AC_PATCH_DAMAGE_DEVELOPER_MODE|Активируется для тестирования. Отключается функция **PlayerKick** и включаются подсказки в чат. По необходимости объявите перед иклудом|
