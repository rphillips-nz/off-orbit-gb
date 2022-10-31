#ifndef _OO_GAME_CHARACTER_H_
#define _OO_GAME_CHARACTER_H_



#include <gb/gb.h>

typedef struct GameCharacter {
	UBYTE sprite_ids[4];
	UBYTE sprite_ids_count;
	UBYTE is_alternate;
	UBYTE is_destroyed;
	UBYTE is_destroying;
	UBYTE frame;
	UINT16 last_animated_at;
	UINT8 direction;
	UINT8 x;
	UINT8 y;
	INT8 velocity_x;
	INT8 velocity_y;
	UINT8 width;
	UINT8 height;
	UINT8 half_width; // saves calculating this all the time
	UINT8 half_height; // saves calculating this all the time
	struct GameCharacter* last_collided_with;
} GameCharacter;

void destroy_game_character(GameCharacter* character);

void render_game_character(GameCharacter* character);

void update_game_character(GameCharacter* character, GameCharacter* player);



#endif
