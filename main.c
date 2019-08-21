#include <gb/gb.h>
#include <rand.h>
#include <stdio.h>
#include <stdlib.h>
#include "game-character.c"
#include "sprite-data.c"
#include "sound.c"
#include "font.c"
#include "background-data.c"
#include "background-map.c"

#define MAX_VELOCITY 4
#define ACCELERATION 1
#define SPRITE_SIZE 8u

#define FIRE_RATE 15u
#define ANIMATION_RATE 4u
#define BULLET_COUNT 2u
#define ENEMY_COUNT 5u

GameCharacter player;
GameCharacter bullets[BULLET_COUNT];
GameCharacter enemies[ENEMY_COUNT];

UBYTE next_sprite_index = 0u;
UBYTE score = 0u;
UINT16 time_last_fired;

const unsigned char blank_map[1] = {0x00};

INT8 rand_range(INT8 min_n, INT8 max_n) {
	max_n++;
	return min_n + abs(rand()) % (max_n - min_n);
}

void reset_enemy(GameCharacter* enemy) {
	INT8 x, y;

	initrand(DIV_REG);

	x = rand_range(8, 168);
	y = rand_range(16, 160);

	if (x < 88) {
		enemy->x = x - 50;
		enemy->velocity_x = rand_range(1, 2);
	} else {
		enemy->x = x + 50;
		enemy->velocity_x = -rand_range(1, 2);
	}

	if (y < 88) {
		enemy->y = y - 50;
		enemy->velocity_y = rand_range(1, 2);
	} else {
		enemy->y = y + 50;
		enemy->velocity_y = -rand_range(1, 2);
	}

	enemy->is_destroyed = 0;
	enemy->frame = 0u;
}

void destroy_game_character(GameCharacter* character) {
	character->is_destroyed = 1;
	character->x = -16;
	character->y = -16;
	character->velocity_x = 0;
	character->velocity_y = 0;
}

void destroy_enemy(GameCharacter* enemy) {
	enemy->frame = 0;
	enemy->is_animating = 1u;
	enemy->last_animated_at = sys_time;
	enemy->velocity_x = 0;
	enemy->velocity_y = 0;
}

void render_game_character(GameCharacter* character) {
	move_sprite(character->sprite_ids[0u], character->x, character->y);

	if (character->sprite_ids_count > 1) {
		move_sprite(character->sprite_ids[1u], character->x + SPRITE_SIZE, character->y);
		move_sprite(character->sprite_ids[2u], character->x, character->y + SPRITE_SIZE);
		move_sprite(character->sprite_ids[3u], character->x + SPRITE_SIZE, character->y + SPRITE_SIZE);
	}
}

void update_game_character(GameCharacter* character) {
	character->x -= player.velocity_x;
	character->y -= player.velocity_y;
	character->x += character->velocity_x;
	character->y += character->velocity_y;
}

void update_bullet(GameCharacter* bullet) {
	UINT8 distance_x, distance_y;

	update_game_character(bullet);

	distance_x = player.x > bullet->x ? player.x - bullet->x : bullet->x - player.x;
	if (distance_x > 80) {
		destroy_game_character(bullet);
		return;
	}

	distance_y = player.y > bullet->y ? player.y - bullet->y : bullet->y - player.y;
	if (distance_y > 72) {
		destroy_game_character(bullet);
	}
}

