#include <gb/gb.h>

typedef struct GameCharacter {
	UBYTE sprite_ids[4];
	UBYTE sprite_ids_count;
	UBYTE is_alternate;
	UBYTE is_destroyed;
	UBYTE is_animating;
	UBYTE frame;
	UINT16 last_animated_at;
	UINT8 direction;
	UINT8 x;
	UINT8 y;
	INT8 velocity_x;
	INT8 velocity_y;
	UINT8 width;
	UINT8 height;
} GameCharacter;
