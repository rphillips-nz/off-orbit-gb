#include <gb/gb.h>
#include "game-character.c"
#include "fade.c"
#include "sprite-data.c"
#include "sound.c"
#include "font.c"
#include "background-data.c"
#include "background-map.c"
#include "title-data.c"
#include "title-map.c"

// Top corner of the screen is actually (8, 16) rather than (0, 0)
#define SCREEN_MIN_X 8u
#define SCREEN_MIN_Y 16u
#define SCREEN_MAX_X 168u
#define SCREEN_MAX_Y 160u

// Screen is 160x144 and add 8 for each side
#define WORLD_MIN_X 0u
#define WORLD_MIN_Y 8u
#define WORLD_MAX_X 176u
#define WORLD_MAX_Y 168u

#include "semi-random.c"

#define MAX_VELOCITY 4
#define ACCELERATION 1
#define SPRITE_SIZE 8u

#define MAX_HEALTH 3u
#define FIRE_RATE 20u
#define ANIMATION_RATE 4u
#define BULLET_COUNT 2u
#define ENEMY_COUNT 5u
#define SMALL_ENEMY_COUNT 2u


#define ABS(x)    (((x) < 0) ? -(x) : (x))
#define MAX(a, b) ((a < b) ? (b) : (a))
#define MIN(a, b) ((a < b) ? (a) : (b))

GameCharacter player;
GameCharacter bullets[BULLET_COUNT];
GameCharacter enemies[ENEMY_COUNT];

UBYTE game_running;
UBYTE next_sprite_index;
UBYTE score;
UBYTE health;

UINT16 time_last_fired;

const UINT8 reset_prompt_map[11u] = {32u, 34u, 21u, 35u, 35u, 0u, 35u, 36u, 17u, 34u, 36u}; // "PRESS START"
const UINT8 heart_map[1u] = {6u}; // The heart tile
const UINT8 blank_map[] = {0u};

void reset_enemy(GameCharacter* enemy) {
	switch (RANDOM_SIDE) {
		case 0: // Top
			enemy->x = RANDOM_WORLD_X;
			enemy->y = WORLD_MIN_Y;
			enemy->velocity_y = RANDOM_POSITIVE_VELOCITY;
			enemy->velocity_x = RANDOM_FLIP == 1 ? RANDOM_VELOCITY : -RANDOM_VELOCITY;
			break;
		case 1: // Right
			enemy->x = WORLD_MAX_X;
			enemy->y = RANDOM_WORLD_Y;
			enemy->velocity_x = RANDOM_NEGATIVE_VELOCITY;
			enemy->velocity_y = RANDOM_FLIP == 1 ? RANDOM_VELOCITY : -RANDOM_VELOCITY;
			break;
		case 2: // Left
			enemy->x = WORLD_MIN_X;
			enemy->y = RANDOM_WORLD_Y;
			enemy->velocity_x = RANDOM_POSITIVE_VELOCITY;
			enemy->velocity_y = RANDOM_FLIP == 1 ? RANDOM_VELOCITY : -RANDOM_VELOCITY;
			break;
		default: // Bottom
			enemy->x = RANDOM_WORLD_X;
			enemy->y = WORLD_MAX_Y;
			enemy->velocity_y = RANDOM_NEGATIVE_VELOCITY;
			enemy->velocity_x = RANDOM_FLIP == 1 ? RANDOM_VELOCITY : -RANDOM_VELOCITY;
			break;
	}

	enemy->is_destroyed = 0u;
	enemy->frame = 0u;
}

void destroy_game_character(GameCharacter* character) {
	character->is_destroyed = 1u;
	character->x = 0u;
	character->y = 0u;
	character->velocity_x = 0;
	character->velocity_y = 0;
	character->frame = 0u;
	character->is_animating = 0u;
}

void destroy_enemy(GameCharacter* enemy) {
	enemy->frame = 0u;
	enemy->is_animating = 1u;
	enemy->last_animated_at = sys_time;
	enemy->velocity_x = 0;
	enemy->velocity_y = 0;
}

void render_game_character(GameCharacter* character) {
	move_sprite(character->sprite_ids[0u], character->x, character->y);

	if (character->sprite_ids_count > 1u) {
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
	update_game_character(bullet);

	if (bullet->x > WORLD_MAX_X || bullet->y > WORLD_MAX_Y) { // No need to check MIN since it wraps around
		destroy_game_character(bullet);
	}
}

void update_enemy(GameCharacter* enemy) {
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
				destroy_game_character(enemy);
				break;
		}
	}

	update_game_character(enemy);

	if (enemy->x > WORLD_MAX_X || enemy->y > WORLD_MAX_Y) {
		destroy_game_character(enemy);
	}
}

