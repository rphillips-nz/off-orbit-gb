#include <gb/gb.h>
#include "game-character.h"
#include "enemy.h"
#include "constants.h"
#include "semi-random.h"

extern GameCharacter player;
extern UBYTE next_sprite_index;

void animate_enemy(GameCharacter* enemy) {
	if (enemy->sprite_ids_count == 1u) {
		switch (enemy->frame) {
			case 0u:
				set_sprite_tile(enemy->sprite_ids[0u], enemy->is_alternate ? 24u : 25u);
				break;
			case 1u:
				set_sprite_tile(enemy->sprite_ids[0u], 40u);
				break;
			case 2u:
				set_sprite_tile(enemy->sprite_ids[0u], 41u);
				break;
			case 3u:
				set_sprite_tile(enemy->sprite_ids[0u], 42u);
				break;
			default:
				destroy_game_character(enemy);
				break;
		}
	} else {
		switch (enemy->frame) {
			case 0u:
				if (enemy->is_alternate) {
					set_sprite_tile(enemy->sprite_ids[0u], 20u);
					set_sprite_tile(enemy->sprite_ids[1u], 21u);
					set_sprite_tile(enemy->sprite_ids[2u], 22u);
					set_sprite_tile(enemy->sprite_ids[3u], 23u);
				} else {
					set_sprite_tile(enemy->sprite_ids[0u], 16u);
					set_sprite_tile(enemy->sprite_ids[1u], 17u);
					set_sprite_tile(enemy->sprite_ids[2u], 18u);
					set_sprite_tile(enemy->sprite_ids[3u], 19u);
				}
				break;
			case 1u:
				set_sprite_tile(enemy->sprite_ids[0u], 28u);
				set_sprite_tile(enemy->sprite_ids[1u], 29u);
				set_sprite_tile(enemy->sprite_ids[2u], 30u);
				set_sprite_tile(enemy->sprite_ids[3u], 31u);
				break;
			case 2u:
				set_sprite_tile(enemy->sprite_ids[0u], 32u);
				set_sprite_tile(enemy->sprite_ids[1u], 33u);
				set_sprite_tile(enemy->sprite_ids[2u], 34u);
				set_sprite_tile(enemy->sprite_ids[3u], 35u);
				break;
			case 3u:
				set_sprite_tile(enemy->sprite_ids[0u], 36u);
				set_sprite_tile(enemy->sprite_ids[1u], 37u);
				set_sprite_tile(enemy->sprite_ids[2u], 38u);
				set_sprite_tile(enemy->sprite_ids[3u], 39u);
				break;
			default:
				destroy_game_character(enemy);
				break;
		}
	}
}

void set_up_enemy(GameCharacter* enemy, UBYTE is_alternate) {
	reset_enemy(enemy);

	enemy->is_alternate = is_alternate;
	enemy->width = 16u;
	enemy->height = 16u;
	enemy->half_width = 8u;
	enemy->half_height = 8u;

	if (is_alternate) {
		set_sprite_tile(next_sprite_index, 20u);
		enemy->sprite_ids[0u] = next_sprite_index++;
		set_sprite_tile(next_sprite_index, 21u);
		enemy->sprite_ids[1u] = next_sprite_index++;
		set_sprite_tile(next_sprite_index, 22u);
		enemy->sprite_ids[2u] = next_sprite_index++;
		set_sprite_tile(next_sprite_index, 23u);
		enemy->sprite_ids[3u] = next_sprite_index++;
	} else {
		set_sprite_tile(next_sprite_index, 16u);
		enemy->sprite_ids[0u] = next_sprite_index++;
		set_sprite_tile(next_sprite_index, 17u);
		enemy->sprite_ids[1u] = next_sprite_index++;
		set_sprite_tile(next_sprite_index, 18u);
		enemy->sprite_ids[2u] = next_sprite_index++;
		set_sprite_tile(next_sprite_index, 19u);
		enemy->sprite_ids[3u] = next_sprite_index++;
	}
	
	enemy->sprite_ids_count = 4u;
}

void set_up_small_enemy(GameCharacter* enemy, UBYTE is_alternate) {
	reset_enemy(enemy);

	enemy->is_alternate = is_alternate;
	enemy->width = 8u;
	enemy->height = 8u;
	enemy->half_width = 4u;
	enemy->half_height = 4u;

	if (is_alternate) {
		set_sprite_tile(next_sprite_index, 24u);
	} else {
		set_sprite_tile(next_sprite_index, 25u);
	}

	enemy->sprite_ids[0u] = next_sprite_index++;
	enemy->sprite_ids_count = 1u;
}

void update_enemy(GameCharacter* enemy) {
	update_game_character(enemy, &player);

	if (enemy->x > WORLD_MAX_X || enemy->y > WORLD_MAX_Y) {
		destroy_game_character(enemy);
	} else if (enemy->is_destroying && (sys_time - enemy->last_animated_at > ANIMATION_RATE)) {
		enemy->frame++;
		enemy->last_animated_at = sys_time;
		animate_enemy(enemy);
	}
}

void reset_enemy(GameCharacter* enemy) {
	switch (RANDOM_SIDE) {
		case 0: // Top
			enemy->x = RANDOM_WORLD_X;
			enemy->y = WORLD_MIN_Y;
			enemy->velocity_y = RANDOM_POSITIVE_VELOCITY;
			enemy->velocity_x = RANDOM_FLIP == 1u ? RANDOM_VELOCITY : -RANDOM_VELOCITY;
			break;
		case 1: // Right
			enemy->x = WORLD_MAX_X;
			enemy->y = RANDOM_WORLD_Y;
			enemy->velocity_x = RANDOM_NEGATIVE_VELOCITY;
			enemy->velocity_y = RANDOM_FLIP == 1u ? RANDOM_VELOCITY : -RANDOM_VELOCITY;
			break;
		case 2: // Left
			enemy->x = WORLD_MIN_X;
			enemy->y = RANDOM_WORLD_Y;
			enemy->velocity_x = RANDOM_POSITIVE_VELOCITY;
			enemy->velocity_y = RANDOM_FLIP == 1u ? RANDOM_VELOCITY : -RANDOM_VELOCITY;
			break;
		default: // Bottom
			enemy->x = RANDOM_WORLD_X;
			enemy->y = WORLD_MAX_Y;
			enemy->velocity_y = RANDOM_NEGATIVE_VELOCITY;
			enemy->velocity_x = RANDOM_FLIP == 1u ? RANDOM_VELOCITY : -RANDOM_VELOCITY;
			break;
	}

	enemy->last_collided_with = 0;
	enemy->is_destroyed = 0u;
	enemy->frame = 0u;
	enemy->is_destroying = 0u;
	animate_enemy(enemy);

	// Prevents asteroids getting stuck at same speed on end screen
	if (player.is_destroyed || player.is_destroying) {
		enemy->velocity_x += player.velocity_x;
		enemy->velocity_y += player.velocity_y;
	}
}

void destroy_enemy(GameCharacter* enemy) {
	enemy->is_destroying = 1u;
	enemy->last_animated_at = sys_time;
}
