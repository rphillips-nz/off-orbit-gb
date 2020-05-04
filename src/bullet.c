#include <gb/gb.h>
#include "game-character.h"
#include "constants.h"
#include "sound/sound.h"

extern GameCharacter player;
extern UBYTE next_sprite_index;
extern GameCharacter bullets[BULLET_COUNT];

UINT16 time_last_fired;

void set_up_bullet(GameCharacter* bullet) {
	bullet->x = 0u;
	bullet->y = 0u;
	bullet->velocity_x = 0;
	bullet->velocity_y = 0;
	bullet->width = 8u;
	bullet->height = 8u;
	bullet->half_width = 4u;
	bullet->half_height = 4u;
	bullet->is_destroyed = 1u;

	set_sprite_tile(next_sprite_index, 26u);
	bullet->sprite_ids[0u] = next_sprite_index++;
	bullet->sprite_ids_count = 1u;
}

void update_bullet(GameCharacter* bullet) {
	update_game_character(bullet, &player);

	if (bullet->x > WORLD_MAX_X || bullet->y > WORLD_MAX_Y) { // No need to check MIN since it wraps around
		destroy_game_character(bullet);
	}
}

void fire_bullet(UINT8 direction) {
	GameCharacter* bullet;
	UBYTE i, valid_bullet_index = BULLET_COUNT;
	INT8 player_mid_x, player_mid_y;

	for (i = 0u; i < BULLET_COUNT; i++) {
		if (bullets[i].is_destroyed) {
			valid_bullet_index = i;
			break;
		}
	}

	if ((valid_bullet_index == BULLET_COUNT) || (sys_time - time_last_fired < FIRE_RATE)) {
		return;
	}

	time_last_fired = sys_time;
	bullet = &bullets[i];

	if (direction & J_LEFT) {
		player.velocity_x = MIN(player.velocity_x + ACCELERATION, MAX_VELOCITY);

		if (player.velocity_x < 0) {
			bullet->velocity_x = player.velocity_x - 5;
		} else {
			bullet->velocity_x = -5;
		}
	} else if (direction & J_RIGHT) {
		player.velocity_x = MAX(player.velocity_x - ACCELERATION, -MAX_VELOCITY);

		if (player.velocity_x > 0) {
			bullet->velocity_x = player.velocity_x + 5;
		} else {
			bullet->velocity_x = 5;
		}
	} else {
		bullet->velocity_x = 0;
	}

	if (direction & J_UP) {
		player.velocity_y = MIN(player.velocity_y + ACCELERATION, MAX_VELOCITY);
		
		if (player.velocity_y < 0) {
			bullet->velocity_y = player.velocity_y - 5;
		} else {
			bullet->velocity_y = -5;
		}
	} else if (direction & J_DOWN) {
		player.velocity_y = MAX(player.velocity_y - ACCELERATION, -MAX_VELOCITY);
		
		if (player.velocity_y > 0) {
			bullet->velocity_y = player.velocity_y + 5;
		} else {
			bullet->velocity_y = 5;
		}
	} else {
		bullet->velocity_y = 0;
	}

	player_mid_x = player.x + player.half_width;
	player_mid_y = player.y + player.half_height;

	bullet->x = player_mid_x - bullet->half_width + bullet->velocity_x;
	bullet->y = player_mid_y - bullet->half_height + bullet->velocity_y;
	bullet->is_destroyed = 0u;

	play_sound_fire_bullet();
}