UBYTE did_collide(GameCharacter* a, GameCharacter* b) {
	if (a->is_destroyed || b->is_destroyed || a->is_animating || b->is_animating) {
		return 0u;
	}

	return !(
		(a->y + a->height) < b->y || // a_bottom < b_top
		a->y > (b->y + b->height) || // a_top > b_bottom
		(a->x + a->width) < b->x ||  // a_right < b_left
		a->x > (b->x + b->width)     // a_left > b_right
	);
}

void set_up_player() {
	player.direction = J_UP;
	player.x = 80u;
	player.y = 80u;
	player.velocity_x = 0;
	player.velocity_y = 0;
	player.width = 16u;
	player.height = 16u;
	player.half_width = 8u;
	player.half_height = 8u;
	player.is_destroyed = 0u;
	player.frame = 0u;
	player.is_animating = 0u;

	set_sprite_tile(next_sprite_index, 0u);
	player.sprite_ids[0u] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 1u);
	player.sprite_ids[1u] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 2u);
	player.sprite_ids[2u] = next_sprite_index++;
	set_sprite_tile(next_sprite_index, 3u);
	player.sprite_ids[3u] = next_sprite_index++;
	player.sprite_ids_count = 4u;
}

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

void set_up_enemy(GameCharacter* enemy, UBYTE is_alternate) {
	reset_enemy(enemy);

	enemy->frame = 0u;
	enemy->is_alternate = is_alternate;
	enemy->is_animating = 0u;
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
	
	enemy->sprite_ids_count = 4;
}

void set_up_small_enemy(GameCharacter* enemy, UBYTE is_alternate) {
	reset_enemy(enemy);

	enemy->frame = 0u;
	enemy->is_alternate = is_alternate;
	enemy->is_animating = 0u;
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
}

void update_score() {
	UINT8 digit_map[1u];
	UBYTE score_iterator = score;
	INT8 digit_render_count = 0u;

	// if (time > sessionhighscore) {
	// 	sessionhighscore = time;
	// }

	do {
		digit_map[0u] = score_iterator % 10u + 7u; // + the win data offset where the digits start
		set_win_tiles(19u - digit_render_count, 0u, 1u, 1u, digit_map);
		digit_render_count++;
		score_iterator /= 10u;
	} while (score_iterator > 0u);
}

void update_health() {
	UBYTE i;

	for (i = 0u; i < MAX_HEALTH; i++) {
		set_win_tiles(i, 0u, 1u, 1u, i > health - 1 ? blank_map : heart_map);
	}
}

void turn_player(UINT8 jpad) {
	if (jpad & J_LEFT) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0u], 12u);
		set_sprite_tile(player.sprite_ids[1u], 13u);
		set_sprite_tile(player.sprite_ids[2u], 14u);
		set_sprite_tile(player.sprite_ids[3u], 15u);
	}

	if (jpad & J_RIGHT) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0u], 8u);
		set_sprite_tile(player.sprite_ids[1u], 9u);
		set_sprite_tile(player.sprite_ids[2u], 10u);
		set_sprite_tile(player.sprite_ids[3u], 11u);
	}

	if (jpad & J_UP) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0u], 0u);
		set_sprite_tile(player.sprite_ids[1u], 1u);
		set_sprite_tile(player.sprite_ids[2u], 2u);
		set_sprite_tile(player.sprite_ids[3u], 3u);
	}

	if (jpad & J_DOWN) {
		player.direction = jpad;
		set_sprite_tile(player.sprite_ids[0u], 4u);
		set_sprite_tile(player.sprite_ids[1u], 5u);
		set_sprite_tile(player.sprite_ids[2u], 6u);
		set_sprite_tile(player.sprite_ids[3u], 7u);
	}
}

void destroy_player() {
	game_running = 0u;
	player.is_destroyed = 1u;
	player.is_animating = 0u;
	player.frame = 0u;
	player.x = 0u;
	player.y = 0u;
	render_game_character(&player);
	player.x = 80u;
	player.y = 80u;
	set_win_tiles(0u, 0u, 11u, 1u, reset_prompt_map);
}

void start_destroying_player() {
	player.is_animating = 1u;
}