void update_enemy(GameCharacter* enemy) {
	UINT8 distance_x, distance_y;

	// if (sys_time % 60 == 0) {
	// 	enemy->velocity_y += player.y < enemy->y ? -1 : 1;
	// 	enemy->velocity_x += player.x < enemy->x ? -1 : 1;
	// }

	if (enemy->is_animating && (sys_time - enemy->last_animated_at > ANIMATION_RATE)) {
		enemy->frame++;
		enemy->last_animated_at = sys_time;
	}

	if (enemy->sprite_ids_count == 1) {
		switch (enemy->frame) {
			case 0u:
				set_sprite_tile(enemy->sprite_ids[0], enemy->is_alternate ? 24u : 25u);
				break;
			case 1u:
				set_sprite_tile(enemy->sprite_ids[0], 40u);
				break;
			case 2u:
				set_sprite_tile(enemy->sprite_ids[0], 41u);
				break;
			case 3u:
				set_sprite_tile(enemy->sprite_ids[0], 42u);
				break;
			default:
				enemy->frame = 0u;
				enemy->is_animating = 0u;
				destroy_game_character(enemy);
				break;
		}
	} else {
		switch (enemy->frame) {
			case 0u:
				if (enemy->is_alternate) {
					set_sprite_tile(enemy->sprite_ids[0], 20u);
					set_sprite_tile(enemy->sprite_ids[1], 21u);
					set_sprite_tile(enemy->sprite_ids[2], 22u);
					set_sprite_tile(enemy->sprite_ids[3], 23u);
				} else {
					set_sprite_tile(enemy->sprite_ids[0], 16u);
					set_sprite_tile(enemy->sprite_ids[1], 17u);
					set_sprite_tile(enemy->sprite_ids[2], 18u);
					set_sprite_tile(enemy->sprite_ids[3], 19u);
				}
				break;
			case 1u:
				set_sprite_tile(enemy->sprite_ids[0], 28u);
				set_sprite_tile(enemy->sprite_ids[1], 29u);
				set_sprite_tile(enemy->sprite_ids[2], 30u);
				set_sprite_tile(enemy->sprite_ids[3], 31u);
				break;
			case 2u:
				set_sprite_tile(enemy->sprite_ids[0], 32u);
				set_sprite_tile(enemy->sprite_ids[1], 33u);
				set_sprite_tile(enemy->sprite_ids[2], 34u);
				set_sprite_tile(enemy->sprite_ids[3], 35u);
				break;
			case 3u:
				set_sprite_tile(enemy->sprite_ids[0], 36u);
				set_sprite_tile(enemy->sprite_ids[1], 37u);
				set_sprite_tile(enemy->sprite_ids[2], 38u);
				set_sprite_tile(enemy->sprite_ids[3], 39u);
				break;
			default:
				enemy->frame = 0u;
				enemy->is_animating = 0u;
				destroy_game_character(enemy);
				break;
		}
	}

	update_game_character(enemy);

	distance_x = player.x > enemy->x ? player.x - enemy->x : enemy->x - player.x;
	if (distance_x > 200) {
		reset_enemy(enemy);
		return;
	}

	distance_y = player.y > enemy->y ? player.y - enemy->y : enemy->y - player.y;
	if (distance_y > 200) {
		reset_enemy(enemy);
	}
}

UBYTE check_collision(GameCharacter* a, GameCharacter* b) {
	INT8 a_left, a_top, a_right, a_bottom, b_left, b_top, b_right, b_bottom;

	if (a->is_destroyed || b->is_destroyed) {
		return 0;
	}

	a_left = a->x;
	a_top = a->y;
	a_right = a->x + a->width;
	a_bottom = a->y + a->height;
	b_left = b->x;
	b_top = b->y;
	b_right = b->x + b->width;
	b_bottom = b->y + b->height;

	return !(a_bottom < b_top || a_top > b_bottom || a_right < b_left || a_left > b_right);
}

void resolve_collision(GameCharacter* a, GameCharacter* b) {
	INT8 a_mid_x = a->x + (a->width / 2);
	INT8 a_mid_y = a->y + (a->height / 2);
	INT8 b_mid_x = b->x + (b->width / 2);
	INT8 b_mid_y = b->y + (b->height / 2);
	INT8 dx = (b_mid_x - a_mid_x) / (b->width / 2);
	INT8 dy = (b_mid_y - a_mid_y) / (b->height / 2);
	INT8 absDX = abs(dx);
	INT8 absDY = abs(dy);

	if (absDX > absDY) { // collision is on the sides
		if (dx < 0) { // a is moving left
			a->x = b->x + b->width;
		} else { // a is moving right
			a->x = b->x - a->width;
		}

		a->velocity_x = -a->velocity_x;
	} else { // collision is on the top or bottom
		if (dy < 0) { // a is moving down
			a->y = b->y + b->height;
		} else { // a is moving up
			a->y = b->y - a->height;
		}

		a->velocity_y = -a->velocity_y;
	}
}

void set_up_player() {
	player.direction = J_UP;
	player.x = 80;
	player.y = 80;
	player.velocity_x = 0;
	player.velocity_y = 0;
	player.width = 16u;
	player.height = 16u;
	player.is_destroyed = 0;

	set_sprite_tile(next_sprite_index, 0u);
	player.sprite_ids[0] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 1u);
	player.sprite_ids[1] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 2u);
	player.sprite_ids[2] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 3u);
	player.sprite_ids[3] = next_sprite_index++;
	player.sprite_ids_count = 4;

	render_game_character(&player);
}

