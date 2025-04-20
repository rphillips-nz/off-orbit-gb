#ifndef _OO_PLAYER_H_
#define _OO_PLAYER_H_

#include <gb/gb.h>

void set_up_player(void);

void turn_player(uint8_t jpad);

void destroy_player(void);

void start_destroying_player(void);

void reset_player(void);

void animate_player(uint8_t jpad);

void update_player(uint8_t jpad);

#endif