void update_player(UINT8 jpad) {
	if (player.is_animating && (sys_time - player.last_animated_at > ANIMATION_RATE)) {
		player.frame++;
		player.last_animated_at = sys_time;
	}

	switch (player.frame) {
		case 0u:
			turn_player(jpad);
			break;
		case 1u:
			set_sprite_tile(player.sprite_ids[0], 44u);
			set_sprite_tile(player.sprite_ids[1], 45u);
			set_sprite_tile(player.sprite_ids[2], 46u);
			set_sprite_tile(player.sprite_ids[3], 47u);
			break;
		case 2u:
			set_sprite_tile(player.sprite_ids[0], 48u);
			set_sprite_tile(player.sprite_ids[1], 49u);
			set_sprite_tile(player.sprite_ids[2], 50u);
			set_sprite_tile(player.sprite_ids[3], 51u);
			break;
		case 3u:
			set_sprite_tile(player.sprite_ids[0], 52u);
			set_sprite_tile(player.sprite_ids[1], 53u);
			set_sprite_tile(player.sprite_ids[2], 54u);
			set_sprite_tile(player.sprite_ids[3], 55u);
			break;
		case 4u:
			set_sprite_tile(player.sprite_ids[0], 56u);
			set_sprite_tile(player.sprite_ids[1], 57u);
			set_sprite_tile(player.sprite_ids[2], 58u);
			set_sprite_tile(player.sprite_ids[3], 59u);
			break;
		default:
			destroy_player();
			break;
	}
}

void reset_game() {
	UBYTE i;

	game_running = 1u;
	health = MAX_HEALTH;
	score = 0;

	for (i = 0u; i < 20u; i++) {		
		set_win_tiles(i, 0, 1u, 1u, blank_map);
	}

	update_score();
	update_health();

	player.direction = J_UP;
	turn_player(J_UP);
	player.is_destroyed = 0u;
	player.x = 80u;
	player.y = 80u;
	player.velocity_x = 0;
	player.velocity_y = 0;

	render_game_character(&player);
}

void main() {
	UINT8 jpad = 0u, j, i;
	BGP_REG = 0x00; // Start palette all white for fade_in_white()

	disable_interrupts();

	set_win_data(6u, 37u, FontData);
	set_bkg_data(0u, 7u, BackgroundData);
	set_bkg_data(43u, 23u, TitleData);
	move_win(7u, 136u);

	for (j = 0u; j < 32u; j++) {
		for (i = 0u; i < 32u; i++) {		
			set_win_tiles(i, j, 1u, 1u, blank_map);
			set_bkg_tiles(i, j, 1u, 1u, blank_map);
		}
	}

	set_bkg_tiles(0, 0, 20u, 19u, TitleMap);
	set_win_tiles(0u, 0u, 11u, 1u, reset_prompt_map);
	move_win(43u, 108u);

	SHOW_BKG;
	SHOW_WIN;
	DISPLAY_ON;

	enable_interrupts();

	fade_in_white();
	waitpad(J_START);
	fade_out_white();

	disable_interrupts();

	move_win(7u, 136u);
	set_bkg_tiles(0, 0, 40u, 34u, BackgroundMap);
	set_sprite_data(0u, 60u, SpriteData);
	set_up_player();

	for (i = 0u; i < BULLET_COUNT; i++) {
		set_up_bullet(&bullets[i]);
	}

	for (i = 0u; i < ENEMY_COUNT - SMALL_ENEMY_COUNT; i++) {
		set_up_enemy(&enemies[i], i % 2 == 0u);
	}

	for (i = ENEMY_COUNT - SMALL_ENEMY_COUNT; i < ENEMY_COUNT; i++) {
		set_up_small_enemy(&enemies[i], i % 2 == 0u);
	}

	reset_game();

	SHOW_SPRITES;

	enable_interrupts();

	fade_in_white();

	while (1) {
		wait_vbl_done();
		wait_vbl_done();

		jpad = joypad();
		update_player(jpad);

		if (game_running) {
			if (!player.is_animating && (jpad & J_A)) {
				fire_bullet(player.direction);
			}
		} else if (jpad & J_START) {
			reset_game();
			continue;
		}

		for (i = 0u; i < BULLET_COUNT; i++) {
			if (!bullets[i].is_destroyed) {
				update_bullet(&bullets[i]);

				for (j = 0u; j < ENEMY_COUNT; j++) {
					if (did_collide(&bullets[i], &enemies[j])) {
						destroy_game_character(&bullets[i]);
						destroy_enemy(&enemies[j]);
						score += 1u;
						update_score();
						break;
					}
				}
			}

			render_game_character(&bullets[i]);
		}

		for (i = 0u; i < ENEMY_COUNT; i++) {
			if (enemies[i].is_destroyed) {
				continue;
			}

			update_enemy(&enemies[i]);

			if (did_collide(&player, &enemies[i])) {
				health--;
				update_health();
				destroy_enemy(&enemies[i]);

				if (health == 0u) {
					start_destroying_player();
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
	}
}