void set_up_bullet(GameCharacter* bullet) {
	bullet->x = -16;
	bullet->y = -16;
	bullet->velocity_x = 0;
	bullet->velocity_y = 0;
	bullet->width = 8u;
	bullet->height = 8u;
	bullet->is_destroyed = 1;

	set_sprite_tile(next_sprite_index, 26u);
	bullet->sprite_ids[0] = next_sprite_index++;
	bullet->sprite_ids_count = 1;
}

void set_up_enemy(GameCharacter* enemy, UBYTE is_alternate) {
	reset_enemy(enemy);

	enemy->frame = 0u;
	enemy->is_alternate = is_alternate;
	enemy->is_animating = 0u;
	enemy->width = 16u;
	enemy->height = 16u;

	if (is_alternate) {
		set_sprite_tile(next_sprite_index, 20u);
		enemy->sprite_ids[0] = next_sprite_index++;
		set_sprite_tile(next_sprite_index, 21u);
		enemy->sprite_ids[1] = next_sprite_index++;
		set_sprite_tile(next_sprite_index, 22u);
		enemy->sprite_ids[2] = next_sprite_index++;
		set_sprite_tile(next_sprite_index, 23u);
		enemy->sprite_ids[3] = next_sprite_index++;
	} else {
		set_sprite_tile(next_sprite_index, 16u);
		enemy->sprite_ids[0] = next_sprite_index++;
		set_sprite_tile(next_sprite_index, 17u);
		enemy->sprite_ids[1] = next_sprite_index++;
		set_sprite_tile(next_sprite_index, 18u);
		enemy->sprite_ids[2] = next_sprite_index++;
		set_sprite_tile(next_sprite_index, 19u);
		enemy->sprite_ids[3] = next_sprite_index++;
	}
	
	enemy->sprite_ids_count = 4;
}

void set_up_small_enemy(GameCharacter* enemy, UBYTE is_alternate) {
	reset_enemy(enemy);

	enemy->frame = 0u;
	enemy->is_alternate = is_alternate;
	enemy->is_animating = 0u;
	enemy->width = 8u;
	enemy->height = 8u;

	if (is_alternate) {
		set_sprite_tile(next_sprite_index, 24u);
	} else {
		set_sprite_tile(next_sprite_index, 25u);
	}

	enemy->sprite_ids[0] = next_sprite_index++;
	enemy->sprite_ids_count = 1;
}

void fire_bullet(UINT8 direction) {
	GameCharacter* bullet;
	UBYTE i;
	INT8 player_mid_x, player_mid_y;

	for (i = 0u; i < BULLET_COUNT; i++) {
		if (bullets[i].is_destroyed) {
			bullet = &bullets[i];
			break;
		}
	}

	if (!bullet) {
		return;
	}

	if (sys_time - time_last_fired < FIRE_RATE) {
		return;
	}

	time_last_fired = sys_time;

	if (direction & J_LEFT) {
		player.velocity_x += ACCELERATION;
		if (player.velocity_x > MAX_VELOCITY) {
			player.velocity_x = MAX_VELOCITY;
		}

		if (player.velocity_x < 0) {
			bullet->velocity_x = player.velocity_x - 5;
		} else {
			bullet->velocity_x = -5;
		}
	} else if (direction & J_RIGHT) {
		player.velocity_x -= ACCELERATION;
		if (player.velocity_x < -MAX_VELOCITY) {
			player.velocity_x = -MAX_VELOCITY;
		}

		if (player.velocity_x > 0) {
			bullet->velocity_x = player.velocity_x + 5;
		} else {
			bullet->velocity_x = 5;
		}
	} else {
		bullet->velocity_x = 0;
	}

	if (direction & J_UP) {
		player.velocity_y += ACCELERATION;
		if (player.velocity_y > MAX_VELOCITY) {
			player.velocity_y = MAX_VELOCITY;
		}
		
		if (player.velocity_y < 0) {
			bullet->velocity_y = player.velocity_y - 5;
		} else {
			bullet->velocity_y = -5;
		}
	} else if (direction & J_DOWN) {
		player.velocity_y -= ACCELERATION;
		if (player.velocity_y < -MAX_VELOCITY) {
			player.velocity_y = -MAX_VELOCITY;
		}
		
		if (player.velocity_y > 0) {
			bullet->velocity_y = player.velocity_y + 5;
		} else {
			bullet->velocity_y = 5;
		}
	} else {
		bullet->velocity_y = 0;
	}

	player_mid_x = player.x + (player.width / 2);
	player_mid_y = player.y + (player.height / 2);

	bullet->x = player_mid_x - (bullet->width / 2) + bullet->velocity_x;
	bullet->y = player_mid_y - (bullet->height / 2)+ bullet->velocity_y;
	bullet->is_destroyed = 0;
}

