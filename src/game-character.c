#include <gb/gb.h>
#include "game-character.h"

#define SPRITE_SIZE 8u

extern GameCharacter player;

void destroy_game_character(GameCharacter* character) {
	character->is_destroyed = 1u;
	character->x = 0u;
	character->y = 0u;
	character->velocity_x = 0;
	character->velocity_y = 0;
	character->frame = 0u;
	character->is_destroying = 0u;
}

void render_game_character(GameCharacter* character) {
	move_sprite(character->sprite_ids[0u], character->x, character->y);

	if (character->sprite_ids_count > 1u) {
		move_sprite(character->sprite_ids[1u], character->x + SPRITE_SIZE, character->y);
		move_sprite(character->sprite_ids[2u], character->x, character->y + SPRITE_SIZE);
		move_sprite(character->sprite_ids[3u], character->x + SPRITE_SIZE, character->y + SPRITE_SIZE);
	}
}

void update_game_character(GameCharacter* character, GameCharacter* player) {
	character->x -= player->velocity_x;
	character->y -= player->velocity_y;
	character->x += character->velocity_x;
	character->y += character->velocity_y;
}
