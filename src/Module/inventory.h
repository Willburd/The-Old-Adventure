#ifndef __INVENTORY_HEADER__
#define __INVENTORY_HEADER__

#define MAX_INVENTORY_SLOTS 256

#define HEALTH_STARTING_HEARTS 3
#define HEALTH_PER_HEART 32

typedef struct {
	// Health
	unsigned int health;
	unsigned int max_hearts;
	// Inventory
	unsigned int item[MAX_INVENTORY_SLOTS];
	unsigned int ammo[MAX_INVENTORY_SLOTS];
	// Item buttons point to slots
	unsigned int item_slot_Main;
	unsigned int item_slot_A; 
	unsigned int item_slot_B;
	unsigned int item_slot_C;
} Inventory;

Inventory* player_inventory;

#endif