void update_score() {
	UINT8 digit_map[1u];
	UBYTE score_iterator = score;
	INT8 digit_render_count = 0u;

	// if (time > sessionhighscore) {
	// 	sessionhighscore = time;
	// }

	do {
		digit_map[0u] = score_iterator % 10u + 6u; // + 6 is the win data offset where the digits start
		set_win_tiles(19u - digit_render_count, 0u, 1u, 1u, digit_map);
		digit_render_count++;
		score_iterator /= 10u;
	} while (score_iterator > 0u);
}

void turn_player(UINT8 jpad) {
	if (jpad & J_LEFT) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0], 12u);
		set_sprite_tile(player.sprite_ids[1], 13u);
		set_sprite_tile(player.sprite_ids[2], 14u);
		set_sprite_tile(player.sprite_ids[3], 15u);
	}

	if (jpad & J_RIGHT) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0], 8u);
		set_sprite_tile(player.sprite_ids[1], 9u);
		set_sprite_tile(player.sprite_ids[2], 10u);
		set_sprite_tile(player.sprite_ids[3], 11u);
	}

	if (jpad & J_UP) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0], 0u);
		set_sprite_tile(player.sprite_ids[1], 1u);
		set_sprite_tile(player.sprite_ids[2], 2u);
		set_sprite_tile(player.sprite_ids[3], 3u);
	}

	if (jpad & J_DOWN) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0], 4u);
		set_sprite_tile(player.sprite_ids[1], 5u);
		set_sprite_tile(player.sprite_ids[2], 6u);
		set_sprite_tile(player.sprite_ids[3], 7u);
	}
}

void main() {
	UINT8 jpad = 0u, j, i;

	disable_interrupts();

	set_win_data(5u, 36u, FontData);
	set_bkg_data(0u, 6u, BackgroundData);
	move_win(7u, 136u);

	for (j = 0u; j < 32u; j++) {
		for (i = 0u; i < 32u; i++) {		
			set_win_tiles(i, j, 1u, 1u, blank_map);
			set_bkg_tiles(i, j, 1u, 1u, blank_map);
		}
	}

	set_bkg_tiles(0, 0, 40, 34, BackgroundMap);
	update_score();

	set_sprite_data(0u, 43u, SpriteData);
	set_up_player();

	for (i = 0u; i < BULLET_COUNT; i++) {
		set_up_bullet(&bullets[i]);
	}

	for (i = 0u; i < ENEMY_COUNT - 2; i++) {
		set_up_enemy(&enemies[i], i % 2 == 0);
	}

	set_up_small_enemy(&enemies[ENEMY_COUNT - 2], 0);
	set_up_small_enemy(&enemies[ENEMY_COUNT - 1], 1);

	SHOW_SPRITES;
	SHOW_BKG;
	SHOW_WIN;
	DISPLAY_ON;

	enable_interrupts();

	while (1) {
		wait_vbl_done();

		jpad = joypad();

		turn_player(jpad);

		if (jpad & J_A) {
			fire_bullet(player.direction);
		}

		for (i = 0u; i < BULLET_COUNT; i++) {
			if (bullets[i].is_destroyed) {
				continue;
			}

			update_bullet(&bullets[i], 0u);

			for (j = 0u; j < ENEMY_COUNT; j++) {
				if (check_collision(&bullets[i], &enemies[j])) {
					destroy_game_character(&bullets[i]);
					destroy_enemy(&enemies[j]);
					score += 1u;
					update_score();
					break;
				}
			}
		}

		for (i = 0u; i < ENEMY_COUNT; i++) {
			if (enemies[i].is_destroyed) {
				continue;
			}

			update_enemy(&enemies[i]);

			if (check_collision(&player, &enemies[i])) {
				resolve_collision(&enemies[i], &player);
			}

			for (j = 0u; j < ENEMY_COUNT; j++) {
				if (i != j && check_collision(&enemies[i], &enemies[j])) {
					resolve_collision(&enemies[i], &enemies[j]);
				}
			}
		}

		scroll_bkg(player.velocity_x, player.velocity_y);

		for (i = 0u; i < ENEMY_COUNT; i++) {
			if (enemies[i].is_destroyed) {
				reset_enemy(&enemies[i]);
			}

			render_game_character(&enemies[i]);
		}

		for (i = 0u; i < BULLET_COUNT; i++) {
			render_game_character(&bullets[i]);
		}
	}
}
