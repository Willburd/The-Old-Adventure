#include "inventory.h"
#include "../globals.h"

static void HandlePlayerHealthUpdate(int modify);

int PlayerDamage(int amount, int forced)
{
	if (amount <= 0)
		return FALSE;
	// TODO - special conditions that would block damage here
	HandlePlayerHealthUpdate(-amount);
	return TRUE;
}

int PlayerHeal(int amount)
{
	if (amount <= 0)
		return FALSE;
	HandlePlayerHealthUpdate(+amount);
	return TRUE;
}

static void HandlePlayerHealthUpdate(int modify)
{
	player_inventory->health += modify;
	if (player_inventory->health > (player_inventory->max_hearts * HEALTH_PER_HEART)) // Cap maximum
		player_inventory->health = player_inventory->max_hearts * HEALTH_PER_HEART;
	if (player_inventory->health > 0) // Still alive
		return;
	player_inventory->health = 0;
	// TODO - Inform player actor of death
	
}