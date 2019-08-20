#include <gb/gb.h>

typedef struct GameCharacter {
	UBYTE sprite_ids[4]; // character uses 4 sprites
	UBYTE is_destroyed;
	UBYTE did_collide;
	UINT8 direction;
	UINT8 x;
	UINT8 y;
	INT8 velocity_x;
	INT8 velocity_y;
	UINT8 width;
	UINT8 height;
} GameCharacter;
